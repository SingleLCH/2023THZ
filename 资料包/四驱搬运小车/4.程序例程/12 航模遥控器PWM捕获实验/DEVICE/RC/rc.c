/***************************************************************************
本程序可供学习使用，经作者允许，也可用于其他用途。如有意见及建议，欢迎来电交流指教
硬件版本：四路麦轮多功能控制板
主控芯片：STM32F103RCT6
程序名称：STM32麦轮全向移动小车程序--RC航模遥控器程序
功能描述：捕获四路PWM脉冲信号，解析航模遥控器的四个通道的遥控数值。


*****************************************************************************/


#include "rc.h"
#include "delay.h"
#include "usart.h"

//定时器8通道输入捕获配置
u8  TIM1CH1_CAPTURE_STA=0;	//通道1输入捕获状态		  用高两位做捕获标志，低六位做溢出计数  				
u16	TIM1CH1_CAPTURE_UPVAL;	//通道1输入捕获值
u16	TIM1CH1_CAPTURE_DOWNVAL;//通道1输入捕获值

u8  TIM1CH2_CAPTURE_STA=0;	//通道2输入捕获状态		    				
u16	TIM1CH2_CAPTURE_UPVAL;	//通道2输入捕获值
u16	TIM1CH2_CAPTURE_DOWNVAL;//通道2输入捕获值

u8  TIM1CH3_CAPTURE_STA=0;	//通道3输入捕获状态		    				
u16	TIM1CH3_CAPTURE_UPVAL;	//通道3输入捕获值
u16	TIM1CH3_CAPTURE_DOWNVAL;//通道3输入捕获值

u8  TIM1CH4_CAPTURE_STA=0;	//通道4输入捕获状态		    				
u16	TIM1CH4_CAPTURE_UPVAL;	//通道4输入捕获值
u16	TIM1CH4_CAPTURE_DOWNVAL;//通道4输入捕获值

u16 RC_CH[4]={1500,1500,1000,1500};


void RC_Init(void)
{
	MY_NVIC_Init(1,0,TIM1_CC_IRQn,2);//抢占0，子优先级3，组2	

	RCC->APB2ENR|=1<<0;			//使能辅助时钟
	RCC->APB2ENR|=1<<11;	//TIM1时钟使能 
 	RCC->APB2ENR|=1<<2;    	//使能PORTA时钟	
	  	 
	GPIOA->CRH&=0XFFFF0000;	//PA8,9,10,11输入
	GPIOA->CRH|=0X00008888;	//上拉输入 
	GPIOA->ODR|=0XF<<8;		//PA8,9,10,11上拉

	GPIOA->CRL&=0XFF0FFFFF;   //PA5设置为强推挽
	GPIOA->CRL|=0X00300000;  	   

	RC_EN=0;
	
 	TIM1->ARR=20000-1;	//设定计数器自动重装值  
	TIM1->PSC=(72 - 1);	//时钟预分频数 72M/72
	TIM1->CR1&=~(3<<8);	//设置时钟分频系数：不分频
	TIM1->CR1&=~(1<<4);	//向上计数模式


	TIM1->CCMR1|=1<<0;		//CC1S=01 	选择输入端 IC1映射到TI1上
	TIM1->CCER|=0<<1; 		//CC1P=0	上升沿捕获
 	TIM1->CCER|=1<<0; 		//CC1E=1 	允许捕获计数器的值到捕获寄存器中	
 	TIM1->CCMR1|=0<<2; 		//IC1PS=00 	配置输入分频,不分频 
	TIM1->CCMR1|=0<<4; 		//IC1F=0000 配置输入滤波器 不滤波


	TIM1->CCMR1|=1<<8;		//CC2S=01 	选择输入端 IC1映射到TI1上
	TIM1->CCER|=0<<5; 		//CC2P=0	上升沿捕获
	TIM1->CCER|=1<<4; 		//CC2E=1 	允许捕获计数器的值到捕获寄存器中
 	TIM1->CCMR1|=0<<10; 	//IC2PS=00 	配置输入分频,不分频 
 	TIM1->CCMR1|=0<<12; 	//IC2F=0000 配置输入滤波器 不滤波

	TIM1->CCMR2|=1<<0;		//CC3S=01 	选择输入端 IC1映射到TI1上
	TIM1->CCER|=0<<9; 		//CC3P=0	上升沿捕获
	TIM1->CCER|=1<<8; 		//CC3E=1 	允许捕获计数器的值到捕获寄存器中
 	TIM1->CCMR2|=0<<2; 		//IC3PS=00 	配置输入分频,不分频 
 	TIM1->CCMR2|=0<<4; 		//IC3F=0000 配置输入滤波器 不滤波

	TIM1->CCMR2|=1<<8;		//CC4S=01 	选择输入端 IC1映射到TI1上
	TIM1->CCER|=0<<13; 		//CC4P=0	上升沿捕获
	TIM1->CCER|=1<<12; 		//CC4E=1 	允许捕获计数器的值到捕获寄存器中
 	TIM1->CCMR2|=0<<10; 	//IC4PS=00 	配置输入分频,不分频 
 	TIM1->CCMR2|=0<<12; 	//IC4F=0000 配置输入滤波器 不滤波

	TIM1->DIER|=1<<1;   	//允许捕获1中断	
	TIM1->DIER|=1<<2;   	//允许捕获2中断	
	TIM1->DIER|=1<<3;   	//允许捕获3中断	
	TIM1->DIER|=1<<4;   	//允许捕获4中断						
	//TIM1->DIER|=1<<0;   	//允许更新中断	
	TIM1->CR1|=0x01;    	//使能定时器4
}

