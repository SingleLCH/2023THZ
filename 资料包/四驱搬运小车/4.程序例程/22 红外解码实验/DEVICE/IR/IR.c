/***************************************************************************
本程序可供学习使用，经作者允许，也可用于其他用途。如有意见及建议，欢迎来电交流指教
硬件版本：四路麦轮多功能控制板
主控芯片：STM32F103RCT6
程序名称：红外解码程序
功能描述：解码红外遥控器。

*****************************************************************************/



#include "ir.h"
#include "delay.h"
#include "usart.h"

u8 	IR_Sta=0;	  //解码状态	  
u16 Dval;				//下降沿时计数器的值  		     
u32 IR_Rec=0;		//解码的原始数据
u8  IR_Cnt=0;		//解码的位数	
u8  Repeat=0;		//重复的次数
u16 times=0;
u16 ti[50];

//红外遥控初始化
//设置IO以及定时器4的输入捕获
void IR_Init(void)    			  
{									   
	RCC->APB2ENR|=1<<4;    	//使能PORTC时钟 
	GPIOC->CRH&=0X0FFFFFFF;	//PC15 输入  
	GPIOC->CRH|=0X80000000;	//上拉输入     
	GPIOC->ODR|=1<<15;			//PC15 上拉
	
	Ex_NVIC_Config(GPIO_C,15,FTIR); 		//下降沿触发
	MY_NVIC_Init(0,0,EXTI15_10_IRQn,2); //抢占2，子优先级2，组2	

	RCC->APB1ENR|=1<<5;		//TIM7时钟使能    
 	TIM7->ARR=0xFFFF;  		//设定计数器自动重装值
	TIM7->PSC=72;  				//预分频器720,得到100Khz的计数时钟	  
	TIM7->DIER&=~(1<<0);	//不允许更新中断	 
	TIM7->CR1|=0x01;    	//使能定时器7								 
}

//外部中断0复服务程序
void EXTI15_10_IRQHandler(void)
{	
	Dval=TIM7->CNT;
	TIM7->CNT=0;
	switch(IR_Sta)
	{
		case 0:
			if(Dval>12500&&Dval<14000)//时间等于13.5ms是数据头
			{
				IR_Cnt=0;
				IR_Sta=1;
				IR_Rec=0;
				Repeat=0;
			}
			else if(Dval>8500&&Dval<=12500)//时间等于11.25ms是重复码
			{
				times=Dval;
				Repeat++;IR_Sta=2;
			}
// 			times=Dval;IR_Sta=2;
			break;
		case 1:
			if(Dval>2100&&Dval<2350)//时间等于2.25ms是数据1
			{
				IR_Cnt++;
				IR_Rec<<=1;
				IR_Rec|=0x0001;
				
			}
			else if(Dval>1000&&Dval<1220)//时间等于1.125ms是数据0
			{
				IR_Cnt++;
				IR_Rec<<=1;
			}
			else if(Dval>=14000)
			{
				IR_Sta=2;
				Repeat=0;
			}
				
				
// 			if(IR_Cnt==32) IR_Sta=2;
			break;	
		default:
			break;
	}
	EXTI->PR=1<<15;  //清除LINE0上的中断标志位
}
































