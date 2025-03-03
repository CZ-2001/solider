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
	// GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);  // ʹ��GPIOGʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE); // ʹ��USART6ʱ��

	// ����6��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource9, GPIO_AF_USART6);  // GPIOA9����ΪUSART6
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource14, GPIO_AF_USART6); // GPIOA10����ΪUSART6

	// USART6�˿�����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_14; // GPIOG9��GPIOG14
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;			// ���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		// �ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;			// ���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;			// ����
	GPIO_Init(GPIOG, &GPIO_InitStructure);					// ��ʼ��GPIOG9��GPIOG14

	// USART6 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;										// ����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;						// �ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;							// һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;								// ����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // ��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					// �շ�ģʽ
	USART_Init(USART6, &USART_InitStructure);										// ��ʼ������6

	USART_Cmd(USART6, ENABLE); // ʹ�ܴ���6

	USART_ClearFlag(USART6, USART_FLAG_TC);

	USART_ITConfig(USART6, USART_IT_RXNE, ENABLE); // ��������ж�

	// USART6 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;		  // ����6�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; // ��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		  // �����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  // IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);							  // ����ָ���Ĳ�����ʼ��VIC�Ĵ�����
}

uint8_t ishead6_f = 0;
uint8_t sta6 = 0;
int LEN6 = 7;
uint8_t rrr;
void USART6_IRQHandler(void) // ��?��1?D??��t??3��D��
{
	if (USART_GetITStatus(USART6, USART_IT_RXNE) != RESET)
	{
		rrr = USART_ReceiveData(USART6);
		USART_ClearITPendingBit(USART6, USART_IT_RXNE);
		if (ishead6_f == 1)
		{
			sta6++;
			USART6_RX_BUF[sta6] = USART_ReceiveData(USART6); //?����??����?��?��?��y?Y
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
// ����λ������
void vision_send(void)
{
	/*
	01234567
	4+5: (0:left 1:right 2:stay)
	6: aim mode (0: amor 1:buff)
	7:aimcolor(O:blue1:red)
	*/
//	USART6_TX_BUF = 0x00; // ���λ
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
		USART6_TX_BUF &= 0xF3;	 // ���
		USART6_TX_BUF |= (0x0C); // 0000 1100
	}
	if (Gimbal_Str->Key_Mode == Gimbal_WindMill)
	{
		USART6_TX_BUF &= 0xFD; //   1111 1101 ��0���
		USART6_TX_BUF |= 0x02;
	}
	else
		USART6_TX_BUF &= 0xFD; //   0000 0010  ��0���
//	
if(JudgeStructure.ext_game_robot_status.robot_id<10)
	USART6_TX_BUF|=0x01;
	else if(JudgeStructure.ext_game_robot_status.robot_id>10)
		USART6_TX_BUF&=0x0E;
//	USART6_TX_BUF|=0xFF;

	
	
	
	USART6_SendChar(USART6_TX_BUF);
}
