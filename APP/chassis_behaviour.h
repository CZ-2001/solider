#ifndef CHASSIS_BEHAVIOUR
#define CHASSIS_BEHAVIOUR
#include "can_rec.h"
#include "rc_task.h"
#include "pid.h"
#include "remote_control.h"
#include "gimbal_behaviour.h"
#include "function.h"
#include "arm_math.h"
#include "judge_behaviour.h"
#define KP_ANGLE 5.0f
#define KI_ANGLE 0.0f
#define KD_ANGLE 0.005f
#define MAX_I_OUT_ANGLE 2000.f
#define MAX_OUT_ANGLE 8000.f

#define KP_SPEED 3.5f
#define KI_SPEED 0.f
#define KD_SPEED 0.f
#define MAX_I_OUT_SPEED 4000.f
#define MAX_OUT_SPEED 7500.f
 

//#define KP_CHASSIS 5.4f
//#define KI_CHASSIS 0.05f
//#define KD_CHASSIS 0.00f
//#define MAX_OUT_CHASSIS 9000.0f
//#define MAX_IOUT_CHASSIS 1500.0f
 
#define KP_CHASSIS         5.4
#define KI_CHASSIS         0.0051
#define KD_CHASSIS         0.00115
#define MAX_OUT_CHASSIS    7500.0f
#define MAX_IOUT_CHASSIS   2000.0f



#define RC_SPEED_K   6.8f
#define KEY_SPEED_K  1.f
#define X_OF_Y_SPEED 0.8f
#define GYRO_SPEED_RATIO 2.f



#define Motor_Ecd_to_Rad 0.000766990394f // 2*  PI  /8192

typedef enum
{
	Chassis_zero,
	Chassis_rc_ctrl,
	Chassis_key_ctrl,
	Chassis_gyro,
	Chassis_cfg,
} Chassis_mode;

typedef enum
{
	Chassis_Zero_,
	Chassis_Gryo_,
	Chassis_Cfg_
}Chassis_Key_Ctrl_;



typedef struct
{
	uint16_t w;
	uint16_t a;
	uint16_t s;
	uint16_t d;

	uint16_t q;
	uint16_t e;
	uint16_t f;
	uint16_t shift;
	uint16_t ctrl;
	uint16_t z;
	uint16_t x;
	uint16_t c;
	uint16_t v;
	uint16_t b;
} KeyTime_TypeDef;

typedef struct
{
	u8 Record_w;
	u8 Record_a;
	u8 Record_s;
	u8 Record_d;
	u8 Record_r;
	u8 Record_q;
	u8 Record_e;
	u8 Record_f;
	u8 Record_shift;
	u8 Record_ctrl;
	u8 Record_g;
	u8 Record_z;
	u8 Record_x;
	u8 Record_c;
	u8 Record_v;
	u8 Record_b;
} RecordTypedef;


typedef struct
{
	fp32 Speed[4];		 
	fp32 Current_Send[4];

	fp32 Speed_x;
	fp32 Speed_y;
	fp32 Speed_turn;

	fp32 Speed_x0;
	fp32 Speed_y0;
	fp32 Speed_turn0;

	fp64 sin_yaw;
	fp64 cos_yaw;
	fp64 theta;
} Chassis_Aim_Send_Current_Typedef;

typedef struct
{
	u8 Key_Press_Grow_Time;		 
	uint16_t Key_Press_Time_Max; 
	fp32 Key_Ratio_Speed;		
	fp32 Key_unPress_Time;	
} Chassis_Key_Press_Typedef;

typedef struct
{
	RC_ctrl_t *RC;

	MotorMeasureTypeDef *Chassis_Motor3508[4];

	Chassis_Key_Ctrl_  Key_Ctrl;
	Chassis_mode mode;

	fp32 Chassis_Angle_Zero;

	RecordTypedef RecordStructure;

	fp32 Chassis_Follow_Yaw_Angle;

	PidTypeDef Chassis_Pid;

	PID_DOUBLE Chassis_Follow_Gimbal;

	Chassis_Aim_Send_Current_Typedef Aim_Send_Current_Structure;

	KeyTime_TypeDef KeyTime_Structure;

	Chassis_Key_Press_Typedef Chassis_Key_Press_Structure;
	
	int16_t Chassis_Max_Sprm;

} Chassis_Typedef;
extern void chassis_init(void);
extern void chassis_mode_set(Chassis_Typedef *Structure);

extern void chassis_mode_conduct(Chassis_Typedef *Structure);

extern void Chassis_Cmd(Chassis_Typedef *Structure);

extern Chassis_Typedef Chassis_TypedefStructure;
extern Chassis_Typedef *Chassis_Str;
extern int GRYO_CHASSIS_SPEED ;
#endif
