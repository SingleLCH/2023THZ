/***************************************************************************
本程序可供学习使用，经作者允许，也可用于其他用途。如有意见及建议，欢迎来电交流指教
硬件版本：四路麦轮多功能控制板
主控芯片：STM32F103RCT6
程序名称：STM32麦轮全向移动小车程序--数据信息交换及处理程序
功能描述：片内FLASH作为EEPROM掉电保存数据，存储和读取参数


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


extern unsigned char Blue_dat[UART_RECV_LEN];		//串口3接收缓存

extern short int Blue_RC[3];

extern int LeftKnob,RightKnob;	//左右轮旋转脉冲计数

extern unsigned int time_tick;				//Time计数器
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


void Flash_Save_PID(void);//写入PID值
void Flash_Read_PID(void);//读取Flash里的的PID值

void Flash_Save_Language(void);//写入语言
void Flash_Read_Language(void);//读取语言

void Flash_Save_VoltWarn(void);//写入低电压报警值
void Flash_Read_VoltWarn(void);//读取低电压报警值
void Flash_Save_MotorDrctAdj(void);//写入电机转向调整值
void Flash_Read_MotorDrctAdj(void);//读取电机转向调整值
void Flash_Save_Delay(void);
#endif


/*************************************END************************************/

