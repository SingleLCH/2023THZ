/***************************************************************************
本程序可供学习使用，经作者允许，也可用于其他用途。如有意见及建议，欢迎来电交流指教
硬件版本：四路麦轮多功能控制板
主控芯片：STM32F103RCT6
程序名称：STM32麦轮全向移动小车程序--独立按键程序
功能描述：独立按键程序



*****************************************************************************/


#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"

#define KEY_DOWN	0
#define KEY_UP		1

#define KEY	 PCin(0)   	//PC0

void KEY_Init(void);//IO初始化
			    
#endif

/*************************************END************************************/

