#ifndef GIMBAL_TASK
#define GIMBAL_TASK

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "gimbal_behaviour.h"
#include "icm20602.h"


extern void gimbal_task(void *pvParameters);


#endif


