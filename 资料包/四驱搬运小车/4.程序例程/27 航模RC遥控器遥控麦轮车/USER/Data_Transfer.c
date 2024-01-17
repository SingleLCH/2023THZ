/***************************************************************************
本程序可供学习使用，经作者允许，也可用于其他用途。如有意见及建议，欢迎来电交流指教
硬件版本：四路麦轮多功能控制板
主控芯片：STM32F103RCT6
程序名称：STM32麦轮全向移动小车程序--数据信息交换及处理程序
功能描述：片内FLASH作为EEPROM掉电保存数据，存储和读取参数



*****************************************************************************/


#include "UI.h"

int LeftKnob=0,RightKnob=0;		//左右轮旋转脉冲计数

unsigned int time_tick=0;			//Time计数器
unsigned char Control_Flag=0;	



//////////////////////////////////////////////////////////////////////////////////////
//写入PID值
void Flash_Save_PID(void)
{
	u8 point5=0;
	unsigned short int PIDflash_data[10];	
	
	//读出Flash数据
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
	
	//写入Flash数据
	STMFLASH_Write(FLASH_SAVE_ADDR + PID_FLASHADR ,PIDflash_data,10);

}

//读取Flash里的的PID值
void Flash_Read_PID(void)
{
	u8 point6=0;
	unsigned short int PIDflash_readdata[10];
	//读出Flash数据
	STMFLASH_Read(FLASH_SAVE_ADDR + PID_FLASHADR ,PIDflash_readdata,10);
	
	//读出当前PID值
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




void Flash_Save_Language(void)//写入语言
{
	u8 point9=0;
	unsigned short int Language_data[10];
	//读出Flash数据
	STMFLASH_Read(FLASH_SAVE_ADDR + LANGUAGE_FLASHADR ,Language_data,10);
	
	Language_data[point9++] =Lang_x;	
	//写入Flash数据
	STMFLASH_Write(FLASH_SAVE_ADDR + LANGUAGE_FLASHADR ,Language_data,10);	
}
void Flash_Read_Language(void)//读取语言
{
	u8 point10=0;
	unsigned short int Language_readdata[10];
	//读出Flash数据
	STMFLASH_Read(FLASH_SAVE_ADDR + LANGUAGE_FLASHADR ,Language_readdata,10);
	
	Lang_x=Language_readdata[point10++];
	if(Lang_x>2 || Lang_x<0) Lang_x=0;
}



void Flash_Save_VoltWarn(void)//写入低电压报警值
{
	u8 point11=0;
	unsigned short int VoltWarn_data[10];
	//读出Flash数据
	STMFLASH_Read(FLASH_SAVE_ADDR + VOLTWARN_FLASHADR ,VoltWarn_data,10);
	
	VoltWarn_data[point11++] =VoltLowWarn;	
	//写入Flash数据
	STMFLASH_Write(FLASH_SAVE_ADDR + VOLTWARN_FLASHADR ,VoltWarn_data,10);	
}
void Flash_Read_VoltWarn(void)//读取低电压报警值
{
	u8 point12=0;
	unsigned short int VoltWarn_readdata[10];
	//读出Flash数据
	STMFLASH_Read(FLASH_SAVE_ADDR + VOLTWARN_FLASHADR ,VoltWarn_readdata,10);
	
	VoltLowWarn=VoltWarn_readdata[point12++];
	if(VoltLowWarn>120 || VoltLowWarn<65) VoltLowWarn=70;
}

void Flash_Save_MotorDrctAdj(void)//写入电机转向调整值
{
	unsigned short int DrctAdj_readdata[4];
	//读出Flash数据
	STMFLASH_Read(FLASH_SAVE_ADDR + MOTORADJ_FLASHADR ,DrctAdj_readdata,4);
	
	DrctAdj_readdata[0] =Motor_DrctAdj[0]==1?0:1;	
	DrctAdj_readdata[1] =Motor_DrctAdj[1]==1?0:1;
	DrctAdj_readdata[2] =Motor_DrctAdj[2]==1?0:1;
	DrctAdj_readdata[3] =Motor_DrctAdj[3]==1?0:1;
	//写入Flash数据
	STMFLASH_Write(FLASH_SAVE_ADDR + MOTORADJ_FLASHADR ,DrctAdj_readdata,4);	
	
}
void Flash_Read_MotorDrctAdj(void)//读取电机转向调整值
{
	unsigned short int DrctAdj_readdata[4];
	//读出Flash数据
	STMFLASH_Read(FLASH_SAVE_ADDR + MOTORADJ_FLASHADR ,DrctAdj_readdata,4);
	
	Motor_DrctAdj[0]=DrctAdj_readdata[0]==0?1:-1;
	Motor_DrctAdj[1]=DrctAdj_readdata[1]==0?1:-1;
	Motor_DrctAdj[2]=DrctAdj_readdata[2]==0?1:-1;
	Motor_DrctAdj[3]=DrctAdj_readdata[3]==0?1:-1;
	
}

void Flash_Save_Delay(void)
{
	EXTI->IMR&=~(1<<3);//  关闭启line BITx上的中断
	delay_ms(900);
	EXTI->IMR|=1<<3;//  开启line BITx上的中断
}


/*************************************END************************************/

