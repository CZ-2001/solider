#ifndef __JUDGE_BEHAVIOUR_
#define __JUDGE_BEHAVIOUR_
#include "main.h"
#include "sys.h"
#include "stm32f4xx.h"
#include "string.h"
#include "check_crc.h"
#define Judge_Header 0xA5

#define JudgeBufferLength 400

/*
ͨ��Э����?
frame_header (5-byte)
cmd_id (2-byte)
data (n-byte)
frame_tail (2-byte��CRC16������У��)
*/
// frame_header ��ʽ˵��
/*
SOF   			1byte
data_length	 	2byte
seq				1byte
CRC8 		    1byte

*/
typedef __packed struct
{
	uint8_t SOF;		 // ������ʼ�ֽڣ��̶�Ϊ0xA5
	uint16_t DataLength; // ���ݳ���
	uint8_t Seq;		 // �����?
	uint8_t CRC8;		 // ֡ͷCRCУ��
} Frame_Header_t;		 // ֡ͷ

// cmd_id ������ ID ˵��


// cmd_id ������ ��ϸ˵��

typedef __packed struct
{
	uint8_t game_type : 4;
	uint8_t game_progress : 4;
	uint16_t stage_remain_time;
	uint64_t SyncTimeStamp;
} ext_game_status_t;

/*
����������ݣ�?0x0002������Ƶ�ʣ�������������

0 ƽ�� 1 �췽ʤ�� 2 ����ʤ��
*/
typedef __packed struct
{
	uint8_t winner;
} ext_game_result_t;

typedef __packed struct
{
	uint16_t red_1_robot_HP;
	uint16_t red_2_robot_HP;
	uint16_t red_3_robot_HP;
	uint16_t red_4_robot_HP;
	uint16_t red_5_robot_HP;
	uint16_t red_7_robot_HP;
	uint16_t red_outpost_HP;
	uint16_t red_base_HP;
	uint16_t blue_1_robot_HP;
	uint16_t blue_2_robot_HP;
	uint16_t blue_3_robot_HP;
	uint16_t blue_4_robot_HP;
	uint16_t blue_5_robot_HP;
	uint16_t blue_7_robot_HP;
	uint16_t blue_outpost_HP;
	uint16_t blue_base_HP;
} ext_game_robot_HP_t;

// ���ڷ���״̬ ������ID 0x0004  ����Ƶ�ʣ����ڷ������?  ���ͷ�Χ�����л�����
typedef __packed struct
{
	uint8_t dart_belong;		  // ������ڵĶ���?: 1���췽����  2����������
	uint8_t stage_remaining_time; // ����ʱʣ��ı���ʱ��?  /s
} ext_dart_status_t;


typedef __packed struct
{
	uint8_t F1_zone_status : 1;
	uint8_t F1_zone_buff_debuff_status : 3;
	uint8_t F2_zone_status : 1;
	uint8_t F2_zone_buff_debuff_status : 3;
	uint8_t F3_zone_status : 1;
	uint8_t F3_zone_buff_debuff_status : 3;
	uint8_t F4_zone_status : 1;
	uint8_t F4_zone_buff_debuff_status : 3;
	uint8_t F5_zone_status : 1;
	uint8_t F5_zone_buff_debuff_status : 3;
	uint8_t F6_zone_status : 1;
	uint8_t F6_zone_buff_debuff_status : 3;
	uint16_t red1_bullet_left;
	uint16_t red2_bullet_left;
	uint16_t blue1_bullet_left;
	uint16_t blue2_bullet_left;
	uint8_t lurk_mode;
	uint8_t res;
} ext_ICRA_buff_debuff_zone_and_lurk_status_t;

typedef __packed struct
{
	uint32_t event_type;
} ext_event_data_t;


typedef __packed struct
{
	uint8_t supply_projectile_id;
	uint8_t supply_robot_id;
	uint8_t supply_projectile_step;
	uint8_t supply_projectile_num;
} ext_supply_projectile_action_t;


