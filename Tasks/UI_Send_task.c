#include "UI_Send_task.h"

uint8_t UI_FLAG = 0;
u8 flag=0;
void UI_SendTask(void *pvParameters)
{
	UI_FLAG = 1;

	while (1)
	{

		if (UI_FLAG)
		{
			make_word(&ext_data.word[MAG_WORD], 1, 2, "Mag:", "A", 1620, _MAG_Word_Y);
			referee_data_pack_handle(0xA5, 0x0301, (uint8_t *)&ext_data.word[MAG_WORD], sizeof(ext_student_interactive_header_word_t));

			make_word(&ext_data.word[FIRC_WORD], 1, 2, "FC:", "B", 1620, _FRIC_Word_Y);
			referee_data_pack_handle(0xA5, 0x0301, (uint8_t *)&ext_data.word[MAG_WORD], sizeof(ext_student_interactive_header_word_t));

			make_word(&ext_data.word[GIMBAL_WORD], 1, 2, "GM:", "C", 50, WORD_GIMBAL);
			referee_data_pack_handle(0xA5, 0x0301, (uint8_t *)&ext_data.word[GIMBAL_WORD], sizeof(ext_student_interactive_header_word_t));

			make_word(&ext_data.word[CHASSIS_WORD], 1, 2, "CH:", "D", 50, WORD_CHASSIS);
			referee_data_pack_handle(0xA5, 0x0301, (uint8_t *)&ext_data.word[CHASSIS_WORD], sizeof(ext_student_interactive_header_word_t));

			make_word(&ext_data.word[CAP], 1, 2, "CAP:", "E", 50, CAP_NUM_Y);
			referee_data_pack_handle(0xA5, 0x0301, (uint8_t *)&ext_data.word[CAP], sizeof(ext_student_interactive_header_word_t));

			make_word(&ext_data.word[STUCK_SHOOT_WORD], 1, 2, "OK ", "F", 1620, _STK_Word_Y);
			referee_data_pack_handle(0xA5, 0x0301, (uint8_t *)&ext_data.word[STUCK_SHOOT_WORD], sizeof(ext_student_interactive_header_word_t));

			
			
			make_num(&ext_data.graph[CAP_], "E", pm01_od.v_out /100, 1, 2, 200, CAP_NUM_Y);
			referee_data_pack_handle(0xA5, 0x0301, (uint8_t *)&ext_data.graph[CAP_], sizeof(ext_student_interactive_header_graph_t));

			make_num(&ext_data.graph[MAG_], "A", (int32_t)Shoot_Str->ALL_Flag_Str.Mag_flag, 1, 2, 1720, _MAG_Word_Y);
			referee_data_pack_handle(0xA5, 0x0301, (uint8_t *)&ext_data.graph[MAG_], sizeof(ext_student_interactive_header_graph_t));

			make_num(&ext_data.graph[STUCK], "F", (int32_t)Shoot_Str->Stuck_Flag, 1, 2, 1720, _STK_Word_Y);
			referee_data_pack_handle(0xA5, 0x0301, (uint8_t *)&ext_data.graph[STUCK], sizeof(ext_student_interactive_header_graph_t));

			make_num(&ext_data.graph[FIRC], "B", (int32_t)Shoot_Str->ALL_Flag_Str.Firc_flag, 1, 2, 1720, _FRIC_Word_Y);
			referee_data_pack_handle(0xA5, 0x0301, (uint8_t *)&ext_data.graph[FIRC], sizeof(ext_student_interactive_header_graph_t));

			make_num(&ext_data.graph[GIMBAL_], "C", 0, 1, 2, 200, WORD_GIMBAL);
			referee_data_pack_handle(0xA5, 0x0301, (uint8_t *)&ext_data.graph[GIMBAL_], sizeof(ext_student_interactive_header_graph_t));

			make_num(&ext_data.graph[CHASSIS_], "D", 0, 1, 2, 200, WORD_CHASSIS);
			referee_data_pack_handle(0xA5, 0x0301, (uint8_t *)&ext_data.graph[CHASSIS_], sizeof(ext_student_interactive_header_graph_t));
		}

		else
		{
			if (Shoot_Str->Stuck_Flag)
				//	make_word(&stuck_state_word,2,4,"STK","STK",1720,_STK_Word_Y);
				make_word(&ext_data.word[STUCK_SHOOT_WORD], 2, 4, "STK", "F", 1720, _STK_Word_Y);
			else if (!Shoot_Str->Stuck_Flag)
				make_word(&ext_data.word[STUCK_SHOOT_WORD], 2, 2, "OK ", "F", 1720, _STK_Word_Y);
			referee_data_pack_handle(0xA5, 0x0301, (uint8_t *)&ext_data.word[STUCK_SHOOT_WORD], sizeof(ext_student_interactive_header_word_t));

			//		_x = 200 * arm_cos_f32(theta+PI/2);
			//		_y = 200 * arm_sin_f32(theta+PI/2);
			//		make_line(&chassis_dir_line,2, "line", 2, 1500 + _x, 500 + _y, 1500,700);
			//		referee_data_pack_handle(0xA5,0x0301,(uint8_t *)&chassis_dir_line,sizeof(chassis_dir_line));

			// make_num(&ecd_value,"ecd", (int32_t)get_upcontrol_point()->relative_angle, 2, 2, 1720,_ECD_Word_Y);
			//	referee_data_pack_handle(0xA5,0x0301,(uint8_t *)&ecd_value,sizeof(ecd_value));

			make_num(&ext_data.graph[MAG_], "A", (int32_t)Shoot_Str->ALL_Flag_Str.Mag_flag, 2, 2, 1720, _MAG_Word_Y);
			referee_data_pack_handle(0xA5, 0x0301, (uint8_t *)&ext_data.graph[MAG_], sizeof(ext_student_interactive_header_graph_t));

			make_num(&ext_data.graph[FIRC], "B", (int32_t)Shoot_Str->ALL_Flag_Str.Firc_flag, 2, 2, 1720, _FRIC_Word_Y);
			referee_data_pack_handle(0xA5, 0x0301, (uint8_t *)&ext_data.graph[FIRC], sizeof(ext_student_interactive_header_graph_t));

			make_num(&ext_data.graph[CAP_], "E", pm01_od.v_out / 100, 2, 2, 200, CAP_NUM_Y);
			referee_data_pack_handle(0xA5, 0x0301, (uint8_t *)&ext_data.graph[CAP_], sizeof(ext_student_interactive_header_graph_t));

			if (Gimbal_Str->Key_Mode == Gimbal_Chassis_Follow_Gimbal)
			{
				make_num(&ext_data.graph[GIMBAL_], "C", 1, 2, 2, 200, WORD_GIMBAL);
				referee_data_pack_handle(0xA5, 0x0301, (uint8_t *)&ext_data.graph[GIMBAL_], sizeof(ext_student_interactive_header_graph_t));
			}
			else if (Gimbal_Str->Key_Mode == Gimbal_Vision)
			{
				make_num(&ext_data.graph[GIMBAL_], "C", 2,2, 2, 200, WORD_GIMBAL);
				referee_data_pack_handle(0xA5, 0x0301, (uint8_t *)&ext_data.graph[GIMBAL_], sizeof(ext_student_interactive_header_graph_t));
			}
			else if (Gimbal_Str->Key_Mode == Gimbal_WindMill)
			{
				make_num(&ext_data.graph[GIMBAL_], "C", 3,2, 2, 200, WORD_GIMBAL);
				referee_data_pack_handle(0xA5, 0x0301, (uint8_t *)&ext_data.graph[GIMBAL_], sizeof(ext_student_interactive_header_graph_t));
			}

			if (Chassis_Str->Key_Ctrl == Chassis_Zero_)
			{
				make_num(&ext_data.graph[CHASSIS_], "D", 1, 2, 2, 200, WORD_CHASSIS);
				referee_data_pack_handle(0xA5, 0x0301, (uint8_t *)&ext_data.graph[CHASSIS_], sizeof(ext_student_interactive_header_graph_t));
			}
			else if (Chassis_Str->Key_Ctrl == Chassis_Gryo_)
			{
				make_num(&ext_data.graph[CHASSIS_], "D", 2, 2, 2, 200, WORD_CHASSIS);
				referee_data_pack_handle(0xA5, 0x0301, (uint8_t *)&ext_data.graph[CHASSIS_], sizeof(ext_student_interactive_header_graph_t));
			}
			else if (Chassis_Str->Key_Ctrl == Chassis_Cfg_)
			{
				make_num(&ext_data.graph[CHASSIS_], "D", 3, 2, 2, 200, WORD_CHASSIS);
				referee_data_pack_handle(0xA5, 0x0301, (uint8_t *)&ext_data.graph[CHASSIS_], sizeof(ext_student_interactive_header_graph_t));
			}
		}
		
		if(ALLRC.key.v&KEY_PRESSED_OFFSET_C)
		{
			flag=1;
		}
		if((flag==1)&&((ALLRC.key.v&KEY_PRESSED_OFFSET_C)==0))
		{
			flag=0;
			UI_FLAG=!UI_FLAG;
		}
		

/*
				// RE_DRAW
				if (UI_FLAG)
				{
					make_word(&ext_data.word[REDRAW_WORD], 1, 3, "draw", "rdw", 1400, 1700);
					referee_data_pack_handle(0xA5, 0x0301, (uint8_t *)&ext_data.word[REDRAW_WORD], sizeof(ext_student_interactive_header_word_t));
				}
				if (ALLRC.key.v & KEY_PRESSED_OFFSET_C)
				{
					vTaskDelay(800);
					if ((ALLRC.key.v & KEY_PRESSED_OFFSET_C) != 0)
					{
						UI_FLAG = !UI_FLAG;
						make_word(&ext_data.word[REDRAW_WORD], 3, 3, "draw", "rdw", 1400, 700);
						referee_data_pack_handle(0xA5, 0x0301, (uint8_t *)&ext_data.word[REDRAW_WORD], sizeof(ext_student_interactive_header_word_t));
					}
				}
				vTaskDelay(1);

				// GIMBAL_MODE        绿色
				if (UI_FLAG)
				{
					make_word(&ext_data.word[GIMBAL_WORD], 1, 2, "GM:", "GM", 50, WORD_GIMBAL);
					make_num(&ext_data.graph[GIMBAL_], "GM", 0, 1, 3, 150, 850);
					referee_data_pack_handle(0xA5, 0x301, (uint8_t *)&ext_data.graph[GIMBAL_], sizeof(ext_student_interactive_header_graph_t));
					referee_data_pack_handle(0xA5, 0x0301, (uint8_t *)&ext_data.word[GIMBAL_WORD], sizeof(ext_student_interactive_header_word_t));
				}

				switch (Gimbal_Str->Key_Mode)
				{
				case Gimbal_Vision:
				{
					make_num(&ext_data.graph[GIMBAL_], "GM", 2, 2, 3, 150, 650);
					break;
				}
				case Gimbal_Chassis_Follow_Gimbal:
				{
					make_num(&ext_data.graph[GIMBAL_], "GM", 1, 2, 3, 150, 650);
					break;
				}
				case Gimbal_WindMill:
				{
					make_num(&ext_data.graph[GIMBAL_], "GM", 3, 2, 3, 150, 650);
					break;
				}
				}
				referee_data_pack_handle(0xA5, 0x301, (uint8_t *)&ext_data.graph[GIMBAL_], sizeof(ext_student_interactive_header_graph_t));
				vTaskDelay(1);

				// CHASSIS_MODE
				if (UI_FLAG)
				{
					make_word(&ext_data.word[CHASSIS_WORD], 1, 2, "CH:", "CH", 50, WORD_CHASSIS);
					make_num(&ext_data.graph[CHASSIS_], "CH", 0, 1, 3, 150, WORD_CHASSIS);
					referee_data_pack_handle(0xA5, 0x301, (uint8_t *)&ext_data.graph[CHASSIS_], sizeof(ext_student_interactive_header_graph_t));
					referee_data_pack_handle(0xA5, 0x0301, (uint8_t *)&ext_data.word[CHASSIS_WORD], sizeof(ext_student_interactive_header_word_t));
				}
				switch (Chassis_Str->Key_Ctrl)
				{
				case Chassis_Zero_:
					make_num(&ext_data.graph[CHASSIS_], "CH", 0, 2, 3, 150, WORD_CHASSIS);
					break;
				case Chassis_Gryo_:
					make_num(&ext_data.graph[CHASSIS_], "CH", 2, 2, 3, 150, WORD_CHASSIS);
					break;
				case Chassis_Cfg_:
					make_num(&ext_data.graph[CHASSIS_], "CH", 1, 2, 3, 150, WORD_CHASSIS);

					break;
				}

				referee_data_pack_handle(0xA5, 0x301, (uint8_t *)&ext_data.graph[CHASSIS_], sizeof(ext_student_interactive_header_graph_t));
				vTaskDelay(1);
				// MAG_STATE
				if (UI_FLAG)
				{
					make_word(&ext_data.word[MAG_WORD], 1, 2, "MAG:", "MG", 50, WORD_MAG);
					make_num(&ext_data.graph[MAG_], "MG", 0, 1, 3, 150, WORD_MAG);
					referee_data_pack_handle(0xA5, 0x301, (uint8_t *)&ext_data.graph[MAG_], sizeof(ext_student_interactive_header_graph_t));
					referee_data_pack_handle(0xA5, 0x0301, (uint8_t *)&ext_data.word[MAG_WORD], sizeof(ext_student_interactive_header_word_t));
				}
				if (Shoot_Str->ALL_Flag_Str.Mag_flag == OPEN)
					make_num(&ext_data.graph[MAG_], "MG", 1, 2, 3, 150, WORD_MAG);
				// make_word(&ext_data.word[MAG_WORD], 2, 4, "MAG:OP", "MG", 50, WORD_MAG);
				else if (Shoot_Str->ALL_Flag_Str.Mag_flag == CLOSE)
					make_num(&ext_data.graph[MAG_], "MG", 0, 2, 3, 150, WORD_MAG);
				// make_word(&ext_data.word[MAG_WORD], 2, 4, "MAG:CL", "MG", 50, WORD_MAG);
				referee_data_pack_handle(0xA5, 0x301, (uint8_t *)&ext_data.graph[MAG_], sizeof(ext_student_interactive_header_graph_t));
				// STUCK_
				if (UI_FLAG)
				{
					make_word(&ext_data.word[STUCK_SHOOT_WORD], 1, 2, "STUCK", "STK", 1720, WORD_STUCK);
					make_num(&ext_data.graph[STUCK], "STUCK", 0, 1, 2, 1820, WORD_STUCK);
					referee_data_pack_handle(0xA5, 0x301, (uint8_t *)&ext_data.graph[STUCK], sizeof(ext_student_interactive_header_graph_t));
					referee_data_pack_handle(0xA5, 0x0301, (uint8_t *)&ext_data.word[STUCK_SHOOT_WORD], sizeof(ext_student_interactive_header_word_t));
				}
				if (Shoot_Str->Stuck_Flag == 1)
				{
					make_num(&ext_data.graph[STUCK], "STUCK", 1, 1, 2, 1880, WORD_STUCK);
				}
				else
				{
					make_num(&ext_data.graph[STUCK], "STUCK", 0, 1, 2, 1880, WORD_STUCK);
				}
				referee_data_pack_handle(0xA5, 0x301, (uint8_t *)&ext_data.graph[STUCK], sizeof(ext_student_interactive_header_graph_t));

			
				if (Shoot_Str->Stuck_Flag == 1)
					make_word(&ext_data.word[STUCK_SHOOT_WORD], 1, 2, "STUCK", "STK", 1720, WORD_STUCK);
				else
					make_word(&ext_data.word[STUCK_SHOOT_WORD], 1, 2, "OK", "STK", 1720, WORD_STUCK);
				vTaskDelay(1);
				*/
		// CAP_LAST
//		if (UI_FLAG)
//		{
//			make_num(&ext_data.graph[CAP], "AP", pm01_od.v_out * 10, 1, 2, 50, CAP_NUM_Y);
//			referee_data_pack_handle(0xA5, 0x0301, (uint8_t *)&ext_data.graph[CAP], sizeof(ext_student_interactive_header_graph_t));
//		}
//		else
//		{
//			make_num(&ext_data.graph[CAP], "AP", pm01_od.v_out * 10, 2, 2, 50, CAP_NUM_Y);
//			referee_data_pack_handle(0xA5, 0x0301, (uint8_t *)&ext_data.graph[CAP], sizeof(ext_student_interactive_header_graph_t));
//		}
//		vTaskDelay(1);

//		// line
//		if (UI_FLAG)
//		{
//			make_guideline_graph(&ext_data.graph[GUIDE_LINE]);
//			referee_data_pack_handle(0xA5, 0x0301, (uint8_t *)&ext_data.graph[GUIDE_LINE], sizeof(ext_student_interactive_header_graph_t));
//		}
		/*
		if(UI_FLAG)
		{
			make_cir(&ext_data.graph[VISION],1,(Gimbal_Str->Key_Mode==))
		}
*/
	vTaskDelay(1);
}
}