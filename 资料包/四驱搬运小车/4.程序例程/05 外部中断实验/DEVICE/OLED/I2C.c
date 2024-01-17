/***************************************************************************
本程序可供学习使用，经作者允许，也可用于其他用途。如有意见及建议，欢迎来电交流指教
硬件版本：FreeLink STM32四路直流电机控制板V2.41
主控芯片：STM32F103RCT6
程序名称：STM32麦轮全向移动小车程序--IIC程序
功能描述：IIC程序，OLED显示屏的物理驱动层。
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


#include "sys.h"
#include "delay.h"
#include "I2C.h"

void IIC_Init(void)
{                        
	RCC->APB2ENR|=1<<0;//使能辅助时钟
  RCC->APB2ENR|=1<<4;//先使能外设IO PORTC时钟
		
	GPIOC->CRH &= 0XFFFF00FF;//PC11 10 推挽输出  
	GPIOC->CRH |= 0X00003300;	   
	GPIOC->ODR |= 0x3<<10;
	
}
//?ú?úIIC????????
void IIC_Start(void)
{
    SDA_OUT();     //sda??????
    IIC_SDA=1;        
    IIC_SCL=1;
//  delay_us(4);
    IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
//  delay_us(4);
    IIC_SCL=0;//??×?I2C×?????×?±?·????ò???????? 
}     
//?ú?úIIC????????
void IIC_Stop(void)
{
    SDA_OUT();//sda??????
    IIC_SCL=0;
    IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
//  delay_us(4);
    IIC_SCL=1; 
    IIC_SDA=1;//·???I2C×????á??????
//  delay_us(4);                                
}
//????????????????
//·???????1???????????§°?
//        0??????????????
u8 IIC_Wait_Ack(void)
{
    u8 ucErrTime=0;
    SDA_IN();      //SDA?è????????  
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
    IIC_SCL=0;//?±??????0      
    return 0;  
} 

void IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
    SDA_OUT();      
    IIC_SCL=0;//?????±??????????????
    for(t=0;t<8;t++)
    {              
        IIC_SDA=(txd&0x80)>>7;
        txd<<=1;    
        delay_us(2);   //??TEA5767?????????±????±?????
        IIC_SCL=1;
        delay_us(2); 
        IIC_SCL=0;  
        delay_us(2);
    }    
    IIC_Wait_Ack();
}  


/*************************************END************************************/

