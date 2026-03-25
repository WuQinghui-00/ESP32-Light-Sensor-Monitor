#ifndef DISPLAY_TASK_H
#define DISPLAY_TASK_H

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

// 显示任务参数结构体
typedef struct {
    QueueHandle_t sensor_queue;
} display_task_params_t;

void display_task(void *pvParameters);

#endif