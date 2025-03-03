#include "chassis_behaviour.h"

Chassis_Typedef Chassis_TypedefStructure;
Chassis_Typedef *Chassis_Str;
int GRYO_CHASSIS_SPEED =4000;
fp32 Chassis_Pid_[5] =
	{KP_CHASSIS, KI_CHASSIS, KD_CHASSIS, MAX_IOUT_CHASSIS, MAX_OUT_CHASSIS};
fp32 Chassis_Follow_Gimbal_Angle[5] =
	{KP_ANGLE, KI_ANGLE, KD_ANGLE, MAX_I_OUT_ANGLE, MAX_OUT_ANGLE};
fp32 Chassis_Follow_Gimbal_Speed[5] =
	{KP_SPEED, KI_SPEED, KD_SPEED, MAX_I_OUT_SPEED, MAX_OUT_SPEED};

void Chassis_Cmd(Chassis_Typedef *Structure)
{
	Structure->Aim_Send_Current_Structure.Speed[0] =
		-Structure->Aim_Send_Current_Structure.Speed_x + Structure->Aim_Send_Current_Structure.Speed_y + Structure->Aim_Send_Current_Structure.Speed_turn;

	Structure->Aim_Send_Current_Structure.Speed[1] =
		-Structure->Aim_Send_Current_Structure.Speed_x - Structure->Aim_Send_Current_Structure.Speed_y + Structure->Aim_Send_Current_Structure.Speed_turn;

	Structure->Aim_Send_Current_Structure.Speed[2] =
		Structure->Aim_Send_Current_Structure.Speed_x - Structure->Aim_Send_Current_Structure.Speed_y + Structure->Aim_Send_Current_Structure.Speed_turn;

	Structure->Aim_Send_Current_Structure.Speed[3] =
		Structure->Aim_Send_Current_Structure.Speed_x + Structure->Aim_Send_Current_Structure.Speed_y + Structure->Aim_Send_Current_Structure.Speed_turn;

	Structure->Aim_Send_Current_Structure.Current_Send[0] = PID_Calc(&Structure->Chassis_Pid, Structure->Chassis_Motor3508[0]->speed_rpm_filter, Structure->Aim_Send_Current_Structure.Speed[0]);
	Structure->Aim_Send_Current_Structure.Current_Send[1] = PID_Calc(&Structure->Chassis_Pid, Structure->Chassis_Motor3508[1]->speed_rpm_filter, Structure->Aim_Send_Current_Structure.Speed[1]);
	Structure->Aim_Send_Current_Structure.Current_Send[2] = PID_Calc(&Structure->Chassis_Pid, Structure->Chassis_Motor3508[2]->speed_rpm_filter, Structure->Aim_Send_Current_Structure.Speed[2]);
	Structure->Aim_Send_Current_Structure.Current_Send[3] = PID_Calc(&Structure->Chassis_Pid, Structure->Chassis_Motor3508[3]->speed_rpm_filter, Structure->Aim_Send_Current_Structure.Speed[3]);
	if (Structure->mode == Chassis_zero)
	{
		Structure->Aim_Send_Current_Structure.Speed[0] = 0;
		Structure->Aim_Send_Current_Structure.Speed[1] = 0;
		Structure->Aim_Send_Current_Structure.Speed[2] = 0;
		Structure->Aim_Send_Current_Structure.Speed[3] = 0;
		Structure->Aim_Send_Current_Structure.Current_Send[0] = 0;
		Structure->Aim_Send_Current_Structure.Current_Send[1] = 0;
		Structure->Aim_Send_Current_Structure.Current_Send[2] = 0;
		Structure->Aim_Send_Current_Structure.Current_Send[3] = 0;
		can_cmd_chassis(0, 0, 0, 0);
	}
	else
	{
		can_cmd_chassis(Structure->Aim_Send_Current_Structure.Current_Send[0],
						Structure->Aim_Send_Current_Structure.Current_Send[1],
						Structure->Aim_Send_Current_Structure.Current_Send[2],
						Structure->Aim_Send_Current_Structure.Current_Send[3]);
	}
	
//	if(pm01_od.v_in==0)
//	{
//	can_cmd_chassis(0,0,0,0);
//	}
	
//can_cmd_chassis(0, 0, 0, 0);
}

