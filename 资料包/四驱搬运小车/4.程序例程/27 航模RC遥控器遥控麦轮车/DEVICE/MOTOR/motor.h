/***************************************************************************
本程序可供学习使用，经作者允许，也可用于其他用途。如有意见及建议，欢迎来电交流指教
硬件版本：四路麦轮多功能控制板
主控芯片：STM32F103RCT6
程序名称：STM32麦轮全向移动小车程序--直流电机驱动程序
功能描述：PWM输出控制四路直流减速电机旋转，及旋转方向。


*****************************************************************************/


#ifndef __MOTOR_H__
#define __MOTOR_H__

#include "sys.h"



#define MotorA1	PCout(5)
#define MotorA2	PCout(4)

#define MotorB1	PCout(3)
#define MotorB2	PCout(2)

#define MotorC1	PBout(5)
#define MotorC2	PBout(4)

#define MotorD1	PCout(8)
#define MotorD2	PCout(9)

void Motor_Init(void);//电机PWM初始化并设置电机的刷新速率50-499
void Set_Motor(int MotorA,int MotorB,int MotorC,int MotorD);

#endif

/*************************************END************************************/



