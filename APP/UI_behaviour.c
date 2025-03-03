#include "UI_behaviour.h"

ext_graph_char ext_data;

/*
custom_grapic_draw ���� һ������7 ��
stun     �ڼ���ͼ��
name     ͼ������
color    ͼ��ɫ
sx 		��ʼXλ��
sy		��ʼYλ��
ex		��βXλ��
ey 		��βYλ��
*/
void make_line(ext_student_interactive_header_graph_t *custom_grapic_draw,
			   uint8_t stun, uint8_t name[3], uint8_t color, uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey)
{
	custom_grapic_draw->graphic.grapic_data_struct[stun].graphic_name[0] = name[0];
	custom_grapic_draw->graphic.grapic_data_struct[stun].graphic_name[1] = name[1];
	custom_grapic_draw->graphic.grapic_data_struct[stun].graphic_name[2] = name[2]; // ͼ����
	// ���������ֽڴ�������ͼ����������ͼ�������������ж���
	custom_grapic_draw->graphic.grapic_data_struct[stun].operate_tpye = 1; // ͼ�β�����0���ղ�����1�����ӣ�2���޸ģ�3��ɾ����
	custom_grapic_draw->graphic.grapic_data_struct[stun].graphic_tpye = 0; // ͼ�����ͣ�0Ϊֱ�ߣ�1����2Բ4Բ��  �����Ĳ鿴�û��ֲ�
	custom_grapic_draw->graphic.grapic_data_struct[stun].layer = 1;		   // ͼ����
	custom_grapic_draw->graphic.grapic_data_struct[stun].color = color;	   // ��ɫ
	custom_grapic_draw->graphic.grapic_data_struct[stun].width = 1;
	custom_grapic_draw->graphic.grapic_data_struct[stun].start_x = sx;
	custom_grapic_draw->graphic.grapic_data_struct[stun].start_y = sy;
	custom_grapic_draw->graphic.grapic_data_struct[stun].end_x = ex;
	custom_grapic_draw->graphic.grapic_data_struct[stun].end_y = ey;
}

void make_guideline_graph(ext_student_interactive_header_graph_t *custom_grapic_draw) // ����׼��
{
	custom_grapic_draw->data_cmd_id = 0x0104;
	custom_grapic_draw->sender_ID = JudgeStructure.ext_game_robot_status.robot_id;
	custom_grapic_draw->receiver_ID = (JudgeStructure.ext_game_robot_status.robot_id + 0x100);
	uint8_t name[3] = {2, 1, 0};
	uint8_t stui = 0;
	//	int16_t _horiz_line[5] = {-45, -20, 10, -20, -30};
	// �Զ���ͼ������
	{
		make_line(&*custom_grapic_draw, stui++, name, 2, // 2Ϊ��ɫ
				  SCREEN_WIDTH / 2 + _SHIFFTING_W, SCREEN_HEIGHT / 2 + 340,
				  SCREEN_WIDTH / 2 + _SHIFFTING_W, SCREEN_HEIGHT / 2 - 340);
		name[2]++;

		make_line(&*custom_grapic_draw, stui++, name, 2,
				  500, 0,
				  700, 300);
		name[2]++;

		make_line(&*custom_grapic_draw, stui++, name, 2,
				  1420, 0,
				  1220, 300);
		name[2]++;
		/*
				for (; stui < 7; stui++)
				{
					make_line(&*custom_grapic_draw, stui++, name, 2,
							  SCREEN_WIDTH / 2 + _SHIFFTING_W - 80, SCREEN_HEIGHT / 2 + _horiz_line[stui - 5],
							  SCREEN_WIDTH / 2 + _SHIFFTING_W + 80, SCREEN_HEIGHT / 2 + _horiz_line[stui - 5]);
					name[2]++;
				}
		*/
	}
}

void make_num(ext_student_interactive_header_graph_t *custom_grapic_draw,
			  char *name, int32_t exfloat1, int type, int color, uint32_t sx, uint32_t sy)
{

	custom_grapic_draw->data_cmd_id = 0x0101;
	custom_grapic_draw->sender_ID = JudgeStructure.ext_game_robot_status.robot_id; // ������ID�������˶�ӦID
	custom_grapic_draw->receiver_ID = (JudgeStructure.ext_game_robot_status.robot_id + 0x100);

	{
		*custom_grapic_draw->graphic.grapic_data_struct[0].graphic_name = *name;

		custom_grapic_draw->graphic.grapic_data_struct[0].operate_tpye = type; // ͼ�β�����0���ղ�����1�����ӣ�2���޸ģ�3��ɾ����
		custom_grapic_draw->graphic.grapic_data_struct[0].graphic_tpye = 6;	   // ͼ�����ͣ�0Ϊֱ�ߣ������Ĳ鿴�û��ֲ�
		custom_grapic_draw->graphic.grapic_data_struct[0].layer = 4;		   // ͼ����
		custom_grapic_draw->graphic.grapic_data_struct[0].color = color;	   // ��ɫ
		custom_grapic_draw->graphic.grapic_data_struct[0].start_angle = 25;
		custom_grapic_draw->graphic.grapic_data_struct[0].end_angle = 4;
		custom_grapic_draw->graphic.grapic_data_struct[0].width = 3;
		custom_grapic_draw->graphic.grapic_data_struct[0].start_x = sx;
		custom_grapic_draw->graphic.grapic_data_struct[0].start_y = sy;
		custom_grapic_draw->graphic.grapic_data_struct[0].radius = (exfloat1 << 22) >> 22; // �����С��뾶
		custom_grapic_draw->graphic.grapic_data_struct[0].end_x = (exfloat1 << 11) >> 21;
		custom_grapic_draw->graphic.grapic_data_struct[0].end_y = exfloat1 >> 21;
	}
}

