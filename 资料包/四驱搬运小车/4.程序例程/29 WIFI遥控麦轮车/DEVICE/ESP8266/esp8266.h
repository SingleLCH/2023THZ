/***************************************************************************
本程序可供学习使用，经作者允许，也可用于其他用途。如有意见及建议，欢迎来电交流指教
硬件版本：四路麦轮多功能控制板
主控芯片：STM32F103RCT6
程序名称：STM32麦轮全向移动小车程序--ESP8266程序
功能描述：ESP8266创建热点，接收手机APP数据



*****************************************************************************/


#ifndef __ESP8266_H
#define __ESP8266_H

#include "sys.h"
#include "usart.h"
#include "string.h"
#include "delay.h"
#include "utils.h"

#define UART_RECV_LEN	320
extern unsigned char Blue_dat[UART_RECV_LEN];		//串口3接收缓存
extern short int Blue_RC[3];

extern u8 hasClient;
extern unsigned char WIFI_CK;


u8 init_server(void);
void send_msg(char *msg);
void recv_msg(void);
#endif

