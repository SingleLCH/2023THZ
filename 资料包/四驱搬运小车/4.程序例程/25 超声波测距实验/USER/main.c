/***************************************************************************
������ɹ�ѧϰʹ�ã�����������Ҳ������������;��������������飬��ӭ���罻��ָ��
Ӳ���汾����·���ֶ๦�ܿ��ư�
����оƬ��STM32F103RCT6
�������ƣ��ڲ�Flash��ΪEEPROM�洢ʵ��--������
����������ʹ��Ƭ��Flash��ΪEEPROM�ɶϵ�洢����


*****************************************************************************/



#include "sys.h"
#include "usart.h"		
#include "delay.h"	
#include "oled_user.h" 	  	 	 	 	 	  	  
#include "string.h"
#include "ULN.h"

int main(void)
{		
  Stm32_Clock_Init(9);				//ϵͳʱ������
	delay_init(72);							//��ʱ��ʼ��
	OLED_Init();								//OLED��ʼ�� 
	OLED_Put12x12CNstr(20,0,"���������ʵ��",LIGHT);	//��ʾ����
	OLED_Put12x12CNstr(0,20,"Right:       cm",LIGHT);		//��ʾ����
	OLED_Put12x12CNstr(0,40,"Left :       cm",LIGHT);		//��ʾ����
	OLED_Refresh_AllGDRAM(); 											//ˢ����ʾ
	Uln_init();								//��������ʼ��

	while(1)
	{
		OLED_Put6x12Num(40,20,Distance1,5,1);			//��ʾ��ص�ѹ
		OLED_Put6x12Num(40,40,Distance2,5,1);
		OLED_Refresh_AllGDRAM(); 	
		delay_ms(50);
		Uln_Trig();						//���ͳ�����
	} 
}


/*************************************END************************************/







