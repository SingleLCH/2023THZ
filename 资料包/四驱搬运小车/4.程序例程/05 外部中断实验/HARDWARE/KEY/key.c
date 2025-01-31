/***************************************************************************
本程序可供学习使用，经作者允许，也可用于其他用途。如有意见及建议，欢迎来电交流指教
硬件版本：FreeLink STM32四路直流电机控制板V2.41
主控芯片：STM32F103RCT6
程序名称：STM32麦轮全向移动小车程序--独立按键程序
功能描述：独立按键程序
设计作者：水木蓝青@FreeLink
联系电话：15802781535
联系邮箱：357704597@qq.com
修改时间：2019/09/16
程序版本：V2.1
Copyright(C) 武汉飞令机器人科技有限公司 
All rights reserved
*****************************************************************************
修改说明：
无


*****************************************************************************/


#include "key.h"
						    
//按键初始化函数
void KEY_Init(void)
{
	RCC->APB2ENR|=1<<0;			//使能辅助时钟
	RCC->APB2ENR|=1<<4;    //使能PORTC时钟	   	 
	GPIOC->CRL&=0XFFFFFFF0; 
	GPIOC->CRL|=0X00000008;//PC0 输入   	 
  GPIOC->ODR|=1<<0;      //PC0 上拉 
} 


/*************************************END************************************/
