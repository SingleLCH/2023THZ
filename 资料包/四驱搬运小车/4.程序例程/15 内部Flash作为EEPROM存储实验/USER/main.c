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
#include "stmflash.h"

u16 DataSave_Buf[4]={1234,1528,2050,4587};	//要存储的数据
u16 DataRead_Buf[4]={0,0,0,0};							//读取的数据缓存

int main(void)
{		
  Stm32_Clock_Init(9);				//系统时钟设置
	delay_init(72);							//延时初始化
	OLED_Init();								//OLED初始化 
	OLED_Put12x12CNstr(20,0,"Flash存储实验",LIGHT);	//显示标题
	OLED_Refresh_AllGDRAM(); 											//刷新显示
	
	STMFLASH_Write(FLASH_SAVE_ADDR + 2 ,DataSave_Buf,4);	//存储数据
	OLED_Put12x12CNstr(0,15,"Data Save:",LIGHT);					//显示标题
	OLED_Put6x12Num(5,30,DataSave_Buf[0],5,LIGHT);				//显示第1个要存储的数据
	OLED_Put6x12Num(37,30,DataSave_Buf[1],5,LIGHT); 			//显示第2个要存储的数据
	OLED_Put6x12Num(69,30,DataSave_Buf[2],5,LIGHT);				//显示第3个要存储的数据
	OLED_Put6x12Num(101,30,DataSave_Buf[3],5,LIGHT);			//显示第4个要存储的数据
	
	STMFLASH_Read(FLASH_SAVE_ADDR + 2 ,DataRead_Buf,4);		//读取数据
	OLED_Put12x12CNstr(0,42,"Data Read:",LIGHT);					//显示标题
	OLED_Put6x12Num(5,54,DataRead_Buf[0],5,LIGHT);				//显示读取的第1个数据
	OLED_Put6x12Num(37,54,DataRead_Buf[1],5,LIGHT); 			//显示读取的第1个数据
	OLED_Put6x12Num(69,54,DataRead_Buf[2],5,LIGHT);				//显示读取的第1个数据
	OLED_Put6x12Num(101,54,DataRead_Buf[3],5,LIGHT);			//显示读取的第1个数据
	OLED_Refresh_AllGDRAM(); 		//刷新显示
	while(1)
	{
		
	} 
}


/*************************************END************************************/







