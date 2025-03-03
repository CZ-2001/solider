#include "chassis_task.h"

u8 flag_caplow = 0;
u8 flag_cap_use = 0;
void chassis_task(void *pvParameters)
{

	chassis_init();
	while (1)
	{
		if (Chassis_Str->RC->key.v & KEY_PRESSED_OFFSET_CTRL)
			flag_cap_use = 1;
		else
			flag_cap_use = 0;
		if (pm01_od.v_out < 1800)
			flag_caplow = 1;
		else if (pm01_od.v_out > 2100)
			flag_caplow = 0;
		if (flag_cap_use)
		{
			switch (JudgeStructure.ext_game_robot_status.chassis_power_limit)
			{

			case 45:
				Chassis_Str->Chassis_Key_Press_Structure.Key_Press_Time_Max = 4000;
				GRYO_CHASSIS_SPEED = 4000 + 3000 * (pm01_od.v_out / 100 - 18) / 5;
				break;
			case 50:
				Chassis_Str->Chassis_Key_Press_Structure.Key_Press_Time_Max = 4000;
				GRYO_CHASSIS_SPEED = 5000 + 3000 * (pm01_od.v_out / 100 - 18) / 5;
				break;
			case 55:
				Chassis_Str->Chassis_Key_Press_Structure.Key_Press_Time_Max = 4000;
				GRYO_CHASSIS_SPEED = 5000 + 3000 * (pm01_od.v_out / 100 - 18) / 5;
				break;
			case 60:
				Chassis_Str->Chassis_Key_Press_Structure.Key_Press_Time_Max = 5000;
				GRYO_CHASSIS_SPEED = 5000 + 3000 * (pm01_od.v_out / 100 - 18) / 5;
				break;
			case 70:
				Chassis_Str->Chassis_Key_Press_Structure.Key_Press_Time_Max = 5500;
				GRYO_CHASSIS_SPEED = 6000 + 4000 * (pm01_od.v_out / 100 - 18) / 5;
				break;
			case 100:
				Chassis_Str->Chassis_Key_Press_Structure.Key_Press_Time_Max = 6000;
				break;
			case 120:
				Chassis_Str->Chassis_Key_Press_Structure.Key_Press_Time_Max = 6500;
				GRYO_CHASSIS_SPEED = 8000;
				break;
			case 200:
				Chassis_Str->Chassis_Key_Press_Structure.Key_Press_Time_Max = 7000;
			default:
				Chassis_Str->Chassis_Key_Press_Structure.Key_Press_Time_Max = 4000;
			}
		}
		else
		{
			switch (JudgeStructure.ext_game_robot_status.chassis_power_limit)
			{
			case 40:
				Chassis_Str->Chassis_Key_Press_Structure.Key_Press_Time_Max = 3000;
			//	GRYO_CHASSIS_SPEED = 2500;
				break;
			case 45:
				Chassis_Str->Chassis_Key_Press_Structure.Key_Press_Time_Max = 3500;
			//	GRYO_CHASSIS_SPEED = 3000;
				break;
			case 50:
				Chassis_Str->Chassis_Key_Press_Structure.Key_Press_Time_Max = 4000;
		//		GRYO_CHASSIS_SPEED = 3000;
				break;
			case 55:
				Chassis_Str->Chassis_Key_Press_Structure.Key_Press_Time_Max = 4000;
			//	GRYO_CHASSIS_SPEED = 3500;
				break;
			case 60:
				Chassis_Str->Chassis_Key_Press_Structure.Key_Press_Time_Max = 4200;
		//		GRYO_CHASSIS_SPEED = 3000 + 3000 * (pm01_od.v_out / 100 - 20) / 5;
				break;
			case 70:
				Chassis_Str->Chassis_Key_Press_Structure.Key_Press_Time_Max = 5000;
		//		GRYO_CHASSIS_SPEED = 3000 + 4000 * (pm01_od.v_out / 100 - 20) / 5;
				break;
			case 100:
				Chassis_Str->Chassis_Key_Press_Structure.Key_Press_Time_Max = 6000;
			//	GRYO_CHASSIS_SPEED = 5000;
				break;
			case 120:
				Chassis_Str->Chassis_Key_Press_Structure.Key_Press_Time_Max = 6000;
			//	GRYO_CHASSIS_SPEED = 5000;
				break;
			case 200:
				Chassis_Str->Chassis_Key_Press_Structure.Key_Press_Time_Max = 6000;
			default:
				Chassis_Str->Chassis_Key_Press_Structure.Key_Press_Time_Max = 4000;
			}
		}
		if (flag_caplow)
			GRYO_CHASSIS_SPEED = 2000;

		if (JudgeStructure.ext_power_heat_data.chassis_power_buffer < 30)
		{
			Chassis_Str->Chassis_Key_Press_Structure.Key_Press_Time_Max = 4500;
		}
		chassis_mode_set(Chassis_Str);
		chassis_mode_conduct(Chassis_Str);
		Chassis_Cmd(Chassis_Str);

		vTaskDelay(1);
	}
}
