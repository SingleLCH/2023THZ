/***************************************************************************
本程序可供学习使用，经作者允许，也可用于其他用途。如有意见及建议，欢迎来电交流指教
硬件版本：四路麦轮多功能控制板
主控芯片：STM32F103RCT6
程序名称：STM32麦轮全向移动小车程序--LED程序
功能描述：通过定时器，控制LED以不同方式、不同频率闪烁



*****************************************************************************/


#ifndef __LED_H
#define __LED_H	 
#include "sys.h"

#define LED_ON 		0
#define LED_OFF 	1

#define	LED_SLOWFLASH			1
#define	LED_SHORTFLASH		2
#define	LED_FASTFLASH			3

#define LED0 	PDout(2)	

extern unsigned char LED_state;//定义led状态

extern unsigned short int LED_timetick;

void LED_Init(void);//配置LED到IO口
void LED_Control(void);
void Led_Flash1(void); //LED 2次快闪
void Led_Flash2(void); //LED 慢闪
void Led_Flash3(void); //LED 频闪

#endif



/*************************************END************************************/

