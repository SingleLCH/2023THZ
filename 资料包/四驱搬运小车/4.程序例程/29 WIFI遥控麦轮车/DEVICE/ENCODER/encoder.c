/***************************************************************************
本程序可供学习使用，经作者允许，也可用于其他用途。如有意见及建议，欢迎来电交流指教
硬件版本：四路麦轮多功能控制板
主控芯片：STM32F103RCT6
程序名称：STM32麦轮全向移动小车程序--编码器程序
功能描述：四路AB相编码器的正交解码程序。电机转动时，可记录编码器脉冲数及电机转向。



*****************************************************************************/


#include "encoder.h"

short int Encoder[4];


void Encoder_Init(void)
{
	Encoder_Init_TIM8();
	Encoder_Init_TIM2();
	Encoder_Init_TIM3();
	Encoder_Init_TIM4();
}
/**************************************************************************
函数功能：把TIM1初始化为编码器接口模式
入口参数：无
返回  值：无
**************************************************************************/
void Encoder_Init_TIM8(void)
{
	RCC->APB1ENR|=1<<0;     //TIM2时钟使能
	RCC->APB2ENR|=1<<13;     //TIM8时钟使能
	RCC->APB2ENR|=1<<4;    //使能PORTC时钟
	GPIOC->CRL&=0X00FFFFFF;//PC6,7
	GPIOC->CRL|=0X44000000;//浮空输入

	TIM8->PSC = 0x0;//预分频器
	TIM8->ARR = 0xFFFF;//设定计数器自动重装值 
	TIM8->CR1 &=~(3<<8);// 选择时钟分频：不分频
	TIM8->CR1 &=~(3<<5);// 选择计数模式:边沿对齐模式
	TIM8->CCMR1 |= 1<<0; //CC1S='01' IC1FP1映射到TI1
	TIM8->CCMR1 |= 1<<8; //CC2S='01' IC2FP2映射到TI2
	TIM8->CCER &= ~(1<<1);	 //CC1P='0'	 IC1FP1不反相，IC1FP1=TI1
	TIM8->CCER &= ~(1<<5);	 //CC2P='0'	 IC2FP2不反相，IC2FP2=TI2
	TIM8->CCMR1 |= 3<<4;	//	IC1F='1000' 输入捕获1滤波器
	TIM8->CCMR1 |= 3<<12; //	IC1F='1000' 输入捕获2滤波器
	TIM8->SMCR |= 3<<0;	 //SMS='011' 所有的输入均在上升沿和下降沿有效
// 	TIM8 -> CNT=5000;
	TIM8->CR1 |= 1<<0;    //CEN=1，使能定时器
}

/**************************************************************************
函数功能：把TIM2初始化为编码器接口模式
入口参数：无
返回  值：无
**************************************************************************/
void Encoder_Init_TIM2(void)
{
//TIM2的输入端口已经重新映射
//部分重映射，CH1---PA15,CH2---PB3
	RCC->APB1ENR|=1<<0;     //TIM2时钟使能
	RCC->APB2ENR|=1<<2;    //使能PORTA时钟
	RCC->APB2ENR|=1<<3;    //使能PORTB时钟
	GPIOA->CRH&=0X0FFFFFFF;//PA15
	GPIOA->CRH|=0X40000000;//浮空输入
	GPIOB->CRL&=0XFFFF0FFF;//PB3
	GPIOB->CRL|=0X00004000;//浮空输入
	TIM2->PSC = 0x0;//预分频器
	TIM2->ARR = 0xFFFF;//设定计数器自动重装值 
	TIM2->CR1 &=~(3<<8);// 选择时钟分频：不分频
	TIM2->CR1 &=~(3<<5);// 选择计数模式:边沿对齐模式
	TIM2->CCMR1 |= 1<<0; //CC1S='01' IC1FP1映射到TI1
	TIM2->CCMR1 |= 1<<8; //CC2S='01' IC2FP2映射到TI2
	TIM2->CCER &= ~(1<<1);	 //CC1P='0'	 IC1FP1不反相，IC1FP1=TI1
	TIM2->CCER &= ~(1<<5);	 //CC2P='0'	 IC2FP2不反相，IC2FP2=TI2
	TIM2->CCMR1 |= 3<<4;	//	IC1F='1000' 输入捕获1滤波器
	TIM2->CCMR1 |= 3<<12; //	IC1F='1000' 输入捕获2滤波器
	TIM2->SMCR |= 3<<0;	 //SMS='011' 所有的输入均在上升沿和下降沿有效
// 	TIM2 -> CNT=5000;
	TIM2->CR1 |= 1<<0;    //CEN=1，使能定时器
}

