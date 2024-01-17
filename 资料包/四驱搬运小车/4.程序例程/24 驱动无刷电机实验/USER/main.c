/***************************************************************************
本程序可供学习使用，经作者允许，也可用于其他用途。如有意见及建议，欢迎来电交流指教
硬件版本：四路麦轮多功能控制板
主控芯片：STM32F103RCT6
程序名称：驱动无刷电机实验--主程序
功能描述：扩展PWM接口输出，控制无刷电机。需要电子调速器

*****************************************************************************/



#include "sys.h"
#include "delay.h"
#include "Brushless.h"
#include "oled_user.h"

int PWM_Val[4]={0,0,0,0}; //4路电机PWM值

int main(void)
{		
  Stm32_Clock_Init(9);				//系统时钟设置
	JTAG_Set(JTAG_SWD_DISABLE); //先关闭JATG SWD端口。4路编码器中，需要占用JTAG端口中的部分引脚
	JTAG_Set(SWD_ENABLE); 			//只开启SWD端口。（必须开启，不然断电后，无法调试或下载程序。此时设置BOOT0为高电平才可以）
	delay_init(72);							//延时初始化
	Brushless_Init();						//无刷电机初始化	 
 	Set_Brushless(0,0,0,0);			//设置无刷电机PWM输出值
	
	OLED_Init();
	OLED_Put12x12CNstr(25,0,"无刷电机实验",LIGHT);	//显示标题
	OLED_Put12x12CNstr(0,16,"PWM1:",LIGHT);		//显示第4路的PWM	
	OLED_Put12x12CNstr(0,32,"PWM2:",LIGHT);		//显示第1路的PWM	
	OLED_Put12x12CNstr(66,16,"PWM3:",LIGHT);	//显示第3路的PWM
	OLED_Put12x12CNstr(66,32,"PWM4:",LIGHT);	//显示第2路的PWM	
	PWM_Val[0]=1000;		//第1路PWM值
	PWM_Val[1]=1000;		//第2路PWM值
	PWM_Val[2]=1000;		//第3路PWM值
	PWM_Val[3]=1000;		//第4路PWM值
	while(1)
	{	
		if(PWM_Val[0]<2000) PWM_Val[0]+=2;		//第1路PWM值
		if(PWM_Val[1]<2000) PWM_Val[1]+=5;		//第2路PWM值
		if(PWM_Val[2]<2000) PWM_Val[2]+=10;		//第3路PWM值
		if(PWM_Val[3]<2000) PWM_Val[3]+=20;		//第4路PWM值
		
		Set_Brushless(PWM_Val[0],PWM_Val[1],PWM_Val[2],PWM_Val[3]);		//电机PWM输出
		OLED_Put6x12Num(32,16,PWM_Val[0],5,LIGHT);	//显示第4路的PWM数值	
		OLED_Put6x12Num(32,32,PWM_Val[1],5,LIGHT); 	//显示第1路的PWM数值	
		OLED_Put6x12Num(98,16,PWM_Val[2],5,LIGHT);	//显示第3路的PWM数值	
		OLED_Put6x12Num(98,32,PWM_Val[3],5,LIGHT);	//显示第2路的PWM数值	
		OLED_Refresh_AllGDRAM(); 	//刷新显示
		delay_ms(10);	
	} 
}





/*************************************END************************************/