void make_cir(ext_student_interactive_header_graph_t *custom_grapic_draw,
			  int type, int color, uint16_t r, uint8_t r_aim_amor, uint16_t s_agl, uint16_t e_agl)
{
	custom_grapic_draw->data_cmd_id = 0x0103;									   // ����7��ͼ�Σ�����ID����ѯ����ϵͳ�ֲᣩ
	custom_grapic_draw->sender_ID = JudgeStructure.ext_game_robot_status.robot_id; // ������ID�������˶�ӦID
	custom_grapic_draw->receiver_ID = (JudgeStructure.ext_game_robot_status.robot_id + 0x100);
	{
		custom_grapic_draw->graphic.grapic_data_struct[0].graphic_name[0] = 'c';
		custom_grapic_draw->graphic.grapic_data_struct[0].graphic_name[1] = ' ';
		custom_grapic_draw->graphic.grapic_data_struct[0].graphic_name[2] = 'c';
		custom_grapic_draw->graphic.grapic_data_struct[0].operate_tpye = type;	 // ͼ�β�����0���ղ�����1�����ӣ�2���޸ģ�3��ɾ����
		custom_grapic_draw->graphic.grapic_data_struct[0].graphic_tpye = circle; // ͼ�����ͣ�0Ϊֱ�ߣ������Ĳ鿴�û��ֲ�
		custom_grapic_draw->graphic.grapic_data_struct[0].layer = 1;			 // ͼ����
		custom_grapic_draw->graphic.grapic_data_struct[0].color = color;		 // ��ɫ

		custom_grapic_draw->graphic.grapic_data_struct[0].width = 2;
		custom_grapic_draw->graphic.grapic_data_struct[0].start_x = SCREEN_WIDTH / 2;
		custom_grapic_draw->graphic.grapic_data_struct[0].start_y = SCREEN_HEIGHT / 2;
		custom_grapic_draw->graphic.grapic_data_struct[0].start_angle = 0;
		custom_grapic_draw->graphic.grapic_data_struct[0].end_angle = 0;
		custom_grapic_draw->graphic.grapic_data_struct[0].radius = r;
		custom_grapic_draw->graphic.grapic_data_struct[0].end_x = 0;
		custom_grapic_draw->graphic.grapic_data_struct[0].end_y = 0;
	}
	{
		custom_grapic_draw->graphic.grapic_data_struct[1].graphic_name[0] = 'x';
		custom_grapic_draw->graphic.grapic_data_struct[1].graphic_name[1] = ' ';
		custom_grapic_draw->graphic.grapic_data_struct[1].graphic_name[2] = 'a';
		custom_grapic_draw->graphic.grapic_data_struct[1].operate_tpye = type;	 // ͼ�β�����0���ղ�����1�����ӣ�2���޸ģ�3��ɾ����
		custom_grapic_draw->graphic.grapic_data_struct[1].graphic_tpye = circle; // ͼ�����ͣ�0Ϊֱ�ߣ������Ĳ鿴�û��ֲ�
		custom_grapic_draw->graphic.grapic_data_struct[1].layer = 1;			 // ͼ����
		custom_grapic_draw->graphic.grapic_data_struct[1].color = color;		 // ��ɫ

		custom_grapic_draw->graphic.grapic_data_struct[1].width = r_aim_amor;
		custom_grapic_draw->graphic.grapic_data_struct[1].start_x = 1720;
		custom_grapic_draw->graphic.grapic_data_struct[1].start_y = SCREEN_HEIGHT / 2 + 300;
		custom_grapic_draw->graphic.grapic_data_struct[1].start_angle = 0;
		custom_grapic_draw->graphic.grapic_data_struct[1].end_angle = 0;
		custom_grapic_draw->graphic.grapic_data_struct[1].radius = 20;
		custom_grapic_draw->graphic.grapic_data_struct[1].end_x = 0;
		custom_grapic_draw->graphic.grapic_data_struct[1].end_y = 0;
	}
	{
		custom_grapic_draw->graphic.grapic_data_struct[2].graphic_name[0] = 'w';
		custom_grapic_draw->graphic.grapic_data_struct[2].graphic_name[1] = 'i';
		custom_grapic_draw->graphic.grapic_data_struct[2].graphic_name[2] = 'd';
		custom_grapic_draw->graphic.grapic_data_struct[2].operate_tpye = type; // ͼ�β�����0���ղ�����1�����ӣ�2���޸ģ�3��ɾ����
		custom_grapic_draw->graphic.grapic_data_struct[2].graphic_tpye = arc;  // ͼ�����ͣ�0Ϊֱ�ߣ������Ĳ鿴�û��ֲ�
		custom_grapic_draw->graphic.grapic_data_struct[2].layer = 1;		   // ͼ����
		custom_grapic_draw->graphic.grapic_data_struct[2].color = color;	   // ��ɫ

		custom_grapic_draw->graphic.grapic_data_struct[2].width = 2;
		custom_grapic_draw->graphic.grapic_data_struct[2].start_x = 1720;
		custom_grapic_draw->graphic.grapic_data_struct[2].start_y = SCREEN_HEIGHT / 2 + 200;

		custom_grapic_draw->graphic.grapic_data_struct[2].start_angle = s_agl;
		custom_grapic_draw->graphic.grapic_data_struct[2].end_angle = e_agl;
		custom_grapic_draw->graphic.grapic_data_struct[2].radius = 0;
		custom_grapic_draw->graphic.grapic_data_struct[2].end_x = 20;
		custom_grapic_draw->graphic.grapic_data_struct[2].end_y = 20;
	}
}

