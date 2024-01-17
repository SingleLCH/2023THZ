/***************************************************************************
本程序可供学习使用，经作者允许，也可用于其他用途。如有意见及建议，欢迎来电交流指教
硬件版本：四路麦轮多功能控制板
主控芯片：STM32F103RCT6
程序名称：STM32麦轮全向移动小车程序--UI界面程序
功能描述：UI切换界面，调参数的人机交互界面



*****************************************************************************/

#include "UI.h"

unsigned short int Keytimes=0;				//按键按下时间计数
unsigned char Kaysta=0;								//按键状态变量
unsigned char WorkMode=0;							//工作模式变量
unsigned int Volt=0;									//电池电压变量，单位0.1V
char Menux=0;													//菜单变量
unsigned short int AdjustAngle_Flag=0;//角度校准标志位
unsigned char BlueCK=0;								//蓝牙信号检测
short int Lang_x=0;										//语言种类
unsigned char UI_Display_Flag=0;			//标记界面写入一次
short int	VoltLowWarn=0;							//低电压报警值
unsigned short int VoltLowTimes=0;		//电压低检测次数

unsigned char Save_PID_Flag=0;

unsigned char Title_Strings[15][3][13]={
"[ 待    机 ]\0",	"[ 待    機 ]\0",	"[ STANDBY  ]\0",	//0
"[ 设    置 ]\0",	"[ 設    置 ]\0",	"[ SETTINGS ]\0",	//1
"[ PID 设置 ]\0",	"[ PID 設置 ]\0",	"[ PID SET  ]\0",	//2		
"[ 角度校准 ]\0",	"[ 角度校準 ]\0",	"[ANGLE     ]\0",	//3
"[ 速度设置 ]\0",	"[ 速度設置 ]\0",	"[SPEED     ]\0",	//4
"[ 语言设置 ]\0",	"[ 語言設置 ]\0",	"[LANGUAGE  ]\0",	//5
"[ 出厂设置 ]\0",	"[ 出廠設置 ]\0",	"[FACTORY   ]\0",	//6
"[ 其他设置 ]\0",	"[ 其他設置 ]\0",	"[ OTHERS   ]\0",	//7
"[ 其他设置 ]\0",	"[ 其他設置 ]\0",	"[ OTHERS   ]\0",	//8
"[ 其他设置 ]\0",	"[ 其他設置 ]\0",	"[ OTHERS   ]\0",	//9
"[ 其他设置 ]\0",	"[ 其他設置 ]\0",	"[ OTHERS   ]\0",	//10
	
"[ 蓝牙遥控 ]\0",	"[ 藍牙遙控 ]\0",	"[BULE CTRL ]\0",	//11
"[  RC遥控  ]\0",	"[  RC遙控  ]\0",	"[ RC CTRL  ]\0",	//12
"[  PS2遥控 ]\0",	"[  PS2遙控 ]\0",	"[ PS2 CTRL ]\0",	//13
"[ WIFI遥控 ]\0",	"[ WIFI遙控 ]\0",	"[WIFI CTRL ]\0",	//13
};

unsigned char StandbyUI_String[3][8][7]={
"前左：\0","前右：\0","后左：\0","后右：\0","电压：\0","WIFI：\0","断开  \0","连接  \0",
"前左：\0","前右：\0","後左：\0","後右：\0","電壓：\0","WIFI：\0","斷開  \0","連接  \0",
"F_L： \0","F_R： \0","B_L： \0","B_R： \0","Volt：\0","WIFI：\0","broke \0","link  \0",
};
unsigned char StandbyUI_String2[2][3][21]={
"<短按启动，长按设置>\0","<短按啟動，長按設置>\0","Run, Short-Press KEY\0",	
"<  短按，切换模式  >\0","<  短按，切換模式  >\0","Stop,Short-Press KEY\0",
};
unsigned char SettingUI_String[3][8][11]={
"PID 设置  \0","语言设置  \0","电机设置  \0","出厂设置  \0","角度校准  \0","其他设置  \0","速度设置  \0","退    出  \0",
"PID 設置  \0","語言設置  \0","電機設置  \0","出厂設置  \0","角度校準  \0","其他設置  \0","速度設置  \0","退    出  \0",
"PID SET   \0","LANGUAGE  \0","MOTOR SET\0","OTHER SET \0","CALI ANGLE\0","FACTORY   \0","SPEED SET \0","EXIT      \0",	
};

