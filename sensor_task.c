#include "sensor_task.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"
#include "esp_sleep.h"
#include "driver/gpio.h"
#include "hardware/light_sensor.h"
#include "include/common_defs.h"
#include "include/queue_defs.h"
#include "include/config.h"

static const char *TAG = "SENSOR_TASK";

void sensor_task(void *pvParameters)
{
    sensor_data_t sensor_data;
    
    light_sensor_init();
    ESP_LOGI(TAG, "Sensor task started");
    
    // 配置 GPIO4 唤醒
    const uint64_t wakeup_pin_mask = 1ULL << GPIO_NUM_4;
    esp_sleep_enable_ext1_wakeup(wakeup_pin_mask, ESP_EXT1_WAKEUP_ANY_HIGH);
    
    gpio_config_t io_conf = {
        .pin_bit_mask = wakeup_pin_mask,
        .mode = GPIO_MODE_INPUT,
        .intr_type = GPIO_INTR_DISABLE,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
    };
    gpio_config(&io_conf);
    
    ESP_LOGI(TAG, "Low power configured, wake on GPIO4");
    
    while (1) {
        // 读取传感器
        sensor_data.is_bright = light_sensor_is_bright();
        
        if (sensor_data.is_bright) {
            sensor_data.voltage_mv = 3000;
            sensor_data.adc_raw = 3700;
            sensor_data.light_level = 90;
            sprintf(sensor_data.state_str, "BRIGHT");
        } else {
            sensor_data.voltage_mv = 500;
            sensor_data.adc_raw = 600;
            sensor_data.light_level = 15;
            sprintf(sensor_data.state_str, "DARK ");
        }
        
        // 发送到队列
        if (sensor_data_queue != NULL) {
            xQueueSend(sensor_data_queue, &sensor_data, pdMS_TO_TICKS(100));
        }
        
        ESP_LOGI(TAG, "State: %s, Voltage: %dmV", sensor_data.state_str, sensor_data.voltage_mv);
        
        // 等待 3 秒后进入睡眠
        vTaskDelay(pdMS_TO_TICKS(3000));
        
        // 进入 light sleep
        ESP_LOGI(TAG, "Entering light sleep...");
        esp_light_sleep_start();
        
        // 唤醒后继续循环
        ESP_LOGI(TAG, "Woken up!");
    }
}