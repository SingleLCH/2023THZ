#include "timer.h"
 	 
//基本定时器6中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器3!
void TIM6_Int_Init(u16 arr,u16 psc)
{
	RCC->APB1ENR|=1<<4;//TIM6时钟使能    
 	TIM6->ARR=arr;  	//设定计数器自动重装值//刚好1ms    
	TIM6->PSC=psc;  	//预分频器7200,得到10Khz的计数时钟	  
	TIM6->DIER|=1<<0;   //允许更新中断	 
	TIM6->CR1|=0x01;    //使能定时器3
  MY_NVIC_Init(1,3,TIM6_IRQn,2);//抢占1，子优先级3，组2									 
}

void TIM1_Int_Init(u16 arr,u16 psc)
{
	RCC->APB2ENR|=1<<11;		//TIM1时钟使能    
 	TIM1->ARR=arr;  			//设定计数器自动重装值  
	TIM1->PSC=psc;  			//预分频器7200,得到10Khz的计数时钟	  
	TIM1->DIER&=~(1<<0);  //不允许更新中断	 
	TIM1->CR1&=~(0x01);   //不使能定时器3								 
}

void TIM7_Int_Init(u16 arr,u16 psc)
{
	RCC->APB1ENR|=1<<5;		//TIM7时钟使能    
 	TIM7->ARR=arr;  			//设定计数器自动重装值  
	TIM7->PSC=psc;  			//预分频器7200,得到10Khz的计数时钟	  
	TIM7->DIER&=~(1<<0);  //不允许更新中断	 
	TIM7->CR1&=~(0x01);   //不使能定时器3								 
}


