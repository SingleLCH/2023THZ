/***************************************************************************
������ɹ�ѧϰʹ�ã�����������Ҳ������������;��������������飬��ӭ���罻��ָ��
Ӳ���汾����·���ֶ๦�ܿ��ư�
����оƬ��STM32F103RCT6
�������ƣ�STM32����ȫ���ƶ�С������--ESP8266����
����������ESP8266�����ȵ㣬�����ֻ�APP����



*****************************************************************************/

#include "esp8266.h"
#include "LED.h"

unsigned char Blue_dat[UART_RECV_LEN];		//����3���ջ���
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
 * wifiģ���ʼ��������tcp������
 */
u8 init_server()
{
	uint8_t i, checkWifi = 0;
	clearRecvFlag();
	// ÿ��һ��ʱ�䷢��һ��ATָ�����Ƿ���������wifi		
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
	// 1������ wifiģʽ
	SendString("AT+CWMODE=3\r\n");	
	while(!check_is_ok()); 	// �ȴ���������
// 	clearRecvFlag();
	if(!check_is_ok()) 
	{
		return 0;
	}
	delay_ms(10);
	clearRecvFlag();
	/*	
	// 2��������Ч
	uart_send_str(SERVER_RST);	// 
	while(g_recv_flag == 0); 	// �ȴ���������
	g_recv_flag = 0;
	if(check_is_error()) {
		return -1;
	}

	delay();
	*/

	// 3������������
	SendString("AT+CIPMUX=1\r\n");	//  �˿ں�8080
	while(!check_is_ok()); 	// �ȴ���������
// 	clearRecvFlag();
	if(!check_is_ok()) 
	{
		return 0;
	}
	delay_ms(10);
	clearRecvFlag();
	// 4������server
	SendString("AT+CIPSERVER=1,8080\r\n");	// 
	while(!check_is_ok()); 	// �ȴ���������
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
 * wifi������Ϣ
 */

void send_msg(char *msg)
{
	uint8_t len = strlen(msg); // ��ȡ��Ϣ����
	if(hasClient) 
	{
		// ֻ�����пͻ�������ʱ�ſɷ�����Ϣ
		printf("AT+CIPSEND=0,%d\r\n", len); // (send_buf);
		while(!check_is_ok()); // �ȴ���������
		clearRecvFlag();
		delay_ms(2);
		if(check_is_ok()) 
		{
			// 4F 4B 0D 0A 3E 20
			// ���Է�����Ϣ
			printf("%s\r\n", msg);
		}
	}
}


/*
 * wifi������Ϣ
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
			for(nm+=3;nm<UART_RECV_LEN-34;nm++)	//�ҵ�������
			{
				WIFI_CK=250;
				if(recv_buf[nm]==':' && recv_buf[nm+1]==0xAA && recv_buf[nm+2]==0x55
					&& recv_buf[nm+8]==0xAA && recv_buf[nm+9]==0x55)
				{
					if(recv_buf[nm+3]&0x80)	//Ϊ����
					{
						Blue_RC[2]=0-(255-recv_buf[nm+3]);
					}
					else
					{
						Blue_RC[2]=recv_buf[nm+3];
					}
					if(recv_buf[nm+5]&0x80)	//Ϊ����
					{
						Blue_RC[1]=0-(255-recv_buf[nm+5]);
					}
					else
					{
						Blue_RC[1]=recv_buf[nm+5];
					}
					if(recv_buf[nm+6]&0x80)	//Ϊ����
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
		hasClient = 1;// �пͻ�������
	} 
	else if(check_is_CLOSED()) 
	{
		hasClient = 0;// �ͻ��˶Ͽ�����
	}
	clearRecvFlag();
}

