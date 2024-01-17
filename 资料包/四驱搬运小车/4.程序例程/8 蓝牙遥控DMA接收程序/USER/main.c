/***************************************************************************
本程序可供学习使用，经作者允许，也可用于其他用途。如有意见及建议，欢迎来电交流指教
硬件说明：四路麦轮多功能控制板
主控芯片：STM32F103RCT6
程序名称：蓝牙遥控DMA接收--主程序
功能描述：DMA接收并解析手机蓝牙APP


*****************************************************************************/



#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "dma.h"
#include "oled_user.h"
#include "bluetooth.h"

int main(void)
{		
  Stm32_Clock_Init(9);		//系统时钟设置
	delay_init(72);					//延时初始化
	MYDMA_Config(DMA1_Channel3,(u32)&USART3->DR,(u32)Blue_dat,UART_RECV_LEN);//DMA1通道4,外设为串口1,存储器为SendBuff,长度200.	
	USART3_Init(36,9600);		//初始化蓝牙串口通信接口
		
	OLED_Init();						//初始化OLED		
	
	OLED_Put12x12CNstr(30,0,"蓝牙遥控",LIGHT);	//显示标题
	OLED_Put12x12CNstr(0,16,"左右:",LIGHT);		//显示左手摇杆“左右方向摇杆”		
	OLED_Put12x12CNstr(0,32,"前后:",LIGHT);		//显示左手摇杆“上下方向摇杆”			

	OLED_Put12x12CNstr(66,16,"左右:",LIGHT);	//显示右手摇杆“左右方向摇杆”
	OLED_Put12x12CNstr(66,32,"前后:",LIGHT);	//显示右手摇杆“上下方向摇杆”
	
	while(1)
	{
		BlueTooth_Receive();	//解析蓝牙遥控数据	
		
		if(BlueCK==1)	OLED_Put12x12CNstr(10,52,"蓝牙已连接",LIGHT);			//判断蓝牙连接，显示“已连接”
		else	OLED_Put12x12CNstr(10,52,"蓝牙已断开",LIGHT);							//显示“已断开”
		
		OLED_Put6x12Num(32,16,Blue_RC[2],5,LIGHT);	//显示左手摇杆“左右方向摇杆”的摇杆数据
		OLED_Put6x12Num(32,32,Blue_RC[3],5,LIGHT);  //显示左手摇杆“上下方向摇杆”的摇杆数据

		OLED_Put6x12Num(98,16,Blue_RC[1],5,LIGHT);	//显示右手摇杆“左右方向摇杆”的摇杆数据
		OLED_Put6x12Num(98,32,Blue_RC[0],5,LIGHT);	//显示右手摇杆“上下方向摇杆”的摇杆数据
		
		OLED_Refresh_AllGDRAM(); 		//刷新OLED屏幕的显示
		delay_ms(5);								//延时
			
	} 
}
/*************************************END************************************/







