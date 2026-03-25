idf_component_register(SRCS "light_sensor_main.c"
    "hardware/light_sensor.c"
    "hardware/lcd1602_i2c.c"
    "tasks/sensor_task.c"
    "tasks/control_task.c"
    "tasks/display_task.c"
    "tasks/monitor_task.c"
    INCLUDE_DIRS "."
    "hardware"
    "tasks"
    "include"
    REQUIRES driver nvs_flash esp_timer freertos)

# 禁用警告转错误，方便编译
target_compile_options(${COMPONENT_TARGET} PRIVATE -Wno-error)