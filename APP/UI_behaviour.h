#ifndef __UI_BEHAVIOUR_
#define __UI_BEHAVIOUR_

#include "main.h"
#include "judge_behaviour.h"
#include "gimbal_behaviour.h"
#include "chassis_behaviour.h"
#include "shoot_behaviour.h"
// ��Ļ�ֱ���1920x1080
#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080
#define _SHIFFTING_W 30

#define WORD_GIMBAL 850
#define WORD_CHASSIS 850 - 50
#define WORD_MAG 850 - 150
#define WORD_STUCK 600

#define CAP_NUM_Y 650

// ���Ƶ�ͼ������
typedef enum
{
	line = 0,
	rect = 1,
	circle = 2,
	ellipse = 3,
	arc = 4,
	fp = 5,
	in = 6,
	txt = 7
} Graphic_e;
typedef enum
{
	GIMBAL_WORD,	 // ��̨
	CHASSIS_WORD,	 // ����
	MAG_WORD,		 // ���ո�
	STUCK_SHOOT_WORD, // ����
	FIRC_WORD,
	CAP
					 //
} EXT_WORD;

typedef enum
{
	CAP_ = 0,
	GUIDE_LINE,
	CHASSIS_,
	GIMBAL_,
	MAG_,
	STUCK,
	FIRC,

} GRAPH;

typedef struct
{
	// CAP
	ext_student_interactive_header_graph_t graph[7];

	//  redraw   gimbal chassis mag stuck  5 ��
	ext_student_interactive_header_word_t word[5];
} ext_graph_char;

extern ext_graph_char ext_data;

extern void make_word(ext_student_interactive_header_word_t *custom_grapic_draw,
					  int type, int color, char *txt, uint8_t name[3], uint32_t sx, uint32_t sy);

extern void make_num(ext_student_interactive_header_graph_t *custom_grapic_draw,
					 char *name, int32_t exfloat1, int type, int color, uint32_t sx, uint32_t sy);

extern void make_line(ext_student_interactive_header_graph_t *custom_grapic_draw,
					  uint8_t stun, uint8_t name[3], uint8_t color, uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey);

extern void make_guideline_graph(ext_student_interactive_header_graph_t *custom_grapic_draw); // ����׼��

extern void make_cir(ext_student_interactive_header_graph_t *custom_grapic_draw,
					 int type, int color, uint16_t r, uint8_t r_aim_amor, uint16_t s_agl, uint16_t e_agl);

extern void referee_data_pack_handle(uint8_t sof, uint16_t cmd_id, uint8_t *p_data, uint16_t leng);
#endif
