#include "gimbal_behaviour.h"

Gimbal_Typedef Gimbal_TypedefStructure;
Gimbal_Typedef *Gimbal_Str;
RecordTypedef *RecordStructure;


float yaw_debug_pid_speed;

fp32 YAW_SPEED[5] = {YAW_S_KP_F, YAW_S_KI_F, YAW_S_KD_F, YAW_S_MAX_IOUT_F, YAW_S_MAX_OUT_F};
fp32 YAW_ANGLE[5] = {YAW_A_KP_F, YAW_A_KI_F, YAW_A_KD_F, YAW_A_MAX_IOUT_F, YAW_A_MAX_OUT_F};

fp32 PITCH_SPEED[5] = {PITCH_SPEED_KP, PITCH_SPEED_KI, PITCH_SPEED_KD, PITCH_SPEED_MAX_IOUT, PITCH_SPEED_MAX_OUT};
fp32 PITCH_ANGLE[5] = {PITCH_ANGLE_KP, PITCH_ANGLE_KI, PITCH_ANGLE_KD, PITCH_ANGLE_MAX_IOUT, PITCH_ANGLE_MAX_OUT};

int16_t aaa=0;
void key_calc(Gimbal_Typedef *Structure)
{
	switch (Structure->Key_Mode)
	{
	case Gimbal_Vision:
	case Gimbal_WindMill:
	{

		Structure->Yaw_current = PID_CALCAL(&Structure->YAW, Structure->Gimbal_AngleStructure.Vision_Yaw_Angle, Structure->GYROSCOPE_YAW, icm_data.gyro_z, 8191);
		break;
	}
	case Gimbal_Chassis_Follow_Gimbal:
	{
		Structure->Yaw_current = PID_CALCAL(&Structure->YAW, Structure->Gimbal_AngleStructure.Yaw_Angle_Target, Structure->GYROSCOPE_YAW, icm_data.gyro_z, 8191);
		break;
	}
	}
}

void gimbal_cmd(Gimbal_Typedef *Structure)
{
	//can_cmd_gimbal(0, 0, 0, 0);
	

		Limit_Max_Min(&Structure->Gimbal_AngleStructure.Pitch_Angle_Target, 3200, 2200);
		Limit_Slove_Zero(&Structure->Gimbal_AngleStructure.Yaw_Angle_Target, 8191, 0);
		Structure->Pitch_current = PID_CALCAL(&Structure->PITCH, Structure->Gimbal_AngleStructure.Pitch_Angle_Target, Structure->Gimbal6020_PITCH->ecd, icm_data.gyro_x, 8191);
		switch (Structure->mode)
		{
			case Gimbal_zero:
			{
				can_cmd_gimbal(0, 0, 0, 0);
				break;
			}
			case Gimbal_cfg:
			{
				Structure->Yaw_current = PID_CALCAL(&Structure->YAW,
													Structure->Gimbal_AngleStructure.Yaw_Angle_Target,
													Structure->GYROSCOPE_YAW, icm_data.gyro_z, 8191);
			//Structure->Yaw_current=PID_Calc(&Structure->YAW.outer,icm_data.gyro_z,yaw_debug_pid_speed);
				break;
			}
			case Gimbal_key_ctrl:
			{
				key_calc(Structure);
				break;
			}
		}
		if (Structure->mode == Gimbal_zero)
		{
			can_cmd_gimbal(0, 0, 0, 0);
		}
		else 
		  can_cmd_gimbal(Structure->Yaw_current, Structure->Pitch_current, 0, 0);
}

void gimbal_chassis_follow_gimbal(Gimbal_Typedef *Structure)
{
	if (Structure->RC->rc.ch[3] > 150)
		Structure->Gimbal_AngleStructure.Pitch_Angle_Target += Structure->Pitch_Angle_Grow;
	else if (Structure->RC->rc.ch[3] < -150)
		Structure->Gimbal_AngleStructure.Pitch_Angle_Target -= Structure->Pitch_Angle_Grow;

	if (Structure->RC->rc.ch[2] > 150)
		Structure->Gimbal_AngleStructure.Yaw_Angle_Target -= Structure->Yaw_Angle_Grow;
	else if (Structure->RC->rc.ch[2] < -150)
		Structure->Gimbal_AngleStructure.Yaw_Angle_Target += Structure->Yaw_Angle_Grow;
	
//yaw_debug_pid_speed=Structure->RC->rc.ch[1]/66.0;
	
	
	
}

