#include "user_task.h"

#define user_is_error() toe_is_error(errorListLength)

uint16_t user_count;  // 计数
uint16_t reset_count; // 重启

void UserTask(void *pvParameters)
{
	laser_configuration();
	while (1)
	{
		if (user_count % 101 == 0)
		{
			pm01_power_set(JudgeStructure.ext_game_robot_status.chassis_power_limit * 100 + JudgeStructure.ext_power_heat_data.chassis_power_buffer / 10, 0);
			pm01_voltage_set(2400, 0);
			vTaskDelay(2);
			pm01_current_set(JudgeStructure.ext_game_robot_status.chassis_power_limit * 100 / 24, 1);
			vTaskDelay(2);
		}
		if (user_count % 100 == 0)
			vision_send();
		if (user_count % 151 == 0) // 功率控制板输出
		{
			pm01_read(pm01inID);
			if (REMAIN_HP == 0)
			{
				pm01_cmd_send(1, 1);
			}
			else if (REMAIN_HP != 0)
			{
				pm01_cmd_send(2, 0);
			}
		}
		/*
		if (user_count % 201 == 0) // 闪灯
		{
			static u8 led_f = 0;
			led_f = !led_f;
			if (led_f)
			{
				led_green_on();
			}
			else
			{
				led_green_off();
			}
		}
		*/
		if (user_count % 301 == 0)
			pm01_read(pm01outID);
		if (user_count % 601 == 0)
			pm01_read(pm01errorID);
		if (user_count % 901 == 0)
			pm01_read(pm01inID);

		//		if(pm01_od.v_out<1300)
		//		{
		//			pm01_cmd_send(1, 0);
		//		}
		//	 if(pm01_od.v_out<1600)
		//		{
		//
		//		pm01_current_set(JudgeStructure.ext_game_robot_status.chassis_power_limit * 100 / 24-20, 1);
		//		}
		//		if(pm01_od.v_in<1200)
		//		{
		//			pm01_cmd_send(1, 0);
		//		}else
		//		{
		//				pm01_cmd_send(2, 0);
		//		}

		if (pm01_od.v_in < 1200)
		{
			pm01_cmd_send(1, 0);
		}
		else if (pm01_od.v_in > 16)
		{
			pm01_cmd_send(2, 0);
		}

		if (((ALLRC.key.v & KEY_PRESSED_OFFSET_CTRL) != 0) && ((ALLRC.key.v & KEY_PRESSED_OFFSET_B) != 0))
		{
			reset_count++;
			if (reset_count > 999)
			{
				__set_FAULTMASK(1); // 关闭总中断
				NVIC_SystemReset(); // 请求单片机重启
				reset_count = 0;
			}
		}
		if (user_count > 999)
			user_count = 0;
		user_count++;
		vTaskDelay(1);
	}
}
