/***************************************************************************
本程序可供学习使用，经作者允许，也可用于其他用途。如有意见及建议，欢迎来电交流指教
硬件版本：四路麦轮多功能控制板
主控芯片：STM32F103RCT6
程序名称：STM32麦轮全向移动小车程序--ADC程序
功能描述：ADC转换，经过计算得出电池电压。


*****************************************************************************/


#ifndef __ADC_H
#define __ADC_H	
#include "sys.h"
							  	   									   
void Adc_Init(void); 				//ADC通道初始化
u16  Get_Adc(u8 ch); 				//获得某个通道值 
 
#endif 




/*************************************END************************************/



