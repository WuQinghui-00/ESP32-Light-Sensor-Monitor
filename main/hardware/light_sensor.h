#ifndef LIGHT_SENSOR_H
#define LIGHT_SENSOR_H

#include <stdbool.h>
#include "driver/gpio.h"

void light_sensor_init(void);
bool light_sensor_is_bright(void);  // true=亮, false=暗

#endif