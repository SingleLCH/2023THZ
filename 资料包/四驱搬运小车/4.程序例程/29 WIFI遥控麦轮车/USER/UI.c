/***************************************************************************
������ɹ�ѧϰʹ�ã�����������Ҳ������������;��������������飬��ӭ���罻��ָ��
Ӳ���汾����·���ֶ๦�ܿ��ư�
����оƬ��STM32F103RCT6
�������ƣ�STM32����ȫ���ƶ�С������--UI�������
����������UI�л����棬���������˻���������



*****************************************************************************/

#include "UI.h"

unsigned short int Keytimes=0;				//��������ʱ�����
unsigned char Kaysta=0;								//����״̬����
unsigned char WorkMode=0;							//����ģʽ����
unsigned int Volt=0;									//��ص�ѹ��������λ0.1V
char Menux=0;													//�˵�����
unsigned short int AdjustAngle_Flag=0;//�Ƕ�У׼��־λ
unsigned char BlueCK=0;								//�����źż��
short int Lang_x=0;										//��������
unsigned char UI_Display_Flag=0;			//��ǽ���д��һ��
short int	VoltLowWarn=0;							//�͵�ѹ����ֵ
unsigned short int VoltLowTimes=0;		//��ѹ�ͼ�����

unsigned char Save_PID_Flag=0;

unsigned char Title_Strings[15][3][13]={
"[ ��    �� ]\0",	"[ ��    �C ]\0",	"[ STANDBY  ]\0",	//0
"[ ��    �� ]\0",	"[ �O    �� ]\0",	"[ SETTINGS ]\0",	//1
"[ PID ���� ]\0",	"[ PID �O�� ]\0",	"[ PID SET  ]\0",	//2		
"[ �Ƕ�У׼ ]\0",	"[ �Ƕ�У�� ]\0",	"[ANGLE     ]\0",	//3
"[ �ٶ����� ]\0",	"[ �ٶ��O�� ]\0",	"[SPEED     ]\0",	//4
"[ �������� ]\0",	"[ �Z���O�� ]\0",	"[LANGUAGE  ]\0",	//5
"[ �������� ]\0",	"[ ���S�O�� ]\0",	"[FACTORY   ]\0",	//6
"[ �������� ]\0",	"[ �����O�� ]\0",	"[ OTHERS   ]\0",	//7
"[ �������� ]\0",	"[ �����O�� ]\0",	"[ OTHERS   ]\0",	//8
"[ �������� ]\0",	"[ �����O�� ]\0",	"[ OTHERS   ]\0",	//9
"[ �������� ]\0",	"[ �����O�� ]\0",	"[ OTHERS   ]\0",	//10
	
"[ ����ң�� ]\0",	"[ �{���b�� ]\0",	"[BULE CTRL ]\0",	//11
"[  RCң��  ]\0",	"[  RC�b��  ]\0",	"[ RC CTRL  ]\0",	//12
"[  PS2ң�� ]\0",	"[  PS2�b�� ]\0",	"[ PS2 CTRL ]\0",	//13
"[ WIFIң�� ]\0",	"[ WIFI�b�� ]\0",	"[WIFI CTRL ]\0",	//13
};

unsigned char StandbyUI_String[3][8][7]={
"ǰ��\0","ǰ�ң�\0","����\0","���ң�\0","��ѹ��\0","WIFI��\0","�Ͽ�  \0","����  \0",
"ǰ��\0","ǰ�ң�\0","����\0","���ң�\0","늉���\0","WIFI��\0","���_  \0","�B��  \0",
"F_L�� \0","F_R�� \0","B_L�� \0","B_R�� \0","Volt��\0","WIFI��\0","broke \0","link  \0",
};
unsigned char StandbyUI_String2[2][3][21]={
"<�̰���������������>\0","<�̰����ӣ��L���O��>\0","Run, Short-Press KEY\0",	
"<  �̰����л�ģʽ  >\0","<  �̰����ГQģʽ  >\0","Stop,Short-Press KEY\0",
};
unsigned char SettingUI_String[3][8][11]={
"PID ����  \0","��������  \0","�������  \0","��������  \0","�Ƕ�У׼  \0","��������  \0","�ٶ�����  \0","��    ��  \0",
"PID �O��  \0","�Z���O��  \0","늙C�O��  \0","�����O��  \0","�Ƕ�У��  \0","�����O��  \0","�ٶ��O��  \0","��    ��  \0",
"PID SET   \0","LANGUAGE  \0","MOTOR SET\0","OTHER SET \0","CALI ANGLE\0","FACTORY   \0","SPEED SET \0","EXIT      \0",	
};

unsigned char PIDsetUI_String[3][5][5]={
"�ٶ�\0","ƽ��\0","ת��\0","����\0","�˳�\0",
"�ٶ�\0","ƽ��\0","�D��\0","����\0","�˳�\0",
"Spd \0","Bal \0","Yaw \0","SAVE\0","EXIT\0",
};

//unsigned char Motor_SetUI_String[3][3][15]={
//"ǰ�㰲ȫ�Ƕȣ�\0","���㰲ȫ�Ƕȣ�\0","�͵�ѹ����ֵ��\0",
//"ǰ�A��ȫ�Ƕȣ�\0","��A��ȫ�Ƕȣ�\0","��늉���ֵ��\0",
//"Front Angle ��\0","Back  Angle ��\0","Power Low Warn\0",
//};

