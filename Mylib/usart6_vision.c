#include "usart6_vision.h"
#include "vision_behaviour.h"
#include <stm32f4xx.h>
#include "rc_task.h"
uint16_t USART6_RX_BUF[BUFF_LEN];
uint16_t USART6_RX_STA = 0;
uint8_t USART6_TX_BUF;

uint8_t rrr;

void USART6_vision_Configuration(u32 bound)
{
	// GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);  // 使能GPIOG时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE); // 使能USART6时钟

	// 串口6对应引脚复用映射
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource9, GPIO_AF_USART6);  // GPIOA9复用为USART6
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource14, GPIO_AF_USART6); // GPIOA10复用为USART6

	// USART6端口配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_14; // GPIOG9与GPIOG14
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;			// 复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		// 速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;			// 推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;			// 上拉
	GPIO_Init(GPIOG, &GPIO_InitStructure);					// 初始化GPIOG9与GPIOG14

	// USART6 初始化设置
	USART_InitStructure.USART_BaudRate = bound;										// 波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;						// 字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;							// 一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;								// 无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // 无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					// 收发模式
	USART_Init(USART6, &USART_InitStructure);										// 初始化串口6

	USART_Cmd(USART6, ENABLE); // 使能串口6

	USART_ClearFlag(USART6, USART_FLAG_TC);

	USART_ITConfig(USART6, USART_IT_RXNE, ENABLE); // 开启相关中断

	// USART6 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;		  // 串口6中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; // 抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		  // 子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  // IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);							  // 根据指定的参数初始化VIC寄存器、
}

uint8_t ishead6_f = 0;
uint8_t sta6 = 0;
int LEN6 = 7;
uint8_t rrr;
void USART6_IRQHandler(void) // ′?ú1?D??·t??3ìDò
{
	if (USART_GetITStatus(USART6, USART_IT_RXNE) != RESET)
	{
		rrr = USART_ReceiveData(USART6);
		USART_ClearITPendingBit(USART6, USART_IT_RXNE);
		if (ishead6_f == 1)
		{
			sta6++;
			USART6_RX_BUF[sta6] = USART_ReceiveData(USART6); //?áè??óê?μ?μ?êy?Y
			if (sta6 == LEN6 - 1)
			{
				ishead6_f = 0;
				if (USART6_RX_BUF[LEN6 - 1] == 0xFE)
				{
					vision_updata(&VisionStructure, USART6_RX_BUF);
				}
				else
					memset(&USART6_RX_BUF, 0, 6);
			}
		}
		else
		{
			sta6 = 0;
			USART6_RX_BUF[sta6] = USART_ReceiveData(USART6);
			ishead6_f = (USART6_RX_BUF[sta6] == 0xAA ? 1 : 0);
		}
	}
}

void USART6_SendChar(uint8_t ch)
{	
	//while (USART_GetFlagStatus(USART6,USART_FLAG_TC) == RESET);
	USART_SendData(USART6,ch);
	while (USART_GetFlagStatus(USART6,USART_FLAG_TC) == RESET);
	 
}
// 向上位机发送
void vision_send(void)
{
	/*
	01234567
	4+5: (0:left 1:right 2:stay)
	6: aim mode (0: amor 1:buff)
	7:aimcolor(O:blue1:red)
	*/
//	USART6_TX_BUF = 0x00; // 清除位
	if (Gimbal_Str->vision_forcast == vision_right)
	{
		USART6_TX_BUF &= 0xF3; // 1111 0011
		USART6_TX_BUF |= 0x04;
	}
	else if (Gimbal_Str->vision_forcast == vision_left)
	{
		USART6_TX_BUF &= 0xF3; // 0000 1100
	}
	else if (Gimbal_Str->vision_forcast == vision_stay)
	{
		USART6_TX_BUF &= 0xF3;	 // 清空
		USART6_TX_BUF |= (0x0C); // 0000 1100
	}
	if (Gimbal_Str->Key_Mode == Gimbal_WindMill)
	{
		USART6_TX_BUF &= 0xFD; //   1111 1101 置0清空
		USART6_TX_BUF |= 0x02;
	}
	else
		USART6_TX_BUF &= 0xFD; //   0000 0010  置0清空
//	
if(JudgeStructure.ext_game_robot_status.robot_id<10)
	USART6_TX_BUF|=0x01;
	else if(JudgeStructure.ext_game_robot_status.robot_id>10)
		USART6_TX_BUF&=0x0E;
//	USART6_TX_BUF|=0xFF;

	
	
	
	USART6_SendChar(USART6_TX_BUF);
}
