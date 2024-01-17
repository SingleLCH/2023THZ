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



#include "oled.h"
#include "math.h"
#include "stdlib.h" 
#include "cn_code.h" 
#include "en_code.h" 
#include "I2C.h"

unsigned char OLED_Buffer[8][128]={0};

//***********************************
//���ܣ�OLEDд����
//***********************************
void OLED_WrDat(unsigned char dat)
{
    IIC_Start();
    IIC_Send_Byte(0x78);
    IIC_Send_Byte(0x40);
    IIC_Send_Byte(dat);
    IIC_Stop();
}
//***********************************
//	���ܣ�OLEDд����
//***********************************
void OLED_WrCmd(unsigned char cmd)
{
	IIC_Start();
	IIC_Send_Byte(0x78);
	IIC_Send_Byte(0x00);
	IIC_Send_Byte(cmd);
	IIC_Stop();
}
//***********************************
//	����OLED ��������
//***********************************
void OLED_Set_Pos(unsigned char x,unsigned char y)
{
	OLED_WrCmd(0xb0+y);
	OLED_WrCmd(((x&0xf0)>>4)|0x10);
	OLED_WrCmd( x&0x0f);
}
//***********************************
//���ܣ�OLEDȫ��
//***********************************
void OLED_Fill(unsigned char bmp_dat)
{
	unsigned char y,x;
	for(y=0;y<8;y++)
	{
		OLED_WrCmd(0xb0+y);
		OLED_WrCmd(0x01);
		OLED_WrCmd(0x10);
		for(x=0;x<X_WIDTH;x++)
		OLED_WrDat(bmp_dat);
	}
}
//**********************************
//���ܣ�OLED��λ
//***********************************
void OLED_CLS(void)
{
	unsigned char y,x;
	for(y=0;y<8;y++)
	{
		OLED_WrCmd(0xb0+y);
		OLED_WrCmd(0x01);
		OLED_WrCmd(0x10);
		for(x=0;x<X_WIDTH;x++)

		OLED_WrDat(0);
	}
}
//***********************************
//   ���ܣ�OLED��ʼ��
//***********************************
void OLED_Init(void)
{
	IIC_Init();

//	OLED_SCL(1);
//	OLED_RST(0);
//	delay_ms(10);
//  delay_ms(10);
//	OLED_RST(1);       //���ϵ絽���濪ʼ��ʼ��Ҫ���㹻��ʱ�䣬���ȴ�RC��λ���
	OLED_WrCmd(0xae);//--turn off oled panel
	OLED_WrCmd(0x00);//---set low column address
	OLED_WrCmd(0x10);//---set high column address
	OLED_WrCmd(0x40);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	OLED_WrCmd(0x81);//--set contrast control register
	OLED_WrCmd(0xcf); // Set SEG Output Current Brightness
	OLED_WrCmd(0xa1);//--Set SEG/Column Mapping     0xa0���ҷ��� 0xa1����
	OLED_WrCmd(0xc8);//Set COM/Row Scan Direction   0xc0���·��� 0xc8����
	OLED_WrCmd(0xa6);//--set normal display
	OLED_WrCmd(0xa8);//--set multiplex ratio(1 to 64)
	OLED_WrCmd(0x3f);//--1/64 duty
	OLED_WrCmd(0xd3);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	OLED_WrCmd(0x00);//-not offset
	OLED_WrCmd(0xd5);//--set display clock divide ratio/oscillator frequency
	OLED_WrCmd(0x80);//--set divide ratio, Set Clock as 100 Frames/Sec
	OLED_WrCmd(0xd9);//--set pre-charge period
	OLED_WrCmd(0xf1);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	OLED_WrCmd(0xda);//--set com pins hardware configuration
	OLED_WrCmd(0x12);
	OLED_WrCmd(0xdb);//--set vcomh
	OLED_WrCmd(0x40);//Set VCOM Deselect Level	 ����VCOMH ��ѹ����
	OLED_WrCmd(0x20);//-Set Page Addressing Mode (0x00/0x01/0x02)
	OLED_WrCmd(0x02);//
	OLED_WrCmd(0x8d);//--set Charge Pump enable/disable
	OLED_WrCmd(0x14);//--set(0x10) disable
	OLED_WrCmd(0xa4);// Disable Entire Display On (0xa4/0xa5) ȫ����ʾ����;bit0:1,����;0,�ر�;(����/����)
	OLED_WrCmd(0xa6);// Disable Inverse Display On (0xa6/a7) ������ʾ��ʽ;bit0:1,������ʾ;0,������ʾ
	OLED_WrCmd(0xaf);//--turn on oled panel
	OLED_Fill(0x00);  //��ʼ����
	OLED_Set_Pos(0,0);
}