unsigned char MasBox_String[3][4][15]={
" �����ѱ��棡 \0"," ����У׼���� \0","�ѻָ���������\0","��ѹ�ͣ�����\0",
" �����ѱ��棡 \0"," ����У�ʡ��� \0","�ѻ֏ͳ��S�O��\0","늉��ͣ�Ո���\0",
"    Saved!    \0"," Calibrating  \0","   Reseted!   \0","  Low Power!  \0",
};
unsigned char CALI_String[3][4][22]={
"    �뽫ƽ�⳵��ֹ�� \0","�ã���֤������ˮƽ�� \0","ƽ�С��̰�<����>��ʼ \0","�Ƕ�У׼��           \0",
"    Ո��ƽ��܇�oֹ�� \0","�ã����C�����cˮƽ�� \0","ƽ�С��̰�<���I>�_ʼ \0","�Ƕ�У�ʡ�           \0",
"Put thecar immovably.\0","Ensure the mainboard\0","level. Press the KEY \0","to start calibration.\0",	
};
unsigned char CALI_String2[3][12]={
"���У׼   \0",
"���У��   \0",
"s later CAL\0",
};
unsigned char Factory_String[3][20]={
" �Ƿ�ָ��������ã�\0", 
" �Ƿ�֏ͳ��S�O�ã�\0",
"  Factory setting? \0",
};

	
void Standby_Mode(void)
{
	if(UI_Display_Flag==1)
	{
		OLED_ClearBuffer(0);
		OLED_Put12x12CNstr(24,0,Title_Strings[WorkMode][Lang_x],1);
		OLED_Put12x12CNstr(0,13,StandbyUI_String[Lang_x][0],1);			OLED_Put12x12CNstr(66,13,StandbyUI_String[Lang_x][1],1);
		OLED_Put12x12CNstr(0,26,StandbyUI_String[Lang_x][2],1);			OLED_Put12x12CNstr(66,26,StandbyUI_String[Lang_x][3],1);
		OLED_Put12x12CNstr(0,39,StandbyUI_String[Lang_x][4],1);			OLED_Put12x12CNstr(66,39,StandbyUI_String[Lang_x][5],1);
		OLED_Put12x12CNstr(0,52,StandbyUI_String2[WorkMode][Lang_x],1);			//��ʾ��ʾ��
		LED_state=LED_SLOWFLASH;UI_Display_Flag=0;
	}
	if(Kaysta==1)				//�����̰�
	{
		PID_Motor[0].OUT=0;PID_Motor[0].iout=0;PID_Motor[0].error=0;PID_Motor[0].Last_error=0;
		PID_Motor[1].OUT=0;PID_Motor[1].iout=0;PID_Motor[1].error=0;PID_Motor[1].Last_error=0;
		PID_Motor[2].OUT=0;PID_Motor[2].iout=0;PID_Motor[2].error=0;PID_Motor[2].Last_error=0;
		PID_Motor[3].OUT=0;PID_Motor[3].iout=0;PID_Motor[3].error=0;PID_Motor[3].Last_error=0;

		WorkMode=14;			//ģʽ14��WIFIң��
		UI_Display_Flag=1;						//��Ǹı����
		Kaysta=0;											//����������
	}
	if(Kaysta==2)	//��������
	{
		ARMED=0;time_tick=0;
		WorkMode=1;	//ģʽ1��settings
		UI_Display_Flag=1;						//��Ǹı����
		Kaysta=0;											//����������
	}
	OLED_Put6x12Num(32,13,Encoder[3],5,1);	OLED_Put6x12Num(98,13,-Encoder[2],5,1);		//��ʾ�Ƕ�MPU6050_accel[2]
	OLED_Put6x12Num(32,26,Encoder[0],5,1);	OLED_Put6x12Num(98,26,-Encoder[1],5,1);	//��ʾת��
	OLED_Put5x7Num1dot(30,42,Volt);																								//��ʾ��ص�ѹ
	if(WIFI_CK==0)	OLED_Put12x12CNstr(98,39,StandbyUI_String[Lang_x][6],1);							//��ʾWIFI�ź�
	else	OLED_Put12x12CNstr(98,39,StandbyUI_String[Lang_x][7],1);											//��ʾWIFI�ź�
}	