//定时器8中断服务程序	 
void TIM1_CC_IRQHandler(void)
{
	if(TIM1->SR&0X0002)//捕获1发生捕获事件
	{
		TIM1->SR&=~(1<<1);//清除捕获1标志位
		
		if(TIM1CH1_CAPTURE_STA == 0)//捕获到上升沿
		{
			TIM1CH1_CAPTURE_UPVAL =TIM1->CCR1;//获取上升沿的数据
			
			TIM1CH1_CAPTURE_STA = 1;		//标记以捕获到了上升沿
			TIM1->CCER|=1<<1; 				//CC1P=1 设置为下降沿捕获
		}
		else                        //捕获到下降沿 (已经捕获到一个完整的高电平脉冲！)
		{
			TIM1CH1_CAPTURE_DOWNVAL = TIM1->CCR1;//获取下降沿的数据
			
			//判读是否超出发生了溢出,计算高电平脉冲时间us
			if(TIM1CH1_CAPTURE_DOWNVAL<TIM1CH1_CAPTURE_UPVAL)
			{
				RC_CH[0] = 20000 - TIM1CH1_CAPTURE_UPVAL + TIM1CH1_CAPTURE_DOWNVAL;
			}
			else
			{
				RC_CH[0] = TIM1CH1_CAPTURE_DOWNVAL- TIM1CH1_CAPTURE_UPVAL;
			}
			
			TIM1CH1_CAPTURE_STA = 0;
			TIM1->CCER&=~(1<<1);			//CC1P=0 设置为上升沿捕获
		}
	}	

	if(TIM1->SR&0X0004)//捕获2发生捕获事件
	{
		TIM1->SR&=~(1<<2);//清除捕获2标志位
		
		if(TIM1CH2_CAPTURE_STA == 0)//捕获到上升沿
		{
			TIM1CH2_CAPTURE_UPVAL =TIM1->CCR2;//获取上升沿的数据
			
			TIM1CH2_CAPTURE_STA = 1;		//标记以捕获到了上升沿
			TIM1->CCER|=1<<5; 				//CC2P=1 设置为下降沿捕获
		}
		else                        //捕获到下降沿 (已经捕获到一个完整的高电平脉冲！)
		{
			TIM1CH2_CAPTURE_DOWNVAL = TIM1->CCR2;//获取下降沿的数据
			
			//判读是否超出发生了溢出,计算高电平脉冲时间us
			if(TIM1CH2_CAPTURE_DOWNVAL<TIM1CH2_CAPTURE_UPVAL)
			{
				RC_CH[1] = 20000 - TIM1CH2_CAPTURE_UPVAL + TIM1CH2_CAPTURE_DOWNVAL;
			}
			else
			{
				RC_CH[1] = TIM1CH2_CAPTURE_DOWNVAL- TIM1CH2_CAPTURE_UPVAL;
			}
			TIM1CH2_CAPTURE_STA = 0;
			TIM1->CCER&=~(1<<5);			//CC1P=0 设置为上升沿捕获
		}
	}
	
	if(TIM1->SR&0X0008)//捕获3发生捕获事件
	{
		TIM1->SR&=~(1<<3);//清除捕获3标志位
		
		if(TIM1CH3_CAPTURE_STA == 0)//捕获到上升沿
		{
			TIM1CH3_CAPTURE_UPVAL =TIM1->CCR3;//获取上升沿的数据
			
			TIM1CH3_CAPTURE_STA = 1;		//标记以捕获到了上升沿
			TIM1->CCER|=1<<9; 				//CC3P=1 设置为下降沿捕获
		}
		else                        //捕获到下降沿 (已经捕获到一个完整的高电平脉冲！)
		{
			TIM1CH3_CAPTURE_DOWNVAL = TIM1->CCR3;//获取下降沿的数据			
			//判读是否超出发生了溢出,计算高电平脉冲时间us
			if(TIM1CH3_CAPTURE_DOWNVAL<TIM1CH3_CAPTURE_UPVAL)
			{
				RC_CH[2] = 20000 - TIM1CH3_CAPTURE_UPVAL + TIM1CH3_CAPTURE_DOWNVAL;
			}
			else
			{
				RC_CH[2] = TIM1CH3_CAPTURE_DOWNVAL- TIM1CH3_CAPTURE_UPVAL;
			}
			
			TIM1CH3_CAPTURE_STA = 0;
			TIM1->CCER&=~(1<<9);			//CC1P=0 设置为上升沿捕获
		}
	}	
	if(TIM1->SR&0X0010)//捕获4发生捕获事件
	{
		TIM1->SR&=~(1<<4);//清除捕获4标志位
		
		if(TIM1CH4_CAPTURE_STA == 0)//捕获到上升沿
		{
			TIM1CH4_CAPTURE_UPVAL =TIM1->CCR4;//获取上升沿的数据
			
			TIM1CH4_CAPTURE_STA = 1;		//标记以捕获到了上升沿
			TIM1->CCER|=1<<13; 				//CC4P=1 设置为下降沿捕获
		}
		else                        //捕获到下降沿 (已经捕获到一个完整的高电平脉冲！)
		{
			TIM1CH4_CAPTURE_DOWNVAL = TIM1->CCR4;//获取下降沿的数据
			
			//判读是否超出发生了溢出,计算高电平脉冲时间us
			if(TIM1CH4_CAPTURE_DOWNVAL<TIM1CH4_CAPTURE_UPVAL)
			{
				RC_CH[3] = 20000 - TIM1CH4_CAPTURE_UPVAL + TIM1CH4_CAPTURE_DOWNVAL;
			}
			else
			{
				RC_CH[3] = TIM1CH4_CAPTURE_DOWNVAL- TIM1CH4_CAPTURE_UPVAL;
			}
			
			TIM1CH4_CAPTURE_STA = 0;
			TIM1->CCER&=~(1<<13);			//CC1P=0 设置为上升沿捕获			
		}
	}
}


/*************************************END************************************/


