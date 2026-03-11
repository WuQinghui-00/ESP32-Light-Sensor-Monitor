# ESP32 光敏传感器监测系统

![GitHub](https://img.shields.io/github/license/WuQinghui-00/ESP32-Light-Sensor-Monitor)
![GitHub last commit](https://img.shields.io/github/last-commit/WuQinghui-00/ESP32-Light-Sensor-Monitor)
![GitHub code size in bytes](https://img.shields.io/github/languages/code-size/WuQinghui-00/ESP32-Light-Sensor-Monitor)

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
