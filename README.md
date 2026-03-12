# ESP32 光敏传感器监测系统

![GitHub](https://img.shields.io/github/license/WuQinghui-00/ESP32-Light-Sensor-Monitor)
![GitHub last commit](https://img.shields.io/github/last-commit/WuQinghui-00/ESP32-Light-Sensor-Monitor)
![GitHub code size in bytes](https://img.shields.io/github/languages/code-size/WuQinghui-00/ESP32-Light-Sensor-Monitor)

## 项目演示
## 项目概览
一个基于ESP32的智能环境光监测系统，可实时采集光照强度自动控制LED开关并通过本地LCD显示，为智能家居、农业大棚等场景提供低成本监测方案。
## 功能特性
- ✅ **实时监测**：通过光敏电阻模块实时采集环境光照强度。
- ✅ **本地显示**：通过I2C接口的LCD1602屏幕实时显示光照状态（亮BRIGHT/暗DARK）和LED的开关状态（关OFF/开ON）。
- ✅ **串口调试**：输出带时间戳的详细日志，方便调试与数据分析。
- ✅ **自动控制**：可根据预设阈值自动控制LED开关（可扩展）。
- ✅ **易于部署**：使用Arduino框架，代码结构清晰，注释完整。

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
