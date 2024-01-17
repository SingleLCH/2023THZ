/***************************************************************************
本程序可供学习使用，经作者允许，也可用于其他用途。如有意见及建议，欢迎来电交流指教
硬件说明：四路麦轮多功能控制板
主控芯片：STM32F103RCT6
程序名称：STM32麦轮全向移动小车程序--蓝牙接收程序
功能描述：通过DMA接收串口蓝牙数据，并解析出数据帧里的遥控数值


*****************************************************************************/


#include "usart.h"	  
#include "bluetooth.h"	

unsigned char BlueCK=0;									//蓝牙信号检测
unsigned char Blue_dat[UART_RECV_LEN];	//串口3接收缓存
short int Blue_RC[4];										//解析的遥控数值


void BlueTooth_Receive(void)
{
	unsigned short int startx,nx;
	unsigned char CMD_Buf[BLUE_CMD_LEN];
	
	for(startx=0;startx<UART_RECV_LEN-1;startx++)//搜索帧头
	{
		if( (Blue_dat[startx]==0xAA )&& (Blue_dat[startx+1]==0xAA))	//找到帧头
		{
			BlueCK=51;//DMA_Count0=startx;
			Blue_dat[startx]=0;Blue_dat[startx+1]=0;//清除帧头
			break; //找到有效帧
		}
	}
	if(BlueCK!=51)
	{	
		if(BlueCK>0) BlueCK--;
		else 
		{
			Blue_RC[2]=0;Blue_RC[1]=0;Blue_RC[0]=0;
		}
		return;
	}
//////////////////////////////////////////////////
	BlueCK=50;
	for(nx=0;startx<UART_RECV_LEN && nx<BLUE_CMD_LEN;startx++)
	{
		CMD_Buf[nx++]=Blue_dat[startx];
	}
	for(startx=0;nx<BLUE_CMD_LEN;nx++)
	{
		CMD_Buf[nx]=Blue_dat[startx++];
	}
//////////////////////////////////////////////////////////////	
	if(CMD_Buf[3]&0x80)	//为负数
		Blue_RC[3]=0-(256-CMD_Buf[3]);
	else
		Blue_RC[3]=CMD_Buf[3];
//	
	if(CMD_Buf[2]&0x80)	//为负数
		Blue_RC[2]=0-(256-CMD_Buf[2]);
	else	
		Blue_RC[2]=CMD_Buf[2];
//	
	if(CMD_Buf[4]&0x80)	//为负数
		Blue_RC[1]=0-(256-CMD_Buf[4]);
	else
		Blue_RC[1]=CMD_Buf[4];
//
	if(CMD_Buf[5]&0x80)	//为负数
		Blue_RC[0]=0-(256-CMD_Buf[5]);
	else
		Blue_RC[0]=CMD_Buf[5];
	
}



