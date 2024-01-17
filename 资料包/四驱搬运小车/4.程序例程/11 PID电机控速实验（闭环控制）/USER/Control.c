/***************************************************************************
������ɹ�ѧϰʹ�ã�����������Ҳ������������;��������������飬��ӭ���罻��ָ��
Ӳ���汾����·���ֶ๦�ܿ��ư�
����оƬ��STM32F103RCT6
�������ƣ�PID������٣��ջ����ƣ�--���Ƴ���
����������PID������·ֱ�����ٵ��ת��


*****************************************************************************/


#include "sys.h"
#include "control.h"

int Motor_Speed[4];			//����ٶ�ֵ

PID PID_Motor[4];

unsigned char nmr=0;
unsigned char Contorl_stick=0;


//��ʱ��6�жϷ������	 
void TIM6_IRQHandler(void)
{ 		    		  			    
	if(TIM6->SR&0X0001)//����ж�
	{			
		Contorl_stick++;
		if(Contorl_stick==4)
		{
			Contorl_stick=0;
			Read_Encoder();//��������		
	////////////////////////////////	
			for(nmr=0;nmr<4;nmr++)
			{
				PID_Motor[nmr].error=Encoder[nmr]-Motor_Speed[nmr];
				//�����Pֵ
				PID_Motor[nmr].pout = PID_Motor[nmr].Pdat * PID_Motor[nmr].error;
				//�����Iֵ
				PID_Motor[nmr].iout += (PID_Motor[nmr].Idat*0.05) * PID_Motor[nmr].error;
				PID_Motor[nmr].iout = Get_MxMi(PID_Motor[nmr].iout,200,-200);//�ж�I�Ƿ񳬳���Χ
				//�����Dֵ
				PID_Motor[nmr].dout = PID_Motor[nmr].Ddat*0.05*(PID_Motor[nmr].error-PID_Motor[nmr].Last_error);	
				PID_Motor[nmr].Last_error=PID_Motor[nmr].error;	
				//PID����ֵ�ں�
				PID_Motor[nmr].OUT += PID_Motor[nmr].pout + PID_Motor[nmr].iout +PID_Motor[nmr].dout;////P,I,Dֵ���
				PID_Motor[nmr].OUT=Get_MxMi(PID_Motor[nmr].OUT,4599,-4599);
			}
			Set_Motor(PID_Motor[0].OUT, PID_Motor[1].OUT, PID_Motor[2].OUT, PID_Motor[3].OUT);
		}
	}
	TIM6->SR&=~(1<<0);//����жϱ�־λ 	    
}


void Car_Control(int Y_Move,int X_Move,int Yaw)
{
	Motor_Speed[0] =  (-Y_Move + X_Move + Yaw);	Motor_Speed[2] = (Y_Move - X_Move + Yaw);
	Motor_Speed[1] = 	(-Y_Move - X_Move + Yaw);	Motor_Speed[3] = (Y_Move + X_Move + Yaw);	
}

void PID_Init(void)
{	
	PID_Motor[0].Pdat=16.0;	PID_Motor[0].Idat=0.0;	PID_Motor[0].Ddat=4.0;	
	PID_Motor[1].Pdat=16.0;	PID_Motor[1].Idat=0.0;	PID_Motor[1].Ddat=4.0;
	PID_Motor[2].Pdat=16.0;	PID_Motor[2].Idat=0.0;	PID_Motor[2].Ddat=4.0;
	PID_Motor[3].Pdat=16.0;	PID_Motor[3].Idat=0.0;	PID_Motor[3].Ddat=4.0;
	
	PID_Motor[0].iout=0;
	PID_Motor[1].iout=0;
	PID_Motor[2].iout=0;
	PID_Motor[3].iout=0;
	
	PID_Motor[0].OUT=0;
	PID_Motor[1].OUT=0;
	PID_Motor[2].OUT=0;
	PID_Motor[3].OUT=0;
	
	PID_Motor[0].error=0;
	PID_Motor[1].error=0;
	PID_Motor[2].error=0;
	PID_Motor[3].error=0;
	
	PID_Motor[0].Last_error=0;
	PID_Motor[1].Last_error=0;
	PID_Motor[2].Last_error=0;
	PID_Motor[3].Last_error=0;

}

float Get_MxMi(float num,float max,float min)
{
	if(num>max)
		return max;
	else if(num<min)
		return min;
	else
		return num;
}

/*************************************END************************************/
