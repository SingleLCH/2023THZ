/***************************************************************************
������ɹ�ѧϰʹ�ã�����������Ҳ������������;��������������飬��ӭ���罻��ָ��
Ӳ���汾����·���ֶ๦�ܿ��ư�
����оƬ��STM32F103RCT6
�������ƣ�STM32����ȫ���ƶ�С������--LED����
����������ͨ����ʱ��������LED�Բ�ͬ��ʽ����ͬƵ����˸


*****************************************************************************/

#include "led.h"

unsigned char LED_state;//����led״̬
unsigned short int LED_timetick;

void LED_Init(void)
{
	RCC->APB2ENR|=1<<0;			//ʹ�ܸ���ʱ��
	RCC->APB2ENR|=1<<5;    //ʹ��PORTDʱ��	   	 
	GPIOD->CRL&=0XFFFFF0FF; 
	GPIOD->CRL|=0X00000300;//PD2 �������   	 
  GPIOD->ODR|=1<<5;      //PD2 ����� 
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
		LED_timetick = 0;//1sһ������ 	
	}	
	
}

void Led_Flash1(void) //LED 1������
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

void Led_Flash2(void) //LED 2�ο���
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

void Led_Flash3(void) //LED Ƶ��
{
	if(LED_timetick%10 == 0 )
	{
		LED0=!LED0;
	}
}


/*************************************END************************************/

