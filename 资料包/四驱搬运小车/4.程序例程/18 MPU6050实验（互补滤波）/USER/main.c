/***************************************************************************
������ɹ�ѧϰʹ�ã�����������Ҳ������������;��������������飬��ӭ���罻��ָ��
Ӳ���汾����·���ֶ๦�ܿ��ư�
����оƬ��STM32F103RCT6
�������ƣ�MPU6050ʵ��--������
����������MPU6050 DMP��ʽ�����Ƕ�


*****************************************************************************/

#include "sys.h"
#include "delay.h"
#include "oled_user.h"
#include "imu_filter.h"
#include "timer.h"

int main(void)
{		
  Stm32_Clock_Init(9);//ϵͳʱ������
	delay_init(72);			//��ʱ��ʼ��
	
	OLED_Init();
	OLED_Put12x12CNstr(10,0,"MPU�Ƕ���ʾ(Filter)",LIGHT);							//��ʾ����
	OLED_Put12x12CNstr(10,16,"Pitch�Ƕ�:",LIGHT);											//��ʾPitch�ĽǶ�
	OLED_Put12x12CNstr(10,32,"Roll �Ƕ�:",LIGHT);											//��ʾRoll�ĽǶ�
	OLED_Put12x12CNstr(16,52,"<��������˿Ƽ�>",LIGHT);	
	OLED_Refresh_AllGDRAM(); 	//ˢ����ʾ
 	MPU6050_initialize();	//6050DMP��ʼ��
	TIM3_Int_Init(4999,71);
	
	while(1)
	{
		
		OLED_Put6x12Num1dot(80,16,Pitch*10,1);
		OLED_Put6x12Num1dot(80,32,Roll*10,1);		//��ʾ�Ƕ�MPU6050_accel[2] yaw_gyro_now
		OLED_Refresh_AllGDRAM(); 	//ˢ����ʾ
		delay_ms(10);
	} 
}

// void EXTI15_10_IRQHandler(void)
// {	
//  	IMU_Filter();           
// 	EXTI->PR=1<<14;  //���LINE3�ϵ��жϱ�־λ
// }

//��ʱ��3�жϷ������	 
void TIM3_IRQHandler(void)
{ 		    		  			    
	if(TIM3->SR&0X0001)//����ж�
	{	
		IMU_Filter();		
	}
	TIM3->SR&=~(1<<0);//����жϱ�־λ 	    
}


/*************************************END************************************/







