#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "esp_http_client.h"
#include "esp_pm.h"
#include "mqtt_client.h"  // 新增 MQTT

// 包含任务头文件
#include "tasks/sensor_task.h"
#include "tasks/control_task.h"
#include "tasks/display_task.h"
#include "tasks/monitor_task.h"
#include "include/queue_defs.h"
#include "include/common_defs.h"
#include "include/config.h"

static const char *TAG = "MAIN";
static const char *TAG_WIFI = "WIFI";
static const char *TAG_MQTT = "MQTT";

// Wi-Fi 配置
#define WIFI_SSID      "Boo"
#define WIFI_PASS      "wqh2005828"

// MQTT 配置
#define MQTT_BROKER_URI "mqtt://broker.emqx.io"  // 免费公共 MQTT 服务器

// 定义队列句柄
QueueHandle_t sensor_data_queue = NULL;
QueueHandle_t control_cmd_queue = NULL;

// MQTT 客户端句柄
esp_mqtt_client_handle_t mqtt_client = NULL;

// ========== MQTT 事件处理 ==========
static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    esp_mqtt_event_handle_t event = event_data;
    
    switch (event->event_id) {
        case MQTT_EVENT_CONNECTED:
            ESP_LOGI(TAG_MQTT, "MQTT connected");
            // 订阅远程控制主题
            esp_mqtt_client_subscribe(mqtt_client, "/esp32/light/control", 0);
            ESP_LOGI(TAG_MQTT, "Subscribed to /esp32/light/control");
            break;
            
        case MQTT_EVENT_DISCONNECTED:
            ESP_LOGI(TAG_MQTT, "MQTT disconnected");
            break;
            
        case MQTT_EVENT_DATA:
            ESP_LOGI(TAG_MQTT, "MQTT data received, topic: %.*s, data: %.*s",
                     event->topic_len, event->topic,
                     event->data_len, event->data);
            
            // 解析远程控制命令
            if (strncmp(event->data, "LED_ON", event->data_len) == 0) {
                // 发送命令到 Control 任务
                control_cmd_t cmd = { .cmd_type = CMD_LED_ON, .param = 1 };
                xQueueSend(control_cmd_queue, &cmd, 0);
                ESP_LOGI(TAG_MQTT, "Remote command: LED ON");
            } else if (strncmp(event->data, "LED_OFF", event->data_len) == 0) {
                control_cmd_t cmd = { .cmd_type = CMD_LED_OFF, .param = 0 };
                xQueueSend(control_cmd_queue, &cmd, 0);
                ESP_LOGI(TAG_MQTT, "Remote command: LED OFF");
            }
            break;
            
        default:
            break;
    }
}

// ========== 发送传感器数据到云端 ==========
void mqtt_publish_sensor_data(sensor_data_t *data)
{
    if (mqtt_client == NULL) return;
    
    char payload[128];
    snprintf(payload, sizeof(payload),
             "{\"voltage\":%d,\"bright\":%d,\"state\":\"%s\"}",
             data->voltage_mv,
             data->is_bright ? 1 : 0,
             data->state_str);
    
    esp_mqtt_client_publish(mqtt_client, "/esp32/light/sensor", payload, 0, 1, 0);
    ESP_LOGD(TAG_MQTT, "Published: %s", payload);
}

// ========== MQTT 初始化 ==========
static void mqtt_init(void)
{
    esp_mqtt_client_config_t mqtt_cfg = {
        .broker.address.uri = MQTT_BROKER_URI,
    };
    
    mqtt_client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(mqtt_client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
    esp_mqtt_client_start(mqtt_client);
    
    ESP_LOGI(TAG_MQTT, "MQTT initialized, broker: %s", MQTT_BROKER_URI);
}

// ========== Wi-Fi 事件处理 ==========
static void wifi_event_handler(void* arg, esp_event_base_t event_base,
                                int32_t event_id, void* event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        ESP_LOGI(TAG_WIFI, "Wi-Fi started, connecting...");
        esp_wifi_connect();
    } 
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        wifi_event_sta_disconnected_t* disconnected = (wifi_event_sta_disconnected_t*) event_data;
        ESP_LOGI(TAG_WIFI, "Wi-Fi disconnected, reason: %d", disconnected->reason);
        esp_wifi_connect();
    } 
    else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        ESP_LOGI(TAG_WIFI, "Got IP: " IPSTR, IP2STR(&event->ip_info.ip));
        
        // Wi-Fi 连接成功后，初始化 MQTT
        mqtt_init();
    }
}