void get_chassis_rate(int16_t* x)
{
	
	if(JudgeStructure.ext_game_robot_status.robot_level==1)
	{
		
	}
	else if(JudgeStructure.ext_game_robot_status.robot_level==2)
	{
		
	}
	
}

void Chassis_Zero(Chassis_Typedef *Structure)
{
	Structure->Aim_Send_Current_Structure.Speed[0] = 0;

	Structure->Aim_Send_Current_Structure.Speed[1] = 0;

	Structure->Aim_Send_Current_Structure.Speed[2] = 0;

	Structure->Aim_Send_Current_Structure.Speed[3] = 0;

	Structure->Aim_Send_Current_Structure.Current_Send[0] = 0;
	Structure->Aim_Send_Current_Structure.Current_Send[1] = 0;
	Structure->Aim_Send_Current_Structure.Current_Send[2] = 0;
	Structure->Aim_Send_Current_Structure.Current_Send[3] = 0;

	can_cmd_chassis(0, 0, 0, 0);
	return;
}

// 梯形函数
void RC_CAL(fp32 *now, int16_t target)
{
	if (ABS(*now - target) > 20)
	{
		if (*now < target)
		{
			*now += 10;
		}
		else if (*now > target)
		{
			*now -= 15;
		}
	}
	else if (ABS(*now - target) <= 20)
	{
		*now = target;
	}
	else if(target==0)
	{
		*now=0;
	}
}


//void limit_max_chassis(fp32 *now,int16_t MAX)
//{
//	if(*now>0)
//	{
//		if(*now>MAX)*now=MAX;
//	}
//	else if(*now<0)
//	{
//		if(*now<-MAX)*now=-MAX;
//	}
//	
//	
//}

void Chassis_Rc_Ctrl(Chassis_Aim_Send_Current_Typedef *Structure)
{
	static int16_t Speed_X;
	static int16_t Speed_Y;
	static int16_t Speed_TURN;
	Speed_X = -ALLRC.rc.ch[0] * RC_SPEED_K * X_OF_Y_SPEED;
	Speed_Y = -ALLRC.rc.ch[1] * RC_SPEED_K;
	Speed_TURN = ALLRC.rc.ch[2] * RC_SPEED_K;
	
	
	RC_CAL(&Structure->Speed_x, Speed_X);
	RC_CAL(&Structure->Speed_y, Speed_Y);
	RC_CAL(&Structure->Speed_turn, Speed_TURN);
	
	
//	limit_max_chassis(&Structure->Speed_x, 4000);
//	limit_max_chassis(&Structure->Speed_y, 4000);
//	limit_max_chassis(&Structure->Speed_turn, 4000);
	
	//Limit_Max(&Structure->Speed_x,5800);
	/*
	Structure->Speed_x = -ALLRC.rc.ch[0] * RC_SPEED_K * X_OF_Y_SPEED;
	Structure->Speed_y = -ALLRC.rc.ch[1] * RC_SPEED_K;
	Structure->Speed_turn = ALLRC.rc.ch[2] * RC_SPEED_K;
	*/
}

void Chassis_Gyro_RC(Chassis_Aim_Send_Current_Typedef *Structure)
{
	Structure->theta = (fp32)Myaw.ecd;

	Structure->theta = (Structure->theta - Gimbal_Str->Yaw_Angle_Zero) * Motor_Ecd_to_Rad;

	Structure->sin_yaw = -arm_sin_f32(Structure->theta);
	Structure->cos_yaw = -arm_cos_f32(Structure->theta);

	Structure->Speed_x0 = Chassis_Str->RC->rc.ch[0] * GYRO_SPEED_RATIO;
	Structure->Speed_y0 = Chassis_Str->RC->rc.ch[1]  * GYRO_SPEED_RATIO;

	Structure->Speed_x =
		(-Structure->Speed_y0) * (Structure->sin_yaw) + (Structure->Speed_x0) * (Structure->cos_yaw);

	Structure->Speed_y =
		(+Structure->Speed_y0) * (Structure->cos_yaw) + (Structure->Speed_x0) * (Structure->sin_yaw);
RC_CAL(&Structure->Speed_turn ,GRYO_CHASSIS_SPEED);
	//Structure->Speed_turn = 2000;
}

