/***************************************************************************
������ɹ�ѧϰʹ�ã�����������Ҳ������������;��������������飬��ӭ���罻��ָ��
Ӳ���汾����·���ֶ๦�ܿ��ư�
����оƬ��STM32F103RCT6
�������ƣ�STM32����ȫ���ƶ�С������--���Ƴ���
����������PID������·ֱ�����ٵ��ת��



*****************************************************************************/

#include "UI.h"


int Motor_Speed[4];			//����ٶ�ֵ
int Motor_DrctAdj[4];		//���ת�����

PID PID_Motor[4];

u8 ARMED=0;


unsigned char Contorl_stick=0;

//��ʱ��6�жϷ������	 
void TIM6_IRQHandler(void)
{ 		
	unsigned char nmr=0;	
	if(TIM6->SR&0X0001)//����ж�
	{		
		if(WIFI_CK>0) WIFI_CK--;
		recv_msg(); // ����wifi������Ϣ
		switch(WorkMode)
		{
			case 14:	//WIFI
				Car_Control(Blue_RC[1]*3/5,Blue_RC[0]*3/5,Blue_RC[2]*2/5);
				break;
			default:			break;
		}
		Contorl_stick++;
		if(Contorl_stick==4)
		{
			Contorl_stick=0;
			Read_Encoder();//��������		
	////////////////////////////////	
			if(ARMED)		
			{
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
			else
			{
				Set_Motor(0, 0, 0, 0);
			}
			LeftKnob	+=	Encoder[0];
			RightKnob	+=	Encoder[1];
			if(LeftKnob>5000)		LeftKnob=5000;
			if(LeftKnob<-5000)	LeftKnob=-5000;
			if(RightKnob>5000)	RightKnob=5000;
			if(RightKnob<-5000)	RightKnob=-5000;	
		}
		
		BEEP_Control();
		LED_Control();				//����LED�ͷ�����
		
		time_tick++;	
		if(time_tick>=250)time_tick=0;
		if(time_tick%2 == 0)//10ms  --  100HZ����һ��
		{	
			
		}
		Control_Flag=1;				
		if(KEY==0)	//��������
		{
			if(Keytimes<300)	Keytimes++;
		}
		else
		{
			if(Keytimes>30 && Keytimes<300)  {Kaysta=1;	BEEP_state=BEEP_FAST1FLASH;BEEP_timetick=1;}//��ǰ����̰�
			if(Keytimes==300)	{ Kaysta=2;BEEP_state=BEEP_FAST1FLASH;BEEP_timetick=1;}				//��ǰ�������
			Keytimes=0;
		}		
	}
	TIM6->SR&=~(1<<0);//����жϱ�־λ 	    
}


void Car_Control(int Y_Move,int X_Move,int Yaw)
{
	Motor_Speed[0] =  (-Y_Move + X_Move + Yaw)*Motor_DrctAdj[0];	Motor_Speed[2] = (Y_Move - X_Move + Yaw)*Motor_DrctAdj[2];
	Motor_Speed[1] = 	(-Y_Move - X_Move + Yaw)*Motor_DrctAdj[1];	Motor_Speed[3] = (Y_Move + X_Move + Yaw)*Motor_DrctAdj[3];	
}

void PID_Init(void)
{	
	Flash_Read_PID();
	
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