void Remote_Mode(void)
{
	if(UI_Display_Flag==1)
	{
		OLED_ClearBuffer(0);
		OLED_Put12x12CNstr(24,0,Title_Strings[WorkMode][Lang_x],1);
		OLED_Put12x12CNstr(0,13,StandbyUI_String[Lang_x][0],1);			OLED_Put12x12CNstr(66,13,StandbyUI_String[Lang_x][1],1);
		OLED_Put12x12CNstr(0,26,StandbyUI_String[Lang_x][2],1);			OLED_Put12x12CNstr(66,26,StandbyUI_String[Lang_x][3],1);
		OLED_Put12x12CNstr(0,39,StandbyUI_String[Lang_x][4],1);			OLED_Put12x12CNstr(66,39,StandbyUI_String[Lang_x][5],1);
		OLED_Put12x12CNstr(0,52,StandbyUI_String2[1][Lang_x],1);			//��ʾ��ʾ��
		LED_state=LED_SHORTFLASH;ARMED=1;time_tick=0;UI_Display_Flag=0;
	}
	if(Kaysta==1)				//�����̰�
	{
		ARMED=0;time_tick=0;
		WorkMode++;	//ģʽ0��Standby
		if(WorkMode==15) WorkMode=0;
		UI_Display_Flag=1;	//��Ǹı����
		Kaysta=0;	//����������
	}
	OLED_Put6x12Num(32,13,Encoder[3],5,1);	OLED_Put6x12Num(98,13,Encoder[2],5,1);		//��ʾ�Ƕ�MPU6050_accel[2]
	OLED_Put6x12Num(32,26,Encoder[0],5,1);	OLED_Put6x12Num(98,26,Encoder[1],5,1);	//��ʾת��
	OLED_Put5x7Num1dot(30,42,Volt);																								//��ʾ��ص�ѹ
	if(WIFI_CK==0)	OLED_Put12x12CNstr(98,39,StandbyUI_String[Lang_x][6],1);							//��ʾWIFI�ź�
	else	OLED_Put12x12CNstr(98,39,StandbyUI_String[Lang_x][7],1);											//��ʾWIFI�ź�

}

void Display_Menux(char Menuxx)
{
	if(Menuxx==1)	OLED_Put5x7Char(0,15,'>',1);
	else 					OLED_Put5x7Char(0,15,' ',1);
	if(Menuxx==2)	OLED_Put5x7Char(0,28,'>',1);
	else					OLED_Put5x7Char(0,28,' ',1);
	if(Menuxx==3)	OLED_Put5x7Char(0,41,'>',1);
	else					OLED_Put5x7Char(0,41,' ',1);
	if(Menuxx==4)	OLED_Put5x7Char(0,54,'>',1);
	else					OLED_Put5x7Char(0,54,' ',1);
	
	if(Menuxx==5)	OLED_Put5x7Char(69,15,'>',1);
	else					OLED_Put5x7Char(69,15,' ',1);
	if(Menuxx==6)	OLED_Put5x7Char(69,28,'>',1);
	else					OLED_Put5x7Char(69,28,' ',1);
	if(Menuxx==7)	OLED_Put5x7Char(69,41,'>',1);
	else					OLED_Put5x7Char(69,41,' ',1);
	if(Menuxx==8)	OLED_Put5x7Char(69,54,'>',1);
	else					OLED_Put5x7Char(69,54,' ',1);
}

void Settings_Mode(void)
{
	if(UI_Display_Flag==1)	
	{
		OLED_ClearBuffer(0);
		OLED_Put12x12CNstr(24,0,Title_Strings[WorkMode][Lang_x],1);	
		OLED_Put12x12CNstr(6,13,SettingUI_String[Lang_x][0],1);		OLED_Put12x12CNstr(75,13,SettingUI_String[Lang_x][1],1);
		OLED_Put12x12CNstr(6,26,SettingUI_String[Lang_x][2],1);		OLED_Put12x12CNstr(75,26,SettingUI_String[Lang_x][3],1);
		OLED_Put12x12CNstr(6,39,SettingUI_String[Lang_x][4],1);		OLED_Put12x12CNstr(75,39,SettingUI_String[Lang_x][5],1);
		OLED_Put12x12CNstr(6,52,SettingUI_String[Lang_x][6],1);		OLED_Put12x12CNstr(75,52,SettingUI_String[Lang_x][7],1);
		LED_state=LED_FASTFLASH;Menux=1;LeftKnob=0;RightKnob=0;	UI_Display_Flag=0;
	}
	if(LeftKnob>150)
	{
		LeftKnob=0;Menux--;
		if( Menux==0 )	Menux=8;
	}
	if(LeftKnob<-150)
	{
		LeftKnob=0;Menux++;
		if(Menux==9)	Menux=1;
	}
	switch(Menux)
	{
		case 1://PID����
			if(Kaysta==1)	//�����̰�
			{
				ARMED=0;time_tick=0;
				WorkMode=0x02;			//ģʽ2��PID����
				UI_Display_Flag=1;	//��Ǹı����
				Kaysta=0;						//����������
			}
			break;
		case 2://����������
			if(Kaysta==1)	//�����̰�
			{
				ARMED=0;time_tick=0;
				WorkMode=0x03;			//ģʽ5����ȫ�Ƕ�����
				UI_Display_Flag=1;	//��Ǹı����
				Kaysta=0;						//����������
			}
			break;
		case 3://�Ƕ�У׼
			if(Kaysta==1)	//�����̰�
			{
				ARMED=0;time_tick=0;
				WorkMode=0x04;			//ģʽ6���Ƕ�У׼
				UI_Display_Flag=1;	//��Ǹı����
				Kaysta=0;						//����������
			}
			break;
		case 4://
			if(Kaysta==1)	//�����̰�
			{
				ARMED=0;time_tick=0;
				WorkMode=0x05;			//�ٶ�����ģʽ
				UI_Display_Flag=1;	//��Ǹı����
				Kaysta=0;						//����������
			}
			break;
		case 5://
			if(Kaysta==1)	//�����̰�
			{
				ARMED=0;time_tick=0;
				WorkMode=0x06;			//��������ģʽ
				UI_Display_Flag=1;	//��Ǹı����
				Kaysta=0;						//����������
			}
			break;
		case 6://
			if(Kaysta==1)	//�����̰�
			{
				ARMED=0;time_tick=0;
				WorkMode=0x07;			//��������ģʽ
				UI_Display_Flag=1;	//��Ǹı����
				Kaysta=0;						//����������
			}
			break;
		case 7://
			if(Kaysta==1)	//�����̰�
			{
				ARMED=0;time_tick=0;
				WorkMode=0x08;			//�ָ���������ģʽ
				UI_Display_Flag=1;	//��Ǹı����
				Kaysta=0;						//����������
			}
			break;
		case 9:	//�˳�
			if(Kaysta==1)	//�����̰�
			{
				ARMED=0;time_tick=0;
				WorkMode=0x00;			//ģʽ0��Standby
				UI_Display_Flag=1;	//��Ǹı����
				Kaysta=0;						//����������
			}
			break;			
		default:
			break;
	}
	Display_Menux(Menux);	//��ʾ���
}

