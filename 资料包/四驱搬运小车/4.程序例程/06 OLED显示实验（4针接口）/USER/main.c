/***************************************************************************
本程序可供学习使用，经作者允许，也可用于其他用途。如有意见及建议，欢迎来电交流指教
硬件说明：四路麦轮多功能控制板
主控芯片：STM32F103RCT6
程序名称：OLED显示实验（4线接口）--主程序
功能描述：控制核心板上的LED闪烁


*****************************************************************************/



#include "sys.h"	
#include "delay.h"	
#include "oled_user.h"


int main(void)
{		
  Stm32_Clock_Init(9);		//系统时钟设置，8.0000MHz晶振，9倍倍频，主频=8.0000MHz*9=72.0000MHz 
	delay_init(72);					//延时初始化
	OLED_Init();        		//LED初始化	 

	while(1)						//主循环
	{
//****************************图标显示*****************************************
		
		OLED_ClearBuffer(DARK);	//清除显示缓存
		OLED_Draw_FreeLink();		//显示图标
		OLED_Refresh_AllGDRAM();//刷新显示	
		delay_ms(1000);					//延时1s
		
//****************************界面显示*****************************************
		
		OLED_ClearBuffer(DARK);	//清除显示缓存
		OLED_Put12x12CNstr(30,0,"*待    机*",LIGHT);
		OLED_Put12x12CNstr(0,13,"前左:",LIGHT);			OLED_Put12x12CNstr(66,13,"前右:",LIGHT);
		OLED_Put12x12CNstr(0,26,"后左:",LIGHT);			OLED_Put12x12CNstr(66,26,"后右:",LIGHT);
		OLED_Put12x12CNstr(0,39,"电压:",LIGHT);			OLED_Put12x12CNstr(66,39,"遥控:",LIGHT);
		OLED_Put12x12CNstr(10,52,"<机器人科技>",LIGHT);			//显示提示语
		
		OLED_Put6x12Num(32,13,213,5,LIGHT);	OLED_Put6x12Num(98,13,126,5,LIGHT);		//显示转速
		OLED_Put6x12Num(32,26,153,5,LIGHT);	OLED_Put6x12Num(98,26,124,5,LIGHT);		//显示转速
		OLED_Put5x7Num1dot(30,42,116);																						//显示电池电压
		OLED_Put12x12CNstr(98,39,"连接",LIGHT);																		//显示蓝牙信号
	
		OLED_Refresh_AllGDRAM();//刷新显示	
		delay_ms(1000);
//*****************************图形显示********************************************		

		OLED_ClearBuffer(DARK);	//清除显示缓存
		
		OLED_DrawCircle(20,30,18);						//画圆
		OLED_DrawRectangle(20,20,58,54,LIGHT);//画矩形
		OLED_DrawLine(90,10,112,50,LIGHT);		//画斜线，三角形的一条边
		OLED_DrawLine(90,10,62,50,LIGHT);			//画斜线，三角形的一条边
		OLED_DrawLine(62,50,112,50,LIGHT);		//画斜线，三角形的一条边
		
		OLED_Refresh_AllGDRAM();//刷新显示	
		delay_ms(1000);		

	} 
	
}

/*************************************END****************************************/





