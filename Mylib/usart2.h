#ifndef __USART2_H__
#define __USART2_H__
#include "main.h"
#include "stm32f4xx.h"
#include "sys.h"
#include "stdio.h"
#define USART2_REC_LEN  			10 	//�����������ֽ��� 200
#define EN_USART2_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����  

__packed struct  DisSTU
{
  uint16_t F;
  uint16_t L;
  uint16_t B;
  uint16_t R;
};
extern struct DisSTU DisStu;
void USART2_Configuration(void);
void USART2_SendChar(unsigned char b);

extern uint8_t USART2_RX_BUF[USART2_REC_LEN];
extern uint16_t USART2_RX_STA;       //����״̬���	 



#endif

