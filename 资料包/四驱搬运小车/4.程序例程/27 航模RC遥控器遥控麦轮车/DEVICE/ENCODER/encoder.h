/***************************************************************************
本程序可供学习使用，经作者允许，也可用于其他用途。如有意见及建议，欢迎来电交流指教
硬件版本：四路麦轮多功能控制板
主控芯片：STM32F103RCT6
程序名称：STM32麦轮全向移动小车程序--编码器程序
功能描述：四路AB相编码器的正交解码程序。电机转动时，可记录编码器脉冲数及电机转向。


*****************************************************************************/



#ifndef __ENCODER_H
#define __ENCODER_H
#include <sys.h>	 

extern short int Encoder[4];
extern short int Encoder_Sum[4];
void Encoder_Init(void);
void Encoder_Init_TIM8(void);
void Encoder_Init_TIM2(void);
void Encoder_Init_TIM3(void);
void Encoder_Init_TIM4(void);
void Read_Encoder(void);

#endif


/*************************************END************************************/

