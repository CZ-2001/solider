/**
  ****************************(C) COPYRIGHT 2016 DJI****************************
  * @file       pid.c/h
  * @brief      pidʵ�ֺ�����������ʼ����PID���㺯����
  * @note
  * @history
  *  Version    Date            Author          Modification
  *  V1.0.0     Dec-26-2018     RM              1. ���
  *
  @verbatim
  ==============================================================================

  ==============================================================================
  @endverbatim
  ****************************(C) COPYRIGHT 2016 DJI****************************
  */
#ifndef PID_H
#define PID_H
#include "main.h"

enum PID_MODE
{
	PID_POSITION = 0,
	PID_DELTA,
	PID_DIFF
};

typedef struct
{
	uint8_t mode;
	// PID ������
	fp32 Kp;
	fp32 Ki;
	fp32 Kd;

	fp32 diff_max;
	fp32 diff_min;

	fp32 max_out;  //������
	fp32 max_iout; //���������
	fp32 fdb;
	fp32 set;
	fp32 out;
	fp32 Pout;
	fp32 Iout;
	fp32 Dout;
	fp32 Dbuf[3];  //΢���� 0���� 1��һ�� 2���ϴ�
	fp32 error[3]; //����� 0���� 1��һ�� 2���ϴ�
} PidTypeDef;

typedef struct
{
PidTypeDef inner;
PidTypeDef outer;

}PID_DOUBLE;




extern void PID_Init(PidTypeDef *pid, uint8_t mode, fp32*PID);
extern void PID_diff_init(PidTypeDef *pid, fp32 diff_max, fp32 diff_min);
extern fp32 PID_Calc(PidTypeDef *pid, fp32 ref, fp32 set);
fp32 PID_Calc_add_limit(PidTypeDef *pid, fp32 ref, fp32 set, float T);
extern void PID_clear(PidTypeDef *pid);
extern fp32 PID_CALCAL(PID_DOUBLE*PID,float angle_target,float angle_feedback,float speed_feedback,float inner_limit);

#endif