void OLED_Draw_FreeLink(void)
{
	unsigned char i,j;
	for(i=0;i<8;i++)
	{
		for(j=0;j<64;j++)
		{
			OLED_Buffer[i][j]=BMP_FreeLink[i][j];
		}
	}
}

void OLED_Refresh_GDRAM(unsigned char displayxx)
{
	unsigned char disjj;
	OLED_Set_Pos(0,displayxx);
	for(disjj=0;disjj<128;disjj++)
	{
		OLED_WrDat(OLED_Buffer[displayxx][disjj]);
	}	
}
void OLED_Refresh_AllGDRAM(void)
{
	OLED_Refresh_GDRAM(0);OLED_Refresh_GDRAM(1);
	OLED_Refresh_GDRAM(2);OLED_Refresh_GDRAM(3);
	OLED_Refresh_GDRAM(4);OLED_Refresh_GDRAM(5);
	OLED_Refresh_GDRAM(6);OLED_Refresh_GDRAM(7);	
}

void OLED_PutChar(unsigned char x, unsigned char y, unsigned char ch,unsigned char Col)
{  
	unsigned char  font_dat;
	unsigned char   i, j, k;
	unsigned char col1,col0;
	if(Col==0)	{	col0=0;col1=1;	}
	else				{	col0=1;col1=0;	}

	if( (ch<0x20) || (ch>0x7f) ) ch = 0x20;   
	ch -= 0x20;

	for(k=0;k<2;k++)
	{
		for(i=0; i<8; i++)
		{   
				font_dat = ASCII8X16[ch*16+i+k*8];
				for(j=0; j<8; j++)
				{  /* ������Ӧ�ĵ�Ϊcolor��Ϊback_color */
					if(font_dat&0x01<<j)
						OLED_Draw_dots(x, y, col0);   
					else 
						OLED_Draw_dots(x, y, col1); 
					y++;
				}  
				x++;
				y-=8;
		}
		y += 8;								// �ָ�yֵ
		x-=8;
	}
}
void OLED_PutChar0612(unsigned char x, unsigned char y, unsigned char ch,unsigned char Col)
{ 
	u8 temp,t1,t;
	u16 y0=y;
	unsigned char col1,col0;
	if(Col==0)	{	col0=0;col1=1;	}
	else				{	col0=1;col1=0;	}
	//????		   
	ch=ch-' ';//???????
	for(t=0;t<12;t++)
	{   
		temp=ASCII6X12[ch][t];	//??2412??
		for(t1=0;t1<8;t1++)
		{	
			if(temp&0x80)	OLED_Draw_dots(x,y,col0);
			else 					OLED_Draw_dots(x,y,col1);

			temp<<=1;
			y++;
			if((y-y0)==12)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
	}  	
}
void OLED_PutChar5x7(unsigned char x, unsigned char y, unsigned char ch,unsigned char Col)
{
	u8 temp,t1,t;
	u16 y0=y;
	unsigned char col1,col0;
	if(Col==0)	{	col0=0;col1=1;	}
	else				{	col0=1;col1=0;	}
	   
	ch=ch-' ';
	for(t=0;t<5;t++)
	{   
		temp=	ASCII5x7[ch][t];	//??2412??
		y=y0;
		for(t1=0;t1<7;t1++)
		{	
			if(temp&0x01)	OLED_Draw_dots(x,y,col0);
			else 					OLED_Draw_dots(x,y,col1);

			temp>>=1;
			y++;
		}  	 
		x++;
	}

}

void OLED_WriteChstr1414(u8 x,u8 y,unsigned char *Chstr,u8 Col)
{
  unsigned char m,c1,c2,i,j,add,length;
	unsigned char tmp_char;
	unsigned char col1,col0;
	m=0;length=0;
	while(Chstr[length]!=0)length++;
	if(Col==0)	{	col0=0;col1=1;	}
	else				{	col0=1;col1=0;	}
	do
	{
		c1=Chstr[m];		
		c2=Chstr[m+1];   
		if(c1>128)//???
		{
			for(j=0;j<sizeof(hanzi_14x14)/sizeof(hanzi_14x14[0]);j++)  //????
			{
					if(c1==hanzi_14x14[j].Index[0]&&c2==hanzi_14x14[j].Index[1]) break;            //???????
			}
			for(i=0;i<14;i++)
			{

				tmp_char=hanzi_14x14[j].Msk[i*2];
				for (add=0;add<8;add++)
				{
					OLED_Draw_dots(x+add,y+i,(tmp_char&0x80) ? col0:col1);//д����ɫֵ
					tmp_char=tmp_char<<1;
				}
				
				tmp_char=hanzi_14x14[j].Msk[i*2+1];
				for (add=0;add<6;add++)
				{
					OLED_Draw_dots(x+add+8,y+i,(tmp_char&0x80) ? col0:col1);//д����ɫֵ
					tmp_char=tmp_char<<1;
				}
			}
      m+=2;x+=14;	
		}
		else 
		{
			OLED_PutChar5x7(x,y,c1,Col);
			m++;
			x+=7;
		}
	}
	while(m<length);
}


void OLED_WriteChstr1212(u8 x,u8 y,unsigned char *Chstr,u8 Col)
{
  unsigned char m,c1,c2,i,j,add,length;
	unsigned char tmp_char;
	unsigned char col1,col0;
	m=0;length=0;
	while(Chstr[length]!=0)length++;
	if(Col==0)	{	col0=0;col1=1;	}
	else				{	col0=1;col1=0;	}
	do
	{
		c1=Chstr[m];		
		c2=Chstr[m+1];   
		if(c1>128)//???
		{
			for(j=0;j<sizeof(hanzi_12x12)/sizeof(hanzi_12x12[0]);j++)  //????
			{
					if(c1==hanzi_12x12[j].Index[0]&&c2==hanzi_12x12[j].Index[1]) break;            //???????
			}
			for(i=0;i<12;i++)
			{

				tmp_char=hanzi_12x12[j].Msk[i*2];
				for (add=0;add<8;add++)
				{
					OLED_Draw_dots(x+add,y+i,(tmp_char&0x80) ? col0:col1);//д����ɫֵ
					tmp_char=tmp_char<<1;
				}
				
				tmp_char=hanzi_12x12[j].Msk[i*2+1];
				for (add=0;add<4;add++)
				{
					OLED_Draw_dots(x+add+8,y+i,(tmp_char&0x80) ? col0:col1);//д����ɫֵ
					tmp_char=tmp_char<<1;
				}
			}
      m+=2;x+=12;	
		}
		else 
		{
			OLED_PutChar0612(x,y,c1,Col);
			m++;
			x+=6;
		}
	}
	while(m<length);
}

void OLED_WriteENstr5x7(u8 x,u8 y,unsigned char *ENstr,u8 Col)
{
	while(*ENstr!='\0')
	{
			OLED_PutChar5x7(x,y,*ENstr,Col);
			*ENstr++;
			x+=6;
	}
}

void OLED_Draw_dots(unsigned char xx,unsigned char yy,unsigned char color)
{
	unsigned char xlabel,xlabel_bit;
	unsigned char Read_Dat; 		//��LCD�е�����
	if(xx>=128) 	return;
	if(yy>=64) 	return;
	xlabel=yy>>3;								//ȡ8*8�׵�ַ
	xlabel_bit=yy & 0x07;				//����õ���16λ���ݵĵڼ�λ	
	Read_Dat=OLED_Buffer[xlabel][xx];
	switch(color)
	{
		case 0:Read_Dat &= (~(0x01<<(xlabel_bit)));//����� 
		break;
		case 1:Read_Dat |= (0x01<<(xlabel_bit));//��Ϳ�� 
		break;
		case 2:Read_Dat ^= (0x01<<(xlabel_bit));//����ת 
		break;	
		default:break;
	}
	OLED_Buffer[xlabel][xx]=Read_Dat;
}

void OLED_Drawline_X(unsigned char X0,unsigned char X1,unsigned char Y,unsigned char Color)//�� X��ˮƽ��
{
	unsigned char temp;
	if (X0>X1)
	{
	   temp=X1;
	   X1=X0;
	   X0=temp;
	}

	for(;X0<X1;X0++)
	{
   		OLED_Draw_dots(X0,Y,Color);
	}
}

void OLED_Drawline_Y(unsigned char X,unsigned char Y0,unsigned char Y1,unsigned char Color)//�� Y��ˮƽ��
{
	unsigned char temp;
	if (Y0>Y1)
	{
	   temp=Y1;
	   Y1=Y0;
	   Y0=temp;
	}

	for(;Y0<Y1;Y0++)
	{
   		OLED_Draw_dots(X,Y0,Color);
	}
}
void OLED_Drawline(short int X0, short int Y0, short int X1 ,short int Y1, short int Color)//��б��
{
    short int t, distance;      /*������Ļ��С�ı��������(���Ϊint��)*/
    short int x = 0 , y = 0 , delta_x, delta_y ;
    short int incx, incy ;

    delta_x = X1 - X0 ;
    delta_y = Y1 - Y0 ;

    if( delta_x > 0 )
    {
        incx = 1;
    }
    else if( delta_x == 0 )
    {
        OLED_Drawline_Y( X0, Y0, Y1, Color ) ;
        return ;
    }
    else
    {
        incx = -1 ;
    }
    if( delta_y > 0 )
    {
        incy = 1 ;
    }
    else if(delta_y == 0 )
    {
        OLED_Drawline_X( X0, X1, Y0, Color ) ;    
        return ;
    }
    else
    {
        incy = -1 ;
    }

    delta_x = abs( delta_x );    
    delta_y = abs( delta_y );
    if( delta_x > delta_y )
    {
        distance = delta_x ;
    }
    else
    {
        distance = delta_y ; 
    }
    OLED_Draw_dots( X0, Y0, Color ) ;    
    /* Draw Line*/
    for( t = 0 ; t <= distance+1 ; t++ )
    {
        OLED_Draw_dots( X0, Y0, Color ) ;
        x += delta_x ;
        y += delta_y ;
        if( x > distance )
        {
            x -= distance ;
            X0 += incx ;
        }
        if( y > distance )
        {
            y -= distance ;
            Y0 += incy ;
        }
    }
}
void OLED_Draw_lingxing(short int X0,short int Y0,short int R)	  //������
{
	short int aa,bb;
	short int dii;
	if(R>31||R==0) return;
	aa=0;
	bb=R;
	dii=3-2*R;//�ж��¸���λ�õı�־
	while(aa<=bb)
	{
		OLED_Draw_dots(X0-bb,Y0-aa,1);//3
		OLED_Draw_dots(X0+bb,Y0-aa,1);//0
		OLED_Draw_dots(X0-aa,Y0+bb,1);//1
		OLED_Draw_dots(X0-bb,Y0-aa,1);//7
		OLED_Draw_dots(X0-aa,Y0-bb,1);//2
		OLED_Draw_dots(X0+bb,Y0+aa,1);//4
		OLED_Draw_dots(X0+aa,Y0-bb,1);//5
		OLED_Draw_dots(X0+aa,Y0+bb,1);//6
		OLED_Draw_dots(X0-bb,Y0+aa,1);
		aa++;
//******ʹ��bresenham�㷨��Բ****/
		if(dii<0)
			dii+=4*aa+6;
		else
		{
			dii+=10+4*(aa-bb);
			bb--;
		}
		OLED_Draw_dots(X0+aa,Y0+bb,1);
	}

}
void OLED_Draw_circle(short int X0,short int Y0,short int R)	  //��Բ
{
	short int aa,bb;
	short int dii;
	if(R>31||R==0) return;
	aa=0;
	bb=R;
	dii=3-2*R;//�ж��¸���λ�õı�־
	while(aa<=bb)
	{
		OLED_Draw_dots(X0-bb,Y0-aa,1);//3
		OLED_Draw_dots(X0+bb,Y0-aa,1);//0
		OLED_Draw_dots(X0-aa,Y0+bb,1);//1
		OLED_Draw_dots(X0-bb,Y0-aa,1);//7
		OLED_Draw_dots(X0-aa,Y0-bb,1);//2
		OLED_Draw_dots(X0+bb,Y0+aa,1);//4
		OLED_Draw_dots(X0+aa,Y0-bb,1);//5
		OLED_Draw_dots(X0+aa,Y0+bb,1);//6
		OLED_Draw_dots(X0-bb,Y0+aa,1);
		aa++;
//******ʹ��bresenham�㷨��Բ****/
		if(dii<0)
			dii+=4*aa+6;
		else
		{
			dii+=10+4*(aa-bb);
			bb--;
		}
		OLED_Draw_dots(X0+aa,Y0+bb,1);
	}

}

void OLED_Clear(unsigned char Col)
{
	unsigned char xxi,yyi;
	for(xxi=0;xxi<8;xxi++)
		for(yyi=0;yyi<128;yyi++)
			OLED_Buffer[xxi][yyi]=Col;
}


void OLED_WriteStr(u8 x,u8 y,u8 sty,u8 ch[])
{
	unsigned char c=0,i=0,j=0;
	while (ch[j]!='\0')
	{
		c =ch[j]-32;     //Ϊ�ֿ������������
		if(x>120){x=0;y++;}
		if(sty == 0)
		{
			OLED_Set_Pos(x,y);
			for(i=0;i<8;i++)
							OLED_WrDat(ASCII8X16[c*16+i]); //һ�����Ӧһ��С��
			OLED_Set_Pos(x,y+1); //������ʼ���꣬�ٴ�д��˸���
			for(i=0;i<8;i++)
							OLED_WrDat(ASCII8X16[c*16+i+8]);
		}
		else
		{
			OLED_Set_Pos(x,y);
			for(i=0;i<8;i++)
							OLED_WrDat(~ASCII8X16[c*16+i]);
			OLED_Set_Pos(x,y+1);
			for(i=0;i<8;i++)
							OLED_WrDat(~ASCII8X16[c*16+i+8]);
		}
		x+=8;
		j++;
	}
}

void OLED_Display0612Num(unsigned char xx,unsigned char yy,int Num,unsigned char bitlen,u8 Col)
{
	unsigned char String[6],bitx=0;
	if(Num<0)
	{
		Num=-Num;
		String[bitx++]='-';
	}
	if(Num>999) 	String[bitx++]=Num/1000%10+48;	//��ֵ��4λ��
	if(Num>99) 		String[bitx++]=Num/100%10+48;	//��ֵ��5λ��
	if(Num>9) 		String[bitx++]=Num/10%10+48;	//��ֵ��2λ��
	String[bitx++]=Num%10+48;		//��ֵ��1λ��
	
	for(;bitx<bitlen;bitx++)
	{
		String[bitx]=' ';
	}
	String[bitx]='\0';
	
	OLED_WriteChstr1212(xx,yy,String,Col);
}

void OLED_Display0612Num1dot(unsigned char xx,unsigned char yy,int Num,unsigned char Col)
{
	unsigned char String[7],bitx=0;
	if(Num<0)
	{
		Num=-Num;
		String[bitx++]='-';
	}
	if(Num>999) 	String[bitx++]=Num/1000%10+48;	//��ֵ��4λ��
	if(Num>99) 		String[bitx++]=Num/100%10+48;	//��ֵ��5λ��
	String[bitx++]=Num/10%10+48;	//��ֵ��2λ��
	String[bitx++]='.';
	String[bitx++]=Num%10+48;		//��ֵ��1λ��
	
	for(;bitx<5;bitx++)
	{
		String[bitx]=' ';
	}
	String[bitx]='\0';
	
	OLED_WriteChstr1212(xx,yy,String,Col);
}

void OLED_Display0507Num1dot(unsigned char xx,unsigned char yy,int Num)
{
	unsigned char String[7],bitx=0;
	if(Num<0)
	{
		Num=-Num;
		String[bitx++]='-';
	}
	if(Num>999) 	String[bitx++]=Num/1000%10+48;	//��ֵ��4λ��
	if(Num>99) 		String[bitx++]=Num/100%10+48;	//��ֵ��5λ��
	String[bitx++]=Num/10%10+48;	//��ֵ��2λ��
	String[bitx++]='.';
	String[bitx++]=Num%10+48;		//��ֵ��1λ��
	
	for(;bitx<4;bitx++)
	{
		String[bitx]=' ';
	}
	String[bitx]='\0';
	
	OLED_WriteENstr5x7(xx,yy,String,1);
}


/*************************************END************************************/



