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

## 硬件清单与接线图
清单：ESP32、光敏传感器、LCD1602、1k电阻、LED灯、杜邦线、面包板。

接线图：![ESP32 光感监测接线图](https://github.com/WuQinghui-00/ESP32-Light-Sensor-Monitor/blob/main/images%20/esp32-light-sensor-wiring.drawio.png)

### 快速开始
1.  **克隆仓库**
bash
git clone https://github.com/WuQinghui-00/ESP32-Light-Sensor-Monitor.git
cd ESP32-Light-Sensor-Monitor
2.  **硬件连接**
参照上方接线图连接你的ESP32开发板。
3.  **环境配置**
- 安装 Arduino IDE 
- 安装必要的库：`LiquidCrystal_I2C`。
4.  **编译与上传**
- 用 Arduino IDE 打开 `light_sensor.ino`。
- 选择正确的开发板（ESP32 Dev Module）和端口。
- 点击上传。
5.  **观察结果**
观察LCD屏幕显示，并打开串口监视器（波特率115200）查看数据流。

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