typedef __packed struct
{
	uint8_t supply_projectile_id;
	uint8_t supply_robot_id;
	uint8_t supply_num;
} ext_supply_projectile_booking_t;


typedef __packed struct
{
	uint8_t level;
	uint8_t foul_robot_id;
} ext_referee_warning_t;


typedef __packed struct
{
	uint8_t dart_remaining_time;
} ext_dart_remaining_time_t;


typedef __packed struct
{
	uint8_t robot_id;
	uint8_t robot_level;
	uint16_t remain_HP;
	uint16_t max_HP;
	uint16_t shooter_id1_17mm_cooling_rate;
	uint16_t shooter_id1_17mm_cooling_limit;
	uint16_t shooter_id1_17mm_speed_limit;

	uint16_t shooter_id2_17mm_cooling_rate;
	uint16_t shooter_id2_17mm_cooling_limit;
	uint16_t shooter_id2_17mm_speed_limit;

	uint16_t shooter_id1_42mm_cooling_rate;
	uint16_t shooter_id1_42mm_cooling_limit;
	uint16_t shooter_id1_42mm_speed_limit;

	uint16_t chassis_power_limit;
	uint8_t mains_power_gimbal_output : 1;
	uint8_t mains_power_chassis_output : 1;
	uint8_t mains_power_shooter_output : 1;
} ext_game_robot_status_t;


typedef __packed struct
{
	uint16_t chassis_volt;
	uint16_t chassis_current;
	float chassis_power;
	uint16_t chassis_power_buffer;
	uint16_t shooter_id1_17mm_cooling_heat;
	uint16_t shooter_id2_17mm_cooling_heat;
	uint16_t shooter_id1_42mm_cooling_heat;
} ext_power_heat_data_t;


typedef __packed struct
{
	float x;
	float y;
	float z;
	float yaw;
} ext_game_robot_pos_t;

/*
���������棺0x0204������Ƶ�ʣ�1Hz
bit 0��������Ѫ����Ѫ״̬
bit 1��ǹ��������ȴ����
bit 2�������˷����ӳ�
bit 3�������˹����ӳ�
���� bit ����
*/
typedef __packed struct
{
	uint8_t power_rune_buff;
} ext_buff_t;

/*
���л���������״̬��0x0205������Ƶ�ʣ�10Hz

�ɹ���ʱ�� ��λ s��30s �ݼ��� 0

*/
typedef __packed struct
{
	uint8_t attack_time;
} aerial_robot_energy_t;

/*
�˺�״̬��0x0206������Ƶ�ʣ��˺���������

bit 0-3����Ѫ���仯����Ϊװ���˺�������װ�� ID��������ֵΪ 0-4 �Ŵ��������˵�
���װ��Ƭ������Ѫ���仯���ͣ��ñ�����ֵ�? 0��
bit 4-7��Ѫ���仯����

0x0 װ���˺���Ѫ��
0x1 ģ����߿�Ѫ��?
0x2 �����ٿ�Ѫ��
0x3 ��ǹ��������Ѫ��
0x4 �����̹��ʿ�Ѫ��
0x5 װ��ײ����Ѫ
*/
typedef __packed struct
{
	uint8_t armor_id : 4;
	uint8_t hurt_type : 4;
} ext_robot_hurt_t;

/*
ʵʱ�����Ϣ��?0x0207������Ƶ�ʣ��������?
bullet_type:
�ӵ�����: 1��17mm ���� 2��42mm ����

shooter_id:
�������? ID��
1��1 �� 17mm �������?
2��2 �� 17mm �������?
3��42mm �������?

bullet_freq:
�ӵ���Ƶ ��λ Hz

bullet_speed:
�ӵ����� ��λ m/s
*/
typedef __packed struct
{
	uint8_t bullet_type;
	uint8_t shooter_id;
	uint8_t bullet_freq;
	float bullet_speed;
} ext_shoot_data_t;

