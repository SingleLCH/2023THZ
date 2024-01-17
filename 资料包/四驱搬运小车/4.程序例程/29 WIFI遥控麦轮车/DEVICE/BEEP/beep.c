/***************************************************************************
������ɹ�ѧϰʹ�ã�����������Ҳ������������;��������������飬��ӭ���罻��ָ��
Ӳ���汾����·���ֶ๦�ܿ��ư�
����оƬ��STM32F103RCT6
�������ƣ�STM32����ȫ���ƶ�С������--����������
����������ͨ����ʱ�������Ʒ������Բ�ͬ��ʽ����ͬ���ʱ������



*****************************************************************************/



#include "beep.h"

unsigned char BEEP_state;//����led״̬
unsigned short int BEEP_timetick;

//��ʼ��PB8Ϊ�����.��ʹ������ڵ�ʱ��		    
//��������ʼ��
void BEEP_Init(void)
{
	RCC->APB2ENR|=1<<0;			//ʹ�ܸ���ʱ��
	RCC->APB2ENR|=1<<3;    	//ʹ��PORTBʱ��	   	  
	GPIOB->CRH&=0XFFF0FFFF; 
	GPIOB->CRH|=0X00030000;	//PB12 �������    
	BEEP=BEEP_OFF;					//�رշ��������
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
		BEEP_timetick = 0;//2sһ������ 	
	}	
	
}

void BEEP_Flash1(void) //LED 1������
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

void BEEP_Flash2(void) //LED 2�ο���
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

void BEEP_Flash3(void) //LED 1������
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

void BEEP_Flash4(void) //LED Ƶ��
{
	if(BEEP_timetick%10 == 1 )
	{
		BEEP=!BEEP;
	}
}
void BEEP_Flash5(void) //����������һ��
{
	if(BEEP_timetick == 10)
	{
		BEEP=BEEP_OFF;
	}
}


/*************************************END************************************/

