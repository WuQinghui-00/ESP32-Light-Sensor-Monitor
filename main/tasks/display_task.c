#include "display_task.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"
#include "hardware/lcd1602_i2c.h"
#include "include/common_defs.h"
#include "include/config.h"

static const char *TAG = "DISPLAY_TASK";

void display_task(void *pvParameters)
{
    display_task_params_t *params = (display_task_params_t *)pvParameters;
    QueueHandle_t sensor_queue = params->sensor_queue;
    
    sensor_data_t sensor_data;
    char line1[17] = "Light Monitor";
    char line2[17] = "State:          ";
    
    lcd_init();
    lcd_clear();
    
    // 显示静态标题
    lcd_set_cursor(0, 0);
    lcd_print_str("Light Monitor");
    lcd_set_cursor(0, 1);
    lcd_print_str("State:");
    
    ESP_LOGI(TAG, "Display task started");
    
    while (1) {
        if (xQueueReceive(sensor_queue, &sensor_data, pdMS_TO_TICKS(500))) {
            // 更新第二行显示
            lcd_set_cursor(6, 1);
            if (!sensor_data.is_bright) {
                lcd_print_str("Dark,ON   ");
                ESP_LOGD(TAG, "LCD: Dark, ON");
            } else {
                lcd_print_str("Bright,OFF");
                ESP_LOGD(TAG, "LCD: Bright, OFF");
            }
        }
        
        vTaskDelay(pdMS_TO_TICKS(DISPLAY_UPDATE_MS));
    }
}