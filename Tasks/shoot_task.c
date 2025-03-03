#include "shoot_task.h"

void shoot_task(void *pvParameters)
{
	shoot_init();
	while (1)
	{
		shoot_mode_set(Shoot_Str);
		shoot_mode_conduct(Shoot_Str);
		shoot_conduct(Shoot_Str);
		vTaskDelay(1);
	}
}
