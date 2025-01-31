#include "UI.h"
extern uint16 ServoPwmDuty[8];
#define MIN_DIS_AVOID  70
u8 time=0;
//#define	LED PDout(2)  
u8 led_time=0;
int Motor_Speed[4];			//电机速度值
int Motor_DrctAdj[4];		//电机转向调整

unsigned char LX=127;
unsigned char LY=127;

PID PID_Motor[4];

u8 ARMED=0;
float Voltage_Count,Voltage_All;  //电压采样相关变量
unsigned char nmr=0;

short int RC_Last=0;
unsigned char Contorl_stick=0;
unsigned char PS2_Stick=0;
unsigned char PS2_Flag=0;
unsigned char ServoCount=0;

u32 key;

void Ps2_Control(void);
void Avoid_Control(void);

void TIM6_IRQHandler(void)
{ 		    		  			    
	if(TIM6->SR&0X0001)//溢出中断
	{		
		Control_Flag=1;		
		if(++ServoCount>=4)
		{
			ServoCount=0;
			ServoPwmDutyCompare();
		}
		Voltage_All+=Get_Battery_vot();
		if(++Voltage_Count==100) Voltage=Voltage_All/100,Voltage_All=0,Voltage_Count=0;//求平均值 获取电池电压	   
		if(KEY==0)	//按键按下
		{
			if(Keytimes<300)	Keytimes++;
		}
		else
		{
			if(Keytimes>30 && Keytimes<300)  {Kaysta=1;	N_Beep(1);}//标记按键短按
			if(Keytimes==300)	{ Kaysta=2;N_Beep(2);}				//标记按键长按
			Keytimes=0;
		}	
		if(PS2_Stick<8) PS2_Stick++;
		if(PS2_Stick==4)
		{	
			if(WorkMode==1){
				if(!PS2_RedLight()) //判断手柄是否为红灯模式，是，指示灯LED点亮
					PS2_Flag=1;	//标记为红灯模式
				else
					PS2_Flag=0; //标记为绿灯模式
			}
		}
		if(PS2_Stick==8)
		{	
			if(WorkMode==1){
				if(PS2_Flag)	//是红灯模式	
				{	
					key=PS2_DataKey();	 //手柄按键捕获处理
				}
			}
				PS2_Stick=0;
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
			//	Set_Motor(0, 0, 0, 0);
			}
		}
	
		if(WorkMode==2)
		{
			App_control_car();
		}
		if(WorkMode==1)
		{
			Ps2_Control();
		}
		if(WorkMode==3)
		{
			Auto_Traversing();
		}
		if(WorkMode==4)
		{
			Avoid_Control();
		}
		if(WorkMode==5)
		{
			 Line_Control();
		}
	}	
	TIM6->SR&=~(1<<0);//清除中断标志位 	    
}