void gimbal_zero(Gimbal_Typedef *Structure)
{
	Structure->Yaw_Angle_Zero = Structure->Gimbal6020_YAW->ecd;
	Structure->Gimbal_AngleStructure.Vision_Yaw_Angle = Structure->GYROSCOPE_YAW;
	Chassis_Str->Chassis_Follow_Yaw_Angle = Structure->Gimbal6020_YAW->ecd;
	Structure->Gimbal_AngleStructure.Yaw_Angle_Target = Structure->GYROSCOPE_YAW;
	can_cmd_gimbal(0, 0, 0, 0);
	return;
}

void Switch_Vision_Mode(Gimbal_Typedef *Structure)
{
	static u8 last_mouse_r = 0;
	static u8 flag = 0;
	static uint16_t last_key;
	if (!Structure->RC->mouse.press_r && last_mouse_r)
	{
		Structure->Key_Mode = Gimbal_Chassis_Follow_Gimbal;
	}
	if (Structure->RC->mouse.press_r)
	{
		Structure->Key_Mode = (flag == 0) ? Gimbal_Vision : Gimbal_WindMill;
	}
	if ((Structure->RC->key.v & KEY_PRESSED_OFFSET_V) && !(last_key & KEY_PRESSED_OFFSET_V))
		flag = !flag;
	last_mouse_r = Structure->RC->mouse.press_r;
	last_key = Structure->RC->key.v;
}

void Swicth_Vision_Size(Gimbal_Typedef *Structure)
{
	if (((Structure->RC->key.v & KEY_PRESSED_OFFSET_C) != 0) && ((Structure->RC->key.v & KEY_PRESSED_OFFSET_SHIFT) != 0))
	{
		RecordStructure->Record_c = 1;
		RecordStructure->Record_shift = 1;
	}
	if ((RecordStructure->Record_c == 1) && (RecordStructure->Record_shift == 1))
	{
		Structure->Size = (Structure->Size == Big) ? Small : Big;

		RecordStructure->Record_c = 0;
		RecordStructure->Record_shift = 0;
	}
}

void Switch_WindMill(Gimbal_Typedef *Structure)
{
	if (((Structure->RC->key.v & KEY_PRESSED_OFFSET_Z) != 0))
		RecordStructure->Record_z = 1;
	if ((RecordStructure->Record_z == 1) && ((Structure->RC->key.v & KEY_PRESSED_OFFSET_Z) == 0))
	{
		RecordStructure->Record_z = 0;
		if (Structure->WindDir == Clockwise)
			Structure->WindDir = Anticlockwise;
		else
			Structure->WindDir = Clockwise;
	}
}

void Switch_Vision_Frocast(Gimbal_Typedef *Structure)
{
	/*
	if (Structure->RC->mouse.x > 0)
		Structure->vision_forcast = vision_right;
	else if (Structure->RC->mouse.x < 0)
		Structure->vision_forcast = vision_left;
	else if (Structure->RC->mouse.x == 0)
		Structure->vision_forcast = vision_stay;
	*/
	// static u8 flag;
	if (Structure->RC->key.v & KEY_PRESSED_OFFSET_Q)
	{
		Structure->vision_forcast = vision_left;
	}
	else if (Structure->RC->key.v & KEY_PRESSED_OFFSET_E)
	{
		Structure->vision_forcast = vision_right;
	}
	else
	{
		Structure->vision_forcast = vision_stay;
	}
}

