#ifndef __RC_TASK_H
#define __RC_TASK_H
#include "main.h"
#include "stm32f4xx.h"
#include "user_lib.h"
#include "user_task.h"
#include "can_rec.h"
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"

#include "can_rec.h"
#include "remote_control.h"

extern RC_ctrl_t ALLRC;
void rc_choose_task(void *pvParameters);
#endif


