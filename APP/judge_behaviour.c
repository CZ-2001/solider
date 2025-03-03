#include "judge_behaviour.h"

uint8_t JudgeUARTtemp;
uint8_t JudgeDataBuffer[JudgeBufferLength];
JudgementDataTypedef JudgeStructure;

void Judge_Calucate(uint8_t Judge[JudgeBufferLength])
{

	static uint16_t start_pos;
	static uint16_t next_start_pos;

	while (1)
	{
		memcpy(&JudgeStructure.Frame_Header, &Judge[start_pos], sizeof(Frame_Header_t));
		/*��У��ͷ֡0xA5 Ȼ��crc8У��֡ͷ ��crc16λУ������*/
		if ((JudgeStructure.Frame_Header.SOF == (uint16_t)0xA5) &&
			(1 == Verify_CRC8_Check_Sum(&Judge[start_pos], sizeof(Frame_Header_t))) &&
			(1 == Verify_CRC16_Check_Sum(&Judge[start_pos], JudgeStructure.Frame_Header.DataLength + sizeof(Frame_Header_t) + 4)))
		{
			memcpy(&JudgeStructure.cmd_id, (&Judge[start_pos] + 5), sizeof(JudgeStructure.cmd_id));
			Judge[start_pos]++;														 // ֱ����֡ͷ0xA5 ��ֹ�ظ�����
			next_start_pos = start_pos + 9 + JudgeStructure.Frame_Header.DataLength; // 9Ϊ 5λ֡ͷ 2λ���ݳ��� 2У��λ
			switch (JudgeStructure.cmd_id)
			{
			case 0x0001: // ����״̬���ݣ�3Hz ���ڷ���
			{
				memcpy(&JudgeStructure.ext_game_status, (&Judge[start_pos] + 7), JudgeStructure.Frame_Header.DataLength);
			}
			break;
			case 0x0002: // ����������ݣ�������������
			{
				memcpy(&JudgeStructure.ext_game_result, (&Judge[start_pos] + 7), JudgeStructure.Frame_Header.DataLength);
			}
			break;
			case 0x0003: // ����������Ѫ�����ݣ�3Hz ���ڷ���
			{
				memcpy(&JudgeStructure.ext_game_robot_HP, (&Judge[start_pos] + 7), JudgeStructure.Frame_Header.DataLength);
			}
			break;
			case 0x0004: // ���ڷ���״̬�����ڷ������
			{
				memcpy(&JudgeStructure.ext_dart_status, (&Judge[start_pos] + 7), JudgeStructure.Frame_Header.DataLength);
			}
			break;
			case 0x0005: // �˹�������ս���ӳ���ͷ�״̬��1Hz ���ڷ���
			{
				memcpy(&JudgeStructure.ext_ICRA_buff_debuff_zone_and_lurk_status, (&Judge[start_pos] + 7), JudgeStructure.Frame_Header.DataLength);
			}
			break;
			case 0x0101: // �����¼����ݣ�3Hz ���ڷ���
			{
				memcpy(&JudgeStructure.ext_event_data, (&Judge[start_pos] + 7), JudgeStructure.Frame_Header.DataLength);
			}
			break;
			case 0x0102: // ���ز���վ������ʶ���ݣ������ı����
			{
				memcpy(&JudgeStructure.ext_supply_projectile_action, (&Judge[start_pos] + 7), JudgeStructure.Frame_Header.DataLength);
			}
			break;
			case 0x0103: // ���󲹸�վ�������ݣ��ɲ����ӷ��ͣ����� 10Hz����RM �Կ�����δ���ţ�
			{
				memcpy(&JudgeStructure.ext_supply_projectile_booking, (&Judge[start_pos] + 7), JudgeStructure.Frame_Header.DataLength);
			}
			break;
			case 0x0104: // ���о������ݣ����淢������
			{
				memcpy(&JudgeStructure.ext_referee_warning, (&Judge[start_pos] + 7), JudgeStructure.Frame_Header.DataLength);
			}
			break;
			case 0x0105: // ���ڷ���ڵ���ʱ��3Hz ���ڷ���
			{
				memcpy(&JudgeStructure.ext_dart_remaining_time, (&Judge[start_pos] + 7), JudgeStructure.Frame_Header.DataLength);
			}
			break;
			case 0x0201: // ������״̬���ݣ�10Hz ���ڷ���
			{
				memcpy(&JudgeStructure.ext_game_robot_status, (&Judge[start_pos] + 7), JudgeStructure.Frame_Header.DataLength);
			}
			break;
			case 0x0202: // ʵʱ�����������ݣ�50Hz ���ڷ���
			{
				memcpy(&JudgeStructure.ext_power_heat_data, (&Judge[start_pos] + 7), JudgeStructure.Frame_Header.DataLength);
			}
			break;
			case 0x0203: // ������λ�����ݣ�10Hz ���ڷ���
			{
				memcpy(&JudgeStructure.ext_game_robot_pos, (&Judge[start_pos] + 7), JudgeStructure.Frame_Header.DataLength);
			}
			break;
			case 0x0204: // �������������ݣ�1Hz ���ڷ���
			{
				memcpy(&JudgeStructure.ext_buff, (&Judge[start_pos] + 7), JudgeStructure.Frame_Header.DataLength);
			}
			break;
			case 0x0205: // ���л���������״̬���ݣ�10Hz ���ڷ��ͣ�ֻ�п��л��������ط���
			{
				memcpy(&JudgeStructure.aerial_robot_energy, (&Judge[start_pos] + 7), JudgeStructure.Frame_Header.DataLength);
			}
			break;
			case 0x0206: // �˺�״̬���ݣ��˺���������
			{
				memcpy(&JudgeStructure.ext_robot_hurt, (&Judge[start_pos] + 7), JudgeStructure.Frame_Header.DataLength);
			}
			break;
			case 0x0207: // ʵʱ������ݣ��ӵ��������
			{
				memcpy(&JudgeStructure.ext_shoot_data, (&Judge[start_pos] + 7), JudgeStructure.Frame_Header.DataLength);
			}
			break;
			case 0x0208: // �ӵ�ʣ�෢���������л������Լ��ڱ������˷��ͣ�10Hz ���ڷ���
			{
				memcpy(&JudgeStructure.ext_bullet_remaining, (&Judge[start_pos] + 7), JudgeStructure.Frame_Header.DataLength);
			}
			break;
			case 0x0209: // ������ RFID ״̬��3Hz ���ڷ���
			{
				memcpy(&JudgeStructure.ext_rfid_status, (&Judge[start_pos] + 7), JudgeStructure.Frame_Header.DataLength);
			}
			break;
			case 0x020A: // ���ڻ����˿ͻ���ָ���飬10Hz ���ڷ���
			{
				memcpy(&JudgeStructure.ext_dart_client_cmd, (&Judge[start_pos] + 7), JudgeStructure.Frame_Header.DataLength);
			}
			break;

			case 0x0301: // �����˼佻�����ݣ����ͷ��������ͣ����� 10Hz
			{
				memcpy(&JudgeStructure.ext_student_interactive_header_data, (&Judge[start_pos] + 7), JudgeStructure.Frame_Header.DataLength);
			}

			default:
			{
				break;
			}
			}
			start_pos = next_start_pos;
		}
		else
		{
			start_pos = 0;
			break;
		}
		if (start_pos > JudgeBufferLength)
		{
			start_pos = 0;
			break;
		}
	}
}

