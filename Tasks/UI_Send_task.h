#ifndef __UI_TASK
#define __UI_TASK
#include "main.h"
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "UI_behaviour.h"
#include "rc_task.h"
#include "Gimbal_behaviour.h"
#define _MAG_Word_Y 700
#define _STK_Word_Y 600
#define _FRIC_Word_Y 800
#define _ECD_Word_Y 900
#define _AUTO_CHANGE_GUN_Y 500
extern void UI_SendTask(void *pvParameters);
#endif
