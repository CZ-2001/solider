
#include "shoot_behaviour.h"

Shoot_Typedef Shoot_Structure;
Shoot_Typedef *Shoot_Str;
fp32 plate_angle[] =
	{KP_ANGLE_PLATE, KI_ANGLE_PLATE, KD_ANGLE_PLATE, MAX_IOUT_ANGLE_PLATE, MAX_OUT_ANGLE_PLATE};
fp32 plate_speed[] =
	{KP_SPEED_PLATE, KI_SPEED_PLATE, KD_SPEED_PLATE, MAX_IOUT_SPEED_PLATE, MAX_OUT_SPEED_PLATE};
fp32 shoot[] =
	{KP_SPEED_SHOOT, KI_SPEED_SHOOT, KD_SPEED_SHOOT, MAX_IOUT_SPEED_SHOOT, MAX_OUT_SPEED_SHOOT};

void shoot_conduct(Shoot_Typedef *Structure)
{

	if (Structure->ALL_Flag_Str.Plate_flag == CLOSE)
		Structure->Shoot_Aim_Limit.Plate_Aim = 0;

	Structure->Shoot_Send_Current.Fric_Current[FRIC_L] =
		PID_Calc(&Structure->Fric_PID,
				 Structure->Shoot_All_Motor[FRIC_L]->speed_rpm,
				 Structure->Shoot_Aim_Limit.Firc_Aim_Speed);

	Structure->Shoot_Send_Current.Fric_Current[FRIC_R] =
		PID_Calc(&Structure->Fric_PID,
				 Structure->Shoot_All_Motor[FRIC_R]->speed_rpm,
				 -Structure->Shoot_Aim_Limit.Firc_Aim_Speed);

	if (Structure->Stuck_Flag == 0)
		Structure->Shoot_Send_Current.Plate_Current =
			PID_Calc(&Structure->Mplate_PID.outer,
					 Structure->Shoot_All_Motor[PlATE]->speed_rpm,
					 Structure->Shoot_Aim_Limit.Plate_Aim);

	if (Structure->Stuck_Flag == 1)
		Structure->Shoot_Send_Current.Plate_Current = 0;

	if (Structure->Shoot_Send_Current.Plate_Current > 6000)
		Structure->Stuck++;

	if ((Structure->Shoot_Send_Current.Plate_Current < 6000) && (Structure->Stuck_Flag == 0))
		Structure->Stuck--;
	if (Structure->Stuck < 0)
		Structure->Stuck = 0;
	if (Structure->Stuck > 400)
	{
		Structure->Stuck_Flag = 1;
		Structure->Stuck = 400;
	}
	if ((Structure->Stuck_Flag == 1) && (Structure->RC->key.v & KEY_PRESSED_OFFSET_X))
	{
		Structure->Stuck -= 2;
		Structure->Shoot_Send_Current.Plate_Current =
			PID_Calc(&Structure->Mplate_PID.outer, Structure->Shoot_All_Motor[PlATE]->speed_rpm, -4000);

		if (Structure->Stuck < 120)
			Structure->Shoot_Send_Current.Plate_Current =
				PID_Calc(&Structure->Mplate_PID.outer, Structure->Shoot_All_Motor[PlATE]->speed_rpm, 0);
		if (Structure->Stuck < 10)
			Structure->Stuck_Flag = 0;
	}

	if (Structure->mode != Shoot_Zero)
		can_cmd_shoot(Structure->Shoot_Send_Current.Fric_Current[FRIC_L],
					  Structure->Shoot_Send_Current.Fric_Current[FRIC_R],
					  Structure->Shoot_Send_Current.Plate_Current, 0);
	else
		can_cmd_shoot(0, 0, 0, 0);
}

