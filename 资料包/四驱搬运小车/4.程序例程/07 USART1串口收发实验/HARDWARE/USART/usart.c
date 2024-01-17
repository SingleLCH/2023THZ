/***************************************************************************
本程序可供学习使用，经作者允许，也可用于其他用途。如有意见及建议，欢迎来电交流指教
硬件说明：四路麦轮多功能控制板
主控芯片：STM32F103RCT6
程序名称：USART1收发程序
功能描述：USART1相关程序

*****************************************************************************/


#include "usart.h"	  

//初始化IO 串口1
//pclk2:PCLK2时钟频率(Mhz)
//bound:波特率
void USART1_Init(unsigned int pclk2,unsigned int bound)
{  	 
	float temp;
	unsigned short int mantissa;
	unsigned short int fraction;	   
	temp=(float)(pclk2*1000000)/(bound*16);//得到USARTDIV
	mantissa=temp;				 				//得到整数部分
	fraction=(temp-mantissa)*16; 	//得到小数部分	 
    mantissa<<=4;
	mantissa+=fraction; 
	RCC->APB2ENR|=1<<2;   		//使能PORTA口时钟  
	RCC->APB2ENR|=1<<14;  		//使能串口时钟 
	GPIOA->CRH &= 0xFFFFF00F;
	GPIOA->CRH |= 0X000004B0;	//IO状态设置
		  
	RCC->APB2RSTR|=1<<14;   	//复位串口1
	RCC->APB2RSTR&=~(1<<14);	//停止复位	   	   
	//波特率设置
 	USART1->BRR=mantissa; 		// 波特率设置	 
	USART1->CR1|=0X200C;  		//1位停止,无校验位.	
	
	//使能接收中断
	USART1->CR1|=1<<8;    //PE中断使能
	USART1->CR1|=1<<5;    //接收缓冲区非空中断使能	    	
	MY_NVIC_Init(3,3,USART1_IRQn,2);//组2，最低优先级 	

}

//串口发送1个字节
void USART1_SendChar(u8 Data)
{	
	while((USART1->SR & 0x40) == 0);
	USART1->DR = Data;	
}

//串口发送字符串
void USART1_SendString(u8 *str)
{
	while(*str!='\0')
	{
		USART1_SendChar(*str++);	
	}
}	



/*************************************END************************************/

