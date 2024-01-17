/***************************************************************************
������ɹ�ѧϰʹ�ã�����������Ҳ������������;��������������飬��ӭ���罻��ָ��
Ӳ���汾��FreeLink STM32��·ֱ��������ư�V2.41
����оƬ��STM32F103RCT6
�������ƣ�STM32����ȫ���ƶ�С������--����������
����������ͨ����ʱ�������Ʒ������Բ�ͬ��ʽ����ͬ���ʱ������
������ߣ�ˮľ����@FreeLink
��ϵ�绰��15802781535
��ϵ���䣺357704597@qq.com
�޸�ʱ�䣺2019/09/16
����汾��V2.1
Copyright(C) �人��������˿Ƽ����޹�˾ 
All rights reserved
*****************************************************************************
�޸�˵����
��


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

//�������˿ڶ���
#define BEEP PBout(12)	// BEEP,�������ӿ�		   

extern unsigned char BEEP_state;//����led״̬
extern unsigned short int BEEP_timetick;

void BEEP_Init(void);	//��ʼ��		

void BEEP_Control(void);

void BEEP_ShortRing(void);

void BEEP_Flash1(void); //LED 1������
void BEEP_Flash2(void); //LED 2�ο���
void BEEP_Flash3(void); //LED Ƶ��
void BEEP_Flash4(void); //LED Ƶ��
void BEEP_Flash5(void); //����������һ��

#endif


/*************************************END************************************/

