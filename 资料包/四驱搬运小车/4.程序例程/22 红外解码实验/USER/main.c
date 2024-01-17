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
#include "ir.h"

u8 Ir_key=0;
void OLED_Dis_Hex(u8 x,u8 y,u32 Hex_Data);

int main(void)
{		
  Stm32_Clock_Init(9);				//系统时钟设置
	delay_init(72);							//延时初始化
	OLED_Init();								//OLED初始化 
	OLED_Put12x12CNstr(20,0,"红外解码实验",LIGHT);	//显示标题
	OLED_Put12x12CNstr(0,15,"Receive:",LIGHT);		//显示标题
	OLED_Refresh_AllGDRAM(); 											//刷新显示
	IR_Init();																		//红外解码端口初始化

	while(1)
	{
		if(IR_Sta==2)
		{
			OLED_Dis_Hex( 10,28,IR_Rec);	//显示十六进制数据
			OLED_Put6x12Num( 80,28,Repeat,5,LIGHT);
			OLED_Put5x7Num(  0,41,times/100,5,LIGHT);
			times=0;
			OLED_Refresh_AllGDRAM(); 		//刷新显示
			IR_Sta=0;
		}
		delay_ms(5);
	} 
}
u8 Hex_to_char(u8 hex)
{
	u8 cha_tmp;
	switch(hex)
	{
		case 0xA:cha_tmp='A';break;
		case 0xB:cha_tmp='B';break;
		case 0xC:cha_tmp='C';break;
		case 0xD:cha_tmp='D';break;
		case 0xE:cha_tmp='E';break;
		case 0xF:cha_tmp='F';break;
		default:
			if(hex<0xA)	cha_tmp=hex+48;
			break;
	}
	return cha_tmp;
}

void OLED_Dis_Hex(u8 x,u8 y,u32 Hex_Data)
{
	u8 cha[8],n;
	cha[0]=(Hex_Data>>28)&0x0F;
	cha[1]=(Hex_Data>>24)&0x0F;
	cha[2]=(Hex_Data>>20)&0x0F;
	cha[3]=(Hex_Data>>16)&0x0F;
	cha[4]=(Hex_Data>>12)&0x0F;
	cha[5]=(Hex_Data>>8)&0x0F;
	cha[6]=(Hex_Data>>4)&0x0F;
	cha[7]=(Hex_Data>>0)&0x0F;
	OLED_Put12x12CNstr(x,y,"0x",LIGHT);
	for(n=0;n<8;n++)
		OLED_Put6x12Char(x+12+n*6,y,Hex_to_char(cha[n]),LIGHT);
	
}




/*************************************END************************************/