void USART3_IRQHandler()
{

	JudgeUARTtemp = USART3->DR;		// ��������  �����ݴ洢��JudgeUARTtemp
	JudgeUARTtemp = USART3->SR;		// �ж��Ƿ���յ�����
	DMA_Cmd(DMA1_Stream1, DISABLE); // USART3_RX
	Judge_Calucate(JudgeDataBuffer);
	// ����DMA
	DMA_ClearFlag(DMA1_Stream1, DMA_FLAG_TCIF1 | DMA_FLAG_HTIF1);
	while (DMA_GetCmdStatus(DMA1_Stream1) != DISABLE)
		;
	DMA_SetCurrDataCounter(DMA1_Stream1, JudgeBufferLength);
	DMA_Cmd(DMA1_Stream1, ENABLE);
}

void USART3_SendBuff(uint8_t *buf, uint16_t len)
{
	int i;
	for (i = 0; i < len; i++)
	{
		USART_SendData(USART3, *((u8 *)&buf + i));
	}
	while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET)
		;
}

// #define __DRIVER_GLOBALS

// #include "string.h"
// #include "crc_check.h"
// #include "usart2.h"
// #include "chassis_behaviour.h"
// void judgeCalculate(uint8_t JudgeDataBuffer[JudgeBufferLength])//����ϵͳ����
//{
//	static uint16_t start_pos=0,next_start_pos=0;
//	while(1)
//	{
//		memcpy(&JudgementData.frameHeader, &JudgeDataBuffer[start_pos],FrameHeader_Len);
//		/*��У��ͷ֡0xA5 Ȼ��crc8У��֡ͷ ��crc16λУ������*/
//		if ((JudgementData.frameHeader.SOF==(uint16_t)JudgeFrameHeader)\
//    &&(1==Verify_CRC8_Check_Sum(&JudgeDataBuffer[start_pos],FrameHeader_Len))\
//    &&(1==Verify_CRC16_Check_Sum(&JudgeDataBuffer[start_pos], JudgementData.frameHeader.DataLenth+FrameHeader_Len+4)))//����λ����+֡ͷ����+�����볤��+У���볤��
//		{
//			memcpy(&JudgementData.rxCmdId, (&JudgeDataBuffer[start_pos]+5), sizeof(JudgementData.rxCmdId));
//			JudgeDataBuffer[start_pos]++;//ÿ������һ�ξ���֡ͷ��һ��ֹ�ٴδ�����֡����
//			next_start_pos=start_pos+9+JudgementData.frameHeader.DataLenth;//9Ϊ 5λ֡ͷ 2λ���ݳ��� 2У��λ
//			switch(JudgementData.rxCmdId)
//			{
//				case ID_game_state://��ȡ����״̬���� 001
//				{
//					memcpy(&JudgementData.GameState,(&JudgeDataBuffer[start_pos]+7),JudgementData.frameHeader.DataLenth);//�������е����ݸ��Ƶ���Ӧ�Ľṹ����ȥ
//				}
//				break;
//				case ID_game_result://��ȡ������� 002
//				{
//					memcpy(&JudgementData.GameResult,(&JudgeDataBuffer[start_pos]+7),JudgementData.frameHeader.DataLenth);//�������е����ݸ��Ƶ���Ӧ�Ľṹ����ȥ
//				}
//				break;
//				case ID_game_robot_HP://��ȡ������Ѫ������003
//				{
//					memcpy(&JudgementData.GameRobotHP,(&JudgeDataBuffer[start_pos]+7),JudgementData.frameHeader.DataLenth);//�������е����ݸ��Ƶ���Ӧ�Ľṹ����ȥ
//				}
//				break;
//				case ID_event_data://��ȡ�����¼� 101
//				{
//					memcpy(&JudgementData.EventData,(&JudgeDataBuffer[start_pos]+7),JudgementData.frameHeader.DataLenth);//�������е����ݸ��Ƶ���Ӧ�Ľṹ����ȥ
//				}
//				break;
//				case ID_supply_projectile_action://��ȡ����վ���� 102
//				{
//					memcpy(&JudgementData.SupplyProjectileAction,(&JudgeDataBuffer[start_pos]+7),JudgementData.frameHeader.DataLenth);//�������е����ݸ��Ƶ���Ӧ�Ľṹ����ȥ
//				}
//				break;
//				case Robot_Status_ID://��ȡ�����˵ȼ�Ѫ���� 201
//				{
//					memcpy(&JudgementData.game_robot_state_t,(&JudgeDataBuffer[start_pos]+7),JudgementData.frameHeader.DataLenth);//�������е����ݸ��Ƶ���Ӧ�Ľṹ����ȥ
//				}
//				break;
//				case power_heat_data_ID://��ȡ������ǹ������ 202
//				{
//					memcpy(&JudgementData.power_heat_data_t,(&JudgeDataBuffer[start_pos]+7),JudgementData.frameHeader.DataLenth);//�������е����ݸ��Ƶ���Ӧ�Ľṹ����ȥ
//			  }
//				break;
//				case shoot_data_ID://��ȡ��������Ƶ���� 207
//				{
//					memcpy(&JudgementData.shoot_data_t,(&JudgeDataBuffer[start_pos]+7),JudgementData.frameHeader.DataLenth);//�������е����ݸ��Ƶ���Ӧ�Ľṹ����ȥ
//				}
//				break;
//				case robot_hurt_ID://��ȡ�������˺� 206
//				{
//					memcpy(&JudgementData.robot_hurt_t,(&JudgeDataBuffer[start_pos]+7),JudgementData.frameHeader.DataLenth);//�������е����ݸ��Ƶ���Ӧ�Ľṹ����ȥ
//				}
//				break;
//				case robot_buff_ID: //���� 204
//         {
//					memcpy(&JudgementData.userinfo,(&JudgeDataBuffer[start_pos]+7),JudgementData.frameHeader.DataLenth);//�������е����ݸ��Ƶ���Ӧ�Ľṹ����ȥ
//				}
//				break;
//				case student_interactive_header_ID:
//         {
//					memcpy(&JudgementData.userinfo,(&JudgeDataBuffer[start_pos]+7),JudgementData.frameHeader.DataLenth);//udgementData.frameHeader.DataLenth);//�������е����ݸ��Ƶ���Ӧ�Ľṹ����ȥ
//					RadarStu.dir		= JudgementData.userinfo.data_leida.data[1]<<8 | JudgementData.userinfo.data_leida.data[2];
//					RadarStu.speed	= JudgementData.userinfo.data_leida.data[4];
//				}
//			}
//			start_pos=next_start_pos;
//		}
//		else
//		{
//			start_pos=0;
//			break;
//		}
//		/**���ͷָ��Խ�����˳�ѭ��**/
//		if(start_pos>JudgeBufferLength)
//		{
//			start_pos=0;
//			break;
//		}
//	}
// }

