/***************************************************************************
������ɹ�ѧϰʹ�ã�����������Ҳ������������;��������������飬��ӭ���罻��ָ��
Ӳ��˵������·���ֶ๦�ܿ��ư�
����оƬ��STM32F103RCT6
�������ƣ�OLED��ʾʵ�飨4�߽ӿڣ�--������
�������������ƺ��İ��ϵ�LED��˸


*****************************************************************************/



#include "sys.h"	
#include "delay.h"	
#include "oled_user.h"


int main(void)
{		
  Stm32_Clock_Init(9);		//ϵͳʱ�����ã�8.0000MHz����9����Ƶ����Ƶ=8.0000MHz*9=72.0000MHz 
	delay_init(72);					//��ʱ��ʼ��
	OLED_Init();        		//LED��ʼ��	 

	while(1)						//��ѭ��
	{
//****************************ͼ����ʾ*****************************************
		
		OLED_ClearBuffer(DARK);	//�����ʾ����
		OLED_Draw_FreeLink();		//��ʾͼ��
		OLED_Refresh_AllGDRAM();//ˢ����ʾ	
		delay_ms(1000);					//��ʱ1s
		
//****************************������ʾ*****************************************
		
		OLED_ClearBuffer(DARK);	//�����ʾ����
		OLED_Put12x12CNstr(30,0,"*��    ��*",LIGHT);
		OLED_Put12x12CNstr(0,13,"ǰ��:",LIGHT);			OLED_Put12x12CNstr(66,13,"ǰ��:",LIGHT);
		OLED_Put12x12CNstr(0,26,"����:",LIGHT);			OLED_Put12x12CNstr(66,26,"����:",LIGHT);
		OLED_Put12x12CNstr(0,39,"��ѹ:",LIGHT);			OLED_Put12x12CNstr(66,39,"ң��:",LIGHT);
		OLED_Put12x12CNstr(10,52,"<�����˿Ƽ�>",LIGHT);			//��ʾ��ʾ��
		
		OLED_Put6x12Num(32,13,213,5,LIGHT);	OLED_Put6x12Num(98,13,126,5,LIGHT);		//��ʾת��
		OLED_Put6x12Num(32,26,153,5,LIGHT);	OLED_Put6x12Num(98,26,124,5,LIGHT);		//��ʾת��
		OLED_Put5x7Num1dot(30,42,116);																						//��ʾ��ص�ѹ
		OLED_Put12x12CNstr(98,39,"����",LIGHT);																		//��ʾ�����ź�
	
		OLED_Refresh_AllGDRAM();//ˢ����ʾ	
		delay_ms(1000);
//*****************************ͼ����ʾ********************************************		

		OLED_ClearBuffer(DARK);	//�����ʾ����
		
		OLED_DrawCircle(20,30,18);						//��Բ
		OLED_DrawRectangle(20,20,58,54,LIGHT);//������
		OLED_DrawLine(90,10,112,50,LIGHT);		//��б�ߣ������ε�һ����
		OLED_DrawLine(90,10,62,50,LIGHT);			//��б�ߣ������ε�һ����
		OLED_DrawLine(62,50,112,50,LIGHT);		//��б�ߣ������ε�һ����
		
		OLED_Refresh_AllGDRAM();//ˢ����ʾ	
		delay_ms(1000);		

	} 
	
}

/*************************************END****************************************/