void PID_Settings(void)
{
	if(UI_Display_Flag==1)	
	{	
		OLED_ClearBuffer(0);
		OLED_Put12x12CNstr(24,0,Title_Strings[WorkMode][Lang_x],1);	
		OLED_Put12x12CNstr(0,13,PIDsetUI_String[Lang_x][0],1);OLED_Put12x12CNstr(24,13,"P��   I��   D��  ",1);
//		OLED_Put12x12CNstr(0,26,PIDsetUI_String[Lang_x][1],1);OLED_Put12x12CNstr(24,26,"P��   I��   D��  ",1);
//		OLED_Put12x12CNstr(0,39,PIDsetUI_String[Lang_x][2],1);OLED_Put12x12CNstr(24,39,"P��   I��   D��  ",1);
		Flash_Read_PID();	//��ȡPID��ֵ
		Display_PID_data();
		LED_state=LED_FASTFLASH;Menux=1;LeftKnob=0;RightKnob=0;	
		UI_Display_Flag=0;		
	}
/////////////////////////////////////////
	if(LeftKnob>150)
	{
		LeftKnob=0;Menux--;
		if( Menux==0 )	Menux=5;
	}
	if(LeftKnob<-150)
	{
		LeftKnob=0;Menux++;
		if(Menux==6)	Menux=1;
	}
////////////////////////////////////////////////
	Display_PID_data();		//��ʾPID��ֵ
	switch(Menux)
	{
		case 1:
			if(RightKnob>100)
			{
				PID_Motor[0].Pdat+=1;RightKnob=0;
			}
			if(RightKnob<-100)
			{
				if(PID_Motor[0].Pdat>0) PID_Motor[0].Pdat-=1;
				RightKnob=0;
			}	
			Keytimes=0;			
			break;
		case 2:
			if(RightKnob>100)
			{
				PID_Motor[0].Idat+=1;RightKnob=0;
			}
			if(RightKnob<-100)
			{
				if(PID_Motor[0].Idat>0) PID_Motor[0].Idat-=1;
				RightKnob=0;
			}
			Keytimes=0;
			break;
		case 3:
			if(RightKnob>100)
			{
				PID_Motor[0].Ddat+=1;RightKnob=0;
			}
			if(RightKnob<-100)
			{
				if(PID_Motor[0].Ddat>0) PID_Motor[0].Ddat-=1;
				RightKnob=0;
			}
			Keytimes=0;
			break;
/*		case 4:
			if(RightKnob>100)
			{
				PID_Motor[1].Pdat+=1;RightKnob=0;
			}
			if(RightKnob<-100)
			{
				if(PID_Motor[1].Pdat>0) PID_Motor[1].Pdat-=1;
				RightKnob=0;
			}
			Keytimes=0;
			break;
		case 5:
			if(RightKnob>100)
			{
				PID_Motor[1].Idat+=1;RightKnob=0;
			}
			if(RightKnob<-100)
			{
				if(PID_Motor[1].Idat>0) PID_Motor[1].Idat-=1;
				RightKnob=0;
			}
			Keytimes=0;
			break;
		case 6:
			if(RightKnob>100)
			{
				PID_Motor[1].Ddat+=1;RightKnob=0;
			}
			if(RightKnob<-100)
			{
				if(PID_Motor[1].Ddat>0) PID_Motor[1].Ddat-=1;
				RightKnob=0;
			}
			Keytimes=0;
			break;
		case 7:
			if(RightKnob>100)
			{
				PID_Motor[2].Pdat+=1;RightKnob=0;
			}
			if(RightKnob<-100)
			{
				if(PID_Motor[2].Pdat>0) PID_Motor[2].Pdat-=1;
				RightKnob=0;
			}
			Keytimes=0;
			break;
		case 8:
			if(RightKnob>100)
			{
				PID_Motor[2].Idat+=1;RightKnob=0;
			}
			if(RightKnob<-100)
			{
				if(PID_Motor[2].Idat>0) PID_Motor[2].Idat-=1;
				RightKnob=0;
			}
			Keytimes=0;
			break;
		case 9:
			if(RightKnob>100)
			{
				PID_Motor[2].Ddat+=1;RightKnob=0;
			}
			if(RightKnob<-100)
			{
				if(PID_Motor[2].Ddat>0) PID_Motor[2].Ddat-=1;
				RightKnob=0;
			}
			Keytimes=0;
			break;*/
		case 4:		//����
			if(Kaysta==1)	//�����̰�
			{			
				Flash_Save_PID();
				Display_SaveMasBox();//��ʾ��Ϣ��
				Flash_Save_Delay();
				WorkMode=0x00;	//ģʽ0��Standby
				UI_Display_Flag=1;	//��Ǹı����
				Kaysta=0;		//����������
				ARMED=0;time_tick=0;
			}		
			break;
		case 5:		//�˳�
			if(Kaysta==1)	//�����̰�
			{
				ARMED=0;time_tick=0;
				WorkMode=0x00;	//ģʽ0��Standby
				UI_Display_Flag=1;	//��Ǹı����
				Kaysta=0;		//����������
			}	
			break;
		default:
			break;
	}				
}

