/***************************************************************************
������ɹ�ѧϰʹ�ã�����������Ҳ������������;��������������飬��ӭ���罻��ָ��
Ӳ���汾��FreeLink STM32��·ֱ��������ư�V2.41
����оƬ��STM32F103RCT6
�������ƣ�STM32����ȫ���ƶ�С������--PS2��Ϸ�ֱ���������
��������������PS2��Ϸ�ֱ���ң����ֵ��
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


#ifndef __PSTWO_H
#define __PSTWO_H
#include "delay.h"
#include "sys.h"


#define DI   PBin(9)           //PB12  ����

#define DO_H PBout(15)=1        //����λ��
#define DO_L PBout(15)=0        //����λ��

#define CS_H PBout(8)=1       //CS����
#define CS_L PBout(8)=0       //CS����

#define CLK_H PBout(13)=1      //ʱ������
#define CLK_L PBout(13)=0      //ʱ������


//These are our button constants
#define PSB_SELECT      1
#define PSB_L3          2
#define PSB_R3          3
#define PSB_START       4
#define PSB_PAD_UP      5
#define PSB_PAD_RIGHT   6
#define PSB_PAD_DOWN    7
#define PSB_PAD_LEFT    8
#define PSB_L2         9
#define PSB_R2          10
#define PSB_L1          11
#define PSB_R1          12
#define PSB_GREEN       13
#define PSB_RED         14
#define PSB_BLUE        15
#define PSB_PINK        16
#define PSB_TRIANGLE    13
#define PSB_CIRCLE      14
#define PSB_CROSS       15
#define PSB_SQUARE      26

//#define WHAMMY_BAR		8

//These are stick values
#define PSS_RX 5                //��ҡ��X������
#define PSS_RY 6
#define PSS_LX 7
#define PSS_LY 8



extern short int PS2_CH[3];
extern u16 Handkey;

void PS2_Init(void);
u8 PS2_RedLight(void);//�ж��Ƿ�Ϊ���ģʽ
void PS2_ReadData(void);
void PS2_Cmd(u8 CMD);		  //
u8 PS2_DataKey(void);		  //��ֵ��ȡ
u8 PS2_AnologData(u8 button); //�õ�һ��ҡ�˵�ģ����
void PS2_ClearData(void);	  //������ݻ�����

#endif


/*************************************END************************************/



