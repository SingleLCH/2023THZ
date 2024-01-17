/***************************************************************************
������ɹ�ѧϰʹ�ã�����������Ҳ������������;��������������飬��ӭ���罻��ָ��
Ӳ���汾����·���ֶ๦�ܿ��ư�
����оƬ��STM32F103RCT6
�������ƣ�STM32����ȫ���ƶ�С������--ESP8266����
����������ESP8266�����ȵ㣬�����ֻ�APP����



*****************************************************************************/


#ifndef __ESP8266_H
#define __ESP8266_H

#include "sys.h"
#include "usart.h"
#include "string.h"
#include "delay.h"
#include "utils.h"

#define UART_RECV_LEN	320
extern unsigned char Blue_dat[UART_RECV_LEN];		//����3���ջ���
extern short int Blue_RC[3];

extern u8 hasClient;
extern unsigned char WIFI_CK;


u8 init_server(void);
void send_msg(char *msg);
void recv_msg(void);
#endif

