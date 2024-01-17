/***************************************************************************
������ɹ�ѧϰʹ�ã�����������Ҳ������������;��������������飬��ӭ���罻��ָ��
Ӳ��˵������·���ֶ๦�ܿ��ư�
����оƬ��STM32F103RCT6
�������ƣ�USART1�շ�����
����������USART1��س���

*****************************************************************************/


#include "usart.h"	  

//��ʼ��IO ����1
//pclk2:PCLK2ʱ��Ƶ��(Mhz)
//bound:������
void USART1_Init(unsigned int pclk2,unsigned int bound)
{  	 
	float temp;
	unsigned short int mantissa;
	unsigned short int fraction;	   
	temp=(float)(pclk2*1000000)/(bound*16);//�õ�USARTDIV
	mantissa=temp;				 				//�õ���������
	fraction=(temp-mantissa)*16; 	//�õ�С������	 
    mantissa<<=4;
	mantissa+=fraction; 
	RCC->APB2ENR|=1<<2;   		//ʹ��PORTA��ʱ��  
	RCC->APB2ENR|=1<<14;  		//ʹ�ܴ���ʱ�� 
	GPIOA->CRH &= 0xFFFFF00F;
	GPIOA->CRH |= 0X000004B0;	//IO״̬����
		  
	RCC->APB2RSTR|=1<<14;   	//��λ����1
	RCC->APB2RSTR&=~(1<<14);	//ֹͣ��λ	   	   
	//����������
 	USART1->BRR=mantissa; 		// ����������	 
	USART1->CR1|=0X200C;  		//1λֹͣ,��У��λ.	
	
	//ʹ�ܽ����ж�
	USART1->CR1|=1<<8;    //PE�ж�ʹ��
	USART1->CR1|=1<<5;    //���ջ������ǿ��ж�ʹ��	    	
	MY_NVIC_Init(3,3,USART1_IRQn,2);//��2��������ȼ� 	

}

//���ڷ���1���ֽ�
void USART1_SendChar(u8 Data)
{	
	while((USART1->SR & 0x40) == 0);
	USART1->DR = Data;	
}

//���ڷ����ַ���
void USART1_SendString(u8 *str)
{
	while(*str!='\0')
	{
		USART1_SendChar(*str++);	
	}
}	



/*************************************END************************************/

