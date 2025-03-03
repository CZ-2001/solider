#ifndef USER_TASK_H
#define USER_TASK_H
#include "main.h"
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "led.h"
#include "laser.h"
#include "Remote_Control.h"
#include "imu901.h"
#include "can_rec.h"
#include "mpu6500driver.h"
#include "rc_task.h"
#include "usart6_vision.h"
#include "ins_task.h"
#include "can.h"
#include "pid.h"
#include "judge_behaviour.h"


#define POWER_LIMIT JudgeStructure.ext_game_robot_status.chassis_power_limit
#define POWER_NOW   JudgeStructure.ext_power_heat_data.chassis_power
#define POWER_MEM 	JudgeStructure.ext_power_heat_data.chassis_power_buffer  //µ×ÅÌ»º³å¹¦ÂÊ
#define REMAIN_HP  JudgeStructure.ext_game_robot_status.remain_HP


typedef struct
{
	float AngleYawRoundCnt;
	float LastAngleYaw;
	float TotalAngleYaw;
	
  float AnglePitch;
	float AngleYaw;
	float AngleRow;
	
	float SpeedPitch;
	float SpeedYaw;
	float SpeedRow;

} ImuFeedbackTypeDef;
void get_imu_feedback(void);
extern ImuFeedbackTypeDef ImuFeedbcakStu;

extern void UserTask(void *pvParameters);

extern 	short out_temp;
#endif
