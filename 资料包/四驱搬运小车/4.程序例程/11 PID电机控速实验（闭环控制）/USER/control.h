/***************************************************************************
本程序可供学习使用，经作者允许，也可用于其他用途。如有意见及建议，欢迎来电交流指教
硬件版本：四路麦轮多功能控制板
主控芯片：STM32F103RCT6
程序名称：STM32麦轮全向移动小车程序--控制程序
功能描述：PID控制四路直流减速电机转动



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


//定义PID
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

extern int Motor_Speed[4];			//电机速度值

extern PID PID_Motor[4];
extern u8 ARMED;

void Car_Control(int X_Move,int Y_Move,int Yaw);
float Get_MxMi(float num,float max,float min);
void PID_Init(void);
#endif


/*************************************END************************************/

