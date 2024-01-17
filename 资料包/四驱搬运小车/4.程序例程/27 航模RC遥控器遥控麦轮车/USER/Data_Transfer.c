/***************************************************************************
������ɹ�ѧϰʹ�ã�����������Ҳ������������;��������������飬��ӭ���罻��ָ��
Ӳ���汾����·���ֶ๦�ܿ��ư�
����оƬ��STM32F103RCT6
�������ƣ�STM32����ȫ���ƶ�С������--������Ϣ�������������
����������Ƭ��FLASH��ΪEEPROM���籣�����ݣ��洢�Ͷ�ȡ����



*****************************************************************************/


#include "UI.h"

int LeftKnob=0,RightKnob=0;		//��������ת�������

unsigned int time_tick=0;			//Time������
unsigned char Control_Flag=0;	



//////////////////////////////////////////////////////////////////////////////////////
//д��PIDֵ
void Flash_Save_PID(void)
{
	u8 point5=0;
	unsigned short int PIDflash_data[10];	
	
	//����Flash����
	STMFLASH_Read(FLASH_SAVE_ADDR + PID_FLASHADR ,PIDflash_data,10);
	
	PIDflash_data[point5++] = (s16)(PID_Motor[0].Pdat);
	PIDflash_data[point5++] = (s16)(PID_Motor[0].Idat);
	PIDflash_data[point5++] = (s16)(PID_Motor[0].Ddat);
	
//	PIDflash_data[point5++] = (s16)(PID_Motor[1].Pdat);
//	PIDflash_data[point5++] = (s16)(PID_Motor[1].Idat);
//	PIDflash_data[point5++] = (s16)(PID_Motor[1].Ddat);
//	
//	PIDflash_data[point5++] = (s16)(PID_Motor[2].Pdat);
//	PIDflash_data[point5++] = (s16)(PID_Motor[2].Idat);
//	PIDflash_data[point5++] = (s16)(PID_Motor[2].Ddat);

//	PIDflash_data[point5++] = (s16)(PID_Motor[3].Pdat);
//	PIDflash_data[point5++] = (s16)(PID_Motor[3].Idat);
//	PIDflash_data[point5++] = (s16)(PID_Motor[3].Ddat);
	
	//д��Flash����
	STMFLASH_Write(FLASH_SAVE_ADDR + PID_FLASHADR ,PIDflash_data,10);

}

//��ȡFlash��ĵ�PIDֵ
void Flash_Read_PID(void)
{
	u8 point6=0;
	unsigned short int PIDflash_readdata[10];
	//����Flash����
	STMFLASH_Read(FLASH_SAVE_ADDR + PID_FLASHADR ,PIDflash_readdata,10);
	
	//������ǰPIDֵ
	PID_Motor[0].Pdat = (float)PIDflash_readdata[point6++];
	PID_Motor[0].Idat = (float)PIDflash_readdata[point6++];
	PID_Motor[0].Ddat = (float)PIDflash_readdata[point6++];
		
	PID_Motor[1].Pdat=PID_Motor[0].Pdat;
	PID_Motor[1].Idat=PID_Motor[0].Idat;
	PID_Motor[1].Ddat=PID_Motor[0].Idat;
	
	PID_Motor[2].Pdat=PID_Motor[0].Pdat;
	PID_Motor[2].Idat=PID_Motor[0].Idat;
	PID_Motor[2].Ddat=PID_Motor[0].Ddat;

	PID_Motor[3].Pdat=PID_Motor[0].Pdat;
	PID_Motor[3].Idat=PID_Motor[0].Idat;
	PID_Motor[3].Ddat=PID_Motor[0].Ddat;
	
//	PID_Motor[1].Pdat = (float)PIDflash_readdata[point6++];
//	PID_Motor[1].Idat = (float)PIDflash_readdata[point6++];
//	PID_Motor[1].Ddat = (float)PIDflash_readdata[point6++];
//	
//	PID_Motor[2].Pdat = (float)PIDflash_readdata[point6++];
//	PID_Motor[2].Idat = (float)PIDflash_readdata[point6++];
//	PID_Motor[2].Ddat = (float)PIDflash_readdata[point6++];
//	
//	PID_Motor[3].Pdat = (float)PIDflash_readdata[point6++];
//	PID_Motor[3].Idat = (float)PIDflash_readdata[point6++];
//	PID_Motor[3].Ddat = (float)PIDflash_readdata[point6++];
}




