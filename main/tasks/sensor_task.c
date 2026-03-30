#include "sensor_task.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"
#include "hardware/light_sensor.h"
#include "include/common_defs.h"
#include "include/queue_defs.h"
#include "include/config.h"

// 外部声明 MQTT 发布函数
extern void mqtt_publish_sensor_data(sensor_data_t *data);

static const char *TAG = "SENSOR_TASK";

void sensor_task(void *pvParameters)
{
    sensor_data_t sensor_data;
    
    light_sensor_init();
    ESP_LOGI(TAG, "Sensor task started");
    
    while (1) {
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
        
        if (sensor_data_queue != NULL) {
            xQueueSend(sensor_data_queue, &sensor_data, pdMS_TO_TICKS(100));
        }
        
        ESP_LOGI(TAG, "State: %s, Voltage: %dmV", sensor_data.state_str, sensor_data.voltage_mv);
        
        // 发布到 MQTT
        mqtt_publish_sensor_data(&sensor_data);
        
        vTaskDelay(pdMS_TO_TICKS(3000));
    }
}