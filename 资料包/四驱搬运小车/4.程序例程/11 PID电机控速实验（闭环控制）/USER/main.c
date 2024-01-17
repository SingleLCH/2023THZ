/***************************************************************************
������ɹ�ѧϰʹ�ã�����������Ҳ������������;��������������飬��ӭ���罻��ָ��
Ӳ���汾����·���ֶ๦�ܿ��ư�
����оƬ��STM32F103RCT6
�������ƣ�PID������٣��ջ����ƣ�--������
����������ͨ��PID�ջ����Ƶ��ת��


*****************************************************************************/



#include "sys.h"
#include "delay.h"
#include "motor.h"
#include "encoder.h"
#include "timer.h"
#include "oled_user.h"
#include "control.h"

int main(void)
{		
  Stm32_Clock_Init(9);				//ϵͳʱ������
	JTAG_Set(JTAG_SWD_DISABLE); //�ȹر�JATG SWD�˿ڡ�4·�������У���Ҫռ��JTAG�˿��еĲ�������
	AFIO->MAPR|=0x1<<8;					//timer2������ӳ�䣬CH1---PA15,CH2---PB3�������������4��timer���й��õ����ţ����Ҫ��ӳ�䣩
	JTAG_Set(SWD_ENABLE); 			//ֻ����SWD�˿ڡ������뿪������Ȼ�ϵ���޷����Ի����س��򡣴�ʱ����BOOT0Ϊ�ߵ�ƽ�ſ��ԣ�
	delay_init(72);							//��ʱ��ʼ��
	Motor_Init();								//PWM�����ʼ��	
  Encoder_Init();							//��ʼ��������
 	Set_Motor(0,0,0,0);					//���PWM���
	PID_Init();									//PID��ʼ��
	TIM6_Int_Init(5000,72);	 		//5ms�ж����
	OLED_Init();								//OLED��ʼ��
	
	OLED_Put12x12CNstr(25,0,"PID�������",LIGHT);	//��ʾ����
	OLED_Put12x12CNstr(0,15," MOTOR-4 ",DARK);		//��ʾ��4·�ĵ��
	OLED_Put12x12CNstr(0,40," MOTOR-1 ",DARK);		//��ʾ��1·�ĵ��
	OLED_Put12x12CNstr(66,15," MOTOR-3 ",DARK);		//��ʾ��3·�ĵ��
	OLED_Put12x12CNstr(66,40," MOTOR-2 ",DARK);		//��ʾ��2·�ĵ��

	Motor_Speed[0]=50;Motor_Speed[1]=-50;Motor_Speed[2]=100;Motor_Speed[3]=-100;	//����ʼ�ٶ�ֵ
	
	OLED_Put5x7Num(32,28,Motor_Speed[3],5,LIGHT);		//��ʾ��4·������趨�ٶ���ֵ	
	OLED_Put5x7Num(32,54,Motor_Speed[0],5,LIGHT); 	//��ʾ��1·������趨�ٶ���ֵ	
	OLED_Put5x7Num(100,28,Motor_Speed[2],5,LIGHT);	//��ʾ��3·������趨�ٶ���ֵ	
	OLED_Put5x7Num(100,54,Motor_Speed[1],5,LIGHT);	//��ʾ��2·������趨�ٶ���ֵ	
	
	OLED_Refresh_AllGDRAM(); 											//ˢ����ʾ
	while(1)
	{
		OLED_Put5x7Num(0,28,Encoder[3],5,LIGHT);	//��ʾ��4·�����ʵʱ�ٶ�
		OLED_Put5x7Num(0,54,Encoder[0],5,LIGHT); 	//��ʾ��1·�����ʵʱ�ٶ�
		OLED_Put5x7Num(66,28,Encoder[2],5,LIGHT);	//��ʾ��3·�����ʵʱ�ٶ�
		OLED_Put5x7Num(66,54,Encoder[1],5,LIGHT);	//��ʾ��2·�����ʵʱ�ٶ�
		OLED_Refresh_AllGDRAM(); 	//ˢ����ʾ
		delay_ms(50);
	} 
}





/*************************************END************************************/







