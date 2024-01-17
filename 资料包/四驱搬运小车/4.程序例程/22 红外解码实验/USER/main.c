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
#include "ir.h"

u8 Ir_key=0;
void OLED_Dis_Hex(u8 x,u8 y,u32 Hex_Data);

int main(void)
{		
  Stm32_Clock_Init(9);				//ϵͳʱ������
	delay_init(72);							//��ʱ��ʼ��
	OLED_Init();								//OLED��ʼ�� 
	OLED_Put12x12CNstr(20,0,"�������ʵ��",LIGHT);	//��ʾ����
	OLED_Put12x12CNstr(0,15,"Receive:",LIGHT);		//��ʾ����
	OLED_Refresh_AllGDRAM(); 											//ˢ����ʾ
	IR_Init();																		//�������˿ڳ�ʼ��

	while(1)
	{
		if(IR_Sta==2)
		{
			OLED_Dis_Hex( 10,28,IR_Rec);	//��ʾʮ����������
			OLED_Put6x12Num( 80,28,Repeat,5,LIGHT);
			OLED_Put5x7Num(  0,41,times/100,5,LIGHT);
			times=0;
			OLED_Refresh_AllGDRAM(); 		//ˢ����ʾ
			IR_Sta=0;
		}
		delay_ms(5);
	} 
}
u8 Hex_to_char(u8 hex)
{
	u8 cha_tmp;
	switch(hex)
	{
		case 0xA:cha_tmp='A';break;
		case 0xB:cha_tmp='B';break;
		case 0xC:cha_tmp='C';break;
		case 0xD:cha_tmp='D';break;
		case 0xE:cha_tmp='E';break;
		case 0xF:cha_tmp='F';break;
		default:
			if(hex<0xA)	cha_tmp=hex+48;
			break;
	}
	return cha_tmp;
}

void OLED_Dis_Hex(u8 x,u8 y,u32 Hex_Data)
{
	u8 cha[8],n;
	cha[0]=(Hex_Data>>28)&0x0F;
	cha[1]=(Hex_Data>>24)&0x0F;
	cha[2]=(Hex_Data>>20)&0x0F;
	cha[3]=(Hex_Data>>16)&0x0F;
	cha[4]=(Hex_Data>>12)&0x0F;
	cha[5]=(Hex_Data>>8)&0x0F;
	cha[6]=(Hex_Data>>4)&0x0F;
	cha[7]=(Hex_Data>>0)&0x0F;
	OLED_Put12x12CNstr(x,y,"0x",LIGHT);
	for(n=0;n<8;n++)
		OLED_Put6x12Char(x+12+n*6,y,Hex_to_char(cha[n]),LIGHT);
	
}




/*************************************END************************************/