void Chassis_Follow_Gimbal_RC(Chassis_Typedef *Structure)
{
	Structure->Aim_Send_Current_Structure.Speed_x = -Structure->RC->rc.ch[0] * RC_SPEED_K * X_OF_Y_SPEED;
	Structure->Aim_Send_Current_Structure.Speed_y = -Structure->RC->rc.ch[1] * RC_SPEED_K;

	Structure->Aim_Send_Current_Structure.Speed_turn =
		PID_CALCAL(&Structure->Chassis_Follow_Gimbal, Structure->Chassis_Follow_Yaw_Angle, Myaw.ecd,
				   ((Structure->Chassis_Motor3508[0]->speed_rpm_filter) + (Structure->Chassis_Motor3508[1]->speed_rpm_filter) +
					(Structure->Chassis_Motor3508[2]->speed_rpm_filter) + (Structure->Chassis_Motor3508[3]->speed_rpm_filter)) /
					   4.f,
				   8191);
}

void Chassis_Key_Zero(Chassis_Aim_Send_Current_Typedef *Structure)
{
	Structure->Speed_x = 0;
	Structure->Speed_y = 0;
	Structure->Speed_turn = 0;
	can_cmd_chassis(0, 0, 0, 0);
	return;
}

// Key press time calculation
void Key_Press_Time_Cal(KeyTime_TypeDef *Structure, u8 Grow_Time, fp32 Dowm_Time, uint16_t Max)
{
	// W
	if (Chassis_Str->RC->key.v & KEY_PRESSED_OFFSET_W)
		Structure->w += Grow_Time;
	else
	{
		if (Structure->w > Grow_Time)
			Structure->w = -Dowm_Time;
		else
			Structure->w = 0;
	}
	Limit_Max(&Structure->w, Max);
	// A
	if (Chassis_Str->RC->key.v & KEY_PRESSED_OFFSET_A)
		Structure->a += Grow_Time;
	else
	{
		if (Structure->a > Grow_Time)
			Structure->a = -Dowm_Time;
		else
			Structure->a = 0;
	}
	Limit_Max(&Structure->a, Max);
	// S
	if (Chassis_Str->RC->key.v & KEY_PRESSED_OFFSET_S)
		Structure->s += Grow_Time;
	else
	{
		if (Structure->s > Grow_Time)
			Structure->s = -Dowm_Time;
		else
			Structure->s = 0;
	}
	Limit_Max(&Structure->s, Max);
	// D
	if (Chassis_Str->RC->key.v & KEY_PRESSED_OFFSET_D)
		Structure->d += Grow_Time;
	else
	{
		if (Structure->d > Grow_Time)
			Structure->d = -Dowm_Time;
		else
			Structure->d = 0;
	}
	Limit_Max(&Structure->d, Max);
}

// Switch low disk mode in key mode
void Chassis_Key_Switch(Chassis_Typedef *Structure)
{

	if (ALLRC.key.v & KEY_PRESSED_OFFSET_G)
	{
		Structure->RecordStructure.Record_g = 1;
	}

	if ((Structure->RecordStructure.Record_g == 1) && ((ALLRC.key.v & KEY_PRESSED_OFFSET_G) == 0))
	{
		Structure->RecordStructure.Record_g = 0;

		Structure->Key_Ctrl = (Structure->Key_Ctrl == Chassis_Cfg_) ? Chassis_Gryo_ : Chassis_Cfg_;
	}
}

//   Shift  Ctrl
void Chassis_Ratio_Switch(Chassis_Typedef *Structure)
{

	if (Structure->RC->key.v & KEY_PRESSED_OFFSET_CTRL)
	{
		Structure->Chassis_Key_Press_Structure.Key_Ratio_Speed = 1.4f;
	}
	else if (Structure->RC->key.v & KEY_PRESSED_OFFSET_SHIFT)
	{
		Structure->Chassis_Key_Press_Structure.Key_Ratio_Speed = 0.8f;
	}
	else
	{
		Structure->Chassis_Key_Press_Structure.Key_Ratio_Speed = 1.f;
	}
}