void Display_PID_data(void)
{
	OLED_Put6x12Num(36,13,PID_Motor[0].Pdat,3,Menux==1?0:1);	
	OLED_Put6x12Num(72,13,PID_Motor[0].Idat,3,Menux==2?0:1);	
	OLED_Put6x12Num(108,13,PID_Motor[0].Ddat,3,Menux==3?0:1);
/*
//	OLED_Put6x12Num(36,26,PID_Motor[1].Pdat,3,Menux==4?0:1);		
//	OLED_Put6x12Num(72,26,PID_Motor[1].Idat,3,Menux==5?0:1);		
//	OLED_Put6x12Num(108,26,PID_Motor[1].Ddat,3,Menux==6?0:1);
//	
//	OLED_Put6x12Num(36,39,PID_Motor[2].Pdat,3,Menux==7?0:1);		
//	OLED_Put6x12Num(72,39,PID_Motor[2].Idat,3,Menux==8?0:1);		
//	OLED_Put6x12Num(108,39,PID_Motor[2].Ddat,3,Menux==9?0:1);	*/
	OLED_Put12x12CNstr(6,52,PIDsetUI_String[Lang_x][3],Menux==4?0:1);
	OLED_Put12x12CNstr(98,52,PIDsetUI_String[Lang_x][4],Menux==5?0:1);
}


/*
void AdjustAngle_Settings(void)
{
//	unsigned char nx;
//	if(UI_Display_Flag==1)	
//	{	
//		OLED_ClearBuffer(0);
//		OLED_Put12x12CNstr(24,0,Title_Strings[WorkMode][Lang_x],1);	
//		OLED_Put12x12CNstr(3,13,CALI_String[Lang_x][0],1);
//		OLED_Put12x12CNstr(3,26,CALI_String[Lang_x][1],1);
//		OLED_Put12x12CNstr(3,39,CALI_String[Lang_x][2],1);
//		OLED_Put12x12CNstr(3,52,CALI_String[Lang_x][3],1);
//		LED_state=LED_FASTFLASH;Menux=1;LeftKnob=0;RightKnob=0;	
//		UI_Display_Flag=0;
//	}
//	if(Kaysta==1)	//�����̰�
//	{		
//		AdjustAngle_Flag=599;
//		Kaysta=0;		//����������
//	}	
//	if(AdjustAngle_Flag==1)	//��ʼУ׼
//	{
//		for(nx=0;nx<16;nx++)
//			OLED_DrawLine_X(22,106,10+nx,1);
//		OLED_Put12x12CNstr(22,26,MasBox_String[Lang_x][1],0);
//		for(nx=28;nx<44;nx++)
//			OLED_DrawLine_X(22,106,10+nx,1);
//		
//		mpu_set_dmp_state(0);delay_ms(10);
//		run_self_test(1);
//		Yaw=0;	   
//		mpu_set_dmp_state(1);
//		time_tick=0;
//		
//		ARMED=0;time_tick=0;
//		WorkMode=0x00;	//ģʽ0��Standby
//		UI_Display_Flag=1;	//��Ǹı����
//		delay_ms(500);
//	}
//	else	if(AdjustAngle_Flag>1)//׼��У׼
//	{
//		for(nx=0;nx<16;nx++)
//			OLED_DrawLine_X(22,106,10+nx,1);
//		OLED_Put12x12CNstr(22,26,"  ",0);
//		OLED_PutChar0612(34,26,AdjustAngle_Flag/200+1+48,0);
//		OLED_Put12x12CNstr(40,26,CALI_String2[Lang_x],0);
//		for(nx=28;nx<44;nx++)
//			OLED_DrawLine_X(22,106,10+nx,1);
//	}
}
*/

