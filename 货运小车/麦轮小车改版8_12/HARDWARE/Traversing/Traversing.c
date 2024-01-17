#include "Traversing.h"
#include "motor.h"
#include "beep.h"
#include "delay.h"
#include "usart.h"	


void Traversing_GPIO_Init(void)	
{
	GPIO_InitTypeDef GPIO_InitStructure;	
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_1|GPIO_Pin_9|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
}

void Auto_Traversing(void)
{
	static uint32_t stop_time = 0;
	if(D2==1 && D3 ==0 && D4==1 )  //ֱ��
		{
				Set_Motor(-2000, 2000,2000, -2000);
		}
	else if (D2==0 && D3 ==1 && D4==1 ) 
		{
				Set_Motor(2000, 2000,2000, 2000);
		}
	else if ( D2==1 && D3 ==1 && D4==0) 
		{
				Set_Motor(-2000, -2000,-2000, -2000);	
		}
	else if ( D2==0 && D3 ==0 && D4==0) 
		{
				Set_Motor(0, 0,0, 0);	
				stop_time++;
				if(stop_time ==200)//ֹͣ1s
				{
					BEEP(0);// 1��0����
					Delay_ms(200);
					BEEP(1);// 1��0����
					Delay_ms(200);
					BEEP(0);// 1��0����
					Delay_ms(200);
					BEEP(1);// 1��0����
					Delay_ms(200);
					BEEP(0);// 1��0����
					UART_PutChar(USART3, 'B');
					while(1)
					{
						
					}
				}
		}


}

