/***************************************************************************
������ɹ�ѧϰʹ�ã�����������Ҳ������������;��������������飬��ӭ���罻��ָ��
Ӳ���汾����·���ֶ๦�ܿ��ư�
����оƬ��STM32F103RCT6
�������ƣ�PS2��Ϸ�ֱ�ʵ��--������
��������������PS2��Ϸ�ֱ���ң�����ݣ�����OLED����ʾ


*****************************************************************************/


#include "sys.h"
#include "oled_user.h"
#include "pstwo.h"
#include "timer.h"

u16 Key_Data=0;

int main(void)
{		
  Stm32_Clock_Init(9);//ϵͳʱ������
	JTAG_Set(JTAG_SWD_DISABLE); //�ȹر�JATG SWD�˿ڡ�4·�������У���Ҫռ��JTAG�˿��еĲ�������
	AFIO->MAPR|=0x1<<8;					//timer2������ӳ�䣬CH1---PA15,CH2---PB3�������������4��timer���й��õ����ţ����Ҫ��ӳ�䣩
	JTAG_Set(SWD_ENABLE); 			//ֻ����SWD�˿ڡ������뿪������Ȼ�ϵ���޷����Ի����س��򡣴�ʱ����BOOT0Ϊ�ߵ�ƽ�ſ��ԣ�
	delay_init(72);			//��ʱ��ʼ��
	PS2_Init();
	TIM6_Int_Init(5000,72);	  //5ms�ж����
	OLED_Init();
	OLED_Refresh_AllGDRAM(); 	//ˢ����ʾ
	
	OLED_Put12x12CNstr(25,0,"PS2��Ϸ�ֱ�",LIGHT);	//��ʾ����

	while(1)
	{
// 		OLED_Put5x7Char(  0,15,(Key_Data&0x8000)==0x8000?'1':'0',LIGHT);
// 		OLED_Put5x7Char(  8,15,(Key_Data&0x4000)==0x4000?'1':'0',LIGHT);
// 		OLED_Put5x7Char( 16,15,(Key_Data&0x2000)==0x2000?'1':'0',LIGHT);
// 		OLED_Put5x7Char( 24,15,(Key_Data&0x1000)==0x1000?'1':'0',LIGHT);		
// 		OLED_Put5x7Char( 32,15,(Key_Data&0x0800)==0x0800?'1':'0',LIGHT);
// 		OLED_Put5x7Char( 40,15,(Key_Data&0x0400)==0x0400?'1':'0',LIGHT);
// 		OLED_Put5x7Char( 48,15,(Key_Data&0x0200)==0x0200?'1':'0',LIGHT);
// 		OLED_Put5x7Char( 56,15,(Key_Data&0x0100)==0x0100?'1':'0',LIGHT);
// 		
// 		OLED_Put5x7Char( 64,15,(Key_Data&0x0080)==0x0080?'1':'0',LIGHT);
// 		OLED_Put5x7Char( 72,15,(Key_Data&0x0040)==0x0040?'1':'0',LIGHT);
// 		OLED_Put5x7Char( 80,15,(Key_Data&0x0020)==0x0020?'1':'0',LIGHT);
// 		OLED_Put5x7Char( 88,15,(Key_Data&0x0010)==0x0010?'1':'0',LIGHT);
// 		OLED_Put5x7Char( 96,15,(Key_Data&0x0008)==0x0008?'1':'0',LIGHT);
// 		OLED_Put5x7Char(104,15,(Key_Data&0x0004)==0x0004?'1':'0',LIGHT);
// 		OLED_Put5x7Char(112,15,(Key_Data&0x0002)==0x0002?'1':'0',LIGHT);
// 		OLED_Put5x7Char(120,15,(Key_Data&0x0001)==0x0001?'1':'0',LIGHT);		
		
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////		
		OLED_Put5x7Char(  0, 0,((Key_Data<<7 )&0x8000)==0?'X':'-',LIGHT);	//7	
		OLED_Put5x7Char(  0,10,((Key_Data<<5 )&0x8000)==0?'X':'-',LIGHT);	//5
		
		OLED_Put5x7Char( 50,20,((Key_Data<<15)&0x8000)==0?'X':'-',LIGHT);  //15
		
		OLED_Put5x7Char( 10,30,((Key_Data<<11)&0x8000)==0?'X':'-',LIGHT);	//11	
		OLED_Put5x7Char(  0,40,((Key_Data<<8 )&0x8000)==0?'X':'-',LIGHT);	//8		
		OLED_Put5x7Char( 20,40,((Key_Data<<10)&0x8000)==0?'X':'-',LIGHT);  //10
		OLED_Put5x7Char( 10,50,((Key_Data<<9 )&0x8000)==0?'X':'-',LIGHT);  //9

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		OLED_Put5x7Char(120, 0,((Key_Data<<6 )&0x8000)==0?'X':'-',LIGHT);	//6	
		OLED_Put5x7Char(120,10,((Key_Data<<4 )&0x8000)==0?'X':'-',LIGHT);	//4
		
		OLED_Put5x7Char( 70,20,((Key_Data<<12)&0x8000)==0?'X':'-',LIGHT); 	//12
		
		OLED_Put5x7Char(110,30,((Key_Data<<3 )&0x8000)==0?'X':'-',LIGHT);	//3	
		OLED_Put5x7Char(120,40,((Key_Data<<2 )&0x8000)==0?'X':'-',LIGHT);	//2		
		OLED_Put5x7Char(100,40,((Key_Data<<0 )&0x8000)==0?'X':'-',LIGHT); 	//0
		OLED_Put5x7Char(110,50,((Key_Data<<1 )&0x8000)==0?'X':'-',LIGHT); 	//1
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////		
		OLED_Put5x7Num(40,40,PS2_CH[3],5,LIGHT);	//��ʾ��4·�����ʵʱ�ٶ�
		OLED_Put5x7Num(40,54,PS2_CH[2],5,LIGHT); 	//��ʾ��1·�����ʵʱ�ٶ�
		
		OLED_Put5x7Num(66,40,PS2_CH[0],5,LIGHT);	//��ʾ��3·�����ʵʱ�ٶ�
		OLED_Put5x7Num(66,54,PS2_CH[1],5,LIGHT);	//��ʾ��2·�����ʵʱ�ٶ�
		OLED_Refresh_AllGDRAM(); 	//ˢ����ʾ
		delay_ms(50);	
	} 
}
unsigned char PS2_Stick=0;
unsigned char PS2_Flag=0;

//��ʱ��6�жϷ������	 
void TIM6_IRQHandler(void)
{ 		    		  			    
	if(TIM6->SR&0X0001)//����ж�
	{			
		if(PS2_Stick<8) PS2_Stick++;
		if(PS2_Stick==4)	//ÿ20ms
		{	
			if(!PS2_RedLight()) //�ж��ֱ��Ƿ�Ϊ���ģʽ���ǣ�ָʾ��LED����
				PS2_Flag=1;	//���Ϊ���ģʽ
			else
				PS2_Flag=0; //���Ϊ���ģʽ
		}
		if(PS2_Stick==8)//ÿ40ms
		{	
			if(PS2_Flag)	//�Ǻ��ģʽ	
			{	
				Key_Data=PS2_DataKey();	 //�ֱ�����������
				PS2_CH[0]=	PS2_AnologData(PSS_RY);						
				PS2_CH[1]=	PS2_AnologData(PSS_RX);
				PS2_CH[2]=	PS2_AnologData(PSS_LX);
				PS2_CH[3]=	PS2_AnologData(PSS_LY);
			}

			PS2_Stick=0;
		}

	
	}
	TIM6->SR&=~(1<<0);//����жϱ�־λ 	    
}




/*************************************END************************************/







