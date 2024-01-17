/***************************************************************************
������ɹ�ѧϰʹ�ã�����������Ҳ������������;��������������飬��ӭ���罻��ָ��
Ӳ���汾����·���ֶ๦�ܿ��ư�
����оƬ��STM32F103RCT6
�������ƣ�STM32����ȫ���ƶ�С������--������
����������ͨ���ֻ�����APP��RC��ģң������PS2��Ϸ�ֱ�����ң�������ķ��С��ȫ���ƶ�


*****************************************************************************/



#include "sys.h"
#include "usart.h"		
#include "delay.h"	
#include "oled_user.h" 	  	 		 	 	 	  	  
#include "string.h"
#include "RC.h"


int main(void)
{		
//	unsigned char displaystick=0;
  Stm32_Clock_Init(9);//ϵͳʱ������
	JTAG_Set(JTAG_SWD_DISABLE); //�ȹر�JATG SWD�˿ڡ�4·�������У���Ҫռ��JTAG�˿��еĲ�������
	AFIO->MAPR|=0x1<<8;					//timer2������ӳ�䣬CH1---PA15,CH2---PB3�������������4��timer���й��õ����ţ����Ҫ��ӳ�䣩
	JTAG_Set(SWD_ENABLE); 			//ֻ����SWD�˿ڡ������뿪������Ȼ�ϵ���޷����Ի����س��򡣴�ʱ����BOOT0Ϊ�ߵ�ƽ�ſ��ԣ�
	delay_init(72);			//��ʱ��ʼ��
	RC_Init();
	OLED_Init();

	RC_CH[0]=1500;RC_CH[1]=1500;RC_CH[2]=1500;RC_CH[3]=1500;
	OLED_Put12x12CNstr(25,0,"PWM����ʵ��",LIGHT);	//��ʾ����
	OLED_Put12x12CNstr( 0,15,"RC-CH4:",LIGHT);		//��ʾ��4ͨ��
	OLED_Put12x12CNstr( 0,40,"RC-CH1:",LIGHT);		//��ʾ��1ͨ��
	OLED_Put12x12CNstr(66,15,"RC-CH3:",LIGHT);		//��ʾ��3ͨ��
	OLED_Put12x12CNstr(66,40,"RC-CH2:",LIGHT);		//��ʾ��2ͨ��
	OLED_Refresh_AllGDRAM(); 
	while(1)
	{
		OLED_Put5x7Num(20,28,RC_CH[3],5,LIGHT);	//��ʾ��4ͨ������
		OLED_Put5x7Num(20,54,RC_CH[0],5,LIGHT); //��ʾ��1ͨ������
		OLED_Put5x7Num(86,28,RC_CH[2],5,LIGHT);	//��ʾ��3ͨ������
		OLED_Put5x7Num(86,54,RC_CH[1],5,LIGHT);	//��ʾ��2ͨ������
		OLED_Refresh_AllGDRAM(); 	//ˢ����ʾ
		delay_ms(50);	
	} 
}





/*************************************END************************************/







