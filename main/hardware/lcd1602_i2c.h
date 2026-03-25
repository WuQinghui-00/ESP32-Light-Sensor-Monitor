#ifndef LCD1602_I2C_H
#define LCD1602_I2C_H

#include <stdint.h>
#include <stdbool.h>
#include "include/config.h"

// I2C 配置（从 config.h 读取）
#define I2C_MASTER_NUM      I2C_NUM_0
#define I2C_MASTER_FREQ_HZ  100000

void lcd_init(void);
void lcd_clear(void);
void lcd_set_cursor(uint8_t col, uint8_t row);
void lcd_print_str(const char* str);
void lcd_print_int(int num);
void lcd_printf(const char* format, ...);

#endif