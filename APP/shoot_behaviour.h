#ifndef SHOOT_BEHAVIOUR
#define SHOOT_BEHAVIOUR
#include "can_rec.h"
#include "pid.h"
#include "chassis_behaviour.h"
#include "rc_task.h"
#include "laser.h"
#include "gimbal_behaviour.h"
#include "judge_behaviour.h"
#define PlATE 2


#define KP_ANGLE_PLATE         150.0f
#define KI_ANGLE_PLATE         0.00f
#define KD_ANGLE_PLATE         0.f
#define MAX_IOUT_ANGLE_PLATE   1000.0f
#define MAX_OUT_ANGLE_PLATE    3000.0f

#define KP_SPEED_PLATE 				 3.0f
#define KI_SPEED_PLATE         0.001f
#define KD_SPEED_PLATE         0.f
#define MAX_IOUT_SPEED_PLATE   3000.0f
#define MAX_OUT_SPEED_PLATE    8000.0f

#define KP_SPEED_SHOOT				 30.0f
#define KI_SPEED_SHOOT				 0.002f
#define KD_SPEED_SHOOT 				 2.0f
#define MAX_IOUT_SPEED_SHOOT   9000.0f
#define MAX_OUT_SPEED_SHOOT    15000.f

//按键控制模式
typedef enum
{
	Key_Wind,	// 打符
	Key_Vision, //  自瞄 
	Key_Nomal   
} Key_Mode;


// 射击模式
typedef enum
{
	Single,
	Tri,
	Dratle
} Fire_Mode;

//拨盘 摩擦�? 弹舱盖状�?
typedef enum
{
	CLOSE=0,
	OPEN
	
}ALL_mode;


// shoot  模式  无力   遥控器控�? 键盘控制 

typedef struct
{
	u8 Mag_flag;  //弹舱�?
	u8 Fire_flag;  //开�? 1 3 连射   比赛中直接设置为连射 
	u8 Plate_flag;// 拨盘
	u8 Firc_flag;//摩擦�?
	
}Module_typedef;



typedef enum
{
	Shoot_Zero,
	Shoot_Rc,
	Shoot_Key_Ctrl
} Shoot_Mode;

// 限制和目标的�? 
typedef struct
{
	fp32 Plate_Limit;   //拨盘 限制速度
	fp32 Plate_Aim;   // 角度环计算后得到的目�? 速度 
	fp32 Firc_Limit_Speed;
	fp32 Firc_Aim_Speed;
	fp32 Plate_Temp; //目标角度 
} Shoot_Aim_Limit_;

// 发送电流结构体 
typedef struct
{
	fp32 Plate_Current;

	fp32 Fric_Current[2];

} Shoot_Send_Current_;

typedef struct
{

	// 电机 结构�? 
	MotorMeasureTypeDef *Shoot_All_Motor[3];
	// 按键 记录 结构�? 
	RecordTypedef *RecordStructure;
	// 遥控器结构体 
	RC_ctrl_t *RC;

	// 目标和限�? 拨盘�? 摩擦�? 
	Shoot_Aim_Limit_ Shoot_Aim_Limit;

	// 模式  无力  遥控器控�? 键盘控制 
	Shoot_Mode mode;
	
	Module_typedef  ALL_Flag_Str;
	
	
	//按键控制模式�?  
	Key_Mode key_mode;
	
	//卡弹
	int16_t    Stuck;
	
	//卡弹标志�?
	u8  Stuck_Flag;
	
	//拨盘PID
	PID_DOUBLE Mplate_PID;
	//摩擦轮PID
	PidTypeDef Fric_PID;

	// 发送电流值结构体 
	Shoot_Send_Current_ Shoot_Send_Current;

} Shoot_Typedef;

extern void shoot_init(void);
extern void shoot_mode_set(Shoot_Typedef *Structure);
extern void shoot_mode_conduct(Shoot_Typedef *Structure);
extern void shoot_conduct(Shoot_Typedef *Structure);
extern Shoot_Typedef Shoot_Structure;
extern Shoot_Typedef *Shoot_Str;
#endif
