#include "lcd1602_i2c.h"
#include "driver/i2c.h"
#include "esp_log.h"
#include <stdio.h>
#include <stdarg.h>

static const char *TAG = "LCD1602";
static uint8_t lcd_backlight = 0x08;  // 背光控制位

// LCD 命令
#define LCD_CLEAR           0x01
#define LCD_HOME            0x02
#define LCD_ENTRY_MODE      0x06
#define LCD_DISPLAY_ON      0x0C
#define LCD_FUNCTION_8BIT   0x30
#define LCD_FUNCTION_4BIT   0x20
#define LCD_SET_DDRAM       0x80

// 发送字节到 LCD
static void lcd_send_byte(uint8_t data, uint8_t mode)
{
    uint8_t high_nib = (data & 0xF0);
    uint8_t low_nib = ((data << 4) & 0xF0);
    uint8_t backlight = lcd_backlight;
    
    uint8_t data_buf[4];
    
    // 发送高4位
    data_buf[0] = high_nib | backlight | mode | 0x04;  // EN = 1
    data_buf[1] = high_nib | backlight | mode | 0x00;  // EN = 0
    // 发送低4位
    data_buf[2] = low_nib | backlight | mode | 0x04;   // EN = 1
    data_buf[3] = low_nib | backlight | mode | 0x00;   // EN = 0
    
    i2c_master_write_to_device(I2C_MASTER_NUM, LCD_I2C_ADDRESS, data_buf, 4, pdMS_TO_TICKS(100));
    vTaskDelay(pdMS_TO_TICKS(2));
}

static void lcd_write_cmd(uint8_t cmd)
{
    lcd_send_byte(cmd, 0x00);
}

static void lcd_write_data(uint8_t data)
{
    lcd_send_byte(data, 0x01);
}

void lcd_init(void)
{
    ESP_LOGI(TAG, "Initializing LCD1602 I2C...");
    
    // 初始化 I2C 总线
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_MASTER_FREQ_HZ,
    };
    i2c_param_config(I2C_MASTER_NUM, &conf);
    i2c_driver_install(I2C_MASTER_NUM, conf.mode, 0, 0, 0);
    
    vTaskDelay(pdMS_TO_TICKS(50));
    
    // 初始化 LCD
    lcd_write_cmd(LCD_FUNCTION_8BIT);
    vTaskDelay(pdMS_TO_TICKS(5));
    lcd_write_cmd(LCD_FUNCTION_8BIT);
    vTaskDelay(pdMS_TO_TICKS(1));
    lcd_write_cmd(LCD_FUNCTION_8BIT);
    lcd_write_cmd(LCD_FUNCTION_4BIT);
    lcd_write_cmd(0x28);  // 4-bit, 2行, 5x8点阵
    lcd_write_cmd(LCD_DISPLAY_ON);
    lcd_write_cmd(LCD_CLEAR);
    lcd_write_cmd(LCD_ENTRY_MODE);
    
    ESP_LOGI(TAG, "LCD1602 initialized");
}

void lcd_clear(void)
{
    lcd_write_cmd(LCD_CLEAR);
    vTaskDelay(pdMS_TO_TICKS(2));
}

void lcd_set_cursor(uint8_t col, uint8_t row)
{
    uint8_t addr = col + (row * 0x40);
    lcd_write_cmd(LCD_SET_DDRAM | addr);
}

void lcd_print_str(const char* str)
{
    while (*str) {
        lcd_write_data(*str++);
    }
}

void lcd_print_int(int num)
{
    char buf[16];
    sprintf(buf, "%d", num);
    lcd_print_str(buf);
}

void lcd_printf(const char* format, ...)
{
    char buf[64];
    va_list args;
    va_start(args, format);
    vsnprintf(buf, sizeof(buf), format, args);
    va_end(args);
    lcd_print_str(buf);
}