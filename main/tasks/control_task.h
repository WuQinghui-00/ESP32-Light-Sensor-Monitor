#ifndef CONTROL_TASK_H
#define CONTROL_TASK_H

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

// 控制任务参数结构体
typedef struct {
    QueueHandle_t sensor_queue;
    QueueHandle_t cmd_queue;
} control_task_params_t;

void control_task(void *pvParameters);

#endif