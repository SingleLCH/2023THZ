/***************************************************************************
本程序可供学习使用，经作者允许，也可用于其他用途。如有意见及建议，欢迎来电交流指教
硬件版本：四路麦轮多功能控制板
主控芯片：STM32F103RCT6
程序名称：PWM输出驱动无刷电机实验--无刷电机驱动程序
功能描述：扩展PWM接口输出，驱动四路无刷电机


*****************************************************************************/


#ifndef __MOTOR_H__
#define __MOTOR_H__

#include "sys.h"

#define PWM_MAX 2499

#define PWM_DIR PAout(5)

void Brushless_Init(void);		
void Set_Brushless(int Brushless1,int Brushless2,int Brushless3,int Brushless4);

#endif

/*************************************END************************************/



