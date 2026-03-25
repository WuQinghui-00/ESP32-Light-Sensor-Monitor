#ifndef QUEUE_DEFS_H
#define QUEUE_DEFS_H

#include "freertos/queue.h"
#include "common_defs.h"

// 队列句柄（外部声明，实际在主文件中定义）
extern QueueHandle_t sensor_data_queue;
extern QueueHandle_t control_cmd_queue;

// 队列长度
#define QUEUE_LENGTH_SENSOR    10
#define QUEUE_LENGTH_CONTROL   10

#endif