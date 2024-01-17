/***************************************************************************
������ɹ�ѧϰʹ�ã�����������Ҳ������������;��������������飬��ӭ���罻��ָ��
Ӳ��˵������·���ֶ๦�ܿ��ư�
����оƬ��STM32F103RCT6
�������ƣ�STM32����ȫ���ƶ�С������--�������ճ���
����������ͨ��DMA���մ����������ݣ�������������֡���ң����ֵ


*****************************************************************************/


#include "usart.h"	  
#include "bluetooth.h"	

unsigned char BlueCK=0;									//�����źż��
unsigned char Blue_dat[UART_RECV_LEN];	//����3���ջ���
short int Blue_RC[4];										//������ң����ֵ


void BlueTooth_Receive(void)
{
	unsigned short int startx,nx;
	unsigned char CMD_Buf[BLUE_CMD_LEN];
	
	for(startx=0;startx<UART_RECV_LEN-1;startx++)//����֡ͷ
	{
		if( (Blue_dat[startx]==0xAA )&& (Blue_dat[startx+1]==0xAA))	//�ҵ�֡ͷ
		{
			BlueCK=51;//DMA_Count0=startx;
			Blue_dat[startx]=0;Blue_dat[startx+1]=0;//���֡ͷ
			break; //�ҵ���Ч֡
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
	if(CMD_Buf[3]&0x80)	//Ϊ����
		Blue_RC[3]=0-(256-CMD_Buf[3]);
	else
		Blue_RC[3]=CMD_Buf[3];
//	
	if(CMD_Buf[2]&0x80)	//Ϊ����
		Blue_RC[2]=0-(256-CMD_Buf[2]);
	else	
		Blue_RC[2]=CMD_Buf[2];
//	
	if(CMD_Buf[4]&0x80)	//Ϊ����
		Blue_RC[1]=0-(256-CMD_Buf[4]);
	else
		Blue_RC[1]=CMD_Buf[4];
//
	if(CMD_Buf[5]&0x80)	//Ϊ����
		Blue_RC[0]=0-(256-CMD_Buf[5]);
	else
		Blue_RC[0]=CMD_Buf[5];
	
}



