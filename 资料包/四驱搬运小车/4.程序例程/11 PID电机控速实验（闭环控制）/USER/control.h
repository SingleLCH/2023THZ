/***************************************************************************
������ɹ�ѧϰʹ�ã�����������Ҳ������������;��������������飬��ӭ���罻��ָ��
Ӳ���汾����·���ֶ๦�ܿ��ư�
����оƬ��STM32F103RCT6
�������ƣ�STM32����ȫ���ƶ�С������--���Ƴ���
����������PID������·ֱ�����ٵ��ת��



*****************************************************************************/


#ifndef __CONTROL_H
#define __CONTROL_H	 
#include "sys.h"
#include "usart.h"		
#include "delay.h"		  	  
#include "timer.h"		 	 	 	 
#include "string.h"	
#include "motor.h"
#include "encoder.h"	


//����PID
typedef struct
{
	float Pdat;
	float pout;
	
	float Idat;
	float IMAX;
	float iout;
	
	float Ddat;
	float dout;
	
	float OUT;
	
	int error;
	int Last_error;	
}PID;

extern int Motor_Speed[4];			//����ٶ�ֵ

extern PID PID_Motor[4];
extern u8 ARMED;

void Car_Control(int X_Move,int Y_Move,int Yaw);
float Get_MxMi(float num,float max,float min);
void PID_Init(void);
#endif


/*************************************END************************************/

