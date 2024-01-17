/***************************************************************************
本程序可供学习使用，经作者允许，也可用于其他用途。如有意见及建议，欢迎来电交流指教
硬件版本：四路麦轮多功能控制板
主控芯片：STM32F103RCT6
程序名称：STM32麦轮全向移动小车程序--蜂鸣器程序
功能描述：通过定时器，控制蜂鸣器以不同方式、不同间隔时间鸣叫


*****************************************************************************/



#ifndef __BEEP_H
#define __BEEP_H	 
#include "sys.h"

#define BEEP_ON 	1
#define BEEP_OFF 	0

#define	BEEP_NOP						0
#define	BEEP_SLOW1FLASH			1
#define	BEEP_SHORT2FLASH		2
#define	BEEP_FAST1FLASH			3
#define	BEEP_FASTFLASH			4
#define	BEEP_SHORT1FLASH		5

//蜂鸣器端口定义
#define BEEP PBout(12)	// BEEP,蜂鸣器接口		   

extern unsigned char BEEP_state;//定义led状态
extern unsigned short int BEEP_timetick;

void BEEP_Init(void);	//初始化		

void BEEP_Control(void);

void BEEP_ShortRing(void);

void BEEP_Flash1(void); //LED 1次慢闪
void BEEP_Flash2(void); //LED 2次快闪
void BEEP_Flash3(void); //LED 频闪
void BEEP_Flash4(void); //LED 频闪
void BEEP_Flash5(void); //蜂鸣器短响一声

#endif


/*************************************END************************************/

