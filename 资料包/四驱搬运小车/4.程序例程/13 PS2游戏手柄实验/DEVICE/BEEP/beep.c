/***************************************************************************
本程序可供学习使用，经作者允许，也可用于其他用途。如有意见及建议，欢迎来电交流指教
硬件版本：FreeLink STM32四路直流电机控制板V2.41
主控芯片：STM32F103RCT6
程序名称：STM32麦轮全向移动小车程序--蜂鸣器程序
功能描述：通过定时器，控制蜂鸣器以不同方式、不同间隔时间鸣叫
设计作者：水木蓝青@FreeLink
联系电话：15802781535
联系邮箱：357704597@qq.com
修改时间：2019/09/16
程序版本：V2.1
Copyright(C) 武汉飞令机器人科技有限公司 
All rights reserved
*****************************************************************************
修改说明：
无


*****************************************************************************/



#include "beep.h"

unsigned char BEEP_state;//定义led状态
unsigned short int BEEP_timetick;

//初始化PB8为输出口.并使能这个口的时钟		    
//蜂鸣器初始化
void BEEP_Init(void)
{
	RCC->APB2ENR|=1<<0;			//使能辅助时钟
	RCC->APB2ENR|=1<<3;    	//使能PORTB时钟	   	  
	GPIOB->CRH&=0XFFF0FFFF; 
	GPIOB->CRH|=0X00030000;	//PB12 推挽输出    
	BEEP=BEEP_OFF;					//关闭蜂鸣器输出
}

void BEEP_ShortRing(void)
{
	BEEP_timetick=0;BEEP_state=BEEP_SHORT1FLASH;
}

void BEEP_Control(void)
{
	switch(BEEP_state)
	{
		case BEEP_SLOW1FLASH:		BEEP_Flash1();break;
		case BEEP_SHORT2FLASH:	BEEP_Flash2();break;
		case BEEP_FAST1FLASH:		BEEP_Flash3();break;
		case BEEP_FASTFLASH:		BEEP_Flash4();break;
		case BEEP_SHORT1FLASH:		BEEP_Flash5();break;
		default:break;
	}
	if(BEEP_timetick>0) BEEP_timetick++;
	if(BEEP_timetick>=100)
	{
		BEEP_timetick = 0;//2s一个周期 	
	}	
	
}

void BEEP_Flash1(void) //LED 1次慢闪
{
	if(BEEP_timetick == 1)
	{
		BEEP=BEEP_ON;
	}
	else if(BEEP_timetick == 25)
	{
		BEEP=BEEP_OFF;BEEP_timetick=0;BEEP_state=BEEP_NOP;
	}
}

void BEEP_Flash2(void) //LED 2次快闪
{
	if(BEEP_timetick == 40)
	{
		BEEP=BEEP_ON;
	}
	else if(BEEP_timetick == 50)
	{
		BEEP=BEEP_OFF;
	}
	else if(BEEP_timetick == 60)
	{
		BEEP=BEEP_ON;
	}
	else if(BEEP_timetick == 70)
	{
		BEEP=BEEP_OFF;BEEP_timetick=0;BEEP_state=BEEP_NOP;
	}
}

void BEEP_Flash3(void) //LED 1次慢闪
{
	if(BEEP_timetick == 1)
	{
		BEEP=BEEP_ON;
	}
	else if(BEEP_timetick == 5)
	{
		BEEP=BEEP_OFF;BEEP_timetick=0;BEEP_state=BEEP_NOP;
	}
}

void BEEP_Flash4(void) //LED 频闪
{
	if(BEEP_timetick%10 == 1 )
	{
		BEEP=!BEEP;
	}
}
void BEEP_Flash5(void) //蜂鸣器短响一声
{
	if(BEEP_timetick == 10)
	{
		BEEP=BEEP_OFF;
	}
}


/*************************************END************************************/