void key_judge_aim(Shoot_Typedef *Structure)
{

	if (Structure->ALL_Flag_Str.Firc_flag == OPEN)
	{
		laser_on();
		Structure->Shoot_Aim_Limit.Firc_Aim_Speed = Structure->Shoot_Aim_Limit.Firc_Limit_Speed;

		switch (Structure->ALL_Flag_Str.Fire_flag)
		{
		case Single:
		case Tri:
		{
			Structure->Shoot_Aim_Limit.Plate_Aim = PID_Calc_add_limit(&Structure->Mplate_PID.inner,
																	  Mplate.round_cnt,
																	  Structure->Shoot_Aim_Limit.Plate_Temp, 35);
			break;
		}
		case Dratle:
		{
			if (Structure->ALL_Flag_Str.Plate_flag == OPEN)
				Structure->Shoot_Aim_Limit.Plate_Aim = -Structure->Shoot_Aim_Limit.Plate_Limit;
			else
				Structure->Shoot_Aim_Limit.Plate_Aim = 0;
			break;
		}
		}
	}
	else if (Structure->ALL_Flag_Str.Firc_flag == CLOSE)
	{
		Structure->Shoot_Aim_Limit.Firc_Aim_Speed = 0;
		laser_off();
		Structure->ALL_Flag_Str.Plate_flag = CLOSE;
	}
}

/*
void switch_fire_mode(Shoot_Typedef *Structure)
{
	if (Structure->RC->key.v & KEY_PRESSED_OFFSET_Z)
		Structure->RecordStructure->Record_z = 1;
	if ((Structure->RecordStructure->Record_z == 1) && ((ALLRC.key.v & KEY_PRESSED_OFFSET_Z) == 0))
	{
		Structure->RecordStructure->Record_z = 0;
		switch (Structure->fire_mode)
		{
		case Single:
		{
			Structure->fire_mode = Tri;
			break;
		}
		case Tri:
		{
			Structure->fire_mode = Dratle;
			break;
		}
		case Dratle:
		{
			Structure->fire_mode = Single;
			break;
		}
		}
	}
}
*/

void switch_frictiongear_mode(Shoot_Typedef *Structure)
{
	if (Structure->RC->key.v & KEY_PRESSED_OFFSET_F)
		Structure->RecordStructure->Record_f = 1;
	if ((Structure->RecordStructure->Record_f == 1) && ((ALLRC.key.v & KEY_PRESSED_OFFSET_F) == 0))
	{
		Structure->RecordStructure->Record_f = 0;
		Structure->ALL_Flag_Str.Firc_flag = Structure->ALL_Flag_Str.Firc_flag == OPEN ? CLOSE : OPEN;
	}
}

void switch_mag_mode(Shoot_Typedef *Structure)
{
	if (Structure->RC->key.v & KEY_PRESSED_OFFSET_R)
	{
		Structure->ALL_Flag_Str.Plate_flag = CLOSE;
		Structure->ALL_Flag_Str.Mag_flag = Structure->ALL_Flag_Str.Mag_flag == OPEN ? CLOSE : OPEN;
		vTaskDelay(300);
	}
	TIM_SetCompare4(TIM5, (Structure->ALL_Flag_Str.Mag_flag == CLOSE ? 16 : 3));
}

void switch_shoot_by_gimbal(Shoot_Typedef *Structure)
{
	if (Gimbal_Str->Key_Mode == Gimbal_Vision)
	{
		Structure->key_mode = Key_Vision;
	}
	else if (Gimbal_Str->Key_Mode == Gimbal_Chassis_Follow_Gimbal)
	{
		Structure->key_mode = Key_Nomal;
	}
	else if (Gimbal_Str->Key_Mode == Gimbal_WindMill)
	{
		Structure->key_mode = Key_Wind;
	}
}

