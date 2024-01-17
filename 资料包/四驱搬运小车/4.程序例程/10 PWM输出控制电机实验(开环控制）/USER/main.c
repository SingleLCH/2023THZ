/***************************************************************************
������ɹ�ѧϰʹ�ã�����������Ҳ������������;��������������飬��ӭ���罻��ָ��
Ӳ��˵������·���ֶ๦�ܿ��ư�
����оƬ��STM32F103RCT6
�������ƣ�PWM������Ƶ��ʵ��--������
����������PWM������Ƶ�����������ƣ�


*****************************************************************************/



#include "sys.h"
#include "delay.h"
#include "motor.h"
#include "oled_user.h"

int PWM_Val[4]={0,0,0,0}; //4·���PWMֵ

int main(void)
{		
  Stm32_Clock_Init(9);				//ϵͳʱ������
	JTAG_Set(JTAG_SWD_DISABLE); //�ȹر�JATG SWD�˿ڡ�4·�������У���Ҫռ��JTAG�˿��еĲ�������
	AFIO->MAPR|=0x1<<8;					//timer2������ӳ�䣬CH1---PA15,CH2---PB3�������������4��timer���й��õ����ţ����Ҫ��ӳ�䣩
	JTAG_Set(SWD_ENABLE); 			//ֻ����SWD�˿ڡ������뿪������Ȼ�ϵ���޷����Ի����س��򡣴�ʱ����BOOT0Ϊ�ߵ�ƽ�ſ��ԣ�
	delay_init(72);							//��ʱ��ʼ��
	Motor_Init();								//PWM�����ʼ��	 
 	Set_Motor(0,0,0,0);					//���PWM���
	
	OLED_Init();
	OLED_Put12x12CNstr(25,0,"PWM���Ƶ��",LIGHT);	//��ʾ����
	OLED_Put12x12CNstr(0,16,"PWM4:",LIGHT);		//��ʾ��4·��PWM	
	OLED_Put12x12CNstr(0,32,"PWM1:",LIGHT);		//��ʾ��1·��PWM	
	OLED_Put12x12CNstr(66,16,"PWM3:",LIGHT);	//��ʾ��3·��PWM
	OLED_Put12x12CNstr(66,32,"PWM2:",LIGHT);	//��ʾ��2·��PWM	
	PWM_Val[0]=1000;PWM_Val[1]=-1000;PWM_Val[2]=3000;PWM_Val[3]=-3000;	//����ʼPWMֵ
	while(1)
	{	
		PWM_Val[0]+=5;		//��1·PWMֵ����
		PWM_Val[1]+=10;		//��2·PWMֵ����
		PWM_Val[2]+=4;		//��3·PWMֵ����
		PWM_Val[3]+=20;		//��4·PWMֵ����
		if(PWM_Val[0]>PWM_MAX) PWM_Val[0]=0;
		if(PWM_Val[1]>PWM_MAX) PWM_Val[1]=0;
		if(PWM_Val[2]>PWM_MAX) PWM_Val[2]=0;
		if(PWM_Val[3]>PWM_MAX) PWM_Val[3]=0;
		
		Set_Motor(PWM_Val[0],PWM_Val[1],PWM_Val[2],PWM_Val[3]);		//���PWM���
		
		OLED_Put6x12Num(32,16,PWM_Val[3]/10,5,LIGHT);	//��ʾ��4·��PWM��ֵ	
		OLED_Put6x12Num(32,32,PWM_Val[0]/10,5,LIGHT); //��ʾ��1·��PWM��ֵ	
		OLED_Put6x12Num(98,16,PWM_Val[2]/10,5,LIGHT);	//��ʾ��3·��PWM��ֵ	
		OLED_Put6x12Num(98,32,PWM_Val[1]/10,5,LIGHT);	//��ʾ��2·��PWM��ֵ	
		OLED_Refresh_AllGDRAM(); 	//ˢ����ʾ
		delay_ms(5);
	} 
}





/*************************************END************************************/