// uint8_t JudgeUARTtemp;
////Judge
// void USART3_IRQHandler(void)
//{
//     JudgeUARTtemp = USART3->DR;
//     JudgeUARTtemp = USART3->SR;
//
//     DMA_Cmd(DMA1_Stream1, DISABLE);
//
//	  judgeCalculate(JudgeDataBuffer);
//
//     //����DMA
//     DMA_ClearFlag(DMA1_Stream1, DMA_FLAG_TCIF1 | DMA_FLAG_HTIF1);
//     while(DMA_GetCmdStatus(DMA1_Stream1) != DISABLE);
//     DMA_SetCurrDataCounter(DMA1_Stream1, JudgeBufferLength);
//     DMA_Cmd(DMA1_Stream1, ENABLE);
// }

// void USART3_SendBuff (uint8_t *buf,uint16_t len)
//{
//   int i;
//	for(i=0; i<len;i++)
//	{
//     USART_SendData(USART3,*((u8*)&buf+i));
//	}
// while( USART_GetFlagStatus(USART3 ,USART_FLAG_TC) == RESET );
//
// }

// tFrame Frame = {.FrameHeader.SOF = 0xA5};
// uint8_t tx_buf[200];
// void Send_FrameData(uint16_t cmdid, uint8_t * pchMessage,uint16_t dwLength)
//{
//	uint16_t frame_length = HEADER_LEN + CMD_LEN + dwLength + CRC_LEN;
//
//	FrameHeader_t *p_header = (FrameHeader_t*)tx_buf;
//
//   p_header->SOF          = 0xA5;
//   p_header->DataLength   = dwLength;
//   p_header->Seq          = 0;
//
//	memcpy(&tx_buf[HEADER_LEN], (uint8_t*)&cmdid, CMD_LEN);
//   Append_CRC8_Check_Sum(tx_buf, HEADER_LEN);
//   memcpy(&tx_buf[HEADER_LEN + CMD_LEN], pchMessage, dwLength);
//   Append_CRC16_Check_Sum(tx_buf, frame_length);
//
//   USART3_SendBuff((uint8_t *)&tx_buf,frame_length);               //֡����
//
// }

