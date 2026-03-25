#ifndef COMMON_DEFS_H
#define COMMON_DEFS_H

#include <stdint.h>

// 传感器数据类型
typedef struct {
    uint16_t adc_raw;        // ADC原始值 (0-4095)
    uint16_t voltage_mv;     // 电压值 (mV)
    uint8_t light_level;     // 光照等级 (0-100%)
    bool is_bright;          // true: 亮, false: 暗
    char state_str[8];       // "BRIGHT" 或 "DARK"
} sensor_data_t;

// 控制命令类型
typedef struct {
    uint8_t cmd_type;        // 命令类型
    uint16_t param;          // 命令参数
} control_cmd_t;

// 命令类型定义
#define CMD_LED_ON          0x01
#define CMD_LED_OFF         0x02
#define CMD_SET_THRESHOLD   0x03

// 系统状态
typedef enum {
    SYS_STATE_INIT,
    SYS_STATE_RUNNING,
    SYS_STATE_STANDBY,
    SYS_STATE_ERROR
} system_state_t;

#endif