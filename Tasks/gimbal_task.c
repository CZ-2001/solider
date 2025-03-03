#include "gimbal_task.h"
uint16_t gimbal_count;
void gimbal_task(void *pvParameters)
{
	
	gimbal_init(&Gimbal_TypedefStructure);
	while (1)
	{
		gimbal_mode_set(Gimbal_Str);

		gimbal_mode_conduct(Gimbal_Str);
		
		gimbal_cmd(Gimbal_Str);

		vTaskDelay(1);
	}
}
