/***************************************************************************
������ɹ�ѧϰʹ�ã�����������Ҳ������������;��������������飬��ӭ���罻��ָ��
Ӳ���汾����·���ֶ๦�ܿ��ư�
����оƬ��STM32F103RCT6
�������ƣ�STM32����ȫ���ƶ�С������--RC��ģң��������
����������������·PWM�����źţ�������ģң�������ĸ�ͨ����ң����ֵ��


*****************************************************************************/


#include "rc.h"
#include "delay.h"
#include "usart.h"

//��ʱ��8ͨ�����벶������
u8  TIM1CH1_CAPTURE_STA=0;	//ͨ��1���벶��״̬		  �ø���λ�������־������λ���������  				
u16	TIM1CH1_CAPTURE_UPVAL;	//ͨ��1���벶��ֵ
u16	TIM1CH1_CAPTURE_DOWNVAL;//ͨ��1���벶��ֵ

u8  TIM1CH2_CAPTURE_STA=0;	//ͨ��2���벶��״̬		    				
u16	TIM1CH2_CAPTURE_UPVAL;	//ͨ��2���벶��ֵ
u16	TIM1CH2_CAPTURE_DOWNVAL;//ͨ��2���벶��ֵ

u8  TIM1CH3_CAPTURE_STA=0;	//ͨ��3���벶��״̬		    				
u16	TIM1CH3_CAPTURE_UPVAL;	//ͨ��3���벶��ֵ
u16	TIM1CH3_CAPTURE_DOWNVAL;//ͨ��3���벶��ֵ

u8  TIM1CH4_CAPTURE_STA=0;	//ͨ��4���벶��״̬		    				
u16	TIM1CH4_CAPTURE_UPVAL;	//ͨ��4���벶��ֵ
u16	TIM1CH4_CAPTURE_DOWNVAL;//ͨ��4���벶��ֵ

u16 RC_CH[4]={1500,1500,1000,1500};


void RC_Init(void)
{
	MY_NVIC_Init(1,0,TIM1_CC_IRQn,2);//��ռ0�������ȼ�3����2	

	RCC->APB2ENR|=1<<0;			//ʹ�ܸ���ʱ��
	RCC->APB2ENR|=1<<11;	//TIM1ʱ��ʹ�� 
 	RCC->APB2ENR|=1<<2;    	//ʹ��PORTAʱ��	
	  	 
	GPIOA->CRH&=0XFFFF0000;	//PA8,9,10,11����
	GPIOA->CRH|=0X00008888;	//�������� 
	GPIOA->ODR|=0XF<<8;		//PA8,9,10,11����

	GPIOA->CRL&=0XFF0FFFFF;   //PA5����Ϊǿ����
	GPIOA->CRL|=0X00300000;  	   

	RC_EN=0;
	
 	TIM1->ARR=20000-1;	//�趨�������Զ���װֵ  
	TIM1->PSC=(72 - 1);	//ʱ��Ԥ��Ƶ�� 72M/72
	TIM1->CR1&=~(3<<8);	//����ʱ�ӷ�Ƶϵ��������Ƶ
	TIM1->CR1&=~(1<<4);	//���ϼ���ģʽ


	TIM1->CCMR1|=1<<0;		//CC1S=01 	ѡ������� IC1ӳ�䵽TI1��
	TIM1->CCER|=0<<1; 		//CC1P=0	�����ز���
 	TIM1->CCER|=1<<0; 		//CC1E=1 	�������������ֵ������Ĵ�����	
 	TIM1->CCMR1|=0<<2; 		//IC1PS=00 	���������Ƶ,����Ƶ 
	TIM1->CCMR1|=0<<4; 		//IC1F=0000 ���������˲��� ���˲�


	TIM1->CCMR1|=1<<8;		//CC2S=01 	ѡ������� IC1ӳ�䵽TI1��
	TIM1->CCER|=0<<5; 		//CC2P=0	�����ز���
	TIM1->CCER|=1<<4; 		//CC2E=1 	�������������ֵ������Ĵ�����
 	TIM1->CCMR1|=0<<10; 	//IC2PS=00 	���������Ƶ,����Ƶ 
 	TIM1->CCMR1|=0<<12; 	//IC2F=0000 ���������˲��� ���˲�

	TIM1->CCMR2|=1<<0;		//CC3S=01 	ѡ������� IC1ӳ�䵽TI1��
	TIM1->CCER|=0<<9; 		//CC3P=0	�����ز���
	TIM1->CCER|=1<<8; 		//CC3E=1 	�������������ֵ������Ĵ�����
 	TIM1->CCMR2|=0<<2; 		//IC3PS=00 	���������Ƶ,����Ƶ 
 	TIM1->CCMR2|=0<<4; 		//IC3F=0000 ���������˲��� ���˲�

	TIM1->CCMR2|=1<<8;		//CC4S=01 	ѡ������� IC1ӳ�䵽TI1��
	TIM1->CCER|=0<<13; 		//CC4P=0	�����ز���
	TIM1->CCER|=1<<12; 		//CC4E=1 	�������������ֵ������Ĵ�����
 	TIM1->CCMR2|=0<<10; 	//IC4PS=00 	���������Ƶ,����Ƶ 
 	TIM1->CCMR2|=0<<12; 	//IC4F=0000 ���������˲��� ���˲�

	TIM1->DIER|=1<<1;   	//������1�ж�	
	TIM1->DIER|=1<<2;   	//������2�ж�	
	TIM1->DIER|=1<<3;   	//������3�ж�	
	TIM1->DIER|=1<<4;   	//������4�ж�						
	//TIM1->DIER|=1<<0;   	//��������ж�	
	TIM1->CR1|=0x01;    	//ʹ�ܶ�ʱ��4
}

