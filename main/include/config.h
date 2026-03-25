#ifndef CONFIG_H
#define CONFIG_H

// 系统配置
#define SYSTEM_NAME "LightMonitor"
#define SYSTEM_VERSION "1.0.0"

// 任务栈大小配置
#define TASK_STACK_SENSOR   4096
#define TASK_STACK_CONTROL  4096
#define TASK_STACK_DISPLAY  4096
#define TASK_STACK_MONITOR  4096

// 任务优先级
#define TASK_PRIO_SENSOR    3
#define TASK_PRIO_CONTROL   2
#define TASK_PRIO_DISPLAY   2
#define TASK_PRIO_MONITOR   1

// 任务延时（ms）
#define SENSOR_SAMPLE_MS    200
#define DISPLAY_UPDATE_MS   500
#define MONITOR_REPORT_MS   5000

// 引脚配置（适配你的硬件）
#define LIGHT_SENSOR_GPIO   GPIO_NUM_4   // DO 接 GPIO4
#define LED_GPIO_PIN        GPIO_NUM_5   // LED 接 GPIO5

// I2C 引脚（LCD）
#define I2C_MASTER_SCL_IO   22
#define I2C_MASTER_SDA_IO   21

// LCD 配置
#define LCD_I2C_ADDRESS     0x27
#define LCD_COLS            16
#define LCD_ROWS            2

#endif