/*void Chassis_Gyro_RC(Chassis_Aim_Send_Current_Typedef *Structure)
{
	Structure->theta = (fp32)Myaw.ecd;

	Structure->theta = (Structure->theta - Gimbal_Str->Yaw_Angle_Zero) * Motor_Ecd_to_Rad;

	Structure->sin_yaw = -arm_sin_f32(Structure->theta);
	Structure->cos_yaw = -arm_cos_f32(Structure->theta);

	Structure->Speed_x0 = Chassis_Str->RC->rc.ch[0] * GYRO_SPEED_RATIO;
	Structure->Speed_y0 = Chassis_Str->RC->rc.ch[1] * GYRO_SPEED_RATIO;

	Structure->Speed_x =
		(-Structure->Speed_y0) * (Structure->sin_yaw) + (Structure->Speed_x0) * (Structure->cos_yaw);

	Structure->Speed_y =
		(+Structure->Speed_y0) * (Structure->cos_yaw) + (Structure->Speed_x0) * (Structure->sin_yaw);
RC_CAL(&Structure->Speed_turn ,4000);
	//Structure->Speed_turn = 2000;
}*/



// Small gyro in key mode
void Chassis_Key_Gyro(Chassis_Typedef *Structure)
{
	Structure->Aim_Send_Current_Structure.theta = (fp32)Myaw.ecd;
	Structure->Aim_Send_Current_Structure.theta =
		(Structure->Aim_Send_Current_Structure.theta -  Gimbal_Str->Yaw_Angle_Zero) * Motor_Ecd_to_Rad;

	Structure->Aim_Send_Current_Structure.sin_yaw =
		-arm_sin_f32(Structure->Aim_Send_Current_Structure.theta);

	Structure->Aim_Send_Current_Structure.cos_yaw =
		arm_cos_f32(Structure->Aim_Send_Current_Structure.theta);

	Structure->Aim_Send_Current_Structure.Speed_x0 =
		-(Structure->KeyTime_Structure.w - Structure->KeyTime_Structure.s) ;

	Structure->Aim_Send_Current_Structure.Speed_y0 =
		(Structure->KeyTime_Structure.a - Structure->KeyTime_Structure.d) ;

	Structure->Aim_Send_Current_Structure.Speed_x =
		(-Structure->Aim_Send_Current_Structure.Speed_y0) * (Structure->Aim_Send_Current_Structure.sin_yaw)+
		(Structure->Aim_Send_Current_Structure.Speed_x0) * (Structure->Aim_Send_Current_Structure.cos_yaw);

	Structure->Aim_Send_Current_Structure.Speed_x =
		(Structure->Aim_Send_Current_Structure.Speed_y0) * (Structure->Aim_Send_Current_Structure.cos_yaw) +
		(Structure->Aim_Send_Current_Structure.Speed_x0) * (Structure->Aim_Send_Current_Structure.sin_yaw);

//	Structure->Aim_Send_Current_Structure.Speed_turn = 4000;
	RC_CAL(&Structure->Aim_Send_Current_Structure.Speed_turn,GRYO_CHASSIS_SPEED);
}

