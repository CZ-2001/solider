#ifndef __USART6_H__
#define __USART6_H__ 
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include "main.h"
#define USART6_REC_LEN  			128  	//定义最大接收字节数 200

#define BUFF_LEN 7

void USART6_vision_Configuration(uint32_t bound);
void USART6_SendChar(uint8_t ch);
extern uint16_t USART6_RX_BUF[BUFF_LEN];
extern uint16_t USART6_RX_STA;       //接收状态标记	
extern void vision_send(void);
#endif








