/***************************************************************************
本程序可供学习使用，经作者允许，也可用于其他用途。如有意见及建议，欢迎来电交流指教
硬件版本：FreeLink STM32四路直流电机控制板V2.41
主控芯片：STM32F103RCT6
程序名称：STM32麦轮全向移动小车程序--OLED驱动程序
功能描述：驱动OLED显示字符、汉字、图像。
设计作者：水木蓝青@FreeLink
联系电话：15802781535
联系邮箱：357704597@qq.com
修改时间：2019/09/16
程序版本：V2.1
Copyright(C) 武汉飞令机器人科技有限公司 
All rights reserved
*****************************************************************************
修改说明：
无


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
void OLED_Drawline_X(unsigned char X0,unsigned char X1,unsigned char Y,unsigned char Color);//画 X轴水平线
void OLED_Drawline_Y(unsigned char X,unsigned char Y0,unsigned char Y1,unsigned char Color);//画 Y轴水平线
void OLED_Drawline(short int X0, short int Y0, short int X1 ,short int Y1, short int Color);//画斜线
void OLED_Draw_lingxing(short int X0,short int Y0,short int R);	  //画菱形
void OLED_Draw_circle(short int X0,short int Y0,short int R);	  //画圆
void OLED_Clear(unsigned char Col);

void OLED_Display0612Num(unsigned char xx,unsigned char yy,int Num,unsigned char bitlen,unsigned char Col);
void OLED_Display0612Num1dot(unsigned char xx,unsigned char yy,int Num,unsigned char Col);
void OLED_Display0507Num1dot(unsigned char xx,unsigned char yy,int Num);

void OLED_WriteStr(u8 x,u8 y,u8 sty,u8 ch[]);
#endif

/*************************************END************************************/


