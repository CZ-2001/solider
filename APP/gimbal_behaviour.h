#ifndef GIMBAL_BEHAVIOUR
#define GIMBAL_BEHAVIOUR
#include "can_rec.h"
#include "pid.h"
#include "rc_task.h"
#include "icm20602.h"
#include "vision_behaviour.h"
#include "chassis_behaviour.h"
#include "usart6_vision.h"



#define YAW_S_KP_F 14000.0f
#define YAW_S_KI_F 1.4f
#define YAW_S_KD_F  0.0f // 0.0015f
#define YAW_S_MAX_OUT_F 20000.0f
#define YAW_S_MAX_IOUT_F 2000.0f


//#define YAW_S_KP_F 18000.0f
//#define YAW_S_KI_F 390.00f
//#define YAW_S_KD_F  0.0f // 0.0015f
//#define YAW_S_MAX_OUT_F 20000.0f
//#define YAW_S_MAX_IOUT_F 7500.0f

#define YAW_A_KP_F 0.011f
#define YAW_A_KI_F 0.f
#define YAW_A_KD_F 0.01f
#define YAW_A_MAX_OUT_F 10.0f
#define YAW_A_MAX_IOUT_F 4.0f

#define PITCH_SPEED_KP 7000.0f
#define PITCH_SPEED_KI 6.2f
#define PITCH_SPEED_KD 0.0f
#define PITCH_SPEED_MAX_OUT 24000.0f
#define PITCH_SPEED_MAX_IOUT 7650.0f

#define PITCH_ANGLE_KP 0.01f
#define PITCH_ANGLE_KI 0.0f
#define PITCH_ANGLE_KD 0.00f
#define PITCH_ANGLE_MAX_OUT 5.0f
#define PITCH_ANGLE_MAX_IOUT 0.0f

//自瞄预测结构体
typedef enum 
{
	vision_stay,
	vision_right,
	vision_left
	
}Vision_Forcast;

typedef struct
{
	fp32 one;
	fp32 two;
	fp32 three;
	fp32 check;
}GRYO_ZREO;

typedef enum
{
	Gimbal_zero,
	Gimbal_cfg,         //RC控制模式 
	Gimbal_key_ctrl
} Gimbal_Mode;

typedef struct
{
	fp32 Pitch_Angle_Target;  
	fp32 Yaw_Angle_Target;
	fp32 Vision_Yaw_Angle;  
} Gimbal_AngleTypedef;

typedef enum
{
	Gimbal_Chassis_Follow_Gimbal,
	Gimbal_Vision,				  
	Gimbal_WindMill			 
} Gimbal_Key_Ctrl;


typedef enum
{
	Big,
	Small
} Vision_Size;

typedef enum
{
	Clockwise,	  
	Anticlockwise 
} WindMill;
typedef struct
{

	MotorMeasureTypeDef *Gimbal6020_YAW;
	MotorMeasureTypeDef *Gimbal6020_PITCH;

	RC_ctrl_t *RC;
	//云台模式
	Gimbal_Mode mode;

	//按键控制模式 
	Gimbal_Key_Ctrl Key_Mode;
	
	// 自瞄预测方向 
	Vision_Forcast vision_forcast;

	// 自瞄装甲板大小
	Vision_Size Size;
	

	// 能量机关的旋转方向
	WindMill WindDir;
	// 陀螺仪的值
	fp32 GYROSCOPE_YAW;

	// yaw和pitch的 PID
	PID_DOUBLE YAW;
	PID_DOUBLE PITCH;

	// 云台角度结构体
	Gimbal_AngleTypedef Gimbal_AngleStructure;

	// 角度增量
	fp32 Yaw_Angle_Grow;
	fp32 Pitch_Angle_Grow;

	// YAW  
	fp32 Yaw_Angle_Zero;
	
	//yaw轴和pitch轴的电流值 
	int16_t Yaw_current,Pitch_current;
	GRYO_ZREO   gryo_zreo;
	
} Gimbal_Typedef;
extern void conduct_gryo(Gimbal_Typedef *Structure);
extern Gimbal_Typedef Gimbal_TypedefStructure;
extern Gimbal_Typedef *Gimbal_Str;
extern float yaw_debug_pid_speed;
extern void gimbal_init(Gimbal_Typedef*Structure);
extern void gimbal_mode_set(Gimbal_Typedef *Structure);
extern void gimbal_mode_conduct(Gimbal_Typedef *Structure);
extern void gimbal_cmd(Gimbal_Typedef *Structure);
#endif
