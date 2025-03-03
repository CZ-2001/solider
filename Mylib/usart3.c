#include "main.h"
#include "sys.h"
#include "usart3.h"
#include "stm32f4xx.h"
#include "usb_behaviour.h"
uint8_t USART_RX_BUF[6];
uint16_t USART_RX_STA=0;

/*-----USART3_TX-----PB10-----*/
/*-----USART3_RX-----PB11-----*/
    
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目

void USART3_Configuration(void)
{
		USART_InitTypeDef usart3;
		GPIO_InitTypeDef  gpio;
		NVIC_InitTypeDef  nvic;

		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);

		GPIO_PinAFConfig(GPIOD,GPIO_PinSource8,GPIO_AF_USART3);
		GPIO_PinAFConfig(GPIOD,GPIO_PinSource9,GPIO_AF_USART3); 

		gpio.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
		gpio.GPIO_Mode = GPIO_Mode_AF;
		gpio.GPIO_OType = GPIO_OType_PP;
		gpio.GPIO_Speed = GPIO_Speed_100MHz;
		gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_Init(GPIOD,&gpio);

		usart3.USART_BaudRate = 115200;
		usart3.USART_WordLength = USART_WordLength_8b;
		usart3.USART_StopBits = USART_StopBits_1;
		usart3.USART_Parity = USART_Parity_No;
		usart3.USART_Mode = USART_Mode_Tx|USART_Mode_Rx;
		usart3.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_Init(USART3,&usart3);

		USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);
		USART_Cmd(USART3,ENABLE);

		nvic.NVIC_IRQChannel =USART3_IRQn;
		nvic.NVIC_IRQChannelPreemptionPriority =4;
		nvic.NVIC_IRQChannelSubPriority =4;
		nvic.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&nvic);
}

void USART3_SendChar(unsigned char b)
{
		while (USART_GetFlagStatus(USART3,USART_FLAG_TC) == RESET);
		USART_SendData(USART3,b);
}

//int fputc(int ch, FILE *f)
//{
//	USART_SendData(USART3, (uint8_t)ch);
//    while (USART_GetFlagStatus(USART3,USART_FLAG_TC) == RESET);
//    return ch;
//}
uint8_t a=0;
void USART3_IRQHandler(void)                	//串口1中断服务程序
{
	uint8_t Res;
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
	  Res =USART_ReceiveData(USART3);	//读取接收到的数据
    USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;//缓存到BUF数组
		USART_RX_STA++;      //状态参数+1
		if((USART_RX_STA&0x8000)==0)//接收未完成
		{	
			if(USART_RX_STA>USART_REC_LEN)//超过6字节，未接收到一帧完整数据,重新开始接收	  
			{
				USART_RX_STA=0;
				a=0;	
			}
			if(a==0&&(Res==0x99))        //接收到0xaa帧头则将参数a加一
				a++;
			else if(a==1&&(Res==0x66))   //接收到0xdd帧尾则将状态参数标志接收完成
			{
				usb_upgrade(&RecBuffStu,USART_RX_BUF);
				USART_RX_STA|=0x8000;//标志接受完一整帧数据
				a=0;USART_RX_STA=0;
			}
		}
  } 
} 




