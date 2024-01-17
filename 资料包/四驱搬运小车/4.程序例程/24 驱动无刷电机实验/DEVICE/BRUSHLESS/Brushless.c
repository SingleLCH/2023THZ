/***************************************************************************
本程序可供学习使用，经作者允许，也可用于其他用途。如有意见及建议，欢迎来电交流指教
硬件版本：四路麦轮多功能控制板
主控芯片：STM32F103RCT6
程序名称：PWM输出驱动无刷电机实验--无刷电机驱动程序
功能描述：扩展PWM接口输出，驱动四路无刷电机

*****************************************************************************/


#include "Brushless.h"


//***************************PWM初始化***************************
//arr,	计数自动重装值
//psc,	分频
void TIM1_PWM_Init(u16 arr,u16 psc)    
{
	RCC->APB2ENR|= 1<<11;	//开启TIM1时钟使能
	RCC->APB2ENR|=1<<2;    	//使能PORTA时钟
	RCC->APB2ENR|=1<<0;     //开启辅助时钟

	GPIOA->CRH&=0xFFFF0000;	//PA8,9,10,11复合输出功能
	GPIOA->CRH|=0x0000BBBB;
	GPIOA->ODR|=0xF<<8;			//PA8,9,10,11上拉

	TIM1->ARR=arr; 				//设定计数自动重装值
	TIM1->PSC=psc; 				//分频

	TIM1->CCMR1|=7<<4;  	//CH1 PWM2模式		 
	TIM1->CCMR1|=1<<3; 		//CH1预装载使能	 

	TIM1->CCMR1|=7<<12;  	//CH2 PWM2模式		 
	TIM1->CCMR1|=1<<11; 	//CH2预装载使能	

	TIM1->CCMR2|=7<<4;  	//CH3 PWM2模式		 
	TIM1->CCMR2|=1<<3; 		//CH3预装载使能	 

	TIM1->CCMR2|=7<<12;  	//CH4 PWM2模式		 
	TIM1->CCMR2|=1<<11; 	//CH5预装载使能	
	
	TIM1->CR1|=1<<7;  		//ARPE使能自动重装载预装载允许位
	TIM1->CR1|=1<<4;  		//向下计数模式	 

	TIM1->CCER|=3<<0;   	//OC1 输出使能
	TIM1->CCER|=3<<4;   	//OC2 输出使能
	TIM1->CCER|=3<<8;   	//OC3 输出使能
	TIM1->CCER|=3<<12;   	//OC4 输出使能

	TIM1->BDTR|=1<<15;		//开启OC和OCN输出

	TIM1->CCR1=(arr+1)/2;	//设置占空比
	TIM1->CCR2=(arr+1)/2;	//设置占空比
	TIM1->CCR3=(arr+1)/2;	//设置占空比
	TIM1->CCR4=(arr+1)/2;	//设置占空比
			
	TIM1->EGR|=1<<0;  		//初始化所有的寄存器
	TIM1->CR1|=1<<0;  		//使能定时器
	TIM1->CR1|=0x01;   //使能定时器1	

}


//***************************无刷电机初始化***************************
void Brushless_Init(void)
{	
	RCC->APB2ENR|=1<<0;       //使能辅助时钟	
	RCC->APB2ENR|=1<<2;       //GPIOA时钟使能	
	GPIOA->CRL&=0XFF0FFFFF;   //PA5设置为强推挽
	GPIOA->CRL|=0X00300000;  	   

	PWM_DIR=1;
	
	TIM1_PWM_Init(2499,71); //3分频,PWM频率=72000000/(71+1)/(2499+1)=400hz,周期2.5ms
	TIM1->CCR1=0;				//设置占空比
	TIM1->CCR2=0;				//设置占空比
	TIM1->CCR3=0;				//设置占空比
	TIM1->CCR4=0;				//设置占空比
	
}

//***************************无刷电机速度控制***************************
//Brushless1,	第一通道输出脉宽，数值范围1000-2000
//Brushless2,	第二通道输出脉宽，数值范围1000-2000
//Brushless3,	第三通道输出脉宽，数值范围1000-2000
//Brushless4,	第四通道输出脉宽，数值范围1000-2000
void Set_Brushless(int Brushless1,int Brushless2,int Brushless3,int Brushless4)
{
	TIM1->CCR1=Brushless1;				//设置占空比
	TIM1->CCR2=Brushless2;				//设置占空比
	TIM1->CCR3=Brushless3;				//设置占空比
	TIM1->CCR4=Brushless4;				//设置占空比
}


/*************************************END************************************/

