/***************************************************************************
本程序可供学习使用，经作者允许，也可用于其他用途。如有意见及建议，欢迎来电交流指教
硬件版本：FreeLink STM32四路直流电机控制板V2.41
主控芯片：STM32F103RCT6
程序名称：PWM输出驱动舵机实验--主程序
功能描述：扩展PWM接口输出，驱动四路舵机
设计作者：水木蓝青@FreeLink
联系电话：15802781535
联系邮箱：357704597@qq.com
修改时间：2019/09/16
程序版本：V2.1
版权所有，盗版必究
Copyright(C) 武汉飞令机器人科技有限公司 
All rights reserved
*****************************************************************************
修改说明：
无


*****************************************************************************/



#include "sys.h"
#include "delay.h"
#include "servo.h"
#include "oled_user.h"

int PWM_Val[4]={0,0,0,0}; //4路电机PWM值

int main(void)
{		
  Stm32_Clock_Init(9);				//系统时钟设置
	JTAG_Set(JTAG_SWD_DISABLE); //先关闭JATG SWD端口。4路编码器中，需要占用JTAG端口中的部分引脚
	AFIO->MAPR|=0x1<<8;					//timer2部分重映射，CH1---PA15,CH2---PB3（解码编码器的4个timer中有共用的引脚，因此要重映射）
	JTAG_Set(SWD_ENABLE); 			//只开启SWD端口。（必须开启，不然断电后，无法调试或下载程序。此时设置BOOT0为高电平才可以）
	delay_init(72);							//延时初始化
	Servo_Init();								//PWM输出初始化	 
 	Set_Servo(0,0,0,0);					//电机PWM输出
	
	OLED_Init();
	OLED_Put12x12CNstr(40,0,"舵机实验",LIGHT);	//显示标题
	OLED_Put12x12CNstr(0,16,"PWM1:",LIGHT);		//显示第4路的PWM	
	OLED_Put12x12CNstr(0,32,"PWM2:",LIGHT);		//显示第1路的PWM	
	OLED_Put12x12CNstr(66,16,"PWM3:",LIGHT);	//显示第3路的PWM
	OLED_Put12x12CNstr(66,32,"PWM4:",LIGHT);	//显示第2路的PWM	
	while(1)
	{	
		PWM_Val[0]=1000;		//第1路PWM输出脉宽
		PWM_Val[1]=1200;		//第2路PWM输出脉宽
		PWM_Val[2]=1500;		//第3路PWM输出脉宽
		PWM_Val[3]=2000;		//第4路PWM输出脉宽
		Set_Servo(PWM_Val[0],PWM_Val[1],PWM_Val[2],PWM_Val[3]);		//电机PWM输出
		OLED_Put6x12Num(32,16,PWM_Val[0],5,LIGHT);//显示第4路的PWM输出脉宽
		OLED_Put6x12Num(32,32,PWM_Val[1],5,LIGHT);//显示第1路的PWM输出脉宽
		OLED_Put6x12Num(98,16,PWM_Val[2],5,LIGHT);//显示第3路的PWM输出脉宽
		OLED_Put6x12Num(98,32,PWM_Val[3],5,LIGHT);//显示第2路的PWM输出脉宽
		OLED_Refresh_AllGDRAM(); 	//刷新显示
		delay_ms(500);
		
		PWM_Val[0]=1500;		//第1路PWM输出脉宽
		PWM_Val[1]=1800;		//第2路PWM输出脉宽
		PWM_Val[2]=1000;		//第3路PWM输出脉宽
		PWM_Val[3]=1200;		//第4路PWM输出脉宽
		Set_Servo(PWM_Val[0],PWM_Val[1],PWM_Val[2],PWM_Val[3]);//电机PWM输出	
		OLED_Put6x12Num(32,16,PWM_Val[0],5,LIGHT);//显示第4路的PWM输出脉宽
		OLED_Put6x12Num(32,32,PWM_Val[1],5,LIGHT);//显示第1路的PWM输出脉宽
		OLED_Put6x12Num(98,16,PWM_Val[2],5,LIGHT);//显示第3路的PWM输出脉宽
		OLED_Put6x12Num(98,32,PWM_Val[3],5,LIGHT);//显示第2路的PWM输出脉宽
		OLED_Refresh_AllGDRAM(); 	//刷新显示	
		delay_ms(500);		
	} 
}





/*************************************END************************************/







