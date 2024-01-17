/***************************************************************************
������ɹ�ѧϰʹ�ã�����������Ҳ������������;��������������飬��ӭ���罻��ָ��
Ӳ���汾��FreeLink STM32��·ֱ��������ư�V2.41
����оƬ��STM32F103RCT6
�������ƣ�STM32����ȫ���ƶ�С������--������
����������ͨ���ֻ�����APP����ң�������ķ��С��ȫ���ƶ�
������ߣ�ˮľ����@FreeLink
��ϵ�绰��15802781535
��ϵ���䣺357704597@qq.com
�޸�ʱ�䣺2019/09/16
����汾��V2.1
��Ȩ���У�����ؾ�
Copyright(C) �人��������˿Ƽ����޹�˾ 
All rights reserved
*****************************************************************************
�޸�˵����
��


*****************************************************************************/



#include "UI.h"


int main(void)
{		
  Stm32_Clock_Init(9);				//ϵͳʱ������
	JTAG_Set(JTAG_SWD_DISABLE); //�ȹر�JATG SWD�˿ڡ�4·�������У���Ҫռ��JTAG�˿��еĲ�������
	AFIO->MAPR|=0x1<<8;					//timer2������ӳ�䣬CH1---PA15,CH2---PB3�������������4��timer���й��õ����ţ����Ҫ��ӳ�䣩
	JTAG_Set(SWD_ENABLE); 			//ֻ����SWD�˿ڡ������뿪������Ȼ�ϵ���޷����Ի����س��򡣴�ʱ����BOOT0Ϊ�ߵ�ƽ�ſ��ԣ�
	delay_init(72);							//��ʱ��ʼ��
	MYDMA_Config(DMA1_Channel3,(u32)&USART3->DR,(u32)Blue_dat,UART_RECV_LEN);//DMA1ͨ��4,����Ϊ����1,�洢��ΪSendBuff,����200.	
	USART3_Init(36,9600);		//��ʼ����������ͨ�Žӿ�
	Motor_Init();						//PWM�����ʼ��	
  Encoder_Init();					//��ʼ��������
 	Set_Motor(0,0,0,0);			//���PWM���
	Adc_Init();				 			//��ʼ��ADC������ص�ѹ
	PID_Init();
	Flash_Read_MotorDrctAdj();//��ȡ���ת�����ֵ
	Flash_Read_Language();		//������������
	Flash_Read_VoltWarn();		//��ȡ������ѹ
	
	KEY_Init();         //KEY��ʼ��
	LED_Init();         //LED��ʼ��	 
	BEEP_Init();
	OLED_Init();
	OLED_Draw_FreeLink();			//��ʾͼ��
	OLED_Refresh_AllGDRAM(); 	//ˢ����ʾ
	Blue_RC[0]=0;Blue_RC[1]=0;Blue_RC[2]=0;
	
	UI_Display_Flag=1;				//��Ǹı����	

	TIM6_Int_Init(5000,72);	  //5ms�ж����
	BEEP_state=BEEP_SHORT2FLASH;BEEP_timetick=1;
	while(1)
	{
		OLED_Refresh_AllGDRAM(); 	//ˢ����ʾ
		if(Control_Flag)
		{	
			switch(WorkMode)
			{
	////////////////////����ģʽ////////////////////////
				case 0:
					Standby_Mode();
				break;
	////////////////////����ģʽ////////////////////////
				case 1:
					Settings_Mode();
					break;	
	////////////////////PID����////////////////////////
				case 2:
					PID_Settings();
					break;
	/////////////////////�������//////////////////////
				case 3:
					Motor_Settings();
					break;
	/////////////////////�Ƕ�У׼//////////////////////
				case 4:
//					AdjustAngle_Settings();
				break;
	/////////////////////�ٶ��趨//////////////////////	
				case 5:break;
	/////////////////////��������//////////////////////		
				case 6:LanguageSettings_Mode();break;
	/////////////////////��������//////////////////////		
				case 7:FactorySettings_Mode();break;						
	/////////////////////��������//////////////////////		
				case 8:Other_Settings_Mode();break;
	/////////////////////��ѹ�ͱ���////////////////////						
				case 10:VoltWarn_Mode();break;

	////////////////////ң��ģʽ////////////////////////
				case 13://PSң��
				case 11:
				case 12:
				case 14:
					Remote_Mode();
				break;	
				default:			break;
			}
			Control_Flag=0;
			if(AdjustAngle_Flag>0)	AdjustAngle_Flag--;	
//			Send_Data();	//��������
			Volt=Get_Adc(4);Volt=Volt*33*11/4096;	//����ѹ
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







