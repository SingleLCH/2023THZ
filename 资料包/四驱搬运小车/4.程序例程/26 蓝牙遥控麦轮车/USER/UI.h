/***************************************************************************
本程序可供学习使用，经作者允许，也可用于其他用途。如有意见及建议，欢迎来电交流指教
硬件版本：FreeLink STM32四路直流电机控制板V2.41
主控芯片：STM32F103RCT6
程序名称：STM32麦轮全向移动小车程序--UI界面程序
功能描述：UI切换界面，调参数的人机交互界面
设计作者：水木蓝青@FreeLink
联系电话：15802781535
联系邮箱：357704597@qq.com
修改时间：2019/09/16
程序版本：V2.1
版权所有，盗版必究
Copyright(C) 武汉飞令机器人科技有限公司 
All rights reserved
*****************************************************************************
修改说明：
无


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



extern unsigned short int Keytimes;					//按键按下时间计数
extern unsigned char Kaysta;								//按键状态变量
extern unsigned char WorkMode;							//工作模式变量
extern unsigned int Volt;										//电池电压变量，单位0.1V
extern char Menux;													//菜单变量
extern unsigned short int AdjustAngle_Flag;	//角度校准标志位
extern short int Lang_x;										//语言种类
extern unsigned char UI_Display_Flag;				//标记界面写入一次
extern unsigned char BlueCK;								//蓝牙信号检测
extern short int	VoltLowWarn;							//低电压报警值
extern unsigned short int VoltLowTimes;					//电压低检测次数

extern unsigned char Save_PID_Flag;

void Standby_Mode(void);				//待机模式
void Remote_Mode(void);				//平衡模式
void Settings_Mode(void);				//设置模式
void PID_Settings(void);				//PID设置模式
void Display_PID_data(void);

void Motor_Settings(void);		//
void Display_SafeAngle_data(void);
void Display_SaveMasBox(void);		//信息框

void AdjustAngle_Settings(void);

void Display_LanguageCursor(char Menuxx);
void LanguageSettings_Mode(void);
void FactorySettings_Mode(void);
void Other_Settings_Mode(void);
void VoltWarn_Mode(void);
#endif


/*************************************END************************************/