void Display_Motor_DrctAdj(unsigned char xx)
{
	if(Motor_DrctAdj[3]==-1)	OLED_Put12x12CNstr(0+36,13,"����",xx==1?0:1);	
	else 										OLED_Put12x12CNstr(0+36,13,"����",xx==1?0:1);	
	
	if(Motor_DrctAdj[2]==-1)	OLED_Put12x12CNstr(66+36,13,"����",xx==3?0:1);
	else										OLED_Put12x12CNstr(66+36,13,"����",xx==3?0:1);
	
	if(Motor_DrctAdj[0]==-1)	OLED_Put12x12CNstr(0+36,26,"����",xx==2?0:1);	
	else 										OLED_Put12x12CNstr(0+36,26,"����",xx==2?0:1);

	if(Motor_DrctAdj[1]==-1)	OLED_Put12x12CNstr(66+36,26,"����",xx==4?0:1);
	else 										OLED_Put12x12CNstr(66+36,26,"����",xx==4?0:1);
}


void Motor_Settings(void)
{
	if(UI_Display_Flag==1)
	{
		OLED_ClearBuffer(0);
		OLED_Put12x12CNstr(24,0,Title_Strings[WorkMode][Lang_x],1);
		OLED_Put12x12CNstr(0,13,StandbyUI_String[Lang_x][0],1);			OLED_Put12x12CNstr(66,13,StandbyUI_String[Lang_x][1],1);
		OLED_Put12x12CNstr(0,26,StandbyUI_String[Lang_x][2],1);			OLED_Put12x12CNstr(66,26,StandbyUI_String[Lang_x][3],1);
		Flash_Read_MotorDrctAdj();//��ȡ���ת�����ֵ
		Display_Motor_DrctAdj(Menux);	//��ʾ���ת�����ֵ
		LED_state=LED_FASTFLASH;Menux=1;LeftKnob=0;RightKnob=0;	UI_Display_Flag=0;
	}

/////////////////////////////////////////
	if(LeftKnob>150)
	{
		LeftKnob=0;Menux--;
		if( Menux==0 )	Menux=6;
	}
	if(LeftKnob<-150)
	{
		LeftKnob=0;Menux++;
		if(Menux==5)	Menux=1;	
	}
////////////////////////////////////////////////
	Display_Motor_DrctAdj(Menux);	//��ʾ���ת�����ֵ
	switch(Menux)
	{
		case 1:
			if(RightKnob>100)
			{
				BEEP_ShortRing();//�������̽�һ��
				if(Motor_DrctAdj[3]==-1) Motor_DrctAdj[3]=1;
				else Motor_DrctAdj[3]=-1;
				RightKnob=0;
			}
			if(RightKnob<-100)
			{
				BEEP_ShortRing();//�������̽�һ��
				if(Motor_DrctAdj[3]==-1) Motor_DrctAdj[3]=1;
				else Motor_DrctAdj[3]=-1;
				RightKnob=0;
			}
			Keytimes=0;
			break;
		case 2:
			if(RightKnob>100)
			{
				BEEP_ShortRing();//�������̽�һ��
				if(Motor_DrctAdj[0]==-1) Motor_DrctAdj[0]=1;
				else Motor_DrctAdj[0]=-1;
				RightKnob=0;
			}
			if(RightKnob<-100)
			{
				BEEP_ShortRing();//�������̽�һ��
				if(Motor_DrctAdj[0]==-1) Motor_DrctAdj[0]=1;
				else Motor_DrctAdj[0]=-1;
				RightKnob=0;
			}
			Keytimes=0;
			break;
		case 3:
			if(RightKnob>100)
			{
				BEEP_ShortRing();//�������̽�һ��
				if(Motor_DrctAdj[2]==-1) Motor_DrctAdj[2]=1;
				else Motor_DrctAdj[2]=-1;
				RightKnob=0;
			}
			if(RightKnob<-100)
			{
				BEEP_ShortRing();//�������̽�һ��
				if(Motor_DrctAdj[2]==-1) Motor_DrctAdj[2]=1;
				else Motor_DrctAdj[2]=-1;
				RightKnob=0;
			}
			Keytimes=0;
			break;
		case 4:
			if(RightKnob>100)
			{
				BEEP_ShortRing();//�������̽�һ��
				if(Motor_DrctAdj[1]==-1) Motor_DrctAdj[1]=1;
				else Motor_DrctAdj[1]=-1;
				RightKnob=0;
			}
			if(RightKnob<-100)
			{
				BEEP_ShortRing();//�������̽�һ��
				if(Motor_DrctAdj[1]==-1) Motor_DrctAdj[1]=1;
				else Motor_DrctAdj[1]=-1;
				RightKnob=0;
			}
			Keytimes=0;
			break;
		case 5:		//����
			if(Kaysta==1)	//�����̰�
			{
				Flash_Save_MotorDrctAdj();
				Display_SaveMasBox();//��ʾ��Ϣ��
				delay_ms(100);
				Flash_Save_Delay();
				WorkMode=0x00;	//ģʽ0��Standby
				UI_Display_Flag=1;	//��Ǹı����
				Kaysta=0;		//����������
				ARMED=0;time_tick=0;
			}	
			break;
		case 6:		//�˳�
			if(Kaysta==1)	//�����̰�
			{
				ARMED=0;time_tick=0;
				WorkMode=0x00;	//ģʽ0��Standby
				UI_Display_Flag=1;	//��Ǹı����
				Kaysta=0;		//����������
			}	
			break;
		default:
			break;
	}				
}


