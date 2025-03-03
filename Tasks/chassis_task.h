#ifndef CHASSIC_TASK
#define CHASSIC_TASK
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "chassis_behaviour.h"
#include "rc_task.h"
#include "judge_behaviour.h"

extern void chassis_task(void *pvParameters);

#endif


