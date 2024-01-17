/***************************************************************************
本程序可供学习使用，经作者允许，也可用于其他用途。如有意见及建议，欢迎来电交流指教
硬件版本：四路麦轮多功能控制板
主控芯片：STM32F103RCT6
程序名称：STM32麦轮全向移动小车程序--控制程序
功能描述：PID控制四路直流减速电机转动



*****************************************************************************/

#include "UI.h"


int Motor_Speed[4];			//电机速度值
int Motor_DrctAdj[4];		//电机转向调整

PID PID_Motor[4];

u8 ARMED=0;


unsigned char Contorl_stick=0;

//定时器6中断服务程序	 
void TIM6_IRQHandler(void)
{ 		
	unsigned char nmr=0;	
	if(TIM6->SR&0X0001)//溢出中断
	{		
		if(WIFI_CK>0) WIFI_CK--;
		recv_msg(); // 接收wifi数据消息
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
			Read_Encoder();//读编码器		
	////////////////////////////////	
			if(ARMED)		
			{
				for(nmr=0;nmr<4;nmr++)
				{
					PID_Motor[nmr].error=Encoder[nmr]-Motor_Speed[nmr];
					//输出的P值
					PID_Motor[nmr].pout = PID_Motor[nmr].Pdat * PID_Motor[nmr].error;
					//输出的I值
					PID_Motor[nmr].iout += (PID_Motor[nmr].Idat*0.05) * PID_Motor[nmr].error;
					PID_Motor[nmr].iout = Get_MxMi(PID_Motor[nmr].iout,200,-200);//判读I是否超出范围
					//输出的D值
					PID_Motor[nmr].dout = PID_Motor[nmr].Ddat*0.05*(PID_Motor[nmr].error-PID_Motor[nmr].Last_error);	
					PID_Motor[nmr].Last_error=PID_Motor[nmr].error;	
					//PID三个值融合
					PID_Motor[nmr].OUT += PID_Motor[nmr].pout + PID_Motor[nmr].iout +PID_Motor[nmr].dout;////P,I,D值相加
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
		LED_Control();				//控制LED和蜂鸣器
		
		time_tick++;	
		if(time_tick>=250)time_tick=0;
		if(time_tick%2 == 0)//10ms  --  100HZ进行一次
		{	
			
		}
		Control_Flag=1;				
		if(KEY==0)	//按键按下
		{
			if(Keytimes<300)	Keytimes++;
		}
		else
		{
			if(Keytimes>30 && Keytimes<300)  {Kaysta=1;	BEEP_state=BEEP_FAST1FLASH;BEEP_timetick=1;}//标记按键短按
			if(Keytimes==300)	{ Kaysta=2;BEEP_state=BEEP_FAST1FLASH;BEEP_timetick=1;}				//标记按键长按
			Keytimes=0;
		}		
	}
	TIM6->SR&=~(1<<0);//清除中断标志位 	    
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


