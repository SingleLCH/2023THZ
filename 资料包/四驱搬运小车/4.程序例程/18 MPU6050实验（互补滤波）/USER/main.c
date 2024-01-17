/***************************************************************************
本程序可供学习使用，经作者允许，也可用于其他用途。如有意见及建议，欢迎来电交流指教
硬件版本：四路麦轮多功能控制板
主控芯片：STM32F103RCT6
程序名称：MPU6050实验--主程序
功能描述：MPU6050 DMP方式解析角度


*****************************************************************************/

#include "sys.h"
#include "delay.h"
#include "oled_user.h"
#include "imu_filter.h"
#include "timer.h"

int main(void)
{		
  Stm32_Clock_Init(9);//系统时钟设置
	delay_init(72);			//延时初始化
	
	OLED_Init();
	OLED_Put12x12CNstr(10,0,"MPU角度显示(Filter)",LIGHT);							//显示标题
	OLED_Put12x12CNstr(10,16,"Pitch角度:",LIGHT);											//显示Pitch的角度
	OLED_Put12x12CNstr(10,32,"Roll 角度:",LIGHT);											//显示Roll的角度
	OLED_Put12x12CNstr(16,52,"<飞令机器人科技>",LIGHT);	
	OLED_Refresh_AllGDRAM(); 	//刷新显示
 	MPU6050_initialize();	//6050DMP初始化
	TIM3_Int_Init(4999,71);
	
	while(1)
	{
		
		OLED_Put6x12Num1dot(80,16,Pitch*10,1);
		OLED_Put6x12Num1dot(80,32,Roll*10,1);		//显示角度MPU6050_accel[2] yaw_gyro_now
		OLED_Refresh_AllGDRAM(); 	//刷新显示
		delay_ms(10);
	} 
}

// void EXTI15_10_IRQHandler(void)
// {	
//  	IMU_Filter();           
// 	EXTI->PR=1<<14;  //清除LINE3上的中断标志位
// }

//定时器3中断服务程序	 
void TIM3_IRQHandler(void)
{ 		    		  			    
	if(TIM3->SR&0X0001)//溢出中断
	{	
		IMU_Filter();		
	}
	TIM3->SR&=~(1<<0);//清除中断标志位 	    
}


/*************************************END************************************/







