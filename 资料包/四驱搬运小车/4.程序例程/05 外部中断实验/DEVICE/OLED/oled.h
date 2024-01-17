/***************************************************************************
������ɹ�ѧϰʹ�ã�����������Ҳ������������;��������������飬��ӭ���罻��ָ��
Ӳ���汾��FreeLink STM32��·ֱ��������ư�V2.41
����оƬ��STM32F103RCT6
�������ƣ�STM32����ȫ���ƶ�С������--OLED��������
��������������OLED��ʾ�ַ������֡�ͼ��
������ߣ�ˮľ����@FreeLink
��ϵ�绰��15802781535
��ϵ���䣺357704597@qq.com
�޸�ʱ�䣺2019/09/16
����汾��V2.1
Copyright(C) �人��������˿Ƽ����޹�˾ 
All rights reserved
*****************************************************************************
�޸�˵����
��


*****************************************************************************/

#ifndef _OLED_H
#define _OLED_H

#include "sys.h"
#include "delay.h"
#include "Bitmap.h"

//#define OLED_SCL(x)  PCout(11) = x
//#define OLED_SDA(x)  PCout(10) = x
//#define OLED_RST(x)  PCout(9) = x
//#define OLED_DC(x)   PCout(8) = x

#define XLevelL			0x00
#define XLevelH			0x10
#define XLevel	    ((XLevelH&0x0F)*16+XLevelL)
#define Max_Column	128
#define Max_Row			64
#define	Brightness	0xCF
#define X_WIDTH 		128
#define Y_WIDTH 		64

extern unsigned char OLED_Buffer[8][128];

void OLED_WrDat(unsigned char dat);
void OLED_WrCmd(unsigned char cmd);
void OLED_Set_Pos(unsigned char x,unsigned char y);
void OLED_CLS(void);
void OLED_Init(void);
void OLED_Draw_FreeLink(void);

void OLED_Refresh_GDRAM(unsigned char displayxx);
void OLED_Refresh_AllGDRAM(void);
void OLED_PutChar(unsigned char x, unsigned char y, unsigned char ch,unsigned char Col);
void OLED_PutChar0612(unsigned char x, unsigned char y, unsigned char ch,unsigned char Col);
void OLED_PutChar5x7(unsigned char x, unsigned char y, unsigned char ch,unsigned char Col);
void OLED_WriteChstr1414(u8 x,u8 y,unsigned char *Chstr,u8 Col);
void OLED_WriteChstr1212(u8 x,u8 y,unsigned char *Chstr,u8 Col);
void OLED_WriteENstr5x7(u8 x,u8 y,unsigned char *ENstr,u8 Col);
void OLED_Draw_dots(unsigned char xx,unsigned char yy,unsigned char color);
void OLED_Drawline_X(unsigned char X0,unsigned char X1,unsigned char Y,unsigned char Color);//�� X��ˮƽ��
void OLED_Drawline_Y(unsigned char X,unsigned char Y0,unsigned char Y1,unsigned char Color);//�� Y��ˮƽ��
void OLED_Drawline(short int X0, short int Y0, short int X1 ,short int Y1, short int Color);//��б��
void OLED_Draw_lingxing(short int X0,short int Y0,short int R);	  //������
void OLED_Draw_circle(short int X0,short int Y0,short int R);	  //��Բ
void OLED_Clear(unsigned char Col);

void OLED_Display0612Num(unsigned char xx,unsigned char yy,int Num,unsigned char bitlen,unsigned char Col);
void OLED_Display0612Num1dot(unsigned char xx,unsigned char yy,int Num,unsigned char Col);
void OLED_Display0507Num1dot(unsigned char xx,unsigned char yy,int Num);

void OLED_WriteStr(u8 x,u8 y,u8 sty,u8 ch[]);
#endif

/*************************************END************************************/


