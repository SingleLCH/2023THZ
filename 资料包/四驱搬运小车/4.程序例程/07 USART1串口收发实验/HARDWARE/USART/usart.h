/***************************************************************************
本程序可供学习使用，经作者允许，也可用于其他用途。如有意见及建议，欢迎来电交流指教
硬件说明：四路麦轮多功能控制板
主控芯片：STM32F103RCT6
程序名称：USART1收发程序
功能描述：USART1相关程序
设计作者：水木蓝青@FreeLink



*****************************************************************************/

#ifndef __USART_H
#define __USART_H
#include "sys.h"
#include "stdio.h"	 

//串口发送
void USART1_SendChar(u8 Data);
void USART1_SendString(u8 *str);
void USART1_Init(unsigned int pclk2,unsigned int bound);

#endif	   





/*************************************END************************************/
