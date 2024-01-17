/***************************************************************************
������ɹ�ѧϰʹ�ã�����������Ҳ������������;��������������飬��ӭ���罻��ָ��
Ӳ���汾����·���ֶ๦�ܿ��ư�
����оƬ��STM32F103RCT6
�������ƣ�����ң��DMA����--������
����������DMA���ղ������ֻ�����APP


*****************************************************************************/



#include "sys.h"
#include "usart.h"		
#include "delay.h"	
#include "oled_user.h" 	 	 	 	  	  
#include "string.h"
#include "DMA.h"
#include "esp8266.h"

	
int main(void)
{	
  Stm32_Clock_Init(9);//ϵͳʱ������
	JTAG_Set(JTAG_SWD_DISABLE); 
	AFIO->MAPR|=0x1<<8;	//������ӳ�䣬CH1---PA15,CH2---PB3
	JTAG_Set(SWD_ENABLE); 
	delay_init(72);			//��ʱ��ʼ��
	OLED_Init();
	OLED_Draw_FreeLink();			//��ʾͼ��
	OLED_Refresh_AllGDRAM(); 	//ˢ����ʾ
	delay_ms(1000);
	USART3_Init(36,115200);		//��ʼ����������ͨ�Žӿ�
	MYDMA_Config(DMA1_Channel3,(u32)&USART3->DR,(u32)Blue_dat,UART_RECV_LEN);//DMA1ͨ��4,����Ϊ����1,�洢��ΪSendBuff,����200.
	while(!init_server()) {delay_ms(200);}
 
	OLED_ClearBuffer(DARK);
	OLED_Put12x12CNstr(30,0,"WIFIң��",LIGHT);	//��ʾ����
	OLED_Put12x12CNstr(0,16,"����:",LIGHT);		//��ʾ����ҡ�ˡ����ҷ���ҡ�ˡ�		
	OLED_Put12x12CNstr(0,32,"ǰ��:",LIGHT);		//��ʾ����ҡ�ˡ����·���ҡ�ˡ�			

	OLED_Put12x12CNstr(66,16,"����:",LIGHT);	//��ʾ����ҡ�ˡ����ҷ���ҡ�ˡ�
	OLED_Put12x12CNstr(66,32,"ǰ��:",LIGHT);	//��ʾ����ҡ�ˡ����·���ҡ�ˡ�
	
	while(1)
	{
		if(WIFI_CK>0) WIFI_CK--;
		recv_msg();	//����WIFIң������	
		
		if(WIFI_CK>10)	OLED_Put12x12CNstr(10,52,"WIFI������",LIGHT);			//�ж��������ӣ���ʾ�������ӡ�
		else	OLED_Put12x12CNstr(10,52,"WIFI�ѶϿ�",LIGHT);							//��ʾ���ѶϿ���
		
		OLED_Put6x12Num(32,16,Blue_RC[2],5,LIGHT);	//��ʾ����ҡ�ˡ����ҷ���ҡ�ˡ���ҡ������
		OLED_Put6x12Num(32,32,Blue_RC[3],5,LIGHT);  //��ʾ����ҡ�ˡ����·���ҡ�ˡ���ҡ������

		OLED_Put6x12Num(98,16,Blue_RC[1],5,LIGHT);	//��ʾ����ҡ�ˡ����ҷ���ҡ�ˡ���ҡ������
		OLED_Put6x12Num(98,32,Blue_RC[0],5,LIGHT);	//��ʾ����ҡ�ˡ����·���ҡ�ˡ���ҡ������
		
		OLED_Refresh_AllGDRAM(); 		//ˢ��OLED��Ļ����ʾ
		delay_ms(5);								//��ʱ
			
	} 
}



/*************************************END************************************/







