#include "Start_Task.h"
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "Calibrate_Task.h"
#include "user_Task.h"
#include "INS_Task.h"
#include "rc_task.h"
#include "vofa_task.h"
#include "chassis_task.h"
#include "gimbal_task.h"
#include "icm20602_task.h"
#include "shoot_task.h"
#include "UI_Send_task.h"
#define START_TASK_PRIO 1
#define START_STK_SIZE 512
static TaskHandle_t StartTask_Handler;

#define UI_TASK_PRIO 5
#define UI_STK_SIZE 1024
static TaskHandle_t UITask_Handler;

#define VOFA_TASK_PRIO 10
#define VOFA_STK_SIZE 256
static TaskHandle_t Vofa_Task_Handler;

#define ICM20602_TASK_PRIO 20
#define ICM20602_STK_SIZE 512
static TaskHandle_t Icm20602Task_Handler;

#define INS_TASK_PRIO 19
#define INS_STK_SIZE 256
static TaskHandle_t INS_Task_Handler;

#define CHASSIC_TASK_PRIO 16
#define CHASSIC_STK_SIZE 256
static TaskHandle_t CHASSIC_Task_Handler;

#define GIMBAL_TASK_PRIO 17
#define GIMBAL_STK_SIZE 512
TaskHandle_t GIMBAL_Task_Handler;

#define RC_TASK_PRIO 18
#define RC_STK_SIZE 512
static TaskHandle_t RCTask_Handler;

#define Calibrate_Task_PRIO 19
#define Calibrate_Task_SIZE 512
static TaskHandle_t Calibrate_Task_Handler;

#define User_TASK_PRIO 19
#define User_STK_SIZE 256
static TaskHandle_t UserTask_Handler;

#define SHOOT_TASK_PRIO 15
#define SHOOT_STK_SIZE 512
static TaskHandle_t SHOOT_Task_Handler;

#define GIMBAL_TASK
#define RC_TASK
#define VOFA_TASK
#define INS_CAL_TASK
#define CHASSIC_TASK
#define ICM20602Task
#define SHOOTTASK
#define UISendTask
void start_task(void *pvParameters)
{
  taskENTER_CRITICAL();

#ifdef UISendTask
  xTaskCreate((TaskFunction_t)UI_SendTask,
              (const char *)"UI_SendTask",
              (uint16_t)UI_STK_SIZE,
              (void *)NULL,
              (UBaseType_t)UI_TASK_PRIO,
              (TaskHandle_t *)&UITask_Handler);
#endif

#ifdef INS_CAL_TASK
  xTaskCreate((TaskFunction_t)INSTask,
              (const char *)"INSTask",
              (uint16_t)INS_STK_SIZE,
              (void *)NULL,
              (UBaseType_t)INS_TASK_PRIO,
              (TaskHandle_t *)&INS_Task_Handler);

#endif

#ifdef CHASSIC_TASK
  xTaskCreate((TaskFunction_t)chassis_task,
              (const char *)"chassis_task",
              (uint16_t)CHASSIC_STK_SIZE,
              (void *)NULL,
              (UBaseType_t)CHASSIC_TASK_PRIO,
              (TaskHandle_t *)&CHASSIC_Task_Handler);

#endif

#ifdef GIMBAL_TASK
  xTaskCreate((TaskFunction_t)gimbal_task,
              (const char *)"gimbal_task",
              (uint16_t)GIMBAL_STK_SIZE,
              (void *)NULL,
              (UBaseType_t)GIMBAL_TASK_PRIO,
              (TaskHandle_t *)&GIMBAL_Task_Handler);
#endif

#ifdef RC_TASK
  xTaskCreate((TaskFunction_t)rc_choose_task,
              (const char *)"rc_choose_task",
              (uint16_t)RC_STK_SIZE,
              (void *)NULL,
              (UBaseType_t)RC_TASK_PRIO,
              (TaskHandle_t *)&RCTask_Handler);
#endif

#ifdef VOFA_TASK
  xTaskCreate((TaskFunction_t)VofaTask,
              (const char *)"VofaTask",
              (uint16_t)VOFA_STK_SIZE,
              (void *)NULL,
              (UBaseType_t)VOFA_TASK_PRIO,
              (TaskHandle_t *)&Vofa_Task_Handler);
#endif

#ifdef ICM20602Task
  xTaskCreate((TaskFunction_t)icm20602_task,
              (const char *)"icm20602_task",
              (uint16_t)ICM20602_STK_SIZE,
              (void *)NULL,
              (UBaseType_t)ICM20602_TASK_PRIO,
              (TaskHandle_t *)&Icm20602Task_Handler);
#endif
#ifdef SHOOTTASK
  xTaskCreate((TaskFunction_t)shoot_task,
              (const char *)"SHOOTTask",
              (uint16_t)SHOOT_STK_SIZE,
              (void *)NULL,
              (UBaseType_t)SHOOT_TASK_PRIO,
              (TaskHandle_t *)&SHOOT_Task_Handler);
#endif

  xTaskCreate((TaskFunction_t)calibrate_task,
              (const char *)"calibrate_task",
              (uint16_t)Calibrate_Task_SIZE,
              (void *)NULL,
              (UBaseType_t)Calibrate_Task_PRIO,
              (TaskHandle_t *)&Calibrate_Task_Handler);

  xTaskCreate((TaskFunction_t)UserTask,
              (const char *)"UserTask",
              (uint16_t)User_STK_SIZE,
              (void *)NULL,
              (UBaseType_t)User_TASK_PRIO,
              (TaskHandle_t *)&UserTask_Handler);

  vTaskDelete(StartTask_Handler); 
  taskEXIT_CRITICAL();            
}

void startTast(void)
{
  xTaskCreate((TaskFunction_t)start_task,          
              (const char *)"start_task",          
              (uint16_t)START_STK_SIZE,            
              (void *)NULL,                        
              (UBaseType_t)START_TASK_PRIO,        // �������ȼ�
              (TaskHandle_t *)&StartTask_Handler); // ������
}
