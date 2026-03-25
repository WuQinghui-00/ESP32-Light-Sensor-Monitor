# ESP32 光敏传感器监测系统

![GitHub](https://img.shields.io/github/license/WuQinghui-00/ESP32-Light-Sensor-Monitor)
![GitHub last commit](https://img.shields.io/github/last-commit/WuQinghui-00/ESP32-Light-Sensor-Monitor)
![GitHub code size in bytes](https://img.shields.io/github/languages/code-size/WuQinghui-00/ESP32-Light-Sensor-Monitor)

## 项目演示视频
https://b23.tv/NFFXEWc

## 🖼️ 硬件实物展示
下图是项目的实际硬件连接图：

![ESP32 光感监测系统实物连接](https://github.com/WuQinghui-00/ESP32-Light-Sensor-Monitor/blob/main/images/physical-setup.jpg)

## 项目概览
基于 **ESP32** 和 **FreeRTOS** 的嵌入式物联网系统，实现环境光照监测、设备自动控制、实时数据显示。项目展示了嵌入式软件开发中的多任务架构设计、实时操作系统应用、外设驱动开发等核心能力。

---
## 📋 功能特性

| 模块 | 实现 | 技术亮点 |
|------|------|---------|
| **系统架构** | 4 个独立任务 + 队列通信 | 模块解耦、高内聚低耦合 |
| **任务调度** | 优先级分配、栈空间优化 | FreeRTOS 深度应用 |
| **硬件抽象** | 分层驱动设计 | 可移植性强 |
| **状态管理** | 实时状态机 | 代码健壮性 |
| **日志系统** | 分级日志输出 | 便于调试与问题定位 |

### 核心功能
- ✅ **实时监测**：光敏传感器毫秒级响应，精准检测环境光照
- ✅ **自动控制**：暗环境自动开启 LED，亮环境自动关闭
- ✅ **本地显示**：I2C 接口 LCD1602 实时显示系统状态
- ✅ **多任务协同**：Sensor（采集）→ Control（控制）→ Display（显示）→ Monitor（监控）数据流闭环

---

## 🛠️ 技术栈

| 技术领域 | 具体实现 |
|---------|---------|
| **芯片平台** | ESP32 (Xtensa LX6 双核) |
| **开发框架** | ESP-IDF v5.3.1 |
| **实时操作系统** | FreeRTOS（任务调度、队列通信、任务通知）|
| **外设驱动** | GPIO（输入/输出）、I2C（LCD 通信）|
| **编程语言** | C11 |
| **构建工具** | CMake / Ninja |
| **版本控制** | Git / GitHub |

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


## 🚀 快速开始

### 环境准备
```bash
# 安装 ESP-IDF（官方文档）
# https://docs.espressif.com/projects/esp-idf/zh_CN/latest/esp32/get-started/

# 克隆项目
git clone https://github.com/WuQinghui-00/ESP32-Light-Sensor-Monitor.git
cd ESP32-Light-Sensor-Monitor

 ### 硬件连接
参照上方接线图连接你的ESP32开发板。


### 进阶使用
- **连接Wi-Fi**：代码已预留网络接口，可轻松扩展MQTT协议上报数据至云端。
- **阈值调节**：可通过修改代码中的 `LIGHT_THRESHOLD` 变量来调整光敏触发灵敏度。

### 未来计划
- [ ] 实现通过手机APP（如Blinker）远程监控
- [ ] 使用FreeRTOS进行多任务管理
- [ ] 设计并打样一体化PCB

### 🤝 贡献
欢迎提交 Issue 和 Pull Request 来帮助改进这个项目。

### 📄 许可证
本项目基于 MIT 许可证开源。详见 [LICENSE](LICENSE) 文件。
