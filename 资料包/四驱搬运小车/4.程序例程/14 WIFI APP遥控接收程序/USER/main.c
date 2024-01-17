/***************************************************************************
本程序可供学习使用，经作者允许，也可用于其他用途。如有意见及建议，欢迎来电交流指教
硬件版本：四路麦轮多功能控制板
主控芯片：STM32F103RCT6
程序名称：蓝牙遥控DMA接收--主程序
功能描述：DMA接收并解析手机蓝牙APP


*****************************************************************************/



#include "sys.h"
#include "usart.h"		
#include "delay.h"	
#include "oled_user.h" 	 	 	 	  	  
#include "string.h"
#include "DMA.h"
#include "esp8266.h"

	
int main(void)
{	
  Stm32_Clock_Init(9);//系统时钟设置
	JTAG_Set(JTAG_SWD_DISABLE); 
	AFIO->MAPR|=0x1<<8;	//部分重映射，CH1---PA15,CH2---PB3
	JTAG_Set(SWD_ENABLE); 
	delay_init(72);			//延时初始化
	OLED_Init();
	OLED_Draw_FreeLink();			//显示图标
	OLED_Refresh_AllGDRAM(); 	//刷新显示
	delay_ms(1000);
	USART3_Init(36,115200);		//初始化蓝牙串口通信接口
	MYDMA_Config(DMA1_Channel3,(u32)&USART3->DR,(u32)Blue_dat,UART_RECV_LEN);//DMA1通道4,外设为串口1,存储器为SendBuff,长度200.
	while(!init_server()) {delay_ms(200);}
 
	OLED_ClearBuffer(DARK);
	OLED_Put12x12CNstr(30,0,"WIFI遥控",LIGHT);	//显示标题
	OLED_Put12x12CNstr(0,16,"左右:",LIGHT);		//显示左手摇杆“左右方向摇杆”		
	OLED_Put12x12CNstr(0,32,"前后:",LIGHT);		//显示左手摇杆“上下方向摇杆”			

	OLED_Put12x12CNstr(66,16,"左右:",LIGHT);	//显示右手摇杆“左右方向摇杆”
	OLED_Put12x12CNstr(66,32,"前后:",LIGHT);	//显示右手摇杆“上下方向摇杆”
	
	while(1)
	{
		if(WIFI_CK>0) WIFI_CK--;
		recv_msg();	//解析WIFI遥控数据	
		
		if(WIFI_CK>10)	OLED_Put12x12CNstr(10,52,"WIFI已连接",LIGHT);			//判断蓝牙连接，显示“已连接”
		else	OLED_Put12x12CNstr(10,52,"WIFI已断开",LIGHT);							//显示“已断开”
		
		OLED_Put6x12Num(32,16,Blue_RC[2],5,LIGHT);	//显示左手摇杆“左右方向摇杆”的摇杆数据
		OLED_Put6x12Num(32,32,Blue_RC[3],5,LIGHT);  //显示左手摇杆“上下方向摇杆”的摇杆数据

		OLED_Put6x12Num(98,16,Blue_RC[1],5,LIGHT);	//显示右手摇杆“左右方向摇杆”的摇杆数据
		OLED_Put6x12Num(98,32,Blue_RC[0],5,LIGHT);	//显示右手摇杆“上下方向摇杆”的摇杆数据
		
		OLED_Refresh_AllGDRAM(); 		//刷新OLED屏幕的显示
		delay_ms(5);								//延时
			
	} 
}



/*************************************END************************************/







