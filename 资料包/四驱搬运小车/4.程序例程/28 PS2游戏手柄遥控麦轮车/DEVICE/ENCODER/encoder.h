/***************************************************************************
������ɹ�ѧϰʹ�ã�����������Ҳ������������;��������������飬��ӭ���罻��ָ��
Ӳ���汾����·���ֶ๦�ܿ��ư�
����оƬ��STM32F103RCT6
�������ƣ�STM32����ȫ���ƶ�С������--����������
������������·AB�������������������򡣵��ת��ʱ���ɼ�¼�����������������ת��


*****************************************************************************/



#ifndef __ENCODER_H
#define __ENCODER_H
#include <sys.h>	 

extern short int Encoder[4];
extern short int Encoder_Sum[4];
void Encoder_Init(void);
void Encoder_Init_TIM8(void);
void Encoder_Init_TIM2(void);
void Encoder_Init_TIM3(void);
void Encoder_Init_TIM4(void);
void Read_Encoder(void);

#endif


/*************************************END************************************/