unsigned char PIDsetUI_String[3][5][5]={
"速度\0","平衡\0","转向\0","保存\0","退出\0",
"速度\0","平衡\0","轉向\0","保存\0","退出\0",
"Spd \0","Bal \0","Yaw \0","SAVE\0","EXIT\0",
};

//unsigned char Motor_SetUI_String[3][3][15]={
//"前倾安全角度：\0","后倾安全角度：\0","低电压报警值：\0",
//"前傾安全角度：\0","後傾安全角度：\0","低電壓報警值：\0",
//"Front Angle ：\0","Back  Angle ：\0","Power Low Warn\0",
//};

unsigned char MasBox_String[3][4][15]={
" 参数已保存！ \0"," 正在校准…… \0","已恢复出厂设置\0","电压低，请充电\0",
" 參數已保存！ \0"," 正在校準…… \0","已恢復出廠設置\0","電壓低，請充電\0",
"    Saved!    \0"," Calibrating  \0","   Reseted!   \0","  Low Power!  \0",
};
unsigned char CALI_String[3][4][22]={
"    请将平衡车静止放 \0","置，保证主板与水平面 \0","平行。短按<按键>开始 \0","角度校准。           \0",
"    請將平衡車靜止放 \0","置，保證主板與水平面 \0","平行。短按<按鍵>開始 \0","角度校準。           \0",
"Put thecar immovably.\0","Ensure the mainboard\0","level. Press the KEY \0","to start calibration.\0",	
};
unsigned char CALI_String2[3][12]={
"秒后校准   \0",
"秒后校準   \0",
"s later CAL\0",
};
unsigned char Factory_String[3][20]={
" 是否恢复出厂设置？\0", 
" 是否恢復出廠設置？\0",
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
		OLED_Put12x12CNstr(0,52,StandbyUI_String2[WorkMode][Lang_x],1);			//显示提示语
		LED_state=LED_SLOWFLASH;UI_Display_Flag=0;
	}
	if(Kaysta==1)				//按键短按
	{
		PID_Motor[0].OUT=0;PID_Motor[0].iout=0;PID_Motor[0].error=0;PID_Motor[0].Last_error=0;
		PID_Motor[1].OUT=0;PID_Motor[1].iout=0;PID_Motor[1].error=0;PID_Motor[1].Last_error=0;
		PID_Motor[2].OUT=0;PID_Motor[2].iout=0;PID_Motor[2].error=0;PID_Motor[2].Last_error=0;
		PID_Motor[3].OUT=0;PID_Motor[3].iout=0;PID_Motor[3].error=0;PID_Motor[3].Last_error=0;

		WorkMode=14;			//模式14，WIFI遥控
		UI_Display_Flag=1;						//标记改变界面
		Kaysta=0;											//清除按键标记
	}
	if(Kaysta==2)	//按键长按
	{
		ARMED=0;time_tick=0;
		WorkMode=1;	//模式1，settings
		UI_Display_Flag=1;						//标记改变界面
		Kaysta=0;											//清除按键标记
	}
	OLED_Put6x12Num(32,13,Encoder[3],5,1);	OLED_Put6x12Num(98,13,-Encoder[2],5,1);		//显示角度MPU6050_accel[2]
	OLED_Put6x12Num(32,26,Encoder[0],5,1);	OLED_Put6x12Num(98,26,-Encoder[1],5,1);	//显示转速
	OLED_Put5x7Num1dot(30,42,Volt);																								//显示电池电压
	if(WIFI_CK==0)	OLED_Put12x12CNstr(98,39,StandbyUI_String[Lang_x][6],1);							//显示WIFI信号
	else	OLED_Put12x12CNstr(98,39,StandbyUI_String[Lang_x][7],1);											//显示WIFI信号
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
		OLED_Put12x12CNstr(0,52,StandbyUI_String2[1][Lang_x],1);			//显示提示语
		LED_state=LED_SHORTFLASH;ARMED=1;time_tick=0;UI_Display_Flag=0;
	}
	if(Kaysta==1)				//按键短按
	{
		ARMED=0;time_tick=0;
		WorkMode++;	//模式0，Standby
		if(WorkMode==15) WorkMode=0;
		UI_Display_Flag=1;	//标记改变界面
		Kaysta=0;	//清除按键标记
	}
	OLED_Put6x12Num(32,13,Encoder[3],5,1);	OLED_Put6x12Num(98,13,Encoder[2],5,1);		//显示角度MPU6050_accel[2]
	OLED_Put6x12Num(32,26,Encoder[0],5,1);	OLED_Put6x12Num(98,26,Encoder[1],5,1);	//显示转速
	OLED_Put5x7Num1dot(30,42,Volt);																								//显示电池电压
	if(WIFI_CK==0)	OLED_Put12x12CNstr(98,39,StandbyUI_String[Lang_x][6],1);							//显示WIFI信号
	else	OLED_Put12x12CNstr(98,39,StandbyUI_String[Lang_x][7],1);											//显示WIFI信号

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
		case 1://PID设置
			if(Kaysta==1)	//按键短按
			{
				ARMED=0;time_tick=0;
				WorkMode=0x02;			//模式2，PID设置
				UI_Display_Flag=1;	//标记改变界面
				Kaysta=0;						//清除按键标记
			}
			break;
		case 2://最大倾角设置
			if(Kaysta==1)	//按键短按
			{
				ARMED=0;time_tick=0;
				WorkMode=0x03;			//模式5，安全角度设置
				UI_Display_Flag=1;	//标记改变界面
				Kaysta=0;						//清除按键标记
			}
			break;
		case 3://角度校准
			if(Kaysta==1)	//按键短按
			{
				ARMED=0;time_tick=0;
				WorkMode=0x04;			//模式6，角度校准
				UI_Display_Flag=1;	//标记改变界面
				Kaysta=0;						//清除按键标记
			}
			break;
		case 4://
			if(Kaysta==1)	//按键短按
			{
				ARMED=0;time_tick=0;
				WorkMode=0x05;			//速度设置模式
				UI_Display_Flag=1;	//标记改变界面
				Kaysta=0;						//清除按键标记
			}
			break;
		case 5://
			if(Kaysta==1)	//按键短按
			{
				ARMED=0;time_tick=0;
				WorkMode=0x06;			//语言设置模式
				UI_Display_Flag=1;	//标记改变界面
				Kaysta=0;						//清除按键标记
			}
			break;
		case 6://
			if(Kaysta==1)	//按键短按
			{
				ARMED=0;time_tick=0;
				WorkMode=0x07;			//其他设置模式
				UI_Display_Flag=1;	//标记改变界面
				Kaysta=0;						//清除按键标记
			}
			break;
		case 7://
			if(Kaysta==1)	//按键短按
			{
				ARMED=0;time_tick=0;
				WorkMode=0x08;			//恢复出厂设置模式
				UI_Display_Flag=1;	//标记改变界面
				Kaysta=0;						//清除按键标记
			}
			break;
		case 9:	//退出
			if(Kaysta==1)	//按键短按
			{
				ARMED=0;time_tick=0;
				WorkMode=0x00;			//模式0，Standby
				UI_Display_Flag=1;	//标记改变界面
				Kaysta=0;						//清除按键标记
			}
			break;			
		default:
			break;
	}
	Display_Menux(Menux);	//显示光标
}

