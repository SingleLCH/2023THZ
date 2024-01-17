/***************************************************************************
������ɹ�ѧϰʹ�ã�����������Ҳ������������;��������������飬��ӭ���罻��ָ��
Ӳ���汾����·���ֶ๦�ܿ��ư�
����оƬ��STM32F103RCT6
�������ƣ�STM32����ȫ���ƶ�С������--�����շ�����
����������ͨ��DMA���մ�����������
*****************************************************************************/


#include "sys.h"
#include "usart.h"	  



//���ڷ���
void USART3_SendData(u8 Data)
{	
	while((USART3->SR & 0x40) == 0);
	USART3->DR = Data;	
}
void USART3_Init(unsigned int pclk2,unsigned int bound)
{  	 
	float temp;
	unsigned short int mantissa;
	unsigned short int fraction;	   
	temp=(float)(pclk2*1000000)/(bound*16);//�õ�USARTDIV
	mantissa=temp;				 //�õ���������
	fraction=(temp-mantissa)*16; //�õ�С������	 
    mantissa<<=4;
	mantissa+=fraction; 
	RCC->APB2ENR|=1<<3;   //ʹ��PORTB��ʱ��  
	GPIOB->CRH &= 0xFFFF00FF;
	GPIOB->CRH |= 0X00004B00;//IO״̬����
	
	RCC->APB1RSTR|=1<<18;   //��λ����3
	RCC->APB1RSTR&=~(1<<18);//ֹͣ��λ	
	RCC->APB1ENR|=1<<18;  //ʹ�ܴ���ʱ�� 
	//����������
 	USART3->BRR=mantissa; // ����������	 
	USART3->CR1|=0X200C;  //1λֹͣ,��У��λ.
	
	USART3->CR3|=1<<6; 		 //ʹ��DMA����
}


/*************************************END************************************/

