#include "control_task.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "include/common_defs.h"
#include "include/config.h"

static const char *TAG = "CONTROL_TASK";

void control_task(void *pvParameters)
{
    control_task_params_t *params = (control_task_params_t *)pvParameters;
    QueueHandle_t sensor_queue = params->sensor_queue;
    
    sensor_data_t sensor_data;
    bool led_state = false;
    
    // 初始化 LED GPIO
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << LED_GPIO_PIN),
        .mode = GPIO_MODE_OUTPUT,
        .intr_type = GPIO_INTR_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .pull_up_en = GPIO_PULLUP_DISABLE,
    };
    gpio_config(&io_conf);
    gpio_set_level(LED_GPIO_PIN, 0);
    
    ESP_LOGI(TAG, "Control task started, LED on GPIO%d", LED_GPIO_PIN);
    
    while (1) {
        if (xQueueReceive(sensor_queue, &sensor_data, pdMS_TO_TICKS(100))) {
            // LED 控制逻辑：暗时开灯，亮时关灯
            bool new_led_state = !sensor_data.is_bright;
            
            if (new_led_state != led_state) {
                led_state = new_led_state;
                gpio_set_level(LED_GPIO_PIN, led_state ? 1 : 0);
                ESP_LOGI(TAG, "LED turned %s", led_state ? "ON" : "OFF");
            }
        }
        
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}