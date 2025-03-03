#include "usart2.h"
#include <string.h>
uint8_t USART2_RX_BUF[10];
uint16_t USART2_RX_STA=0;
struct DisSTU DisStu;
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ

void USART2_Configuration(void)
{
		USART_InitTypeDef usart2;
		GPIO_InitTypeDef  gpio;
		NVIC_InitTypeDef  nvic;

		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);

		GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2);
		GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2); 

		gpio.GPIO_Pin = GPIO_Pin_2| GPIO_Pin_3;
		gpio.GPIO_Mode = GPIO_Mode_AF;
		gpio.GPIO_OType = GPIO_OType_PP;
		gpio.GPIO_Speed = GPIO_Speed_100MHz;
		gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_Init(GPIOA,&gpio);

		usart2.USART_BaudRate 	= 115200;
		usart2.USART_WordLength = USART_WordLength_8b;
		usart2.USART_StopBits 	= USART_StopBits_1;
		usart2.USART_Parity 		= USART_Parity_No;
		usart2.USART_Mode 			= USART_Mode_Tx|USART_Mode_Rx;
		usart2.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_Init(USART2,&usart2);

		USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
		USART_Cmd(USART2,ENABLE);

		nvic.NVIC_IRQChannel =USART2_IRQn;
		nvic.NVIC_IRQChannelPreemptionPriority =3;
		nvic.NVIC_IRQChannelSubPriority =4;
		nvic.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&nvic);
}

void USART2_SendChar(unsigned char b)
{
		while (USART_GetFlagStatus(USART2,USART_FLAG_TC) == RESET);
		USART_SendData(USART2,b);
}

static u8 isHead=0;
void USART2_IRQHandler(void)                	//����1�жϷ������
{
	uint8_t Res;
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
	  Res = USART_ReceiveData(USART2);	//��ȡ���յ�������
    if(Res==0xaa)
      isHead=1;
    if(isHead)
    {
      USART2_RX_BUF[USART2_RX_STA&0X3FFF] = Res ;//���浽BUF����
      USART2_RX_STA++;
    }
    if( USART2_RX_STA==10 )
    {
      if(Res==0xdd){
        memcpy(&DisStu,&USART2_RX_BUF[1],8);
  //      DisStu.LF=USART2_RX_BUF[1]<<8||USART2_RX_BUF[2];
  //      DisStu.RF=USART2_RX_BUF[3]<<8||USART2_RX_BUF[4];
  //      DisStu.RB=USART2_RX_BUF[5]<<8||USART2_RX_BUF[6];
  //      DisStu.LB=USART2_RX_BUF[7]<<8||USART2_RX_BUF[8];
        
      }
      isHead=0;
      USART2_RX_STA=0;
    }
  }
} 




