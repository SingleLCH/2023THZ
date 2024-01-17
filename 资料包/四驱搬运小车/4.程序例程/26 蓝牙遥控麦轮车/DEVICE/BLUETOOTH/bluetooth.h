/***************************************************************************
本程序可供学习使用，经作者允许，也可用于其他用途。如有意见及建议，欢迎来电交流指教
硬件版本：四路麦轮多功能控制板
主控芯片：STM32F103RCT6
程序名称：STM32麦轮全向移动小车程序--串口收发程序
功能描述：通过手机蓝牙APP、RC航模遥控器、PS2游戏手柄无线遥控麦克纳姆轮小车全向移动


*****************************************************************************/



#ifndef __BLUETOOTH_H
#define __BLUETOOTH_H
#include "dma.h"

#define UART_RECV_LEN			16		//DMA缓存区大小
#define BLUE_CMD_LEN			8			//蓝牙APP遥控数据包大小
	  
		
extern unsigned char BlueCK;				 					//蓝牙信号检测
extern unsigned char Blue_dat[UART_RECV_LEN];	//串口3接收缓存
extern short int Blue_RC[3];									//解析的遥控数值


void BlueTooth_Receive(void);

#endif	   


/*************************************END************************************/
