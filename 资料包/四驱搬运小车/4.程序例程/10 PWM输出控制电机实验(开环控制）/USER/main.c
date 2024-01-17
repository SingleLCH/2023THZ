/***************************************************************************
本程序可供学习使用，经作者允许，也可用于其他用途。如有意见及建议，欢迎来电交流指教
硬件说明：四路麦轮多功能控制板
主控芯片：STM32F103RCT6
程序名称：PWM输出控制电机实验--主程序
功能描述：PWM输出控制电机（开环控制）


*****************************************************************************/



#include "sys.h"
#include "delay.h"
#include "motor.h"
#include "oled_user.h"

int PWM_Val[4]={0,0,0,0}; //4路电机PWM值

int main(void)
{		
  Stm32_Clock_Init(9);				//系统时钟设置
	JTAG_Set(JTAG_SWD_DISABLE); //先关闭JATG SWD端口。4路编码器中，需要占用JTAG端口中的部分引脚
	AFIO->MAPR|=0x1<<8;					//timer2部分重映射，CH1---PA15,CH2---PB3（解码编码器的4个timer中有共用的引脚，因此要重映射）
	JTAG_Set(SWD_ENABLE); 			//只开启SWD端口。（必须开启，不然断电后，无法调试或下载程序。此时设置BOOT0为高电平才可以）
	delay_init(72);							//延时初始化
	Motor_Init();								//PWM输出初始化	 
 	Set_Motor(0,0,0,0);					//电机PWM输出
	
	OLED_Init();
	OLED_Put12x12CNstr(25,0,"PWM控制电机",LIGHT);	//显示标题
	OLED_Put12x12CNstr(0,16,"PWM4:",LIGHT);		//显示第4路的PWM	
	OLED_Put12x12CNstr(0,32,"PWM1:",LIGHT);		//显示第1路的PWM	
	OLED_Put12x12CNstr(66,16,"PWM3:",LIGHT);	//显示第3路的PWM
	OLED_Put12x12CNstr(66,32,"PWM2:",LIGHT);	//显示第2路的PWM	
	PWM_Val[0]=1000;PWM_Val[1]=-1000;PWM_Val[2]=3000;PWM_Val[3]=-3000;	//给初始PWM值
	while(1)
	{	
		PWM_Val[0]+=5;		//第1路PWM值递增
		PWM_Val[1]+=10;		//第2路PWM值递增
		PWM_Val[2]+=4;		//第3路PWM值递增
		PWM_Val[3]+=20;		//第4路PWM值递增
		if(PWM_Val[0]>PWM_MAX) PWM_Val[0]=0;
		if(PWM_Val[1]>PWM_MAX) PWM_Val[1]=0;
		if(PWM_Val[2]>PWM_MAX) PWM_Val[2]=0;
		if(PWM_Val[3]>PWM_MAX) PWM_Val[3]=0;
		
		Set_Motor(PWM_Val[0],PWM_Val[1],PWM_Val[2],PWM_Val[3]);		//电机PWM输出
		
		OLED_Put6x12Num(32,16,PWM_Val[3]/10,5,LIGHT);	//显示第4路的PWM数值	
		OLED_Put6x12Num(32,32,PWM_Val[0]/10,5,LIGHT); //显示第1路的PWM数值	
		OLED_Put6x12Num(98,16,PWM_Val[2]/10,5,LIGHT);	//显示第3路的PWM数值	
		OLED_Put6x12Num(98,32,PWM_Val[1]/10,5,LIGHT);	//显示第2路的PWM数值	
		OLED_Refresh_AllGDRAM(); 	//刷新显示
		delay_ms(5);
	} 
}





/*************************************END************************************/







