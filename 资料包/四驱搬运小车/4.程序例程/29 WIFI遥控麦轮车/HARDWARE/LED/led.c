/***************************************************************************
本程序可供学习使用，经作者允许，也可用于其他用途。如有意见及建议，欢迎来电交流指教
硬件版本：四路麦轮多功能控制板
主控芯片：STM32F103RCT6
程序名称：STM32麦轮全向移动小车程序--LED程序
功能描述：通过定时器，控制LED以不同方式、不同频率闪烁


*****************************************************************************/

#include "led.h"

unsigned char LED_state;//定义led状态
unsigned short int LED_timetick;

void LED_Init(void)
{
	RCC->APB2ENR|=1<<0;			//使能辅助时钟
	RCC->APB2ENR|=1<<5;    //使能PORTD时钟	   	 
	GPIOD->CRL&=0XFFFFF0FF; 
	GPIOD->CRL|=0X00000300;//PD2 推挽输出   	 
  GPIOD->ODR|=1<<5;      //PD2 输出高 
}

void LED_Control(void)
{
	switch(LED_state)
	{
		case LED_SLOWFLASH:		Led_Flash1();break;
		case LED_SHORTFLASH:	Led_Flash2();break;
		case LED_FASTFLASH:		Led_Flash3();break;
		default:break;
	}
	LED_timetick++;
	if(LED_timetick>=200)
	{
		LED_timetick = 0;//1s一个周期 	
	}	
	
}

void Led_Flash1(void) //LED 1次慢闪
{
	if(LED_timetick == 0)
	{
		LED0=LED_ON;
	}
	else if(LED_timetick == 100)
	{
		LED0=LED_OFF;
	}
}

void Led_Flash2(void) //LED 2次快闪
{
	if(LED_timetick == 40)
	{
		LED0=LED_ON;
	}
	else if(LED_timetick == 50)
	{
		LED0=LED_OFF;
	}
	else if(LED_timetick == 60)
	{
		LED0=LED_ON;
	}
	else if(LED_timetick == 70)
	{
		LED0=LED_OFF;
	}
}

void Led_Flash3(void) //LED 频闪
{
	if(LED_timetick%10 == 0 )
	{
		LED0=!LED0;
	}
}


/*************************************END************************************/

