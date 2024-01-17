/***************************************************************************
������ɹ�ѧϰʹ�ã�����������Ҳ������������;��������������飬��ӭ���罻��ָ��
Ӳ���汾����·���ֶ๦�ܿ��ư�
����оƬ��STM32F103RCT6
�������ƣ�����������
�����������������ң������

*****************************************************************************/



#include "ir.h"
#include "delay.h"
#include "usart.h"

u8 	IR_Sta=0;	  //����״̬	  
u16 Dval;				//�½���ʱ��������ֵ  		     
u32 IR_Rec=0;		//�����ԭʼ����
u8  IR_Cnt=0;		//�����λ��	
u8  Repeat=0;		//�ظ��Ĵ���
u16 times=0;
u16 ti[50];

//����ң�س�ʼ��
//����IO�Լ���ʱ��4�����벶��
void IR_Init(void)    			  
{									   
	RCC->APB2ENR|=1<<4;    	//ʹ��PORTCʱ�� 
	GPIOC->CRH&=0X0FFFFFFF;	//PC15 ����  
	GPIOC->CRH|=0X80000000;	//��������     
	GPIOC->ODR|=1<<15;			//PC15 ����
	
	Ex_NVIC_Config(GPIO_C,15,FTIR); 		//�½��ش���
	MY_NVIC_Init(0,0,EXTI15_10_IRQn,2); //��ռ2�������ȼ�2����2	

	RCC->APB1ENR|=1<<5;		//TIM7ʱ��ʹ��    
 	TIM7->ARR=0xFFFF;  		//�趨�������Զ���װֵ
	TIM7->PSC=72;  				//Ԥ��Ƶ��720,�õ�100Khz�ļ���ʱ��	  
	TIM7->DIER&=~(1<<0);	//����������ж�	 
	TIM7->CR1|=0x01;    	//ʹ�ܶ�ʱ��7								 
}

//�ⲿ�ж�0���������
void EXTI15_10_IRQHandler(void)
{	
	Dval=TIM7->CNT;
	TIM7->CNT=0;
	switch(IR_Sta)
	{
		case 0:
			if(Dval>12500&&Dval<14000)//ʱ�����13.5ms������ͷ
			{
				IR_Cnt=0;
				IR_Sta=1;
				IR_Rec=0;
				Repeat=0;
			}
			else if(Dval>8500&&Dval<=12500)//ʱ�����11.25ms���ظ���
			{
				times=Dval;
				Repeat++;IR_Sta=2;
			}
// 			times=Dval;IR_Sta=2;
			break;
		case 1:
			if(Dval>2100&&Dval<2350)//ʱ�����2.25ms������1
			{
				IR_Cnt++;
				IR_Rec<<=1;
				IR_Rec|=0x0001;
				
			}
			else if(Dval>1000&&Dval<1220)//ʱ�����1.125ms������0
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
	EXTI->PR=1<<15;  //���LINE0�ϵ��жϱ�־λ
}
