/**************************************************************************
函数功能：把TIM2初始化为编码器接口模式
入口参数：无
返回  值：无
**************************************************************************/
void Encoder_Init_TIM3(void)
{
	RCC->APB1ENR|=1<<0;     //TIM2时钟使能
	RCC->APB1ENR|=1<<1;     //TIM3时钟使能
	RCC->APB2ENR|=1<<2;    //使能PORTA时钟
	GPIOA->CRL&=0X00FFFFFF;//PA6 PA7
	GPIOA->CRL|=0X44000000;//浮空输入
	TIM3->PSC = 0x0;//预分频器
	TIM3->ARR = 0xFFFF;//设定计数器自动重装值 
	TIM3->CR1 &=~(3<<8);// 选择时钟分频：不分频
	TIM3->CR1 &=~(3<<5);// 选择计数模式:边沿对齐模式
	TIM3->CCMR1 |= 1<<0; //CC1S='01' IC1FP1映射到TI1
	TIM3->CCMR1 |= 1<<8; //CC2S='01' IC2FP2映射到TI2
	TIM3->CCER &= ~(1<<1);	 //CC1P='0'	 IC1FP1不反相，IC1FP1=TI1
	TIM3->CCER &= ~(1<<5);	 //CC2P='0'	 IC2FP2不反相，IC2FP2=TI2
	TIM3->CCMR1 |= 3<<4;	//	IC1F='1000' 输入捕获1滤波器
	TIM3->CCMR1 |= 3<<12; //	IC1F='1000' 输入捕获2滤波器
	TIM3->SMCR |= 3<<0;	 //SMS='011' 所有的输入均在上升沿和下降沿有效
// 	TIM3 -> CNT=5000;
	TIM3->CR1 |= 1<<0;    //CEN=1，使能定时器
}
/**************************************************************************
函数功能：把TIM4初始化为编码器接口模式
入口参数：无
返回  值：无
**************************************************************************/
void Encoder_Init_TIM4(void)
{
	RCC->APB1ENR|=1<<0;     //TIM2时钟使能
	RCC->APB1ENR|=1<<2;     //TIM4时钟使能
	RCC->APB2ENR|=1<<3;    //使能PORTb时钟
	GPIOB->CRL&=0X00FFFFFF;//PB6 PB7
	GPIOB->CRL|=0X44000000;//浮空输入
	TIM4->PSC = 0x0;//预分频器
	TIM4->ARR = 0xFFFF;//设定计数器自动重装值 
	TIM4->CR1 &=~(3<<8);// 选择时钟分频：不分频
	TIM4->CR1 &=~(3<<5);// 选择计数模式:边沿对齐模式
	TIM4->CCMR1 |= 1<<0; //CC1S='01' IC1FP1映射到TI1
	TIM4->CCMR1 |= 1<<8; //CC2S='01' IC2FP2映射到TI2
	TIM4->CCER &= ~(1<<1);	 //CC1P='0'	 IC1FP1不反相，IC1FP1=TI1
	TIM4->CCER &= ~(1<<5);	 //CC2P='0'	 IC2FP2不反相，IC2FP2=TI2
	TIM4->CCMR1 |= 3<<4;	//	IC1F='1000' 输入捕获1滤波器
	TIM4->CCMR1 |= 3<<12; //	IC1F='1000' 输入捕获2滤波器
	TIM4->SMCR |= 3<<0;	 //SMS='011' 所有的输入均在上升沿和下降沿有效
// 	TIM4 -> CNT=5000;
	TIM4->CR1 |= 1<<0;    //CEN=1，使能定时器
}
/**************************************************************************
函数功能：单位时间读取编码器计数
入口参数：定时器
返回  值：速度值
**************************************************************************/

short int Encoder_Sum[4]={0,0,0,0};
void Read_Encoder(void)
{

	Encoder[0]= (short int)TIM2 -> CNT;  TIM2 -> CNT=0;//后左轮
	Encoder[1]= (short int)TIM8 -> CNT;  TIM8 -> CNT=0;//后右轮
	Encoder[2]= (short int)TIM3 -> CNT;  TIM3 -> CNT=0;//前右轮
	Encoder[3]= (short int)TIM4 -> CNT;  TIM4 -> CNT=0;//前左轮
	
	
	Encoder_Sum[0]+=Encoder[0];
	Encoder_Sum[1]+=Encoder[1];
	Encoder_Sum[2]+=Encoder[2];
	Encoder_Sum[3]+=Encoder[3];
}


/*************************************END************************************/


