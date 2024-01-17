/***************************************************************************
������ɹ�ѧϰʹ�ã�����������Ҳ������������;��������������飬��ӭ���罻��ָ��
Ӳ���汾����·���ֶ๦�ܿ��ư�
����оƬ��STM32F103RCT6
�������ƣ�������ˢ���ʵ��--������
������������չPWM�ӿ������������ˢ�������Ҫ���ӵ�����

*****************************************************************************/



#include "sys.h"
#include "delay.h"
#include "Brushless.h"
#include "oled_user.h"

int PWM_Val[4]={0,0,0,0}; //4·���PWMֵ

int main(void)
{		
  Stm32_Clock_Init(9);				//ϵͳʱ������
	JTAG_Set(JTAG_SWD_DISABLE); //�ȹر�JATG SWD�˿ڡ�4·�������У���Ҫռ��JTAG�˿��еĲ�������
	JTAG_Set(SWD_ENABLE); 			//ֻ����SWD�˿ڡ������뿪������Ȼ�ϵ���޷����Ի����س��򡣴�ʱ����BOOT0Ϊ�ߵ�ƽ�ſ��ԣ�
	delay_init(72);							//��ʱ��ʼ��
	Brushless_Init();						//��ˢ�����ʼ��	 
 	Set_Brushless(0,0,0,0);			//������ˢ���PWM���ֵ
	
	OLED_Init();
	OLED_Put12x12CNstr(25,0,"��ˢ���ʵ��",LIGHT);	//��ʾ����
	OLED_Put12x12CNstr(0,16,"PWM1:",LIGHT);		//��ʾ��4·��PWM	
	OLED_Put12x12CNstr(0,32,"PWM2:",LIGHT);		//��ʾ��1·��PWM	
	OLED_Put12x12CNstr(66,16,"PWM3:",LIGHT);	//��ʾ��3·��PWM
	OLED_Put12x12CNstr(66,32,"PWM4:",LIGHT);	//��ʾ��2·��PWM	
	PWM_Val[0]=1000;		//��1·PWMֵ
	PWM_Val[1]=1000;		//��2·PWMֵ
	PWM_Val[2]=1000;		//��3·PWMֵ
	PWM_Val[3]=1000;		//��4·PWMֵ
	while(1)
	{	
		if(PWM_Val[0]<2000) PWM_Val[0]+=2;		//��1·PWMֵ
		if(PWM_Val[1]<2000) PWM_Val[1]+=5;		//��2·PWMֵ
		if(PWM_Val[2]<2000) PWM_Val[2]+=10;		//��3·PWMֵ
		if(PWM_Val[3]<2000) PWM_Val[3]+=20;		//��4·PWMֵ
		
		Set_Brushless(PWM_Val[0],PWM_Val[1],PWM_Val[2],PWM_Val[3]);		//���PWM���
		OLED_Put6x12Num(32,16,PWM_Val[0],5,LIGHT);	//��ʾ��4·��PWM��ֵ	
		OLED_Put6x12Num(32,32,PWM_Val[1],5,LIGHT); 	//��ʾ��1·��PWM��ֵ	
		OLED_Put6x12Num(98,16,PWM_Val[2],5,LIGHT);	//��ʾ��3·��PWM��ֵ	
		OLED_Put6x12Num(98,32,PWM_Val[3],5,LIGHT);	//��ʾ��2·��PWM��ֵ	
		OLED_Refresh_AllGDRAM(); 	//ˢ����ʾ
		delay_ms(10);	
	} 
}





/*************************************END************************************/







