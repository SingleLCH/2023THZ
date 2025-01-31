# 基于FMCW技术的云端免拆封成像系统
 项目简介：由于现在有些工业产品为一次性塑封，无法拆封观看内部情况。针对此，本项目通过采用调频连续波（FMCW）毫米波雷达技术，结合电机快速扫描成像和AI图像识别算法，实现了低功耗雷达在免拆封检测、瑕疵分析方面的应用，满足了相关工业的需求。本项目分为两个部分，即本地数据采集与云端成像。首先，通过STM32F7实现了电机的精确控制和雷达数据处理，并结合了板载GUI以提高扫描进程的操作性和可视性。同时，STM32F7内置了预扫描算法，可以有效提高点阵数据的采集效率。此外，本项目结合云端数据处理功能，将雷达所采集到的点阵数据通过主控STM32F7上的WiFi模块上传到云端MySQL数据库进行保存。之后通过部署在云端的python脚本，将这些点阵数据进行成像处理，并结合AI分析，得出相关结论。最后，通过自行研发的手机端app，在输入相关信息后，即可查看成像内容和相关结论。
---

## AI测试
README: [go to this file](./AI测试/README.md)

**TODO**

- [x] 进行第一轮数据集采集
- [x] 进行第二轮数据采集
- [x] 能对目标进行识别x
- [x] 进行脚本自动识别缺陷


## uni-app开发
README: [go to this file](./uni-app/README.md)

**TODO**

- [x] 读取MySQL数据库并将图片呈现
- [x] 界面优化，以及识别结果的呈现
- [x] 控制电机启动
- [x] 3d成像呈现


## 数据传输
### 数据传输格式要求  'b'+ int X + int Y + int Z
### 'a'-> 数据传输完成
README: [go to this file](./数据传输/README.md)

**TODO**
#### esp8266部分
- [x] 通过uart读取到stm32发来的数据，并将其插入到MySQL数据库中
- [x] 接收app信息，并启动电机
- [x] （optional）读取MySQL数据库，并将成像图片在oled上显示

#### 服务器部分
- [x] 监听数据端口，一旦发生变化便执行脚本
- [x] bug修改以及优化
- [x] app操控
### 数据传输部分 5.28 全部完成 


## 显示
README: [go to this file](./显示/README.md)


**TODO**

- [x] 对电机进行控制
- [x] （optional）读取MySQL数据库，并将成像图片在oled上显示
- [x]  lvgl的移植


## 电机
README: [go to this file](./电机/README.md)

**TODO**

- [x] 数据采集
- [x] 预扫描


## 雷达
README: [go to this file](./雷达/README.md)

**TODO**

- [x] TLVs数据格式纠正
- [x] 代码函数封装

## 货运小车

**TODO**

- [x] NFC芯片定位
- [x] openmv识别

## APP

**TODO**

- [x] 操控电机运行
- [ ] 3D结果展示