//��ʱ��8�жϷ������	 
void TIM1_CC_IRQHandler(void)
{
	if(TIM1->SR&0X0002)//����1���������¼�
	{
		TIM1->SR&=~(1<<1);//�������1��־λ
		
		if(TIM1CH1_CAPTURE_STA == 0)//����������
		{
			TIM1CH1_CAPTURE_UPVAL =TIM1->CCR1;//��ȡ�����ص�����
			
			TIM1CH1_CAPTURE_STA = 1;		//����Բ�����������
			TIM1->CCER|=1<<1; 				//CC1P=1 ����Ϊ�½��ز���
		}
		else                        //�����½��� (�Ѿ�����һ�������ĸߵ�ƽ���壡)
		{
			TIM1CH1_CAPTURE_DOWNVAL = TIM1->CCR1;//��ȡ�½��ص�����
			
			//�ж��Ƿ񳬳����������,����ߵ�ƽ����ʱ��us
			if(TIM1CH1_CAPTURE_DOWNVAL<TIM1CH1_CAPTURE_UPVAL)
			{
				RC_CH[0] = 20000 - TIM1CH1_CAPTURE_UPVAL + TIM1CH1_CAPTURE_DOWNVAL;
			}
			else
			{
				RC_CH[0] = TIM1CH1_CAPTURE_DOWNVAL- TIM1CH1_CAPTURE_UPVAL;
			}
			
			TIM1CH1_CAPTURE_STA = 0;
			TIM1->CCER&=~(1<<1);			//CC1P=0 ����Ϊ�����ز���
		}
	}	

	if(TIM1->SR&0X0004)//����2���������¼�
	{
		TIM1->SR&=~(1<<2);//�������2��־λ
		
		if(TIM1CH2_CAPTURE_STA == 0)//����������
		{
			TIM1CH2_CAPTURE_UPVAL =TIM1->CCR2;//��ȡ�����ص�����
			
			TIM1CH2_CAPTURE_STA = 1;		//����Բ�����������
			TIM1->CCER|=1<<5; 				//CC2P=1 ����Ϊ�½��ز���
		}
		else                        //�����½��� (�Ѿ�����һ�������ĸߵ�ƽ���壡)
		{
			TIM1CH2_CAPTURE_DOWNVAL = TIM1->CCR2;//��ȡ�½��ص�����
			
			//�ж��Ƿ񳬳����������,����ߵ�ƽ����ʱ��us
			if(TIM1CH2_CAPTURE_DOWNVAL<TIM1CH2_CAPTURE_UPVAL)
			{
				RC_CH[1] = 20000 - TIM1CH2_CAPTURE_UPVAL + TIM1CH2_CAPTURE_DOWNVAL;
			}
			else
			{
				RC_CH[1] = TIM1CH2_CAPTURE_DOWNVAL- TIM1CH2_CAPTURE_UPVAL;
			}
			TIM1CH2_CAPTURE_STA = 0;
			TIM1->CCER&=~(1<<5);			//CC1P=0 ����Ϊ�����ز���
		}
	}
	
	if(TIM1->SR&0X0008)//����3���������¼�
	{
		TIM1->SR&=~(1<<3);//�������3��־λ
		
		if(TIM1CH3_CAPTURE_STA == 0)//����������
		{
			TIM1CH3_CAPTURE_UPVAL =TIM1->CCR3;//��ȡ�����ص�����
			
			TIM1CH3_CAPTURE_STA = 1;		//����Բ�����������
			TIM1->CCER|=1<<9; 				//CC3P=1 ����Ϊ�½��ز���
		}
		else                        //�����½��� (�Ѿ�����һ�������ĸߵ�ƽ���壡)
		{
			TIM1CH3_CAPTURE_DOWNVAL = TIM1->CCR3;//��ȡ�½��ص�����			
			//�ж��Ƿ񳬳����������,����ߵ�ƽ����ʱ��us
			if(TIM1CH3_CAPTURE_DOWNVAL<TIM1CH3_CAPTURE_UPVAL)
			{
				RC_CH[2] = 20000 - TIM1CH3_CAPTURE_UPVAL + TIM1CH3_CAPTURE_DOWNVAL;
			}
			else
			{
				RC_CH[2] = TIM1CH3_CAPTURE_DOWNVAL- TIM1CH3_CAPTURE_UPVAL;
			}
			
			TIM1CH3_CAPTURE_STA = 0;
			TIM1->CCER&=~(1<<9);			//CC1P=0 ����Ϊ�����ز���
		}
	}	
	if(TIM1->SR&0X0010)//����4���������¼�
	{
		TIM1->SR&=~(1<<4);//�������4��־λ
		
		if(TIM1CH4_CAPTURE_STA == 0)//����������
		{
			TIM1CH4_CAPTURE_UPVAL =TIM1->CCR4;//��ȡ�����ص�����
			
			TIM1CH4_CAPTURE_STA = 1;		//����Բ�����������
			TIM1->CCER|=1<<13; 				//CC4P=1 ����Ϊ�½��ز���
		}
		else                        //�����½��� (�Ѿ�����һ�������ĸߵ�ƽ���壡)
		{
			TIM1CH4_CAPTURE_DOWNVAL = TIM1->CCR4;//��ȡ�½��ص�����
			
			//�ж��Ƿ񳬳����������,����ߵ�ƽ����ʱ��us
			if(TIM1CH4_CAPTURE_DOWNVAL<TIM1CH4_CAPTURE_UPVAL)
			{
				RC_CH[3] = 20000 - TIM1CH4_CAPTURE_UPVAL + TIM1CH4_CAPTURE_DOWNVAL;
			}
			else
			{
				RC_CH[3] = TIM1CH4_CAPTURE_DOWNVAL- TIM1CH4_CAPTURE_UPVAL;
			}
			
			TIM1CH4_CAPTURE_STA = 0;
			TIM1->CCER&=~(1<<13);			//CC1P=0 ����Ϊ�����ز���			
		}
	}
}


/*************************************END************************************/


