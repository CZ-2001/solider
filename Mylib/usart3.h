#ifndef __USART3_H__
#define __USART3_H__
#include "main.h"
#define USART_REC_LEN  			128 	//定义最大接收字节数 200
#define EN_USART1_RX 			1		//使能（1）/禁止（0）串口1接收  
#define UART3_Waiting 0
#define UART3_Receiving 1
#define UART3_Success 2 
#define UART3_Failed  3

void USART3_Configuration(void);
void USART3_SendChar(unsigned char b);

extern uint8_t USART_RX_BUF[6];
extern uint16_t USART_RX_STA;       //接收状态标记	 



#endif

