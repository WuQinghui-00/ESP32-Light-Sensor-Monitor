# ESP32 智能光照监测系统（FreeRTOS + MQTT）

## 项目概览
基于 ESP32 和 FreeRTOS 的物联网智能光照监测系统，实现环境光照检测、LED 自动控制、LCD 实时显示，并支持 **Wi-Fi 连接** 和 **MQTT 云端数据上报与远程控制**。
---

![GitHub](https://img.shields.io/github/license/WuQinghui-00/ESP32-Light-Sensor-Monitor)
![GitHub last commit](https://img.shields.io/github/last-commit/WuQinghui-00/ESP32-Light-Sensor-Monitor)
![GitHub code size in bytes](https://img.shields.io/github/languages/code-size/WuQinghui-00/ESP32-Light-Sensor-Monitor)

## 项目演示视频
【基于ESP32和FreeRTOS的智能光照监测系统-哔哩哔哩】 https://b23.tv/EJGWNB5

## 项目博客记录
【ESP32 + FreeRTOS 智能光照监测系统（附源码） -  CSDN App】https://blog.csdn.net/2501_92470428/article/details/159498961?sharetype=blogdetail&shareId=159498961&sharerefer=APP&sharesource=2501_92470428&sharefrom=link

## 🖼️ 硬件实物展示
下图是项目的实际硬件连接图：

![ESP32 光感监测系统实物连接](https://github.com/WuQinghui-00/ESP32-Light-Sensor-Monitor/blob/main/images/physical-setup.jpg)


## 📋 功能特性

| 模块 | 实现 | 技术亮点 |
|------|------|---------|
| **系统架构** | 4 个独立任务 + 队列通信 | 模块解耦、高内聚低耦合 |
| **任务调度** | 优先级分配、栈空间优化 | FreeRTOS 深度应用 |
| **硬件抽象** | 分层驱动设计 | 可移植性强 |
| **状态管理** | 实时状态机 | 代码健壮性 |
| **日志系统** | 分级日志输出 | 便于调试与问题定位 |


### 本地功能
| 功能 | 说明 |
|------|------|
| ✅ **FreeRTOS 多任务架构** | Sensor/Control/Display/Monitor 4 个独立任务，队列通信，模块解耦 |
| ✅ **光照检测** | 光敏传感器实时采集环境光照 |
| ✅ **LED 自动控制** | 暗环境自动开灯，亮环境自动关灯 |
| ✅ **LCD 实时显示** | 显示光照状态（BRIGHT/DARK）和 LED 状态 |
| ✅ **低功耗优化** | light sleep 模式，待机电流 80mA → 2mA，续航提升 **40 倍** |
| ✅ **串口日志** | 实时输出系统状态，便于调试 |

### 云端功能
| 功能 | 说明 |
|------|------|
| ✅ **Wi-Fi 连接** | 自动连接，支持断线重连 |
| ✅ **MQTT 数据上报** | 每 3 秒上报光照数据（JSON 格式） |
| ✅ **远程控制** | 手机/电脑发送 `LED_ON` / `LED_OFF` 远程控制 LED |

---

## 🛠️ 技术栈

| 层级 | 技术 |
|------|------|
| 芯片 | ESP32 |
| 框架 | ESP-IDF v5.3.1 |
| RTOS | FreeRTOS |
| 通信协议 | Wi-Fi、MQTT |
| 外设驱动 | ADC、GPIO、I2C |
| 云平台 | EMQX 公共 MQTT 服务器 |

---

### 任务职责说明

| 任务 | 优先级 | 栈大小 | 职责 |
|------|--------|--------|------|
| **Sensor Task** | 3（最高） | 4KB | 采集光敏传感器状态，发送到队列 |
| **Control Task** | 2 | 4KB | 接收传感器数据，控制 LED 开关 |
| **Display Task** | 2 | 4KB | 接收传感器数据，更新 LCD 显示 |
| **Monitor Task** | 1（最低）| 4KB | 系统监控、日志上报、统计信息 |

> **设计考量**：Sensor 任务作为数据源头优先级最高，确保数据实时采集；Monitor 任务仅做日志上报，优先级最低，不影响核心控制逻辑。

---
## 硬件清单与接线图
清单：ESP32、光敏传感器、LCD1602、1k电阻、LED灯、杜邦线、面包板。

接线图：![ESP32 光感监测接线图](https://github.com/WuQinghui-00/ESP32-Light-Sensor-Monitor/blob/main/images%20/esp32-light-sensor-wiring.drawio.png)




---

## 🚀 快速开始

### 环境准备

安装 ESP-IDF（官方文档）  
https://docs.espressif.com/projects/esp-idf/zh_CN/latest/esp32/get-started/

克隆项目
```bash
git clone https://github.com/WuQinghui-00/ESP32-Light-Sensor-Monitor.git
cd ESP32-Light-Sensor-Monitor
```

### 硬件连接

参照上方接线图连接你的 ESP32 开发板。

### 编译与烧录

设置目标芯片
```bash
idf.py set-target esp32
```

编译项目
```bash
idf.py build
```

烧录固件（替换 COM3 为实际端口）
```bash
idf.py -p COM3 flash
```

打开串口监视器
```bash
idf.py -p COM3 monitor
```

### 预期输出

```
I (xxx) MAIN: System: LightMonitor v1.0.0
I (xxx) SENSOR_TASK: Sensor task started
I (xxx) CONTROL_TASK: Control task started, LED on GPIO5
I (xxx) DISPLAY_TASK: Display task started
I (xxx) MONITOR_TASK: Monitor task started
I (xxx) SENSOR_TASK: State: DARK
I (xxx) CONTROL_TASK: LED turned ON
I (xxx) DISPLAY_TASK: LCD updated
```

---

## 📁 代码结构

```
├── main/
│ ├── light_sensor_main.c # 主函数、Wi-Fi、MQTT、任务创建
│ ├── hardware/ # 硬件抽象层 (HAL)
│ │ ├── light_sensor.c/h # 光敏传感器驱动
│ │ └── lcd1602_i2c.c/h # LCD1602 I2C 驱动
│ ├── tasks/ # FreeRTOS 任务层
│ │ ├── sensor_task.c/h # 传感器采集任务（含 MQTT 发布）
│ │ ├── control_task.c/h # 控制逻辑任务
│ │ ├── display_task.c/h # 显示更新任务
│ │ └── monitor_task.c/h # 系统监控任务
│ └── include/ # 公共配置
│ ├── config.h # 系统配置（引脚、栈大小、优先级）
│ ├── common_defs.h # 公共数据结构
│ └── queue_defs.h # 队列句柄声明
├── CMakeLists.txt
├── LICENSE
└── README.md
```

---

## 📈 后续规划

- [ ] **PWM 调光**：LED 亮度平滑调节
- [ ] **自适应算法**：动态调整光照阈值
- [ ] **Wi-Fi + MQTT**：设备上云，远程监控✔️
- [ ] **OTA 升级**：远程固件更新
- [ ] **低功耗模式**：深度睡眠，电池供电✔️


---


### 🤝 贡献
欢迎提交 Issue 和 Pull Request 来帮助改进这个项目。

### 📄 许可证
本项目基于 MIT 许可证开源。详见 [LICENSE](LICENSE) 文件。
