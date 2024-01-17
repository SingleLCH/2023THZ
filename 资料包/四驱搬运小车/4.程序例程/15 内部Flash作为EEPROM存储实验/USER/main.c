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
#include "stmflash.h"

u16 DataSave_Buf[4]={1234,1528,2050,4587};	//Ҫ�洢������
u16 DataRead_Buf[4]={0,0,0,0};							//��ȡ�����ݻ���

int main(void)
{		
  Stm32_Clock_Init(9);				//ϵͳʱ������
	delay_init(72);							//��ʱ��ʼ��
	OLED_Init();								//OLED��ʼ�� 
	OLED_Put12x12CNstr(20,0,"Flash�洢ʵ��",LIGHT);	//��ʾ����
	OLED_Refresh_AllGDRAM(); 											//ˢ����ʾ
	
	STMFLASH_Write(FLASH_SAVE_ADDR + 2 ,DataSave_Buf,4);	//�洢����
	OLED_Put12x12CNstr(0,15,"Data Save:",LIGHT);					//��ʾ����
	OLED_Put6x12Num(5,30,DataSave_Buf[0],5,LIGHT);				//��ʾ��1��Ҫ�洢������
	OLED_Put6x12Num(37,30,DataSave_Buf[1],5,LIGHT); 			//��ʾ��2��Ҫ�洢������
	OLED_Put6x12Num(69,30,DataSave_Buf[2],5,LIGHT);				//��ʾ��3��Ҫ�洢������
	OLED_Put6x12Num(101,30,DataSave_Buf[3],5,LIGHT);			//��ʾ��4��Ҫ�洢������
	
	STMFLASH_Read(FLASH_SAVE_ADDR + 2 ,DataRead_Buf,4);		//��ȡ����
	OLED_Put12x12CNstr(0,42,"Data Read:",LIGHT);					//��ʾ����
	OLED_Put6x12Num(5,54,DataRead_Buf[0],5,LIGHT);				//��ʾ��ȡ�ĵ�1������
	OLED_Put6x12Num(37,54,DataRead_Buf[1],5,LIGHT); 			//��ʾ��ȡ�ĵ�1������
	OLED_Put6x12Num(69,54,DataRead_Buf[2],5,LIGHT);				//��ʾ��ȡ�ĵ�1������
	OLED_Put6x12Num(101,54,DataRead_Buf[3],5,LIGHT);			//��ʾ��ȡ�ĵ�1������
	OLED_Refresh_AllGDRAM(); 		//ˢ����ʾ
	while(1)
	{
		
	} 
}


/*************************************END************************************/