void key_ctrl_shoot(Shoot_Typedef *Structure)
{
	//	static u8 flag;
	switch (Structure->key_mode)
	{

	case Key_Nomal:
	case Key_Wind:
	{
		Structure->ALL_Flag_Str.Plate_flag = (Structure->RC->mouse.press_l == 1 ? OPEN : CLOSE);
		{
			//		if (Structure->fire_mode == Single)
			//		{
			//			Structure->plate_mode = Spin;
			//			if (Structure->RC->mouse.press_l == 1)
			//				flag = 1;
			//			if ((flag == 1) && (Structure->RC->mouse.press_l == 0))
			//			{
			//				Structure->Shoot_Aim_Limit.Plate_Temp -= 4.5f;

			//				Limit_Slove_Zero(&Structure->Shoot_Aim_Limit.Plate_Temp, 36, 0);
			//				flag = 0;
			//			}
			//		}
			//		else if (Structure->fire_mode == Tri)
			//		{
			//			Structure->plate_mode = Spin;
			//			if (Structure->RC->mouse.press_l == 1)
			//				flag = 1;
			//			if ((flag == 1) && (Structure->RC->mouse.press_l == 0))
			//			{
			//				Structure->Shoot_Aim_Limit.Plate_Temp -= 13.5f;
			//				Limit_Slove_Zero(&Structure->Shoot_Aim_Limit.Plate_Temp, 36, 0);
			//				flag = 0;
			//			}
			//		}
			//		else if (Structure->fire_mode == Dratle)
			//		{
			//			if (Structure->RC->mouse.press_l == 1)
			//				Structure->plate_mode = Spin;
			//			else
			//				Structure->plate_mode = Nspin;
			//		}
		}
		break;
	}
	case Key_Vision:
	{
		Structure->Shoot_Aim_Limit.Plate_Aim = 2000;
		if (Structure->ALL_Flag_Str.Firc_flag == OPEN)
			Structure->ALL_Flag_Str.Plate_flag = (Structure->RC->mouse.press_l == 1 ? OPEN : CLOSE);
		break;
	}
	}
}
void shoot_key_ctrl(Shoot_Typedef *Structure)
{
	if (Structure == NULL)
		return;

	// switch_fire_mode(Structure);
	// 赛中保持连射
	Structure->ALL_Flag_Str.Fire_flag = Dratle;
	
	switch_frictiongear_mode(Structure);
	switch_mag_mode(Structure);
	switch_shoot_by_gimbal(Structure);
	key_ctrl_shoot(Structure);
	key_judge_aim(Structure);
}

void rc_judge_aim(Shoot_Typedef *Structure)
{
	if (Structure->ALL_Flag_Str.Firc_flag == OPEN)
	{
		laser_on();
		Structure->Shoot_Aim_Limit.Firc_Aim_Speed =
			Structure->Shoot_Aim_Limit.Firc_Limit_Speed;
	}
	else if (Structure->ALL_Flag_Str.Firc_flag == CLOSE)
	{
		laser_off();
		Structure->Shoot_Aim_Limit.Firc_Aim_Speed = 0;
		Structure->ALL_Flag_Str.Plate_flag = CLOSE;
	}

	Structure->Shoot_Aim_Limit.Plate_Aim =
		PID_Calc_add_limit(&Structure->Mplate_PID.inner,
						   Structure->Shoot_All_Motor[2]->round_cnt,
						   Structure->Shoot_Aim_Limit.Plate_Temp, 35);
}

// RC ctrl to realize  switch
void shoot_rc_ctrl(Shoot_Typedef *Structure)
{
	if (Structure == NULL)
		return;
	static u8 flag;

	if (switch_is_up(Structure->RC->rc.s[0]))
		flag = 1;
	// Structure->ALL_Flag_Str.Firc_flag=OPEN;
	else if (switch_is_down(Structure->RC->rc.s[0]))
	{
		flag = 2;
		// Structure->ALL_Flag_Str.Plate_flag=OPEN;
		Structure->ALL_Flag_Str.Firc_flag = OPEN;
	}
	if ((flag == 1) && (switch_is_mid(Structure->RC->rc.s[0])))
	{
		if (Structure->ALL_Flag_Str.Firc_flag == OPEN)
		{
			Structure->ALL_Flag_Str.Plate_flag = CLOSE;
			Structure->ALL_Flag_Str.Firc_flag = CLOSE;
		}
		else if (Structure->ALL_Flag_Str.Firc_flag == CLOSE)
		{
			Structure->ALL_Flag_Str.Firc_flag = OPEN;
		}
		flag = 0;
	}
	else if ((flag == 2) && (switch_is_mid(Structure->RC->rc.s[0])))
	{
		Structure->Shoot_Aim_Limit.Plate_Temp -= 4.5f;
		Limit_Slove_Zero(&Structure->Shoot_Aim_Limit.Plate_Temp, 36, 0);
		if (Structure->ALL_Flag_Str.Firc_flag == OPEN)
			Structure->ALL_Flag_Str.Plate_flag = OPEN;

		flag = 0;
	}
	rc_judge_aim(Structure);
}

