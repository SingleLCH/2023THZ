/***************************************************************************
������ɹ�ѧϰʹ�ã�����������Ҳ������������;��������������飬��ӭ���罻��ָ��
Ӳ���汾��FreeLink STM32��·ֱ��������ư�V2.41
����оƬ��STM32F103RCT6
�������ƣ�STM32����ȫ���ƶ�С������--IIC����
����������IIC����OLED��ʾ�������������㡣
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


#include "sys.h"
#include "delay.h"
#include "I2C.h"

void IIC_Init(void)
{                        
	RCC->APB2ENR|=1<<0;//ʹ�ܸ���ʱ��
  RCC->APB2ENR|=1<<4;//��ʹ������IO PORTCʱ��
		
	GPIOC->CRH &= 0XFFFF00FF;//PC11 10 �������  
	GPIOC->CRH |= 0X00003300;	   
	GPIOC->ODR |= 0x3<<10;
	
}
//?��?��IIC????????
void IIC_Start(void)
{
    SDA_OUT();     //sda??????
    IIC_SDA=1;        
    IIC_SCL=1;
//  delay_us(4);
    IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
//  delay_us(4);
    IIC_SCL=0;//??��?I2C��?????��?��?��????��???????? 
}     
//?��?��IIC????????
void IIC_Stop(void)
{
    SDA_OUT();//sda??????
    IIC_SCL=0;
    IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
//  delay_us(4);
    IIC_SCL=1; 
    IIC_SDA=1;//��???I2C��????��??????
//  delay_us(4);                                
}
//????????????????
//��???????1???????????���?
//        0??????????????
u8 IIC_Wait_Ack(void)
{
    u8 ucErrTime=0;
    SDA_IN();      //SDA?��????????  
    IIC_SDA=1;delay_us(1);     
    IIC_SCL=1;delay_us(1);   
    while(READ_SDA)
    {
        ucErrTime++;
        if(ucErrTime>250)
        {
            IIC_Stop();
            return 1;
        }
    }
    IIC_SCL=0;//?��??????0      
    return 0;  
} 

void IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
    SDA_OUT();      
    IIC_SCL=0;//?????��??????????????
    for(t=0;t<8;t++)
    {              
        IIC_SDA=(txd&0x80)>>7;
        txd<<=1;    
        delay_us(2);   //??TEA5767?????????��????��?????
        IIC_SCL=1;
        delay_us(2); 
        IIC_SCL=0;  
        delay_us(2);
    }    
    IIC_Wait_Ack();
}  


/*************************************END************************************/