// �����ַ�ͼ��
void make_word(ext_student_interactive_header_word_t *custom_grapic_draw,
			   int type, int color, char *txt, uint8_t name[3], uint32_t sx, uint32_t sy)
{
	// �Զ���ͼ�λ���
	custom_grapic_draw->data_cmd_id = 0x0110;									   // ��������
	custom_grapic_draw->sender_ID = JudgeStructure.ext_game_robot_status.robot_id; // ������ID�������˶�ӦID
	custom_grapic_draw->receiver_ID = (JudgeStructure.ext_game_robot_status.robot_id + 0x100);
	// �Զ���ͼ������
	{
		*custom_grapic_draw->character.grapic_data_struct.graphic_name = *name;
		// ���������ֽڴ�������ͼ����������ͼ�������������ж���
		custom_grapic_draw->character.grapic_data_struct.operate_tpye = type; // ͼ�β�����0���ղ�����1�����ӣ�2���޸ģ�3��ɾ����
		custom_grapic_draw->character.grapic_data_struct.graphic_tpye = 7;	  // ͼ�����ͣ�0Ϊֱ�ߣ������Ĳ鿴�û��ֲ�
		custom_grapic_draw->character.grapic_data_struct.layer = 3;			  // ͼ����
		custom_grapic_draw->character.grapic_data_struct.color = color;		  // ��ɫ
		custom_grapic_draw->character.grapic_data_struct.start_angle = 25;	  // �����С
		custom_grapic_draw->character.grapic_data_struct.end_angle = 4;		  // �ַ�����
		custom_grapic_draw->character.grapic_data_struct.width = 3;			  // ������ϸ
		custom_grapic_draw->character.grapic_data_struct.start_x = sx;
		custom_grapic_draw->character.grapic_data_struct.start_y = sy;
		custom_grapic_draw->character.grapic_data_struct.end_x = 0;
		custom_grapic_draw->character.grapic_data_struct.end_y = 0;
		custom_grapic_draw->character.grapic_data_struct.radius = 0;
	}
	// ��������
	memset(custom_grapic_draw->character.data, 0, 30); // �洢���ݵ���������

	memcpy(custom_grapic_draw->character.data, txt, strlen(txt));
}

uint8_t seq = 0;
void referee_data_pack_handle(uint8_t sof, uint16_t cmd_id, uint8_t *p_data, uint16_t leng)
{
	unsigned char i = i;
	uint8_t tx_buff[228];
	//   frameheader_len ֡ͷ����   		define      5
	//    cmd_len      �����볤��  		define  		2
	//		leng           ���ݳ���      ��С���
	//    crc_len        У�鳤��     define     	2

	uint16_t frame_length = 5 + 2 + leng + 2; // ����֡����	 ֡ͷ 5  ID 2  CRC 2
	memset(tx_buff, 0, frame_length);		  // �洢���ݵ���������

	/*****֡ͷ���*****/
	tx_buff[0] = sof; // ����֡��ʼ�ֽ� �̶�ֵ Ϊ  0xA5

	memcpy(&tx_buff[1], (uint8_t *)&leng, sizeof(leng)); // ����֡��data�ĳ���

	tx_buff[3] = seq; // �����

	Append_CRC8_Check_Sum(tx_buff, 5); // ֡ͷУ��CRC8

	/*****��������*****/
	memcpy(&tx_buff[5], (uint8_t *)&cmd_id, 2);

	/*****���ݴ��*****/
	memcpy(&tx_buff[5 + 2], p_data, leng); //  p_data Ϊ���ݵĵ�ַ   leng Ϊ ���ݵĴ�С

	Append_CRC16_Check_Sum(tx_buff, frame_length); // һ֡����У��CRC16

	if (seq == 0xff)
		seq = 0;
	else
		seq++;

	/*****�����ϴ�*****/
	USART_ClearFlag(USART3, USART_FLAG_TC);

	for (i = 0; i < frame_length; i++)
	{
		USART_SendData(USART3, tx_buff[i]);
		while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET)
			;
	}
}
