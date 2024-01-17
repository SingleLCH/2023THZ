#include "timer.h"
 	 
//������ʱ��6�жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//����ʹ�õ��Ƕ�ʱ��3!
void TIM6_Int_Init(u16 arr,u16 psc)
{
	RCC->APB1ENR|=1<<4;//TIM6ʱ��ʹ��    
 	TIM6->ARR=arr;  	//�趨�������Զ���װֵ//�պ�1ms    
	TIM6->PSC=psc;  	//Ԥ��Ƶ��7200,�õ�10Khz�ļ���ʱ��	  
	TIM6->DIER|=1<<0;   //��������ж�	 
	TIM6->CR1|=0x01;    //ʹ�ܶ�ʱ��3
  MY_NVIC_Init(1,3,TIM6_IRQn,2);//��ռ1�������ȼ�3����2									 
}

void TIM1_Int_Init(u16 arr,u16 psc)
{
	RCC->APB2ENR|=1<<11;		//TIM1ʱ��ʹ��    
 	TIM1->ARR=arr;  			//�趨�������Զ���װֵ  
	TIM1->PSC=psc;  			//Ԥ��Ƶ��7200,�õ�10Khz�ļ���ʱ��	  
	TIM1->DIER&=~(1<<0);  //����������ж�	 
	TIM1->CR1&=~(0x01);   //��ʹ�ܶ�ʱ��3								 
}

void TIM7_Int_Init(u16 arr,u16 psc)
{
	RCC->APB1ENR|=1<<5;		//TIM7ʱ��ʹ��    
 	TIM7->ARR=arr;  			//�趨�������Զ���װֵ  
	TIM7->PSC=psc;  			//Ԥ��Ƶ��7200,�õ�10Khz�ļ���ʱ��	  
	TIM7->DIER&=~(1<<0);  //����������ж�	 
	TIM7->CR1&=~(0x01);   //��ʹ�ܶ�ʱ��3								 
}


