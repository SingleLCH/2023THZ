/***************************************************************************
������ɹ�ѧϰʹ�ã�����������Ҳ������������;��������������飬��ӭ���罻��ָ��
Ӳ���汾��FreeLink STM32��·ֱ��������ư�V2.41
����оƬ��STM32F103RCT6
�������ƣ�STM32����ȫ���ƶ�С������--IIC����
����������IIC����OLED��ʾ�������������㡣
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

#ifndef _I2C_H
#define _I2C_H

#include "sys.h"
#include "delay.h"

//IO����
//IO��������
#define SDA_IN()  {GPIOC->CRH&=0XFFFFF0FF;GPIOC->CRH|=0X00000800;}
#define SDA_OUT() {GPIOC->CRH&=0XFFFFF0FF;GPIOC->CRH|=0X00000300;}

#define IIC_SCL        PCout(11) 
#define IIC_SDA        PCout(10)

#define READ_SDA      PCin(10)

void IIC_Init(void);
void IIC_Start(void);
void IIC_Stop(void);
u8 IIC_Wait_Ack(void);
void IIC_Send_Byte(u8 txd);


#endif

/*************************************END************************************/



