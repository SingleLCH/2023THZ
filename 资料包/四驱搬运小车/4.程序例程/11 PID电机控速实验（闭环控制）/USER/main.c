/***************************************************************************
本程序可供学习使用，经作者允许，也可用于其他用途。如有意见及建议，欢迎来电交流指教
硬件版本：四路麦轮多功能控制板
主控芯片：STM32F103RCT6
程序名称：PID电机控速（闭环控制）--主程序
功能描述：通过PID闭环控制电机转速


*****************************************************************************/



#include "sys.h"
#include "delay.h"
#include "motor.h"
#include "encoder.h"
#include "timer.h"
#include "oled_user.h"
#include "control.h"

int main(void)
{		
  Stm32_Clock_Init(9);				//系统时钟设置
	JTAG_Set(JTAG_SWD_DISABLE); //先关闭JATG SWD端口。4路编码器中，需要占用JTAG端口中的部分引脚
	AFIO->MAPR|=0x1<<8;					//timer2部分重映射，CH1---PA15,CH2---PB3（解码编码器的4个timer中有共用的引脚，因此要重映射）
	JTAG_Set(SWD_ENABLE); 			//只开启SWD端口。（必须开启，不然断电后，无法调试或下载程序。此时设置BOOT0为高电平才可以）
	delay_init(72);							//延时初始化
	Motor_Init();								//PWM输出初始化	
  Encoder_Init();							//初始化编码器
 	Set_Motor(0,0,0,0);					//电机PWM输出
	PID_Init();									//PID初始化
	TIM6_Int_Init(5000,72);	 		//5ms中断溢出
	OLED_Init();								//OLED初始化
	
	OLED_Put12x12CNstr(25,0,"PID电机控速",LIGHT);	//显示标题
	OLED_Put12x12CNstr(0,15," MOTOR-4 ",DARK);		//显示第4路的电机
	OLED_Put12x12CNstr(0,40," MOTOR-1 ",DARK);		//显示第1路的电机
	OLED_Put12x12CNstr(66,15," MOTOR-3 ",DARK);		//显示第3路的电机
	OLED_Put12x12CNstr(66,40," MOTOR-2 ",DARK);		//显示第2路的电机

	Motor_Speed[0]=50;Motor_Speed[1]=-50;Motor_Speed[2]=100;Motor_Speed[3]=-100;	//给初始速度值
	
	OLED_Put5x7Num(32,28,Motor_Speed[3],5,LIGHT);		//显示第4路电机的设定速度数值	
	OLED_Put5x7Num(32,54,Motor_Speed[0],5,LIGHT); 	//显示第1路电机的设定速度数值	
	OLED_Put5x7Num(100,28,Motor_Speed[2],5,LIGHT);	//显示第3路电机的设定速度数值	
	OLED_Put5x7Num(100,54,Motor_Speed[1],5,LIGHT);	//显示第2路电机的设定速度数值	
	
	OLED_Refresh_AllGDRAM(); 											//刷新显示
	while(1)
	{
		OLED_Put5x7Num(0,28,Encoder[3],5,LIGHT);	//显示第4路电机的实时速度
		OLED_Put5x7Num(0,54,Encoder[0],5,LIGHT); 	//显示第1路电机的实时速度
		OLED_Put5x7Num(66,28,Encoder[2],5,LIGHT);	//显示第3路电机的实时速度
		OLED_Put5x7Num(66,54,Encoder[1],5,LIGHT);	//显示第2路电机的实时速度
		OLED_Refresh_AllGDRAM(); 	//刷新显示
		delay_ms(50);
	} 
}





/*************************************END************************************/