void PID_Settings(void)
{
	if(UI_Display_Flag==1)	
	{	
		OLED_ClearBuffer(0);
		OLED_Put12x12CNstr(24,0,Title_Strings[WorkMode][Lang_x],1);	
		OLED_Put12x12CNstr(0,13,PIDsetUI_String[Lang_x][0],1);OLED_Put12x12CNstr(24,13,"P：   I：   D：  ",1);
//		OLED_Put12x12CNstr(0,26,PIDsetUI_String[Lang_x][1],1);OLED_Put12x12CNstr(24,26,"P：   I：   D：  ",1);
//		OLED_Put12x12CNstr(0,39,PIDsetUI_String[Lang_x][2],1);OLED_Put12x12CNstr(24,39,"P：   I：   D：  ",1);
		Flash_Read_PID();	//读取PID数值
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
	Display_PID_data();		//显示PID数值
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
		case 4:		//保存
			if(Kaysta==1)	//按键短按
			{			
				Flash_Save_PID();
				Display_SaveMasBox();//显示信息框
				Flash_Save_Delay();
				WorkMode=0x00;	//模式0，Standby
				UI_Display_Flag=1;	//标记改变界面
				Kaysta=0;		//清除按键标记
				ARMED=0;time_tick=0;
			}		
			break;
		case 5:		//退出
			if(Kaysta==1)	//按键短按
			{
				ARMED=0;time_tick=0;
				WorkMode=0x00;	//模式0，Standby
				UI_Display_Flag=1;	//标记改变界面
				Kaysta=0;		//清除按键标记
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
//	if(Kaysta==1)	//按键短按
//	{		
//		AdjustAngle_Flag=599;
//		Kaysta=0;		//清除按键标记
//	}	
//	if(AdjustAngle_Flag==1)	//开始校准
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
//		WorkMode=0x00;	//模式0，Standby
//		UI_Display_Flag=1;	//标记改变界面
//		delay_ms(500);
//	}
//	else	if(AdjustAngle_Flag>1)//准备校准
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
	if(Motor_DrctAdj[3]==-1)	OLED_Put12x12CNstr(0+36,13,"正向",xx==1?0:1);	
	else 										OLED_Put12x12CNstr(0+36,13,"反向",xx==1?0:1);	
	
	if(Motor_DrctAdj[2]==-1)	OLED_Put12x12CNstr(66+36,13,"正向",xx==3?0:1);
	else										OLED_Put12x12CNstr(66+36,13,"反向",xx==3?0:1);
	
	if(Motor_DrctAdj[0]==-1)	OLED_Put12x12CNstr(0+36,26,"正向",xx==2?0:1);	
	else 										OLED_Put12x12CNstr(0+36,26,"反向",xx==2?0:1);

	if(Motor_DrctAdj[1]==-1)	OLED_Put12x12CNstr(66+36,26,"正向",xx==4?0:1);
	else 										OLED_Put12x12CNstr(66+36,26,"反向",xx==4?0:1);
}


void Motor_Settings(void)
{
	if(UI_Display_Flag==1)
	{
		OLED_ClearBuffer(0);
		OLED_Put12x12CNstr(24,0,Title_Strings[WorkMode][Lang_x],1);
		OLED_Put12x12CNstr(0,13,StandbyUI_String[Lang_x][0],1);			OLED_Put12x12CNstr(66,13,StandbyUI_String[Lang_x][1],1);
		OLED_Put12x12CNstr(0,26,StandbyUI_String[Lang_x][2],1);			OLED_Put12x12CNstr(66,26,StandbyUI_String[Lang_x][3],1);
		Flash_Read_MotorDrctAdj();//读取电机转向调整值
		Display_Motor_DrctAdj(Menux);	//显示电机转向调整值
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
	Display_Motor_DrctAdj(Menux);	//显示电机转向调整值
	switch(Menux)
	{
		case 1:
			if(RightKnob>100)
			{
				BEEP_ShortRing();//蜂鸣器短叫一声
				if(Motor_DrctAdj[3]==-1) Motor_DrctAdj[3]=1;
				else Motor_DrctAdj[3]=-1;
				RightKnob=0;
			}
			if(RightKnob<-100)
			{
				BEEP_ShortRing();//蜂鸣器短叫一声
				if(Motor_DrctAdj[3]==-1) Motor_DrctAdj[3]=1;
				else Motor_DrctAdj[3]=-1;
				RightKnob=0;
			}
			Keytimes=0;
			break;
		case 2:
			if(RightKnob>100)
			{
				BEEP_ShortRing();//蜂鸣器短叫一声
				if(Motor_DrctAdj[0]==-1) Motor_DrctAdj[0]=1;
				else Motor_DrctAdj[0]=-1;
				RightKnob=0;
			}
			if(RightKnob<-100)
			{
				BEEP_ShortRing();//蜂鸣器短叫一声
				if(Motor_DrctAdj[0]==-1) Motor_DrctAdj[0]=1;
				else Motor_DrctAdj[0]=-1;
				RightKnob=0;
			}
			Keytimes=0;
			break;
		case 3:
			if(RightKnob>100)
			{
				BEEP_ShortRing();//蜂鸣器短叫一声
				if(Motor_DrctAdj[2]==-1) Motor_DrctAdj[2]=1;
				else Motor_DrctAdj[2]=-1;
				RightKnob=0;
			}
			if(RightKnob<-100)
			{
				BEEP_ShortRing();//蜂鸣器短叫一声
				if(Motor_DrctAdj[2]==-1) Motor_DrctAdj[2]=1;
				else Motor_DrctAdj[2]=-1;
				RightKnob=0;
			}
			Keytimes=0;
			break;
		case 4:
			if(RightKnob>100)
			{
				BEEP_ShortRing();//蜂鸣器短叫一声
				if(Motor_DrctAdj[1]==-1) Motor_DrctAdj[1]=1;
				else Motor_DrctAdj[1]=-1;
				RightKnob=0;
			}
			if(RightKnob<-100)
			{
				BEEP_ShortRing();//蜂鸣器短叫一声
				if(Motor_DrctAdj[1]==-1) Motor_DrctAdj[1]=1;
				else Motor_DrctAdj[1]=-1;
				RightKnob=0;
			}
			Keytimes=0;
			break;
		case 5:		//保存
			if(Kaysta==1)	//按键短按
			{
				Flash_Save_MotorDrctAdj();
				Display_SaveMasBox();//显示信息框
				delay_ms(100);
				Flash_Save_Delay();
				WorkMode=0x00;	//模式0，Standby
				UI_Display_Flag=1;	//标记改变界面
				Kaysta=0;		//清除按键标记
				ARMED=0;time_tick=0;
			}	
			break;
		case 6:		//退出
			if(Kaysta==1)	//按键短按
			{
				ARMED=0;time_tick=0;
				WorkMode=0x00;	//模式0，Standby
				UI_Display_Flag=1;	//标记改变界面
				Kaysta=0;		//清除按键标记
			}	
			break;
		default:
			break;
	}				
}


//显示信息框
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
		OLED_Put12x12CNstr(24,13,"简体中文",1);
		OLED_Put12x12CNstr(24,26,"繁體中文",1);
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
		case 1://简体中文
			if(Kaysta==1)	//按键短按
			{
				ARMED=0;time_tick=0;Lang_x=0;						//界面语言设置
				Flash_Save_Language();
				delay_ms(100);
				Flash_Save_Delay();
				WorkMode=0x00;	//模式
				UI_Display_Flag=1;	//标记改变界面
				Kaysta=0;		//清除按键标记
			}
			break;
		case 2://繁体中文
			if(Kaysta==1)	//按键短按
			{
				ARMED=0;time_tick=0;Lang_x=1;						//界面语言设置
				Flash_Save_Language();
				delay_ms(100);
				Flash_Save_Delay();
				WorkMode=0x00;	//模式
				UI_Display_Flag=1;	//标记改变界面
				Kaysta=0;		//清除按键标记
			}
			break;
		case 3://英文
			if(Kaysta==1)	//按键短按
			{
				ARMED=0;time_tick=0;Lang_x=2;						//界面语言设置
				Flash_Save_Language();
				delay_ms(100);
				Flash_Save_Delay();
				WorkMode=0x00;	//模式
				UI_Display_Flag=1;	//标记改变界面
				Kaysta=0;		//清除按键标记
			}
			break;
		default:
			
			break;
	}
	Display_LanguageCursor(Menux);
}

//显示信息框
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
		case 1://否
			if(Kaysta==1)	//按键短按
			{
				ARMED=0;time_tick=0;
				WorkMode=0x00;	//模式
				UI_Display_Flag=1;	//标记改变界面
				Kaysta=0;		//清除按键标记
			}
			if(Lang_x<2)
			{	OLED_Put12x12CNstr(30,52,"否",0);OLED_Put12x12CNstr(86,52,"是",1);	}
			else
			{	OLED_Put12x12CNstr(30,52,"NO",0);OLED_Put12x12CNstr(83,52,"YES",1);	}			
			break;
		case 2://是
			if(Kaysta==1)	//按键短按
			{					
				PID_Motor[0].Pdat=16.0;	PID_Motor[0].Idat=0.0;	PID_Motor[0].Ddat=4.0;	
				PID_Motor[1].Pdat=16.0;	PID_Motor[1].Idat=0.0;	PID_Motor[1].Ddat=4.0;
				PID_Motor[2].Pdat=16.0;	PID_Motor[2].Idat=0.0;	PID_Motor[2].Ddat=4.0;
				PID_Motor[3].Pdat=16.0;	PID_Motor[3].Idat=0.0;	PID_Motor[3].Ddat=4.0;
				Motor_DrctAdj[0]=1;Motor_DrctAdj[1]=1;Motor_DrctAdj[2]=1;Motor_DrctAdj[3]=1;
				Lang_x=0;						//界面语言设置				
//				Front_SafeAngle=60.0;			Back_SafeAngle=-60.0;
				Flash_Save_PID();				//写入PID值
				Flash_Save_MotorDrctAdj(); //写入电机方向配置
//				Flash_Save_SafeAngle();	//写入安全角度值	
				Display_FactoryBox();		//显示提示信息
				delay_ms(100);
				Flash_Save_Delay();
				ARMED=0;time_tick=0;
				WorkMode=0x00;					//模式
				UI_Display_Flag=1;	//标记改变界面
				Kaysta=0;								//清除按键标记
			}
			if(Lang_x<2)
			{	OLED_Put12x12CNstr(30,52,"否",1);OLED_Put12x12CNstr(86,52,"是",0);	 }
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
		Flash_Read_VoltWarn();				//读取数值
		Display_OtherSettings_data();	//显示数值
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
	Display_OtherSettings_data();	//显示数值
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
		case 3:		//保存
			if(Kaysta==1)	//按键短按
			{
				Flash_Save_VoltWarn();
				Display_SaveMasBox();//显示信息框
				delay_ms(100);
				Flash_Save_Delay();
				WorkMode=0x00;	//模式0，Standby
				UI_Display_Flag=1;	//标记改变界面
				Kaysta=0;		//清除按键标记
				ARMED=0;time_tick=0;
			}	
			break;
		case 4:		//退出
			if(Kaysta==1)	//按键短按
			{
				ARMED=0;time_tick=0;
				WorkMode=0x00;	//模式0，Standby
				UI_Display_Flag=1;	//标记改变界面
				Kaysta=0;		//清除按键标记
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
		WorkMode=0;		UI_Display_Flag=1;						//标记改变界面
	}
}

