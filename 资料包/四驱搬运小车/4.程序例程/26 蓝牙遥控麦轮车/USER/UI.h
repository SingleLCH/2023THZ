/***************************************************************************
������ɹ�ѧϰʹ�ã�����������Ҳ������������;��������������飬��ӭ���罻��ָ��
Ӳ���汾��FreeLink STM32��·ֱ��������ư�V2.41
����оƬ��STM32F103RCT6
�������ƣ�STM32����ȫ���ƶ�С������--UI�������
����������UI�л����棬���������˻���������
������ߣ�ˮľ����@FreeLink
��ϵ�绰��15802781535
��ϵ���䣺357704597@qq.com
�޸�ʱ�䣺2019/09/16
����汾��V2.1
��Ȩ���У�����ؾ�
Copyright(C) �人��������˿Ƽ����޹�˾ 
All rights reserved
*****************************************************************************
�޸�˵����
��


*****************************************************************************/


#ifndef _UI_H_
#define	_UI_H_

#include "sys.h"
#include "usart.h"		
#include "delay.h"	
#include "led.h" 
#include "beep.h"	
#include "oled_user.h"
#include "key.h"	 	  	 
#include "timer.h"		 	 	 	  	  
#include "string.h"
#include "motor.h"
#include "encoder.h"	 
#include "control.h"
#include "DMA.h"
#include "ADC.h"
#include "data_transfer.h"
#include "stmflash.h"



extern unsigned short int Keytimes;					//��������ʱ�����
extern unsigned char Kaysta;								//����״̬����
extern unsigned char WorkMode;							//����ģʽ����
extern unsigned int Volt;										//��ص�ѹ��������λ0.1V
extern char Menux;													//�˵�����
extern unsigned short int AdjustAngle_Flag;	//�Ƕ�У׼��־λ
extern short int Lang_x;										//��������
extern unsigned char UI_Display_Flag;				//��ǽ���д��һ��
extern unsigned char BlueCK;								//�����źż��
extern short int	VoltLowWarn;							//�͵�ѹ����ֵ
extern unsigned short int VoltLowTimes;					//��ѹ�ͼ�����

extern unsigned char Save_PID_Flag;

void Standby_Mode(void);				//����ģʽ
void Remote_Mode(void);				//ƽ��ģʽ
void Settings_Mode(void);				//����ģʽ
void PID_Settings(void);				//PID����ģʽ
void Display_PID_data(void);

void Motor_Settings(void);		//
void Display_SafeAngle_data(void);
void Display_SaveMasBox(void);		//��Ϣ��

void AdjustAngle_Settings(void);

void Display_LanguageCursor(char Menuxx);
void LanguageSettings_Mode(void);
void FactorySettings_Mode(void);
void Other_Settings_Mode(void);
void VoltWarn_Mode(void);
#endif


/*************************************END************************************/

