/***************************************************************************
本程序可供学习使用，经作者允许，也可用于其他用途。如有意见及建议，欢迎来电交流指教
硬件版本：四路麦轮多功能控制板
主控芯片：STM32F103RCT6
程序名称：AB相编码器实验--主程序
功能描述：解码4路AB相编码器

*****************************************************************************/



#include "sys.h"
#include "delay.h"
#include "encoder.h"
#include "oled_user.h"

int main(void)
{		
  Stm32_Clock_Init(9);				//系统时钟设置
	JTAG_Set(JTAG_SWD_DISABLE); //先关闭JATG SWD端口。4路编码器中，需要占用JTAG端口中的部分引脚
	AFIO->MAPR|=0x1<<8;					//timer2部分重映射，CH1---PA15,CH2---PB3（解码编码器的4个timer中有共用的引脚，因此要重映射）
	JTAG_Set(SWD_ENABLE); 			//只开启SWD端口。（必须开启，不然断电后，无法调试或下载程序。此时设置BOOT0为高电平才可以）
	delay_init(72);							//延时初始化
  Encoder_Init();							//初始化编码器
 	
	OLED_Init();
	
	OLED_Put12x12CNstr(10,0,"AB相编码器解码",LIGHT);	//显示标题
	OLED_Put12x12CNstr(0,16,"4路:",LIGHT);	//显示第4路的编码器		
	OLED_Put12x12CNstr(0,32,"1路:",LIGHT);	//显示第1路的编码器	
	OLED_Put12x12CNstr(66,16,"3路:",LIGHT);	//显示第3路的编码器	
	OLED_Put12x12CNstr(66,32,"2路:",LIGHT);	//显示第2路的编码器	
	
	while(1)
	{
		Read_Encoder();//读编码器
		
		OLED_Put6x12Num(32,16,Encoder[3],5,LIGHT);	//显示第4路的编码器数值	
		OLED_Put6x12Num(32,32,Encoder[0],5,LIGHT);  //显示第1路的编码器数值	
		OLED_Put6x12Num(98,16,Encoder[2],5,LIGHT);	//显示第3路的编码器数值	
		OLED_Put6x12Num(98,32,Encoder[1],5,LIGHT);	//显示第2路的编码器数值			
		OLED_Refresh_AllGDRAM(); 	//刷新显示
		delay_ms(20);
		
	} 
}





/*************************************END************************************/