void Ps2_Control(void)
{	
		uint16 servoPwmDutyTemp;	
		PS2_CH[0]=	PS2_AnologData(PSS_RY);						
		PS2_CH[1]=	PS2_AnologData(PSS_RX);
		PS2_CH[2]=	PS2_AnologData(PSS_LX);
		Car_Control((PS2_CH[1]-127)*3/5,(127-PS2_CH[0])*3/5,(PS2_CH[2]-127)/2);
		switch (key)
		{
			case PSB_PAD_DOWN:
				servoPwmDutyTemp = ServoPwmDuty[1];
				servoPwmDutyTemp += 2;
				if(servoPwmDutyTemp >= 2500)
				{
					servoPwmDutyTemp = 2500;
				}
				ServoPwmDuty[1] = servoPwmDutyTemp;
				break; 
			case PSB_PAD_UP:
				servoPwmDutyTemp = ServoPwmDuty[1];
				servoPwmDutyTemp -= 2;
				if(servoPwmDutyTemp <= 500)
				{
					servoPwmDutyTemp = 500;
				}
				ServoPwmDuty[1] = servoPwmDutyTemp;
				break;
			case PSB_PAD_RIGHT:
				servoPwmDutyTemp = ServoPwmDuty[0];
				servoPwmDutyTemp += 2;
				if(servoPwmDutyTemp >= 2500)
				{
					servoPwmDutyTemp = 2500;
				}
				ServoPwmDuty[0] = servoPwmDutyTemp;
				break; 
				
			case PSB_PAD_LEFT:
				servoPwmDutyTemp = ServoPwmDuty[0];
				servoPwmDutyTemp -= 2;
				if(servoPwmDutyTemp <= 500)
				{
					servoPwmDutyTemp = 500;
				}
				ServoPwmDuty[0] = servoPwmDutyTemp;
				break;

			case PSB_TRIANGLE:
				servoPwmDutyTemp = ServoPwmDuty[2];
				servoPwmDutyTemp += 2;
				if(servoPwmDutyTemp >= 2500)
				{
					servoPwmDutyTemp = 2500;
				}
				ServoPwmDuty[2] = servoPwmDutyTemp;
				break;
			case PSB_CROSS:
				servoPwmDutyTemp = ServoPwmDuty[2];
				servoPwmDutyTemp -= 2;
				if(servoPwmDutyTemp <= 500)
				{
					servoPwmDutyTemp = 500;
				}
				ServoPwmDuty[2] = servoPwmDutyTemp;
				break;
			case PSB_PINK:
				servoPwmDutyTemp = ServoPwmDuty[3];
				servoPwmDutyTemp += 2;
				if(servoPwmDutyTemp >= 2500)
				{
					servoPwmDutyTemp = 2500;
				}
				ServoPwmDuty[3] = servoPwmDutyTemp;
				break;
			case PSB_CIRCLE:
				servoPwmDutyTemp = ServoPwmDuty[3];
				servoPwmDutyTemp -= 2;
				if(servoPwmDutyTemp <= 500)
				{
					servoPwmDutyTemp = 500;
				}
				ServoPwmDuty[3] = servoPwmDutyTemp;
				break;

			case PSB_L1:
				servoPwmDutyTemp = ServoPwmDuty[4];
				servoPwmDutyTemp += 2;
				if(servoPwmDutyTemp >= 2500)
				{
					servoPwmDutyTemp = 2500;
				}
				ServoPwmDuty[4] = servoPwmDutyTemp;
				break;
			case PSB_L2:
				servoPwmDutyTemp = ServoPwmDuty[4];
				servoPwmDutyTemp -= 2;
				if(servoPwmDutyTemp <= 500)
				{
					servoPwmDutyTemp = 500;
				}
				ServoPwmDuty[4] = servoPwmDutyTemp;
				break;
			case PSB_R1:
				servoPwmDutyTemp = ServoPwmDuty[5];
				servoPwmDutyTemp += 2;
				if(servoPwmDutyTemp >= 2500)
				{
					servoPwmDutyTemp = 2500;
				}
				ServoPwmDuty[5] = servoPwmDutyTemp;
				break;
			case PSB_R2:
				servoPwmDutyTemp = ServoPwmDuty[5];
				servoPwmDutyTemp -= 2;
				if(servoPwmDutyTemp <= 500)
				{
					servoPwmDutyTemp = 500;
				}
				ServoPwmDuty[5] = servoPwmDutyTemp;
				break;
			default:break;
		}

}

void Line_Control(void)
{
	 App_control_car();
	 Car_Control((127-LY)*3/5,(LX-127)*3/5,(127-127)/2);
}

void Avoid_Control(void)
{
	 if(Distance1>=MIN_DIS_AVOID)
				//map(Distance1,MIN_DIS_AVOID,150,40,70);
				Set_Motor(-2000, 2000,2000, -2000);
  else if(Distance1>50 && Distance1<MIN_DIS_AVOID){
				Set_Motor(2000, 2000,2000, 2000);
  }
  else {
			Set_Motor(2000, -2000,-2000, 2000);
		
  }
}

void Car_Control(int Y_Move,int X_Move,int Yaw)
{
	Motor_Speed[0] =  (-Y_Move + X_Move + Yaw)*Motor_DrctAdj[0];	Motor_Speed[2] = (Y_Move - X_Move + Yaw)*Motor_DrctAdj[2];
	Motor_Speed[1] = 	(-Y_Move - X_Move + Yaw)*Motor_DrctAdj[1];	Motor_Speed[3] = (Y_Move + X_Move + Yaw)*Motor_DrctAdj[3];	
}

void PID_Init(void)
{	

	PID_Motor[0].Pdat=16.0;	PID_Motor[0].Idat=0.0;	PID_Motor[0].Ddat=4.0;	
	PID_Motor[1].Pdat=16.0;	PID_Motor[1].Idat=0.0;	PID_Motor[1].Ddat=4.0;
	PID_Motor[2].Pdat=16.0;	PID_Motor[2].Idat=0.0;	PID_Motor[2].Ddat=4.0;
	PID_Motor[3].Pdat=16.0;	PID_Motor[3].Idat=0.0;	PID_Motor[3].Ddat=4.0;
	Motor_DrctAdj[0]=1;Motor_DrctAdj[1]=1;Motor_DrctAdj[2]=1;Motor_DrctAdj[3]=1;


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

float map(float val,float I_Min,float I_Max,float O_Min,float O_Max)
{
	return (((val-I_Min)*((O_Max-O_Min)/(I_Max-I_Min)))+O_Min);
}