void Flash_Save_Language(void)//д������
{
	u8 point9=0;
	unsigned short int Language_data[10];
	//����Flash����
	STMFLASH_Read(FLASH_SAVE_ADDR + LANGUAGE_FLASHADR ,Language_data,10);
	
	Language_data[point9++] =Lang_x;	
	//д��Flash����
	STMFLASH_Write(FLASH_SAVE_ADDR + LANGUAGE_FLASHADR ,Language_data,10);	
}
void Flash_Read_Language(void)//��ȡ����
{
	u8 point10=0;
	unsigned short int Language_readdata[10];
	//����Flash����
	STMFLASH_Read(FLASH_SAVE_ADDR + LANGUAGE_FLASHADR ,Language_readdata,10);
	
	Lang_x=Language_readdata[point10++];
	if(Lang_x>2 || Lang_x<0) Lang_x=0;
}



void Flash_Save_VoltWarn(void)//д��͵�ѹ����ֵ
{
	u8 point11=0;
	unsigned short int VoltWarn_data[10];
	//����Flash����
	STMFLASH_Read(FLASH_SAVE_ADDR + VOLTWARN_FLASHADR ,VoltWarn_data,10);
	
	VoltWarn_data[point11++] =VoltLowWarn;	
	//д��Flash����
	STMFLASH_Write(FLASH_SAVE_ADDR + VOLTWARN_FLASHADR ,VoltWarn_data,10);	
}
void Flash_Read_VoltWarn(void)//��ȡ�͵�ѹ����ֵ
{
	u8 point12=0;
	unsigned short int VoltWarn_readdata[10];
	//����Flash����
	STMFLASH_Read(FLASH_SAVE_ADDR + VOLTWARN_FLASHADR ,VoltWarn_readdata,10);
	
	VoltLowWarn=VoltWarn_readdata[point12++];
	if(VoltLowWarn>120 || VoltLowWarn<65) VoltLowWarn=70;
}

void Flash_Save_MotorDrctAdj(void)//д����ת�����ֵ
{
	unsigned short int DrctAdj_readdata[4];
	//����Flash����
	STMFLASH_Read(FLASH_SAVE_ADDR + MOTORADJ_FLASHADR ,DrctAdj_readdata,4);
	
	DrctAdj_readdata[0] =Motor_DrctAdj[0]==1?0:1;	
	DrctAdj_readdata[1] =Motor_DrctAdj[1]==1?0:1;
	DrctAdj_readdata[2] =Motor_DrctAdj[2]==1?0:1;
	DrctAdj_readdata[3] =Motor_DrctAdj[3]==1?0:1;
	//д��Flash����
	STMFLASH_Write(FLASH_SAVE_ADDR + MOTORADJ_FLASHADR ,DrctAdj_readdata,4);	
	
}
void Flash_Read_MotorDrctAdj(void)//��ȡ���ת�����ֵ
{
	unsigned short int DrctAdj_readdata[4];
	//����Flash����
	STMFLASH_Read(FLASH_SAVE_ADDR + MOTORADJ_FLASHADR ,DrctAdj_readdata,4);
	
	Motor_DrctAdj[0]=DrctAdj_readdata[0]==0?1:-1;
	Motor_DrctAdj[1]=DrctAdj_readdata[1]==0?1:-1;
	Motor_DrctAdj[2]=DrctAdj_readdata[2]==0?1:-1;
	Motor_DrctAdj[3]=DrctAdj_readdata[3]==0?1:-1;
	
}

void Flash_Save_Delay(void)
{
	EXTI->IMR&=~(1<<3);//  �ر���line BITx�ϵ��ж�
	delay_ms(900);
	EXTI->IMR|=1<<3;//  ����line BITx�ϵ��ж�
}


/*************************************END************************************/

