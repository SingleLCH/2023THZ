/***************************************************************************
本程序可供学习使用，经作者允许，也可用于其他用途。如有意见及建议，欢迎来电交流指教
硬件版本：四路麦轮多功能控制板
主控芯片：STM32F103RCT6
程序名称：STM32麦轮全向移动小车程序--串口收发程序
功能描述：通过手机蓝牙APP、RC航模遥控器、PS2游戏手柄无线遥控麦克纳姆轮小车全向移动


*****************************************************************************/



#ifndef __USART_H
#define __USART_H
#include "sys.h"
#include "stdio.h"	 


void USART3_SendData(u8 Data);
void USART3_SendString(char *Str);
void USART3_Init(unsigned int pclk2,unsigned int bound);


#endif	   


/*************************************END************************************/
