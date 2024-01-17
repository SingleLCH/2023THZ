/***************************************************************************
本程序可供学习使用，经作者允许，也可用于其他用途。如有意见及建议，欢迎来电交流指教
硬件版本：FreeLink STM32四路直流电机控制板V2.41
主控芯片：STM32F103RCT6
程序名称：STM32麦轮全向移动小车程序--主程序
功能描述：通过手机蓝牙APP无线遥控麦克纳姆轮小车全向移动
设计作者：水木蓝青@FreeLink
联系电话：15802781535
联系邮箱：357704597@qq.com
修改时间：2019/09/16
程序版本：V2.1
版权所有，盗版必究
Copyright(C) 武汉飞令机器人科技有限公司 
All rights reserved
*****************************************************************************
修改说明：
无


*****************************************************************************/



#include "UI.h"


int main(void)
{		
  Stm32_Clock_Init(9);				//系统时钟设置
	JTAG_Set(JTAG_SWD_DISABLE); //先关闭JATG SWD端口。4路编码器中，需要占用JTAG端口中的部分引脚
	AFIO->MAPR|=0x1<<8;					//timer2部分重映射，CH1---PA15,CH2---PB3（解码编码器的4个timer中有共用的引脚，因此要重映射）
	JTAG_Set(SWD_ENABLE); 			//只开启SWD端口。（必须开启，不然断电后，无法调试或下载程序。此时设置BOOT0为高电平才可以）
	delay_init(72);							//延时初始化
	MYDMA_Config(DMA1_Channel3,(u32)&USART3->DR,(u32)Blue_dat,UART_RECV_LEN);//DMA1通道4,外设为串口1,存储器为SendBuff,长度200.	
	USART3_Init(36,9600);		//初始化蓝牙串口通信接口
	Motor_Init();						//PWM输出初始化	
  Encoder_Init();					//初始化编码器
 	Set_Motor(0,0,0,0);			//电机PWM输出
	Adc_Init();				 			//初始化ADC，检测电池电压
	PID_Init();
	Flash_Read_MotorDrctAdj();//读取电机转向调整值
	Flash_Read_Language();		//界面语言设置
	Flash_Read_VoltWarn();		//读取报警电压
	
	KEY_Init();         //KEY初始化
	LED_Init();         //LED初始化	 
	BEEP_Init();
	OLED_Init();
	OLED_Draw_FreeLink();			//显示图标
	OLED_Refresh_AllGDRAM(); 	//刷新显示
	Blue_RC[0]=0;Blue_RC[1]=0;Blue_RC[2]=0;
	
	UI_Display_Flag=1;				//标记改变界面	

	TIM6_Int_Init(5000,72);	  //5ms中断溢出
	BEEP_state=BEEP_SHORT2FLASH;BEEP_timetick=1;
	while(1)
	{
		OLED_Refresh_AllGDRAM(); 	//刷新显示
		if(Control_Flag)
		{	
			switch(WorkMode)
			{
	////////////////////待机模式////////////////////////
				case 0:
					Standby_Mode();
				break;
	////////////////////设置模式////////////////////////
				case 1:
					Settings_Mode();
					break;	
	////////////////////PID设置////////////////////////
				case 2:
					PID_Settings();
					break;
	/////////////////////电机设置//////////////////////
				case 3:
					Motor_Settings();
					break;
	/////////////////////角度校准//////////////////////
				case 4:
//					AdjustAngle_Settings();
				break;
	/////////////////////速度设定//////////////////////	
				case 5:break;
	/////////////////////语言设置//////////////////////		
				case 6:LanguageSettings_Mode();break;
	/////////////////////出厂设置//////////////////////		
				case 7:FactorySettings_Mode();break;						
	/////////////////////其他设置//////////////////////		
				case 8:Other_Settings_Mode();break;
	/////////////////////电压低报警////////////////////						
				case 10:VoltWarn_Mode();break;

	////////////////////遥控模式////////////////////////
				case 13://PS遥控
				case 11:
				case 12:
				case 14:
					Remote_Mode();
				break;	
				default:			break;
			}
			Control_Flag=0;
			if(AdjustAngle_Flag>0)	AdjustAngle_Flag--;	
//			Send_Data();	//发送数据
			Volt=Get_Adc(4);Volt=Volt*33*11/4096;	//检测电压
			if(WorkMode==0x00 || WorkMode==0x01)
			{
				if( Volt<VoltLowWarn) 
				{
					if(VoltLowTimes<600)VoltLowTimes++;
					else {WorkMode=0x0A;ARMED=0;time_tick=0;}
				}
				else	VoltLowTimes=0;
			}
		}
	} 
}





/*************************************END************************************/