//��ʾ��Ϣ��
void Display_SaveMasBox(void)
{
	unsigned char mx;
	for(mx=0;mx<16;mx++)
		OLED_DrawLine_X(22,106,10+mx,1);
	OLED_Put12x12CNstr(22,26,MasBox_String[Lang_x][0],0);
	for(mx=28;mx<44;mx++)
		OLED_DrawLine_X(22,106,10+mx,1);
}

void Display_LanguageCursor(char Menuxx)
{
	if(Menuxx==1)	OLED_Put12x12CNstr(6,13,">",1);
	else 	OLED_Put12x12CNstr(6,13," ",1);
	if(Menuxx==2)	OLED_Put12x12CNstr(6,26,">",1);
	else	OLED_Put12x12CNstr(6,26," ",1);
	if(Menuxx==3)	OLED_Put12x12CNstr(6,39,">",1);
	else	OLED_Put12x12CNstr(6,39," ",1);
} 

void LanguageSettings_Mode(void)
{
	if(UI_Display_Flag==1)	
	{	
		OLED_ClearBuffer(0);
		OLED_Put12x12CNstr(24,0,Title_Strings[WorkMode][Lang_x],1);	
		OLED_Put12x12CNstr(24,13,"��������",1);
		OLED_Put12x12CNstr(24,26,"���w����",1);
		OLED_Put12x12CNstr(24,39,"English",1);
		LED_state=LED_FASTFLASH;Menux=1;LeftKnob=0;RightKnob=0;	
		UI_Display_Flag=0;
	}
	if(LeftKnob>150)
	{
		LeftKnob=0;Menux--;
		if( Menux==0 )	Menux=3;
	}
	if(LeftKnob<-150)
	{
		LeftKnob=0;Menux++;
		if(Menux==4)	Menux=1;
	}
	switch(Menux)
	{
		case 1://��������
			if(Kaysta==1)	//�����̰�
			{
				ARMED=0;time_tick=0;Lang_x=0;						//������������
				Flash_Save_Language();
				delay_ms(100);
				Flash_Save_Delay();
				WorkMode=0x00;	//ģʽ
				UI_Display_Flag=1;	//��Ǹı����
				Kaysta=0;		//����������
			}
			break;
		case 2://��������
			if(Kaysta==1)	//�����̰�
			{
				ARMED=0;time_tick=0;Lang_x=1;						//������������
				Flash_Save_Language();
				delay_ms(100);
				Flash_Save_Delay();
				WorkMode=0x00;	//ģʽ
				UI_Display_Flag=1;	//��Ǹı����
				Kaysta=0;		//����������
			}
			break;
		case 3://Ӣ��
			if(Kaysta==1)	//�����̰�
			{
				ARMED=0;time_tick=0;Lang_x=2;						//������������
				Flash_Save_Language();
				delay_ms(100);
				Flash_Save_Delay();
				WorkMode=0x00;	//ģʽ
				UI_Display_Flag=1;	//��Ǹı����
				Kaysta=0;		//����������
			}
			break;
		default:
			
			break;
	}
	Display_LanguageCursor(Menux);
}

//��ʾ��Ϣ��
void Display_FactoryBox(void)
{
	unsigned char mx;
	for(mx=0;mx<16;mx++)
		OLED_DrawLine_X(22,106,10+mx,1);
	OLED_Put12x12CNstr(22,26,MasBox_String[Lang_x][2],0);
	for(mx=28;mx<44;mx++)
		OLED_DrawLine_X(22,106,10+mx,1);
}

void FactorySettings_Mode(void)
{
	if(UI_Display_Flag==1)	
	{
		OLED_ClearBuffer(0);
		OLED_Put12x12CNstr(24,0,Title_Strings[WorkMode][Lang_x],1);	
		OLED_Put12x12CNstr(3,26,Factory_String[Lang_x],1);
		LED_state=LED_FASTFLASH;Menux=1;LeftKnob=0;RightKnob=0;	
		UI_Display_Flag=0;
	}
	if(LeftKnob>150)
	{
		LeftKnob=0;Menux--;
		if( Menux==0 )	Menux=2;
	}
	if(LeftKnob<-150)
	{
		LeftKnob=0;Menux++;
		if(Menux==3)	Menux=1;
	}
	switch(Menux)
	{
		case 1://��
			if(Kaysta==1)	//�����̰�
			{
				ARMED=0;time_tick=0;
				WorkMode=0x00;	//ģʽ
				UI_Display_Flag=1;	//��Ǹı����
				Kaysta=0;		//����������
			}
			if(Lang_x<2)
			{	OLED_Put12x12CNstr(30,52,"��",0);OLED_Put12x12CNstr(86,52,"��",1);	}
			else
			{	OLED_Put12x12CNstr(30,52,"NO",0);OLED_Put12x12CNstr(83,52,"YES",1);	}			
			break;
		case 2://��
			if(Kaysta==1)	//�����̰�
			{					
				PID_Motor[0].Pdat=16.0;	PID_Motor[0].Idat=0.0;	PID_Motor[0].Ddat=4.0;	
				PID_Motor[1].Pdat=16.0;	PID_Motor[1].Idat=0.0;	PID_Motor[1].Ddat=4.0;
				PID_Motor[2].Pdat=16.0;	PID_Motor[2].Idat=0.0;	PID_Motor[2].Ddat=4.0;
				PID_Motor[3].Pdat=16.0;	PID_Motor[3].Idat=0.0;	PID_Motor[3].Ddat=4.0;
				Motor_DrctAdj[0]=1;Motor_DrctAdj[1]=1;Motor_DrctAdj[2]=1;Motor_DrctAdj[3]=1;
				Lang_x=0;						//������������				
//				Front_SafeAngle=60.0;			Back_SafeAngle=-60.0;
				Flash_Save_PID();				//д��PIDֵ
				Flash_Save_MotorDrctAdj(); //д������������
//				Flash_Save_SafeAngle();	//д�밲ȫ�Ƕ�ֵ	
				Display_FactoryBox();		//��ʾ��ʾ��Ϣ
				delay_ms(100);
				Flash_Save_Delay();
				ARMED=0;time_tick=0;
				WorkMode=0x00;					//ģʽ
				UI_Display_Flag=1;	//��Ǹı����
				Kaysta=0;								//����������
			}
			if(Lang_x<2)
			{	OLED_Put12x12CNstr(30,52,"��",1);OLED_Put12x12CNstr(86,52,"��",0);	 }
			else
			{	OLED_Put12x12CNstr(30,52,"NO",1);OLED_Put12x12CNstr(83,52,"YES",0);}				
			break;
		default:
			
			break;
	}
}



