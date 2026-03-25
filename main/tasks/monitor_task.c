#include "monitor_task.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "include/common_defs.h"
#include "include/config.h"

static const char *TAG = "MONITOR_TASK";

// 获取系统运行时间（ms）
static uint32_t get_system_uptime_ms(void)
{
    return (uint32_t)(esp_timer_get_time() / 1000);
}

void monitor_task(void *pvParameters)
{
    monitor_task_params_t *params = (monitor_task_params_t *)pvParameters;
    QueueHandle_t sensor_queue = params->sensor_queue;
    
    sensor_data_t sensor_data;
    uint32_t last_report_time = 0;
    uint32_t sample_count = 0;
    uint16_t max_voltage = 0;
    uint16_t min_voltage = 3300;
    
    ESP_LOGI(TAG, "Monitor task started");
    
    while (1) {
        // 接收传感器数据
        if (xQueueReceive(sensor_queue, &sensor_data, pdMS_TO_TICKS(1000))) {
            sample_count++;
            
            // 统计最大最小值
            if (sensor_data.voltage_mv > max_voltage) max_voltage = sensor_data.voltage_mv;
            if (sensor_data.voltage_mv < min_voltage) min_voltage = sensor_data.voltage_mv;
            
            // 定期上报统计信息
            uint32_t now = get_system_uptime_ms();
            if (now - last_report_time >= MONITOR_REPORT_MS) {
                ESP_LOGI(TAG, "=== System Status Report ===");
                ESP_LOGI(TAG, "Uptime: %lu ms", now);
                ESP_LOGI(TAG, "Samples: %lu", sample_count);
                ESP_LOGI(TAG, "Current: %dmV (%s)", 
                    sensor_data.voltage_mv, sensor_data.state_str);
                ESP_LOGI(TAG, "Min: %dmV, Max: %dmV", min_voltage, max_voltage);
                ESP_LOGI(TAG, "Light level: %d%%", sensor_data.light_level);
                
                // 打印任务统计信息
                char task_stats[512];
                vTaskList(task_stats);
                ESP_LOGI(TAG, "\nTask Stats:\n%s", task_stats);
                
                last_report_time = now;
            }
        }
        
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}