/*
�ӵ�ʣ�෢������0x0208������Ƶ�ʣ�10Hz ���ڷ��ͣ�
���л����˷���

bullet_remaining_num_17mm:
17mm �ӵ�ʣ�෢����Ŀ

bullet_remaining_num_42mm:
42mm �ӵ�ʣ�෢����Ŀ

coin_remaining_num:
ʣ��������

*/

typedef __packed struct
{
	uint16_t bullet_remaining_num_17mm;
	uint16_t bullet_remaining_num_42mm;
	uint16_t coin_remaining_num;
} ext_bullet_remaining_t;

/*
������ RFID ״̬��0x0209������Ƶ�ʣ�3Hz��
���ͷ�Χ����һ������

bit 0�����������? RFID ״̬��
bit 1���ߵ������? RFID ״̬��
bit 2���������ؼ����? RFID ״̬��
bit 3�����������? RFID ״̬��
bit 4��ǰ�ڸ������? RFID ״̬��
bit 6����Ѫ�������? RFID ״̬��
bit 7�����̻����˸�� RFID ״̬��
bit 8-31������

*/
typedef __packed struct
{
	uint32_t rfid_status;
} ext_rfid_status_t;

/*
���ڻ����˿ͻ���ָ�����ݣ�0x020A������Ƶ�ʣ�10Hz��
���ͷ�Χ����һ������

dart_launch_opening_status:
��ǰ���ڷ���ڵ�״�?
1���رգ�
2�����ڿ������߹ر���
0���Ѿ�����

dart_attack_target:
���ڵĴ��Ŀ�꣬Ĭ��Ϊǰ��վ��?
0��ǰ��վ��
1�����ء�

target_change_time:
�л����Ŀ��ʱ�ı���ʣ��ʱ�䣬��λ�룬��δ�л�Ĭ���? 0��

operate_launch_cmd_time:
���һ�β�����ȷ������ָ��ʱ�ı���ʣ��ʱ�䣬��λ��?, ��ʼֵΪ 0��


*/
typedef __packed struct
{
	uint8_t dart_launch_opening_status;
	uint8_t dart_attack_target;
	uint16_t target_change_time;
	uint16_t operate_launch_cmd_time;
} ext_dart_client_cmd_t;

/*
�������ݽ�����Ϣ��0x0301

data_cmd_id:
��ҪУ�鷢���ߵ� ID ��ȷ�ԣ������? 1 ���͸��� 5��������Ҫ
У���? 1

sender_ID:
��ҪУ������ߵ�? ID ��ȷ�ԣ����粻�ܷ��͵��жԻ����˵� ID

�������ݶ�
*/
// typedef __packed struct
//{
//  uint16_t data_cmd_id;
//  uint16_t sender_ID;
//  uint16_t receiver_ID;
// }ext_student_interactive_header_data_t;



/*
0x0301

*/

// typedef __packed struct
//{
//  uint8_t data[];
// } robot_interactive_data_t;

// 0x0301

typedef __packed struct
{
	uint8_t operate_tpye;
	uint8_t layer;
} ext_client_custom_graphic_delete_t;

// ͼ������
typedef __packed struct
{
	uint8_t graphic_name[3];
	uint32_t operate_tpye : 3;
	uint32_t graphic_tpye : 3;
	uint32_t layer : 4;
	uint32_t color : 4;
	uint32_t start_angle : 9;
	uint32_t end_angle : 9;
	uint32_t width : 10;
	uint32_t start_x : 11;
	uint32_t start_y : 11;
	uint32_t radius : 10;
	uint32_t end_x : 11;
	uint32_t end_y : 11;
} graphic_data_struct_t;

// 0x0301
typedef __packed struct
{
	graphic_data_struct_t grapic_data_struct;
} ext_client_custom_graphic_single_t;