void shoot_zreo(Shoot_Typedef *Structure)
{
	if (Structure == NULL)
		return;

	Structure->Shoot_Aim_Limit.Firc_Aim_Speed = 0;
	Structure->Shoot_Aim_Limit.Plate_Aim = 0;
	Structure->Shoot_Aim_Limit.Plate_Temp = Mplate.round_cnt;

	Structure->ALL_Flag_Str.Firc_flag = CLOSE;
	Structure->ALL_Flag_Str.Plate_flag = CLOSE;

	can_cmd_shoot(0, 0, 0, 0);
	return;
}

void shoot_judge(Shoot_Aim_Limit_ *Structure)
{
	if (Structure == NULL)
		return;
	switch (JudgeStructure.ext_game_robot_status.shooter_id1_17mm_speed_limit)
	{ 
	case 15:
		Structure->Firc_Limit_Speed = 4200;
		break;
	case 18:
		Structure->Firc_Limit_Speed = 5100;
		break;
	case 22:
		Structure->Firc_Limit_Speed = 5750;
		break;
	case 30:
		Structure->Firc_Limit_Speed = 7200;
		break;
	}


	
	//
	//Structure->Firc_Limit_Speed = 6000;
}

void shoot_mode_conduct(Shoot_Typedef *Structure)
{
	if (Structure == NULL)
		return;

	shoot_judge(&Structure->Shoot_Aim_Limit);

	if (Structure->mode == Shoot_Key_Ctrl)
		shoot_key_ctrl(Structure);
	else if (Structure->mode == Shoot_Rc)
		shoot_rc_ctrl(Structure);
	else if (Structure->mode == Shoot_Zero)
		shoot_zreo(Structure);
}

void shoot_mode_set(Shoot_Typedef *Structure)
{
	if (Structure == NULL)
		return;

	if (switch_is_down(Structure->RC->rc.s[1]))
		Structure->mode = Shoot_Zero;
	else if (switch_is_up(Structure->RC->rc.s[1]))
		Structure->mode = Shoot_Rc;
	else if (switch_is_mid(Structure->RC->rc.s[1]))
	{
		if (switch_is_mid(Structure->RC->rc.s[0]))
			Structure->mode = Shoot_Key_Ctrl;
		else
			Structure->mode = Shoot_Zero;
	}
}

void shoot_init(void)
{
	Shoot_Str = &Shoot_Structure;
	Shoot_Str->Shoot_All_Motor[PlATE] = &Mplate;
	Shoot_Str->Shoot_All_Motor[1] = &Mfricl[1];
	Shoot_Str->Shoot_All_Motor[0] = &Mfricl[0];
	Shoot_Str->RecordStructure = &Chassis_Str->RecordStructure;
	Shoot_Str->RC = &ALLRC;
	Shoot_Str->Shoot_Aim_Limit.Plate_Limit = 3500;
	PID_Init(&Shoot_Str->Fric_PID, PID_DIFF, shoot);
	PID_diff_init(&Shoot_Str->Fric_PID, 200, 20);
	PID_Init(&Shoot_Str->Mplate_PID.inner, PID_POSITION, plate_angle);
	PID_Init(&Shoot_Str->Mplate_PID.outer, PID_POSITION, plate_speed);
}
