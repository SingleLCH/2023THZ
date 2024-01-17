/***************************************************************************
本程序可供学习使用，经作者允许，也可用于其他用途。如有意见及建议，欢迎来电交流指教
硬件版本：四路麦轮多功能控制板
主控芯片：STM32F103RCT6
程序名称：STM32麦轮全向移动小车程序--ESP8266程序
功能描述：ESP8266创建热点，接收手机APP数据



*****************************************************************************/

#include "esp8266.h"
#include "LED.h"

unsigned char Blue_dat[UART_RECV_LEN];		//串口3接收缓存
short int Blue_RC[3];

unsigned char WIFI_CK=0;

uint8_t check_is_ok(void) 
{
	unsigned short int nn;
	for(nn=0;nn<UART_RECV_LEN-1;nn++)
	{
		if(Blue_dat[nn]=='O' && Blue_dat[nn+1]=='K')
			return 1;
	}
	return 0;
}
//check_is_contain("+IPD")
uint8_t check_is_IPD(void) 
{
	unsigned short int nn;
	for(nn=0;nn<UART_RECV_LEN-4;nn++)
	{
		if(Blue_dat[nn]=='+' && Blue_dat[nn+1]=='I' && Blue_dat[nn+2]=='P' && Blue_dat[nn+3]=='D')
			return 1;
	}
	return 0;
}
//check_is_contain("CONNECT")
uint8_t check_is_CONNECT(void) 
{
	unsigned short int nn;
	for(nn=0;nn<UART_RECV_LEN-7;nn++)
	{
		if(Blue_dat[nn]=='C' && Blue_dat[nn+1]=='O' && Blue_dat[nn+2]=='N' && Blue_dat[nn+3]=='N'
			&& Blue_dat[nn+4]=='E' && Blue_dat[nn+5]=='C' && Blue_dat[nn+6]=='T')
			return 1;
	}
	return 0;
}
//check_is_contain("CLOSED")
uint8_t check_is_CLOSED(void) 
{
	unsigned short int nn;
	for(nn=0;nn<UART_RECV_LEN-6;nn++)
	{
		if(Blue_dat[nn]=='C' && Blue_dat[nn+1]=='L' && Blue_dat[nn+2]=='O' && Blue_dat[nn+3]=='S'
			&& Blue_dat[nn+4]=='E' && Blue_dat[nn+5]=='D' )
			return 1;
	}
	return 0;
}

// u16 isRecvMsg() 
// {
// 	return USART_RX_STA & 0x8000;
// }


void clearRecvFlag() 
{
	unsigned short int nn;
	for(nn=0;nn<UART_RECV_LEN;nn++)
	{
		Blue_dat[nn]=0;
	}
}


/*
 * wifi模块初始化，创建tcp服务器
 */
u8 init_server()
{
	uint8_t i, checkWifi = 0;
	clearRecvFlag();
	// 每隔一段时间发送一次AT指令，检测是否连接上有wifi		
	while(!checkWifi) 
	{
		SendString("AT\r\n"); 
		for(i = 0; i < 20; i++) 
		{
			if(check_is_ok()) 
			{
				checkWifi = 1;
				break;
			}
			clearRecvFlag();
			delay_ms(10);
		}
	}
	clearRecvFlag();
	LED0=0;
	// 1、设置 wifi模式
	SendString("AT+CWMODE=3\r\n");	
	while(!check_is_ok()); 	// 等待接收数据
// 	clearRecvFlag();
	if(!check_is_ok()) 
	{
		return 0;
	}
	delay_ms(10);
	clearRecvFlag();
	/*	
	// 2、重启生效
	uart_send_str(SERVER_RST);	// 
	while(g_recv_flag == 0); 	// 等待接收数据
	g_recv_flag = 0;
	if(check_is_error()) {
		return -1;
	}

	delay();
	*/

	// 3、启动多连接
	SendString("AT+CIPMUX=1\r\n");	//  端口号8080
	while(!check_is_ok()); 	// 等待接收数据
// 	clearRecvFlag();
	if(!check_is_ok()) 
	{
		return 0;
	}
	delay_ms(10);
	clearRecvFlag();
	// 4、建立server
	SendString("AT+CIPSERVER=1,8080\r\n");	// 
	while(!check_is_ok()); 	// 等待接收数据
// 	clearRecvFlag();
	if(!check_is_ok()) 
	{
		return 0;
	}
	delay_ms(10);
	clearRecvFlag();
	return 1;
}
/*
 * wifi发送消息
 */

void send_msg(char *msg)
{
	uint8_t len = strlen(msg); // 获取信息长度
	if(hasClient) 
	{
		// 只有在有客户端连接时才可发送消息
		printf("AT+CIPSEND=0,%d\r\n", len); // (send_buf);
		while(!check_is_ok()); // 等待接收数据
		clearRecvFlag();
		delay_ms(2);
		if(check_is_ok()) 
		{
			// 4F 4B 0D 0A 3E 20
			// 可以发送消息
			printf("%s\r\n", msg);
		}
	}
}


/*
 * wifi接收消息
 */
u8 hasClient = 0;
unsigned char recv_buf[UART_RECV_LEN];
void recv_msg()
{	
	unsigned short int nm;
// 	char lens[6];
	for(nm=0;nm<UART_RECV_LEN;nm++)
	{
		recv_buf[nm]=Blue_dat[nm];
	}		
	for(nm=0;nm<UART_RECV_LEN-34;nm++)
	{
		if(recv_buf[nm]=='+' && recv_buf[nm+1]=='I' && recv_buf[nm+2]=='P' && recv_buf[nm+3]=='D')
		{
			for(nm+=3;nm<UART_RECV_LEN-34;nm++)	//找到“：”
			{
				WIFI_CK=250;
				if(recv_buf[nm]==':' && recv_buf[nm+1]==0xAA && recv_buf[nm+2]==0x55
					&& recv_buf[nm+8]==0xAA && recv_buf[nm+9]==0x55)
				{
					if(recv_buf[nm+3]&0x80)	//为负数
					{
						Blue_RC[2]=0-(255-recv_buf[nm+3]);
					}
					else
					{
						Blue_RC[2]=recv_buf[nm+3];
					}
					if(recv_buf[nm+5]&0x80)	//为负数
					{
						Blue_RC[1]=0-(255-recv_buf[nm+5]);
					}
					else
					{
						Blue_RC[1]=recv_buf[nm+5];
					}
					if(recv_buf[nm+6]&0x80)	//为负数
					{
						Blue_RC[0]=0-(255-recv_buf[nm+6]);
					}
					else
					{
						Blue_RC[0]=recv_buf[nm+6];
					}
					clearRecvFlag();
				}
			}	
		}	
	}

	if(check_is_CONNECT()) 
	{
		hasClient = 1;// 有客户端连接
	} 
	else if(check_is_CLOSED()) 
	{
		hasClient = 0;// 客户端断开连接
	}
	clearRecvFlag();
}

