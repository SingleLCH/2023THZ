/***************************************************************************
������ɹ�ѧϰʹ�ã�����������Ҳ������������;��������������飬��ӭ���罻��ָ��
Ӳ���汾����·���ֶ๦�ܿ��ư�
����оƬ��STM32F103RCT6
�������ƣ�STM32����ȫ���ƶ�С������--LED����
����������ͨ����ʱ��������LED�Բ�ͬ��ʽ����ͬƵ����˸



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

extern unsigned char LED_state;//����led״̬

extern unsigned short int LED_timetick;

void LED_Init(void);//����LED��IO��
void LED_Control(void);
void Led_Flash1(void); //LED 2�ο���
void Led_Flash2(void); //LED ����
void Led_Flash3(void); //LED Ƶ��

#endif



/*************************************END************************************/