void Key_mode_Conduct(Gimbal_Typedef *Structure)
{
	if (Structure->Key_Mode == Gimbal_Chassis_Follow_Gimbal)
	{
		if (Structure->RC->key.v & KEY_PRESSED_OFFSET_CTRL)
		{
			Structure->Gimbal_AngleStructure.Pitch_Angle_Target += Structure->RC->mouse.y * 0.04f;
			Structure->Gimbal_AngleStructure.Yaw_Angle_Target -= Structure->RC->mouse.x * 0.01f;
		}
		else
		{
			Structure->Gimbal_AngleStructure.Pitch_Angle_Target += Structure->RC->mouse.y * 0.04f;
			Structure->Gimbal_AngleStructure.Yaw_Angle_Target -= Structure->RC->mouse.x * 0.018f;
		}
		Structure->Gimbal_AngleStructure.Vision_Yaw_Angle = Structure->Gimbal_AngleStructure.Yaw_Angle_Target;
	}
	else if (Structure->Key_Mode == Gimbal_Vision)
	{
		Structure->Gimbal_AngleStructure.Pitch_Angle_Target -= (VisionStructure.y - 90) * 0.5f;
		Structure->Gimbal_AngleStructure.Vision_Yaw_Angle += (VisionStructure.x - 90) * 1.2f;

		/*	if( ABS(VisionStructure.x - 90)>70)
			Structure->Gimbal_AngleStructure.Vision_Yaw_Angle += (VisionStructure.x - 90) * 1.0f;
			else if(ABS(VisionStructure.x - 90)<70)
				Structure->Gimbal_AngleStructure.Vision_Yaw_Angle += (VisionStructure.x - 90) * 1.2f;
		*/
		VisionStructure.y = 90;
		VisionStructure.x = 90;
		Structure->Gimbal_AngleStructure.Yaw_Angle_Target = Structure->Gimbal_AngleStructure.Vision_Yaw_Angle;
	}
	else if (Structure->Key_Mode == Gimbal_WindMill)
	{
		Structure->Gimbal_AngleStructure.Pitch_Angle_Target -= (VisionStructure.y - 90) * 3.0f;
		Structure->Gimbal_AngleStructure.Vision_Yaw_Angle += (VisionStructure.x - 90) * 1.5f;
		VisionStructure.y = 90;
		VisionStructure.x = 90;
		Structure->Gimbal_AngleStructure.Yaw_Angle_Target = Structure->Gimbal_AngleStructure.Vision_Yaw_Angle;
	}
}

void gimbal_key_ctrl(Gimbal_Typedef *Structure)
{

	Switch_Vision_Mode(Structure);

	Swicth_Vision_Size(Structure);

	Switch_WindMill(Structure);
	Switch_Vision_Frocast(Structure);

	Key_mode_Conduct(Structure);
}

void gimbal_mode_conduct(Gimbal_Typedef *Structure)
{
	
	Structure->GYROSCOPE_YAW = eulerAngle.yaw * 22.752777778f + 4096.f ;

	if (Structure == NULL)
	{
		return;
	}
	if (Structure->mode == Gimbal_cfg)
		gimbal_chassis_follow_gimbal(Structure);
	else if (Structure->mode == Gimbal_zero)
		gimbal_zero(Structure);
	else if (Structure->mode == Gimbal_key_ctrl)
		gimbal_key_ctrl(Structure);
}

void gimbal_mode_behaviour_set(Gimbal_Typedef *Structure)
{

	if (switch_is_down(Structure->RC->rc.s[1]))
	{

		if (switch_is_down(Structure->RC->rc.s[0]))
			Structure->mode = Gimbal_zero;
		else
			Structure->mode = Gimbal_cfg;
	}
	else if (switch_is_mid(Structure->RC->rc.s[1]))
	{
		if (switch_is_up(Structure->RC->rc.s[0]))
			Structure->mode = Gimbal_cfg;
		else if (switch_is_mid(Structure->RC->rc.s[0]))
			Structure->mode = Gimbal_key_ctrl;
		else if (switch_is_down(Structure->RC->rc.s[0]))
			Structure->mode = Gimbal_cfg;
	}

	else if (switch_is_up(Structure->RC->rc.s[1]))
	{
		Structure->mode = Gimbal_cfg;
	}
}
void gimbal_mode_set(Gimbal_Typedef *Structure)
{
	if (Structure == NULL)
		return;

	gimbal_mode_behaviour_set(Structure);
}

void gimbal_init(Gimbal_Typedef *Structure)
{
	Gimbal_Str = Structure;
	RecordStructure = &Chassis_Str->RecordStructure;
	Structure->RC = &ALLRC;
	Structure->Gimbal6020_PITCH = &Mpitch;
	Structure->Gimbal6020_YAW = &Myaw;
	Structure->Pitch_Angle_Grow = 0.6f;
	Structure->Yaw_Angle_Grow = 3.0f;
	Structure->Gimbal_AngleStructure.Pitch_Angle_Target=2800;
	PID_Init(&Structure->PITCH.inner, PID_POSITION, PITCH_ANGLE);

	PID_Init(&Structure->PITCH.outer, PID_POSITION, PITCH_SPEED);

	PID_Init(&Structure->YAW.inner, PID_POSITION, YAW_ANGLE);

	PID_Init(&Structure->YAW.outer, PID_POSITION, YAW_SPEED);
}
