/***************************************************************************
������ɹ�ѧϰʹ�ã�����������Ҳ������������;��������������飬��ӭ���罻��ָ��
Ӳ��˵������·���ֶ๦�ܿ��ư�
����оƬ��STM32F103RCT6
�������ƣ�����ң��DMA����--������
����������DMA���ղ������ֻ�����APP


*****************************************************************************/



#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "dma.h"
#include "oled_user.h"
#include "bluetooth.h"

int main(void)
{		
  Stm32_Clock_Init(9);		//ϵͳʱ������
	delay_init(72);					//��ʱ��ʼ��
	MYDMA_Config(DMA1_Channel3,(u32)&USART3->DR,(u32)Blue_dat,UART_RECV_LEN);//DMA1ͨ��4,����Ϊ����1,�洢��ΪSendBuff,����200.	
	USART3_Init(36,9600);		//��ʼ����������ͨ�Žӿ�
		
	OLED_Init();						//��ʼ��OLED		
	
	OLED_Put12x12CNstr(30,0,"����ң��",LIGHT);	//��ʾ����
	OLED_Put12x12CNstr(0,16,"����:",LIGHT);		//��ʾ����ҡ�ˡ����ҷ���ҡ�ˡ�		
	OLED_Put12x12CNstr(0,32,"ǰ��:",LIGHT);		//��ʾ����ҡ�ˡ����·���ҡ�ˡ�			

	OLED_Put12x12CNstr(66,16,"����:",LIGHT);	//��ʾ����ҡ�ˡ����ҷ���ҡ�ˡ�
	OLED_Put12x12CNstr(66,32,"ǰ��:",LIGHT);	//��ʾ����ҡ�ˡ����·���ҡ�ˡ�
	
	while(1)
	{
		BlueTooth_Receive();	//��������ң������	
		
		if(BlueCK==1)	OLED_Put12x12CNstr(10,52,"����������",LIGHT);			//�ж��������ӣ���ʾ�������ӡ�
		else	OLED_Put12x12CNstr(10,52,"�����ѶϿ�",LIGHT);							//��ʾ���ѶϿ���
		
		OLED_Put6x12Num(32,16,Blue_RC[2],5,LIGHT);	//��ʾ����ҡ�ˡ����ҷ���ҡ�ˡ���ҡ������
		OLED_Put6x12Num(32,32,Blue_RC[3],5,LIGHT);  //��ʾ����ҡ�ˡ����·���ҡ�ˡ���ҡ������

		OLED_Put6x12Num(98,16,Blue_RC[1],5,LIGHT);	//��ʾ����ҡ�ˡ����ҷ���ҡ�ˡ���ҡ������
		OLED_Put6x12Num(98,32,Blue_RC[0],5,LIGHT);	//��ʾ����ҡ�ˡ����·���ҡ�ˡ���ҡ������
		
		OLED_Refresh_AllGDRAM(); 		//ˢ��OLED��Ļ����ʾ
		delay_ms(5);								//��ʱ
			
	} 
}
/*************************************END************************************/







