#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"
#include "nvs_flash.h"

// 包含任务头文件
#include "tasks/sensor_task.h"
#include "tasks/control_task.h"
#include "tasks/display_task.h"
#include "tasks/monitor_task.h"
#include "include/queue_defs.h"
#include "include/common_defs.h"
#include "include/config.h"

static const char *TAG = "MAIN";

// 定义队列句柄
QueueHandle_t sensor_data_queue = NULL;
QueueHandle_t control_cmd_queue = NULL;

// 创建所有任务
static void create_all_tasks(void)
{
    sensor_data_queue = xQueueCreate(QUEUE_LENGTH_SENSOR, sizeof(sensor_data_t));
    control_cmd_queue = xQueueCreate(QUEUE_LENGTH_CONTROL, sizeof(control_cmd_t));
    
    // 只创建 Sensor 任务
    xTaskCreate(sensor_task, "Sensor", TASK_STACK_SENSOR, NULL, TASK_PRIO_SENSOR, NULL);
    
    // 暂时注释掉其他任务
    // xTaskCreate(control_task, "Control", TASK_STACK_CONTROL, &control_params, TASK_PRIO_CONTROL, NULL);
    // xTaskCreate(display_task, "Display", TASK_STACK_DISPLAY, &display_params, TASK_PRIO_DISPLAY, NULL);
    // xTaskCreate(monitor_task, "Monitor", TASK_STACK_MONITOR, &monitor_params, TASK_PRIO_MONITOR, NULL);
    
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
    
    ESP_LOGI(TAG, "========================================");
    ESP_LOGI(TAG, "System: %s v%s", SYSTEM_NAME, SYSTEM_VERSION);
    ESP_LOGI(TAG, "========================================");
    
    // 创建所有任务
    create_all_tasks();
    
    ESP_LOGI(TAG, "System started successfully");
    
    // 主任务删除自己
    vTaskDelete(NULL);
}