// Chassis and gimbal in key mode
void Chassis_Key_Follow_Gimbal(Chassis_Typedef *Structure)
{

	/*
	if (ALLRC.key.v & KEY_PRESSED_OFFSET_Q)
		Structure->Chassis_Angle_Zero = Gimbal_Str->Yaw_Angle_Zero - 1024;
	else if (ALLRC.key.v & KEY_PRESSED_OFFSET_E)
		Structure->Chassis_Angle_Zero = Gimbal_Str->Yaw_Angle_Zero + 1024;
	else
		Structure->Chassis_Angle_Zero = Gimbal_Str->Yaw_Angle_Zero;
	Limit_Slove_Zero(&Structure->Chassis_Angle_Zero, 8191, 0);
	Structure->Aim_Send_Current_Structure.theta = (fp32)Myaw.ecd;
	Structure->Aim_Send_Current_Structure.theta =
		(Structure->Aim_Send_Current_Structure.theta - Structure->Chassis_Angle_Zero) * Motor_Ecd_to_Rad;
	Structure->Aim_Send_Current_Structure.sin_yaw = arm_sin_f32(Structure->Aim_Send_Current_Structure.theta);
	Structure->Aim_Send_Current_Structure.cos_yaw = arm_cos_f32(Structure->Aim_Send_Current_Structure.theta);

	Structure->Aim_Send_Current_Structure.Speed_x0 =
		(Structure->KeyTime_Structure.a - Structure->KeyTime_Structure.d) * (Structure->Chassis_Key_Press_Structure.Key_Ratio_Speed);

	Structure->Aim_Send_Current_Structure.Speed_y0 =
		(Structure->KeyTime_Structure.w - Structure->KeyTime_Structure.s) * (Structure->Chassis_Key_Press_Structure.Key_Ratio_Speed);

	Structure->Aim_Send_Current_Structure.Speed_x = Structure->Aim_Send_Current_Structure.Speed_y0 * Structure->Aim_Send_Current_Structure.sin_yaw -
													Structure->Aim_Send_Current_Structure.Speed_x0 * Structure->Aim_Send_Current_Structure.cos_yaw;

	Structure->Aim_Send_Current_Structure.Speed_y = Structure->Aim_Send_Current_Structure.Speed_y0 * Structure->Aim_Send_Current_Structure.cos_yaw +
													Structure->Aim_Send_Current_Structure.Speed_x0 * Structure->Aim_Send_Current_Structure.sin_yaw;

	Structure->Aim_Send_Current_Structure.Speed_turn = PID_CALCAL(&Structure->Chassis_Follow_Gimbal, Structure->Chassis_Follow_Yaw_Angle, Myaw.ecd,
															  ((Structure->Chassis_Motor3508[0]->speed_rpm) + (Structure->Chassis_Motor3508[1]->speed_rpm) + (Structure->Chassis_Motor3508[2]->speed_rpm) + (Structure->Chassis_Motor3508[3]->speed_rpm)) / 4.f, 8191);
	*/
	//Structure->Chassis_Angle_Zero = Gimbal_Str->Yaw_Angle_Zero;
	//Limit_Slove_Zero(&Structure->Chassis_Angle_Zero, 8191, 0);

	Structure->Aim_Send_Current_Structure.Speed_x =
		(Structure->KeyTime_Structure.a - Structure->KeyTime_Structure.d) * (Structure->Chassis_Key_Press_Structure.Key_Ratio_Speed);
	Structure->Aim_Send_Current_Structure.Speed_y =
		-(Structure->KeyTime_Structure.w - Structure->KeyTime_Structure.s) * (Structure->Chassis_Key_Press_Structure.Key_Ratio_Speed);
		
		
	Structure->Aim_Send_Current_Structure.Speed_turn=PID_CALCAL(&Structure->Chassis_Follow_Gimbal, Structure->Chassis_Follow_Yaw_Angle, Myaw.ecd,
			   ((Structure->Chassis_Motor3508[0]->speed_rpm_filter) + (Structure->Chassis_Motor3508[1]->speed_rpm_filter) +
				(Structure->Chassis_Motor3508[2]->speed_rpm_filter) + (Structure->Chassis_Motor3508[3]->speed_rpm_filter)) /
				   4.f,
			   8191);
}

// Key control mode
void Chassis_Key_Ctrl(Chassis_Typedef *Structure)
{
	static u8 last_mode;
	static u8 use_single = 1;
	static u8 switch_flag;
	if (use_single)
	{
	
		Structure->Key_Ctrl = Chassis_Cfg_;
		use_single = 0; 
	}
	// switch follow and gryo
	Chassis_Key_Switch(Structure);

	// Motion scaling factor switching
	Chassis_Ratio_Switch(Structure);

	// Key press time calculation
	Key_Press_Time_Cal(&Structure->KeyTime_Structure,
					   Structure->Chassis_Key_Press_Structure.Key_Press_Grow_Time,
					   Structure->Chassis_Key_Press_Structure.Key_unPress_Time,
					   (Structure->Chassis_Key_Press_Structure.Key_Press_Time_Max) * (Structure->Chassis_Key_Press_Structure.Key_Ratio_Speed));

	// hit energy machine mode  and   force   chassis zero
	if (Gimbal_Str->Key_Mode == Gimbal_WindMill)
	{
		last_mode = Structure->Key_Ctrl; //recode last mode
		Structure->Key_Ctrl = Chassis_Zero_;
		switch_flag=1;
	}
	else if(switch_flag==1)
	{
		// return last mode 
		Structure->Key_Ctrl = (last_mode == Chassis_Gryo_) ? Chassis_Gryo_ : Chassis_Cfg_;
		switch_flag=0;
	}
	
	if (Structure->Key_Ctrl == Chassis_Gryo_)
		Chassis_Key_Gyro(Structure);

	else if (Structure->Key_Ctrl == Chassis_Zero_)
		Chassis_Key_Zero(&Structure->Aim_Send_Current_Structure);

	else if (Structure->Key_Ctrl == Chassis_Cfg_)
		Chassis_Key_Follow_Gimbal(Structure);
}