// 0x0301
typedef __packed struct
{
	graphic_data_struct_t grapic_data_struct[2];
} ext_client_custom_graphic_double_t;

// 0x0301
typedef __packed struct
{
	graphic_data_struct_t grapic_data_struct[5];
} ext_client_custom_graphic_five_t;

// 0x0301
typedef __packed struct
{
	graphic_data_struct_t grapic_data_struct[7];
} ext_client_custom_graphic_seven_t;

typedef __packed struct
{
	graphic_data_struct_t grapic_data_struct;
	uint8_t data[30];
} ext_client_custom_character_t;

// 0x0302
typedef __packed struct
{
	uint8_t *data;
} robot_interactive_data_t;

// 0x0303

// typedef __packed struct
//{
// float target_position_x;
// float target_position_y;
// float target_position_z;
// uint8_t commd_keyboard;
// uint16_t target_robot_ID;
// } ext_robot_command_t;

typedef __packed struct
{
	uint16_t target_robot_ID;
	float target_position_x;
	float target_position_y;
} ext_client_map_command_t;

// 0x0304

typedef __packed struct
{
	int16_t mouse_x;
	int16_t mouse_y;
	int16_t mouse_z;
	int8_t left_button_down;
	int8_t right_button_down;
	uint16_t keyboard_value;
	uint16_t reserved;
} ext_robot_command_t;

// 0x301
typedef __packed struct
{
	uint16_t data_cmd_id;
	uint16_t sender_ID;
	uint16_t receiver_ID;
} ext_student_interactive_header_data_t;
// ��������
typedef __packed struct
{
	uint16_t data_cmd_id;
	uint16_t sender_ID;
	uint16_t receiver_ID;
	ext_client_custom_character_t character;
} ext_student_interactive_header_word_t;

// ����ͼ
typedef __packed struct
{
	uint16_t data_cmd_id;
	uint16_t sender_ID;
	uint16_t receiver_ID;
	ext_client_custom_graphic_seven_t graphic;
} ext_student_interactive_header_graph_t;

typedef struct
{
	Frame_Header_t Frame_Header;
	uint16_t cmd_id;

	ext_game_status_t ext_game_status;	   // 0x0001
	ext_game_result_t ext_game_result;	   // 0x0002
	ext_game_robot_HP_t ext_game_robot_HP; // 0x0003
	ext_dart_status_t ext_dart_status;	   // 0x0004

	ext_ICRA_buff_debuff_zone_and_lurk_status_t ext_ICRA_buff_debuff_zone_and_lurk_status; // 0x005
	ext_event_data_t ext_event_data;													   // 0x0101
	ext_supply_projectile_action_t ext_supply_projectile_action;						   // 0x0102
	ext_supply_projectile_booking_t ext_supply_projectile_booking;						   // 0x0103
	ext_referee_warning_t ext_referee_warning;											   // 0x0104
	ext_dart_remaining_time_t ext_dart_remaining_time;									   // 0x0105

	ext_game_robot_status_t ext_game_robot_status; //  0x201
	ext_power_heat_data_t ext_power_heat_data;	   //  0x0202
	ext_game_robot_pos_t ext_game_robot_pos;	   // 0x203
	ext_buff_t ext_buff;						   // 0x204

	aerial_robot_energy_t aerial_robot_energy;	 // 0x205
	ext_robot_hurt_t ext_robot_hurt;			 // 0x206
	ext_shoot_data_t ext_shoot_data;			 // 0x207
	ext_bullet_remaining_t ext_bullet_remaining; // 0x208
	ext_rfid_status_t ext_rfid_status;			 //  0x209
	ext_dart_client_cmd_t ext_dart_client_cmd;	 // 0x20A

	ext_student_interactive_header_data_t ext_student_interactive_header_data; // 0x301

} JudgementDataTypedef;

extern JudgementDataTypedef JudgeStructure;
extern uint8_t JudgeDataBuffer[JudgeBufferLength];

#endif
