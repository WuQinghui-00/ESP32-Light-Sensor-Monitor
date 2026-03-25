#ifndef MONITOR_TASK_H
#define MONITOR_TASK_H

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

// 监控任务参数结构体
typedef struct {
    QueueHandle_t sensor_queue;
} monitor_task_params_t;

void monitor_task(void *pvParameters);

#endif