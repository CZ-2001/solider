#include "user_task.h"
#include "main.h"
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "led.h"
//#include "judge_analysis.h"
#include "laser.h"
#include "can_rec.h"
#include "uart_vofa.h"
#include "vofa_task.h"
#include "user_lib.h"
#include "rc_task.h"
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include "pid.h"
#include "gimbal_behaviour.h"
#include "INS_task.h"
#include "chassis_behaviour.h"
#include "judge_behaviour.h"


#define user_is_error() toe_is_error(errorListLength)

#if INCLUDE_uxTaskGetStackHighWaterMark
uint32_t VofaTaskStack;
#endif







float send_message[channel];
int16_t FBK_I[4];
float power=0;
uint8_t buf[2];
Vofa_t Vofa;

const fp32* INSangle;
const fp32* MPU_angle;
int strlength(char *str,int max)
{
  int temp=0;
  for(int i=0;i<max;i++)
  {
    if(str[i]!='\0')
      temp=i+1;
  }
  return temp;
}


//初始化帧尾数据
void Vofa_tail_init(void)
{
	Vofa.tail[0] = 0x00;
	Vofa.tail[1] = 0x00;
	Vofa.tail[2] = 0x80;
	Vofa.tail[3] = 0x7F;
}

//如需改变通道数量 只需将channel值改变

//将浮点数f转化为4个字节数据存放在byte[4]中 
void Float_to_Byte(float f,unsigned char byte[])  
{  
    FloatLongType Float;  
    Float.float_data=f;  
	
    byte[0]=(unsigned char) Float.long_data;  
    byte[1]=(unsigned char)(Float.long_data>>8);  
    byte[2]=(unsigned char)(Float.long_data>>16);  
    byte[3]=(unsigned char)(Float.long_data>>24);  
}

void Vofa_send_justfloat(float *send_message)
{
	//对每个通道进行操作
	for(uint8_t count1 = 0; count1 <= (channel - 1); count1 ++ )  
	{
		Float_to_Byte(send_message[count1], Vofa.temp);

		//将拆分后的字节存入buf数组
		for(uint8_t count2 = 0; count2 <= 3; count2 ++ )  
		{
			Vofa.message[(count1 * 4) + count2] = Vofa.temp[count2];
		}
	}
	
	//对帧尾打包
	for(uint8_t count3 = 0; count3 <= 3; count3 ++)  
	{
		Vofa.message[channel * 4 + count3] = Vofa.tail[count3];
	}
	
}

#include "math.h"
#include "vision_behaviour.h"
void VofaTask(void *pvParameters)
{
		Vofa_tail_init();
		INSangle	=	get_INS_angle_point();
		MPU_angle	=	get_MPU6500_Gyro_Data_Point();
    while (1)
    {	
//			send_message[0]=Chassis_Str->Aim_Send_Current_Structure.Speed[0];
//				send_message[1]=Chassis_Str->Chassis_Motor3508[0]->speed_rpm;
//		send_message[0]=JudgeStructure.ext_game_robot_status.chassis_power_limit;
//			send_message[1]=JudgeStructure.ext_power_heat_data.chassis_power_buffer;
//			send_message[4]=	Gimbal_Str->Gimbal_AngleStructure.Pitch_Angle_Target;
//			
		//	send_message[3]=Gimbal_Str->Gimbal_AngleStructure.Vision_Yaw_Angle;
//				send_message[3]=	 Chassis_Str->Chassis_Motor3508[0]->speed_rpm;
			//send_message[4]=Chassis_Str->Chassis_Key_Press_Structure.Key_Press_Time_Max;//JudgeStructure.ext_power_heat_data.chassis_power; //pm01_od.v_out;
			//send_message[0]=Gimbal_Str->Gimbal_AngleStructure.Vision_Yaw_Angle;
			//send_message[1]=Gimbal_Str->GYROSCOPE_YAW;
		//	send_message[2]=JudgeStructure.ext_power_heat_data.chassis_power;
//send_message[3]=pm01_od.v_out;			
			//send_message[0]=Gimbal_Str->GYROSCOPE_YAW;
			
		//	send_message[0]=USART6_RX_BUF[3]<<8|USART6_RX_BUF[4];
			
			
			
//			if(USART6_RX_BUF[5]&0x01)
//			{
//				send_message[0]=	send_message[0];
//			}else if((USART6_RX_BUF[5]&0x01)==0)
//			{
//				send_message[0]=	-send_message[0];
//			}
			
			
			
			send_message[0]=yaw_debug_pid_speed;
			send_message[1]=icm_data.gyro_z;
			Vofa_send_justfloat(send_message);
			
			Dma_Send_Enable(DMA1_Stream6,sizeof(Vofa.message));
			//printf("%f,%f\n",send_message[0],send_message[1]);
			vTaskDelay(20);
			
# if INCLUDE_uxTaskGetStackHighWaterMark
        VofaTaskStack = uxTaskGetStackHighWaterMark(NULL);
#endif
    }
}