// chassis mode conduct
void chassis_mode_conduct(Chassis_Typedef *Structure)
{
	if (Structure == NULL)
	{
		return;
	}

	if (Structure->mode == Chassis_zero)
	{
		Chassis_Zero(Structure);
		return;
	}
	else if (Structure->mode == Chassis_rc_ctrl)
	{
		Chassis_Rc_Ctrl(&Structure->Aim_Send_Current_Structure);
	}
	else if (Structure->mode == Chassis_gyro)
	{
		Chassis_Gyro_RC(&Structure->Aim_Send_Current_Structure);
	}
	else if (Structure->mode == Chassis_key_ctrl)
	{
		Chassis_Key_Ctrl(Structure);
	}
	else if (Structure->mode == Chassis_cfg)
	{
		Chassis_Follow_Gimbal_RC(Structure);
	}
}

void chassis_mode_behaviour_set(Chassis_Typedef *Structure)
{
	if (Structure == NULL)
	{
		return;
	}

	if (switch_is_down(Structure->RC->rc.s[1]))
	{
		if (switch_is_down(Structure->RC->rc.s[0]))
			Structure->mode = Chassis_zero;
		else if (switch_is_mid(Structure->RC->rc.s[0]))
			Structure->mode = Chassis_zero;
		else if (switch_is_up(Structure->RC->rc.s[0]))
			Structure->mode = Chassis_cfg;
	}
	else if (switch_is_mid(Structure->RC->rc.s[1]))
	{
		if (switch_is_down(Structure->RC->rc.s[0]))
			Structure->mode = Chassis_gyro;
		else if (switch_is_mid(Structure->RC->rc.s[0]))
			Structure->mode = Chassis_key_ctrl;
		else if (switch_is_up(Structure->RC->rc.s[0]))
			Structure->mode = Chassis_rc_ctrl;
	}
	else if (switch_is_up(Structure->RC->rc.s[1]))
	{
		Structure->mode = Chassis_zero;
	}
}

void chassis_mode_set(Chassis_Typedef *Structure)
{
	if (Structure == NULL)
	{
		return;
	}
	chassis_mode_behaviour_set(Structure);
}

void chassis_init(void)
{
	Chassis_Str = &Chassis_TypedefStructure;

	Chassis_Str->RC = &ALLRC;

	Chassis_Str->Chassis_Key_Press_Structure.Key_Press_Grow_Time = 4;
	Chassis_Str->Chassis_Key_Press_Structure.Key_Press_Time_Max = 4500;
	Chassis_Str->Chassis_Key_Press_Structure.Key_Ratio_Speed = 1.0;   
	Chassis_Str->Chassis_Key_Press_Structure.Key_unPress_Time = 12;

	for (int i = 0; i < 4; i++)
	{
		Chassis_TypedefStructure.Chassis_Motor3508[i] = &Mchassis[i];
	}

	PID_Init(&Chassis_TypedefStructure.Chassis_Pid, PID_POSITION, Chassis_Pid_);

	PID_Init(&Chassis_TypedefStructure.Chassis_Follow_Gimbal.inner, PID_POSITION, Chassis_Follow_Gimbal_Angle);
	PID_Init(&Chassis_TypedefStructure.Chassis_Follow_Gimbal.outer, PID_POSITION, Chassis_Follow_Gimbal_Speed);
}