// extern uint8_t client_custom_Send[28]={0xA5,0x13,0,0,0x6D,  0x01,0x03,  0x80,0xD1,   0x00,0x00,  0x00,0x00,  0,0,0,0, 0,0,0,0, 0,0,0,0, 0,    0,0};
// extern uint8_t robot_custom_Send[28]={0xA5,0x13,0,0,0x6D,  0x01,0x03,  0x00,0x02,   0x00,0x00,  0x00,0x02,  0,0,0,0, 0,0,0,0, 0,0,0,0, 0,    0,0};
//
////void client_send(uint8_t * data)
////{
////		JudgementData.userinfo.id.send_id=JudgementData.game_robot_state_t.robot_id;
////
////		if(JudgementData.game_robot_state_t.robot_id>10)
////			JudgementData.userinfo.id.receive_id=0x0106+JudgementData.userinfo.id.send_id;
////		else
////			JudgementData.userinfo.id.receive_id=0x0100+JudgementData.userinfo.id.send_id;

////		JudgementData.userinfo.data3=8;

////		memcpy(data,&JudgementData.userinfo,28);
////		Append_CRC16_Check_Sum(data,28);
////		USART3_SendBuff(data,28);
////}

// void robot_send(uint8_t * data)
//{
//		JudgementData.robot_data_t.id.send_id=JudgementData.game_robot_state_t.robot_id;
//		if(JudgementData.game_robot_state_t.robot_id<10)
//			JudgementData.robot_data_t.id.receive_id=3;
//		else
//			JudgementData.robot_data_t.id.receive_id=17;
//		memcpy(data,&JudgementData.robot_data_t,28);
//		Append_CRC16_Check_Sum(data,28);
//		USART3_SendBuff(data,28);
// }

// void client_init(uint8_t * data1,uint8_t * data2)
//{
//		memcpy(&JudgementData.userinfo,data1,28);
//		memcpy(&JudgementData.robot_data_t,data2,28);
// }

// void get_chassis_power_and_buffer(float *power, float *buffer, uint16_t *powmax)
//{
//     *power = JudgementData.power_heat_data_t.chassis_power;
//     *buffer = JudgementData.power_heat_data_t.chassis_power_buffer;
//		*powmax = JudgementData.game_robot_state_t.chassis_power_limit;

//}
