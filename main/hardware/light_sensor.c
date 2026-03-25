#include "light_sensor.h"
#include "esp_log.h"
#include "include/config.h"

static const char *TAG = "LIGHT_SENSOR";

void light_sensor_init(void)
{
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << LIGHT_SENSOR_GPIO),
        .mode = GPIO_MODE_INPUT,
        .intr_type = GPIO_INTR_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .pull_up_en = GPIO_PULLUP_ENABLE,
    };
    gpio_config(&io_conf);
    ESP_LOGI(TAG, "Light sensor initialized on GPIO%d", LIGHT_SENSOR_GPIO);
}

bool light_sensor_is_bright(void)
{
    // 根据你的模块：暗时输出 HIGH，亮时输出 LOW
    // 如果相反，把下面的 == 0 改成 == 1
    return gpio_get_level(LIGHT_SENSOR_GPIO) == 0;
}