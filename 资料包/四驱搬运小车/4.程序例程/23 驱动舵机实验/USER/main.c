/***************************************************************************
������ɹ�ѧϰʹ�ã�����������Ҳ������������;��������������飬��ӭ���罻��ָ��
Ӳ���汾��FreeLink STM32��·ֱ��������ư�V2.41
����оƬ��STM32F103RCT6
�������ƣ�PWM����������ʵ��--������
������������չPWM�ӿ������������·���
������ߣ�ˮľ����@FreeLink
��ϵ�绰��15802781535
��ϵ���䣺357704597@qq.com
�޸�ʱ�䣺2019/09/16
����汾��V2.1
��Ȩ���У�����ؾ�
Copyright(C) �人��������˿Ƽ����޹�˾ 
All rights reserved
*****************************************************************************
�޸�˵����
��


*****************************************************************************/



#include "sys.h"
#include "delay.h"
#include "servo.h"
#include "oled_user.h"

int PWM_Val[4]={0,0,0,0}; //4·���PWMֵ

int main(void)
{		
  Stm32_Clock_Init(9);				//ϵͳʱ������
	JTAG_Set(JTAG_SWD_DISABLE); //�ȹر�JATG SWD�˿ڡ�4·�������У���Ҫռ��JTAG�˿��еĲ�������
	AFIO->MAPR|=0x1<<8;					//timer2������ӳ�䣬CH1---PA15,CH2---PB3�������������4��timer���й��õ����ţ����Ҫ��ӳ�䣩
	JTAG_Set(SWD_ENABLE); 			//ֻ����SWD�˿ڡ������뿪������Ȼ�ϵ���޷����Ի����س��򡣴�ʱ����BOOT0Ϊ�ߵ�ƽ�ſ��ԣ�
	delay_init(72);							//��ʱ��ʼ��
	Servo_Init();								//PWM�����ʼ��	 
 	Set_Servo(0,0,0,0);					//���PWM���
	
	OLED_Init();
	OLED_Put12x12CNstr(40,0,"���ʵ��",LIGHT);	//��ʾ����
	OLED_Put12x12CNstr(0,16,"PWM1:",LIGHT);		//��ʾ��4·��PWM	
	OLED_Put12x12CNstr(0,32,"PWM2:",LIGHT);		//��ʾ��1·��PWM	
	OLED_Put12x12CNstr(66,16,"PWM3:",LIGHT);	//��ʾ��3·��PWM
	OLED_Put12x12CNstr(66,32,"PWM4:",LIGHT);	//��ʾ��2·��PWM	
	while(1)
	{	
		PWM_Val[0]=1000;		//��1·PWM�������
		PWM_Val[1]=1200;		//��2·PWM�������
		PWM_Val[2]=1500;		//��3·PWM�������
		PWM_Val[3]=2000;		//��4·PWM�������
		Set_Servo(PWM_Val[0],PWM_Val[1],PWM_Val[2],PWM_Val[3]);		//���PWM���
		OLED_Put6x12Num(32,16,PWM_Val[0],5,LIGHT);//��ʾ��4·��PWM�������
		OLED_Put6x12Num(32,32,PWM_Val[1],5,LIGHT);//��ʾ��1·��PWM�������
		OLED_Put6x12Num(98,16,PWM_Val[2],5,LIGHT);//��ʾ��3·��PWM�������
		OLED_Put6x12Num(98,32,PWM_Val[3],5,LIGHT);//��ʾ��2·��PWM�������
		OLED_Refresh_AllGDRAM(); 	//ˢ����ʾ
		delay_ms(500);
		
		PWM_Val[0]=1500;		//��1·PWM�������
		PWM_Val[1]=1800;		//��2·PWM�������
		PWM_Val[2]=1000;		//��3·PWM�������
		PWM_Val[3]=1200;		//��4·PWM�������
		Set_Servo(PWM_Val[0],PWM_Val[1],PWM_Val[2],PWM_Val[3]);//���PWM���	
		OLED_Put6x12Num(32,16,PWM_Val[0],5,LIGHT);//��ʾ��4·��PWM�������
		OLED_Put6x12Num(32,32,PWM_Val[1],5,LIGHT);//��ʾ��1·��PWM�������
		OLED_Put6x12Num(98,16,PWM_Val[2],5,LIGHT);//��ʾ��3·��PWM�������
		OLED_Put6x12Num(98,32,PWM_Val[3],5,LIGHT);//��ʾ��2·��PWM�������
		OLED_Refresh_AllGDRAM(); 	//ˢ����ʾ	
		delay_ms(500);		
	} 
}





/*************************************END************************************/







