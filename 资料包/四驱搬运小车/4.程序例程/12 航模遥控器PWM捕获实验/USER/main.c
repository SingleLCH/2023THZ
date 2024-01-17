/***************************************************************************
本程序可供学习使用，经作者允许，也可用于其他用途。如有意见及建议，欢迎来电交流指教
硬件版本：四路麦轮多功能控制板
主控芯片：STM32F103RCT6
程序名称：STM32麦轮全向移动小车程序--主程序
功能描述：通过手机蓝牙APP、RC航模遥控器、PS2游戏手柄无线遥控麦克纳姆轮小车全向移动


*****************************************************************************/



#include "sys.h"
#include "usart.h"		
#include "delay.h"	
#include "oled_user.h" 	  	 		 	 	 	  	  
#include "string.h"
#include "RC.h"


int main(void)
{		
//	unsigned char displaystick=0;
  Stm32_Clock_Init(9);//系统时钟设置
	JTAG_Set(JTAG_SWD_DISABLE); //先关闭JATG SWD端口。4路编码器中，需要占用JTAG端口中的部分引脚
	AFIO->MAPR|=0x1<<8;					//timer2部分重映射，CH1---PA15,CH2---PB3（解码编码器的4个timer中有共用的引脚，因此要重映射）
	JTAG_Set(SWD_ENABLE); 			//只开启SWD端口。（必须开启，不然断电后，无法调试或下载程序。此时设置BOOT0为高电平才可以）
	delay_init(72);			//延时初始化
	RC_Init();
	OLED_Init();

	RC_CH[0]=1500;RC_CH[1]=1500;RC_CH[2]=1500;RC_CH[3]=1500;
	OLED_Put12x12CNstr(25,0,"PWM捕获实验",LIGHT);	//显示标题
	OLED_Put12x12CNstr( 0,15,"RC-CH4:",LIGHT);		//显示第4通道
	OLED_Put12x12CNstr( 0,40,"RC-CH1:",LIGHT);		//显示第1通道
	OLED_Put12x12CNstr(66,15,"RC-CH3:",LIGHT);		//显示第3通道
	OLED_Put12x12CNstr(66,40,"RC-CH2:",LIGHT);		//显示第2通道
	OLED_Refresh_AllGDRAM(); 
	while(1)
	{
		OLED_Put5x7Num(20,28,RC_CH[3],5,LIGHT);	//显示第4通道数据
		OLED_Put5x7Num(20,54,RC_CH[0],5,LIGHT); //显示第1通道数据
		OLED_Put5x7Num(86,28,RC_CH[2],5,LIGHT);	//显示第3通道数据
		OLED_Put5x7Num(86,54,RC_CH[1],5,LIGHT);	//显示第2通道数据
		OLED_Refresh_AllGDRAM(); 	//刷新显示
		delay_ms(50);	
	} 
}





/*************************************END************************************/







