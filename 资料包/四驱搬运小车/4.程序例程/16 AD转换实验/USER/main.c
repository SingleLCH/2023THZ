/***************************************************************************
本程序可供学习使用，经作者允许，也可用于其他用途。如有意见及建议，欢迎来电交流指教
硬件版本：四路麦轮多功能控制板
主控芯片：STM32F103RCT6
程序名称：AD转换实验--主程序
功能描述：AD转换检测电池电压



*****************************************************************************/



#include "sys.h"
#include "delay.h"
#include "adc.h"
#include "oled_user.h"

unsigned int AD_Val=0;	//AD值
unsigned int Volt=0;		//电池电压变量，单位0.1V

int main(void)
{		
	Stm32_Clock_Init(9);				//系统时钟设置
	JTAG_Set(JTAG_SWD_DISABLE); //先关闭JATG SWD端口。4路编码器中，需要占用JTAG端口中的部分引脚
	AFIO->MAPR|=0x1<<8;					//timer2部分重映射，CH1---PA15,CH2---PB3（解码编码器的4个timer中有共用的引脚，因此要重映射）
	JTAG_Set(SWD_ENABLE); 			//只开启SWD端口。（必须开启，不然断电后，无法调试或下载程序。此时设置BOOT0为高电平才可以）
	delay_init(72);							//延时初始化
	Adc_Init();				 					//初始化ADC，检测电池电压	
	OLED_Init();								//OLED初始化
	OLED_Put12x12CNstr(40,0,"AD 转换",LIGHT);	//显示标题
	OLED_Put12x12CNstr(10,20,"AD  值:",LIGHT);				//显示“AD值”
	OLED_Put12x12CNstr(10,40,"电压值:",LIGHT);				//显示“电压值”
	OLED_Put12x12CNstr(90,40,"V",LIGHT);						//显示单位
	while(1)
	{
		AD_Val=Get_Adc(4);											//AD转换
		Volt=AD_Val*33*11/4096;									//计算电压，电池的电压，用1K和10K电阻串联进行分压，Volt=( (3.3V/4096)*(1K+10K)/1K )*AD_Val
		
		OLED_Put6x12Num(60,20,AD_Val,4,LIGHT);	//显示AD值
		OLED_Put6x12Num1dot(60,40,Volt,LIGHT);	//显示电池电压，带小数点显示，单位V
		OLED_Refresh_AllGDRAM(); 								//刷新显示	
		delay_ms(10);														//延时
	} 
}





/*************************************END************************************/







