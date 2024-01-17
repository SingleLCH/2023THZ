/***************************************************************************
本程序可供学习使用，经作者允许，也可用于其他用途。如有意见及建议，欢迎来电交流指教
硬件版本：四路麦轮多功能控制板
主控芯片：STM32F103RCT6
程序名称：PS2游戏手柄实验--主程序
功能描述：接收PS2游戏手柄的遥控数据，并在OLED上显示


*****************************************************************************/


#include "sys.h"
#include "oled_user.h"
#include "pstwo.h"
#include "timer.h"

u16 Key_Data=0;

int main(void)
{		
  Stm32_Clock_Init(9);//系统时钟设置
	JTAG_Set(JTAG_SWD_DISABLE); //先关闭JATG SWD端口。4路编码器中，需要占用JTAG端口中的部分引脚
	AFIO->MAPR|=0x1<<8;					//timer2部分重映射，CH1---PA15,CH2---PB3（解码编码器的4个timer中有共用的引脚，因此要重映射）
	JTAG_Set(SWD_ENABLE); 			//只开启SWD端口。（必须开启，不然断电后，无法调试或下载程序。此时设置BOOT0为高电平才可以）
	delay_init(72);			//延时初始化
	PS2_Init();
	TIM6_Int_Init(5000,72);	  //5ms中断溢出
	OLED_Init();
	OLED_Refresh_AllGDRAM(); 	//刷新显示
	
	OLED_Put12x12CNstr(25,0,"PS2游戏手柄",LIGHT);	//显示标题

	while(1)
	{
// 		OLED_Put5x7Char(  0,15,(Key_Data&0x8000)==0x8000?'1':'0',LIGHT);
// 		OLED_Put5x7Char(  8,15,(Key_Data&0x4000)==0x4000?'1':'0',LIGHT);
// 		OLED_Put5x7Char( 16,15,(Key_Data&0x2000)==0x2000?'1':'0',LIGHT);
// 		OLED_Put5x7Char( 24,15,(Key_Data&0x1000)==0x1000?'1':'0',LIGHT);		
// 		OLED_Put5x7Char( 32,15,(Key_Data&0x0800)==0x0800?'1':'0',LIGHT);
// 		OLED_Put5x7Char( 40,15,(Key_Data&0x0400)==0x0400?'1':'0',LIGHT);
// 		OLED_Put5x7Char( 48,15,(Key_Data&0x0200)==0x0200?'1':'0',LIGHT);
// 		OLED_Put5x7Char( 56,15,(Key_Data&0x0100)==0x0100?'1':'0',LIGHT);
// 		
// 		OLED_Put5x7Char( 64,15,(Key_Data&0x0080)==0x0080?'1':'0',LIGHT);
// 		OLED_Put5x7Char( 72,15,(Key_Data&0x0040)==0x0040?'1':'0',LIGHT);
// 		OLED_Put5x7Char( 80,15,(Key_Data&0x0020)==0x0020?'1':'0',LIGHT);
// 		OLED_Put5x7Char( 88,15,(Key_Data&0x0010)==0x0010?'1':'0',LIGHT);
// 		OLED_Put5x7Char( 96,15,(Key_Data&0x0008)==0x0008?'1':'0',LIGHT);
// 		OLED_Put5x7Char(104,15,(Key_Data&0x0004)==0x0004?'1':'0',LIGHT);
// 		OLED_Put5x7Char(112,15,(Key_Data&0x0002)==0x0002?'1':'0',LIGHT);
// 		OLED_Put5x7Char(120,15,(Key_Data&0x0001)==0x0001?'1':'0',LIGHT);		
		
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////		
		OLED_Put5x7Char(  0, 0,((Key_Data<<7 )&0x8000)==0?'X':'-',LIGHT);	//7	
		OLED_Put5x7Char(  0,10,((Key_Data<<5 )&0x8000)==0?'X':'-',LIGHT);	//5
		
		OLED_Put5x7Char( 50,20,((Key_Data<<15)&0x8000)==0?'X':'-',LIGHT);  //15
		
		OLED_Put5x7Char( 10,30,((Key_Data<<11)&0x8000)==0?'X':'-',LIGHT);	//11	
		OLED_Put5x7Char(  0,40,((Key_Data<<8 )&0x8000)==0?'X':'-',LIGHT);	//8		
		OLED_Put5x7Char( 20,40,((Key_Data<<10)&0x8000)==0?'X':'-',LIGHT);  //10
		OLED_Put5x7Char( 10,50,((Key_Data<<9 )&0x8000)==0?'X':'-',LIGHT);  //9

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		OLED_Put5x7Char(120, 0,((Key_Data<<6 )&0x8000)==0?'X':'-',LIGHT);	//6	
		OLED_Put5x7Char(120,10,((Key_Data<<4 )&0x8000)==0?'X':'-',LIGHT);	//4
		
		OLED_Put5x7Char( 70,20,((Key_Data<<12)&0x8000)==0?'X':'-',LIGHT); 	//12
		
		OLED_Put5x7Char(110,30,((Key_Data<<3 )&0x8000)==0?'X':'-',LIGHT);	//3	
		OLED_Put5x7Char(120,40,((Key_Data<<2 )&0x8000)==0?'X':'-',LIGHT);	//2		
		OLED_Put5x7Char(100,40,((Key_Data<<0 )&0x8000)==0?'X':'-',LIGHT); 	//0
		OLED_Put5x7Char(110,50,((Key_Data<<1 )&0x8000)==0?'X':'-',LIGHT); 	//1
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////		
		OLED_Put5x7Num(40,40,PS2_CH[3],5,LIGHT);	//显示第4路电机的实时速度
		OLED_Put5x7Num(40,54,PS2_CH[2],5,LIGHT); 	//显示第1路电机的实时速度
		
		OLED_Put5x7Num(66,40,PS2_CH[0],5,LIGHT);	//显示第3路电机的实时速度
		OLED_Put5x7Num(66,54,PS2_CH[1],5,LIGHT);	//显示第2路电机的实时速度
		OLED_Refresh_AllGDRAM(); 	//刷新显示
		delay_ms(50);	
	} 
}
unsigned char PS2_Stick=0;
unsigned char PS2_Flag=0;

//定时器6中断服务程序	 
void TIM6_IRQHandler(void)
{ 		    		  			    
	if(TIM6->SR&0X0001)//溢出中断
	{			
		if(PS2_Stick<8) PS2_Stick++;
		if(PS2_Stick==4)	//每20ms
		{	
			if(!PS2_RedLight()) //判断手柄是否为红灯模式，是，指示灯LED点亮
				PS2_Flag=1;	//标记为红灯模式
			else
				PS2_Flag=0; //标记为红灯模式
		}
		if(PS2_Stick==8)//每40ms
		{	
			if(PS2_Flag)	//是红灯模式	
			{	
				Key_Data=PS2_DataKey();	 //手柄按键捕获处理
				PS2_CH[0]=	PS2_AnologData(PSS_RY);						
				PS2_CH[1]=	PS2_AnologData(PSS_RX);
				PS2_CH[2]=	PS2_AnologData(PSS_LX);
				PS2_CH[3]=	PS2_AnologData(PSS_LY);
			}

			PS2_Stick=0;
		}

	
	}
	TIM6->SR&=~(1<<0);//清除中断标志位 	    
}




/*************************************END************************************/







