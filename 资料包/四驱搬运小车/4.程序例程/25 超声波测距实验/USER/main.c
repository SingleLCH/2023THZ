/***************************************************************************
本程序可供学习使用，经作者允许，也可用于其他用途。如有意见及建议，欢迎来电交流指教
硬件版本：四路麦轮多功能控制板
主控芯片：STM32F103RCT6
程序名称：内部Flash作为EEPROM存储实验--主程序
功能描述：使用片内Flash作为EEPROM可断电存储数据


*****************************************************************************/



#include "sys.h"
#include "usart.h"		
#include "delay.h"	
#include "oled_user.h" 	  	 	 	 	 	  	  
#include "string.h"
#include "ULN.h"

int main(void)
{		
  Stm32_Clock_Init(9);				//系统时钟设置
	delay_init(72);							//延时初始化
	OLED_Init();								//OLED初始化 
	OLED_Put12x12CNstr(20,0,"超声波测距实验",LIGHT);	//显示标题
	OLED_Put12x12CNstr(0,20,"Right:       cm",LIGHT);		//显示标题
	OLED_Put12x12CNstr(0,40,"Left :       cm",LIGHT);		//显示标题
	OLED_Refresh_AllGDRAM(); 											//刷新显示
	Uln_init();								//超声波初始化

	while(1)
	{
		OLED_Put6x12Num(40,20,Distance1,5,1);			//显示电池电压
		OLED_Put6x12Num(40,40,Distance2,5,1);
		OLED_Refresh_AllGDRAM(); 	
		delay_ms(50);
		Uln_Trig();						//发送超声波
	} 
}


/*************************************END************************************/







