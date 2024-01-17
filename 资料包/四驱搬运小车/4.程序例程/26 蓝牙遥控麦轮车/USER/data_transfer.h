/***************************************************************************
������ɹ�ѧϰʹ�ã�����������Ҳ������������;��������������飬��ӭ���罻��ָ��
Ӳ���汾����·���ֶ๦�ܿ��ư�
����оƬ��STM32F103RCT6
�������ƣ�STM32����ȫ���ƶ�С������--������Ϣ�������������
����������Ƭ��FLASH��ΪEEPROM���籣�����ݣ��洢�Ͷ�ȡ����


*****************************************************************************/



#ifndef _DATA_TRANSFER_H_
#define _DATA_TRANSFER_H_
#include "sys.h"
#include "control.h"
#include "led.h"
#include "usart.h"
#include "delay.h"
#include "bluetooth.h"

#define DATA_TRANSFER_USE_USART


extern unsigned char Blue_dat[UART_RECV_LEN];		//����3���ջ���

extern short int Blue_RC[3];

extern int LeftKnob,RightKnob;	//��������ת�������

extern unsigned int time_tick;				//Time������
extern unsigned char Control_Flag;	


void Data_Send_Attitude(void);	
void Data_Send_PID1(void);
void Send_Data(void);

#define GYRO_OFFSET_FLASHADR	0
#define ACC_OFFSET_FLASHADR		20
#define PID_FLASHADR					40
#define MOTORADJ_FLASHADR			80
#define LANGUAGE_FLASHADR			100
#define VOLTWARN_FLASHADR			110


void Flash_Save_PID(void);//д��PIDֵ
void Flash_Read_PID(void);//��ȡFlash��ĵ�PIDֵ

void Flash_Save_Language(void);//д������
void Flash_Read_Language(void);//��ȡ����

void Flash_Save_VoltWarn(void);//д��͵�ѹ����ֵ
void Flash_Read_VoltWarn(void);//��ȡ�͵�ѹ����ֵ
void Flash_Save_MotorDrctAdj(void);//д����ת�����ֵ
void Flash_Read_MotorDrctAdj(void);//��ȡ���ת�����ֵ
void Flash_Save_Delay(void);
#endif


/*************************************END************************************/

