/***************************************************************************
������ɹ�ѧϰʹ�ã�����������Ҳ������������;��������������飬��ӭ���罻��ָ��
Ӳ���汾����·���ֶ๦�ܿ��ư�
����оƬ��STM32F103RCT6
�������ƣ�ADת��ʵ��--������
����������ADת������ص�ѹ



*****************************************************************************/



#include "sys.h"
#include "delay.h"
#include "adc.h"
#include "oled_user.h"

unsigned int AD_Val=0;	//ADֵ
unsigned int Volt=0;		//��ص�ѹ��������λ0.1V

int main(void)
{		
	Stm32_Clock_Init(9);				//ϵͳʱ������
	JTAG_Set(JTAG_SWD_DISABLE); //�ȹر�JATG SWD�˿ڡ�4·�������У���Ҫռ��JTAG�˿��еĲ�������
	AFIO->MAPR|=0x1<<8;					//timer2������ӳ�䣬CH1---PA15,CH2---PB3�������������4��timer���й��õ����ţ����Ҫ��ӳ�䣩
	JTAG_Set(SWD_ENABLE); 			//ֻ����SWD�˿ڡ������뿪������Ȼ�ϵ���޷����Ի����س��򡣴�ʱ����BOOT0Ϊ�ߵ�ƽ�ſ��ԣ�
	delay_init(72);							//��ʱ��ʼ��
	Adc_Init();				 					//��ʼ��ADC������ص�ѹ	
	OLED_Init();								//OLED��ʼ��
	OLED_Put12x12CNstr(40,0,"AD ת��",LIGHT);	//��ʾ����
	OLED_Put12x12CNstr(10,20,"AD  ֵ:",LIGHT);				//��ʾ��ADֵ��
	OLED_Put12x12CNstr(10,40,"��ѹֵ:",LIGHT);				//��ʾ����ѹֵ��
	OLED_Put12x12CNstr(90,40,"V",LIGHT);						//��ʾ��λ
	while(1)
	{
		AD_Val=Get_Adc(4);											//ADת��
		Volt=AD_Val*33*11/4096;									//�����ѹ����صĵ�ѹ����1K��10K���贮�����з�ѹ��Volt=( (3.3V/4096)*(1K+10K)/1K )*AD_Val
		
		OLED_Put6x12Num(60,20,AD_Val,4,LIGHT);	//��ʾADֵ
		OLED_Put6x12Num1dot(60,40,Volt,LIGHT);	//��ʾ��ص�ѹ����С������ʾ����λV
		OLED_Refresh_AllGDRAM(); 								//ˢ����ʾ	
		delay_ms(10);														//��ʱ
	} 
}





/*************************************END************************************/







