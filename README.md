# 2023嵌入式比赛(stm32F767)
## 基于云端的太赫兹波成像
 项目简介：由于现在有些工业产品为一次性塑封，无法拆封观看内部情况。针对此，本项目通过采用调频连续波（FMCW）毫米波雷达技术，结合电机快速扫描成像和AI图像识别算法，实现了低功耗雷达在免拆封检测、瑕疵分析方面的应用，满足了相关工业的需求。本项目分为两个部分，即本地数据采集与云端成像。首先，通过STM32F7实现了电机的精确控制和雷达数据处理，并结合了板载GUI以提高扫描进程的操作性和可视性。同时，STM32F7内置了预扫描算法，可以有效提高点阵数据的采集效率。此外，本项目结合云端数据处理功能，将雷达所采集到的点阵数据通过主控STM32F7上的WiFi模块上传到云端MySQL数据库进行保存。之后通过部署在云端的python脚本，将这些点阵数据进行成像处理，并结合AI分析，得出相关结论。最后，通过自行研发的手机端app，在输入相关信息后，即可查看成像内容和相关结论。

---

## 电机
README: [go to this file](./电机/README.md)

**TODO**

- [ ] 数据采集
- [ ] 预扫描

## 显示
README: [go to this file](./显示/README.md)

**TODO**

- [ ] 对电机进行控制
- [ ] （optional）读取MySQL数据库，并将成像图片在oled上显示

## AI测试
README: [go to this file](./AI测试/README.md)

**TODO**

- [ ] 进行数据集采集
- [ ] 能对目标进行识别

## uni-app开发
README: [go to this file](./uni-app/README.md)

**TODO**

- [ ] 读取MySQL数据库并将图片呈现

## 数据传输
README: [go to this file](./数据传输/README.md)

**TODO**

- [ ] 通过uart读取到stm32发来的数据，并将其插入到MySQL数据库中
- [ ] （optional）读取MySQL数据库，并将成像图片在oled上显示

## 雷达
README: [go to this file](./雷达/README.md)

**TODO**

- [ ] TLVs数据格式纠正
- [ ] 代码函数封装