// ========== Wi-Fi 初始化 ==========
static void wifi_init(void)
{
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();
    
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    
    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &wifi_event_handler, NULL));
    
    wifi_config_t wifi_config = {
        .sta = {
            .ssid = WIFI_SSID,
            .password = WIFI_PASS,
            .threshold.authmode = WIFI_AUTH_WPA2_PSK,
        },
    };
    
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());
    
    ESP_LOGI(TAG_WIFI, "Wi-Fi initialization done.");
}

// ========== HTTP 网络测试 ==========
static void test_http_request(void)
{
    esp_http_client_config_t config = {
        .url = "http://httpbin.org/get",
        .timeout_ms = 5000,
    };
    esp_http_client_handle_t client = esp_http_client_init(&config);
    
    esp_err_t err = esp_http_client_perform(client);
    if (err == ESP_OK) {
        int status_code = esp_http_client_get_status_code(client);
        ESP_LOGI(TAG_WIFI, "HTTP status: %d", status_code);
    } else {
        ESP_LOGE(TAG_WIFI, "HTTP request failed: %s", esp_err_to_name(err));
    }
    
    esp_http_client_cleanup(client);
}

// ========== 创建所有任务 ==========
static void create_all_tasks(void)
{
    sensor_data_queue = xQueueCreate(QUEUE_LENGTH_SENSOR, sizeof(sensor_data_t));
    control_cmd_queue = xQueueCreate(QUEUE_LENGTH_CONTROL, sizeof(control_cmd_t));
    
    if (sensor_data_queue == NULL || control_cmd_queue == NULL) {
        ESP_LOGE(TAG, "Failed to create queues");
        return;
    }
    
    control_task_params_t control_params = {
        .sensor_queue = sensor_data_queue,
        .cmd_queue = control_cmd_queue,
    };
    
    display_task_params_t display_params = {
        .sensor_queue = sensor_data_queue,
    };
    
    monitor_task_params_t monitor_params = {
        .sensor_queue = sensor_data_queue,
    };
    
    xTaskCreate(sensor_task, "Sensor", TASK_STACK_SENSOR, NULL, TASK_PRIO_SENSOR, NULL);
    xTaskCreate(control_task, "Control", TASK_STACK_CONTROL, &control_params, TASK_PRIO_CONTROL, NULL);
    xTaskCreate(display_task, "Display", TASK_STACK_DISPLAY, &display_params, TASK_PRIO_DISPLAY, NULL);
    xTaskCreate(monitor_task, "Monitor", TASK_STACK_MONITOR, &monitor_params, TASK_PRIO_MONITOR, NULL);
    
    ESP_LOGI(TAG, "All tasks created");
}

void app_main(void)
{
    // 初始化 NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
    
    // 电源管理配置
    esp_pm_config_esp32_t pm_config = {
        .light_sleep_enable = true,
        .max_freq_mhz = 80,
        .min_freq_mhz = 40,
    };
    esp_pm_configure(&pm_config);
    ESP_LOGI(TAG, "Power management configured");
    
    ESP_LOGI(TAG, "========================================");
    ESP_LOGI(TAG, "System: %s v%s", SYSTEM_NAME, SYSTEM_VERSION);
    ESP_LOGI(TAG, "========================================");
    
    // 初始化 Wi-Fi
    wifi_init();
    
    // 等待网络稳定
    vTaskDelay(pdMS_TO_TICKS(3000));
    
    // 测试 HTTP
    test_http_request();
    
    // 创建所有任务
    create_all_tasks();
    
    ESP_LOGI(TAG, "System started successfully");
    
    vTaskDelete(NULL);
}