void Display_OtherSettings_data(void)
{
	OLED_Put6x12Num1dot(88,17,VoltLowWarn,Menux==1?0:1);	
//	OLED_Put6x12Num1dot(88,32,VoltLowWarn,Menux==2?0:1);	
	
	OLED_Put12x12CNstr(6,52,PIDsetUI_String[Lang_x][3],Menux==3?0:1);
//	OLED_Put12x12CNstr(98,52,PIDsetUI_String[Lang_x][4],Menux==4?0:1);
}

void Other_Settings_Mode(void)
{
	if(UI_Display_Flag==1)	
	{
		OLED_ClearBuffer(0);
		OLED_Put12x12CNstr(24,0,Title_Strings[WorkMode][Lang_x],1);
//		OLED_Put12x12CNstr(0,17,SafeAngleUI_String[Lang_x][2],1);
//		OLED_Put12x12CNstr(0,32,SafeAngleUI_String[Lang_x][2],1);
		Flash_Read_VoltWarn();				//��ȡ��ֵ
		Display_OtherSettings_data();	//��ʾ��ֵ
		LED_state=LED_FASTFLASH;Menux=1;LeftKnob=0;RightKnob=0;	UI_Display_Flag=0;
	}
/////////////////////////////////////////
	if(LeftKnob>150)
	{
		LeftKnob=0;Menux--;
		if( Menux==0 )	Menux=4;
	}
	if(LeftKnob<-150)
	{
		LeftKnob=0;Menux++;
		if(Menux==5)	Menux=1;
	}
////////////////////////////////////////////////
	Display_OtherSettings_data();	//��ʾ��ֵ
	switch(Menux)
	{
		case 1:
			if(RightKnob>100)
			{
				if(VoltLowWarn<120) VoltLowWarn+=1;
				RightKnob=0;
			}
			if(RightKnob<-100)
			{
				if(VoltLowWarn>70) VoltLowWarn-=1;
				RightKnob=0;
			}	
			Keytimes=0;			
			break;
		case 2:
//			if(RightKnob>100)
//			{
//				if(VoltLowWarn<-10) VoltLowWarn+=1;
//				RightKnob=0;
//			}
//			if(RightKnob<-100)
//			{
//				if(VoltLowWarn>-80) VoltLowWarn-=1;
//				RightKnob=0;
//			}
//			Keytimes=0;
			break;
		case 3:		//����
			if(Kaysta==1)	//�����̰�
			{
				Flash_Save_VoltWarn();
				Display_SaveMasBox();//��ʾ��Ϣ��
				delay_ms(100);
				Flash_Save_Delay();
				WorkMode=0x00;	//ģʽ0��Standby
				UI_Display_Flag=1;	//��Ǹı����
				Kaysta=0;		//����������
				ARMED=0;time_tick=0;
			}	
			break;
		case 4:		//�˳�
			if(Kaysta==1)	//�����̰�
			{
				ARMED=0;time_tick=0;
				WorkMode=0x00;	//ģʽ0��Standby
				UI_Display_Flag=1;	//��Ǹı����
				Kaysta=0;		//����������
			}	
			break;
		default:
			break;
	}				
}


void VoltWarn_Mode(void)
{
	unsigned char mx;
	
	for(mx=0;mx<16;mx++)
		OLED_DrawLine_X(22,106,10+mx,1);
	OLED_Put12x12CNstr(22,26,MasBox_String[Lang_x][3],0);
	for(mx=28;mx<44;mx++)
		OLED_DrawLine_X(22,106,10+mx,1);	
	VoltLowTimes-=2;
	if(VoltLowTimes==0) 
	{
		WorkMode=0;		UI_Display_Flag=1;						//��Ǹı����
	}
}

