#ifndef __ESP8266_H

#define __ESP8266_H

#include "sys.h"
#include "usart.h"
#include "string.h"
#include "delay.h"

#define UART_RECV_LEN	320
extern unsigned char Blue_dat[UART_RECV_LEN];		//´®¿Ú3½ÓÊÕ»º´æ
extern short int Blue_RC[4];

extern u8 hasClient;
extern unsigned char WIFI_CK;

u8 init_server(void);
void send_msg(char *msg);
void recv_msg(void);
#endif

