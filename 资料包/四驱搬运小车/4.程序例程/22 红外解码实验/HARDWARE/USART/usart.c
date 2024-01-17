/***************************************************************************
本程序可供学习使用，经作者允许，也可用于其他用途。如有意见及建议，欢迎来电交流指教
硬件版本：四路麦轮多功能控制板
主控芯片：STM32F103RCT6
程序名称：STM32麦轮全向移动小车程序--串口收发程序
功能描述：通过DMA接收串口蓝牙数据
*****************************************************************************/


#include "sys.h"
#include "usart.h"	  



//串口发送
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
	temp=(float)(pclk2*1000000)/(bound*16);//得到USARTDIV
	mantissa=temp;				 //得到整数部分
	fraction=(temp-mantissa)*16; //得到小数部分	 
    mantissa<<=4;
	mantissa+=fraction; 
	RCC->APB2ENR|=1<<3;   //使能PORTB口时钟  
	GPIOB->CRH &= 0xFFFF00FF;
	GPIOB->CRH |= 0X00004B00;//IO状态设置
	
	RCC->APB1RSTR|=1<<18;   //复位串口3
	RCC->APB1RSTR&=~(1<<18);//停止复位	
	RCC->APB1ENR|=1<<18;  //使能串口时钟 
	//波特率设置
 	USART3->BRR=mantissa; // 波特率设置	 
	USART3->CR1|=0X200C;  //1位停止,无校验位.
	
	USART3->CR3|=1<<6; 		 //使能DMA接收
}


/*************************************END************************************/

