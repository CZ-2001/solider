#include "can_rec.h"
#include "stm32f4xx.h"
#include "func_lib.h"
#include "string.h"
static CanTxMsg GIMBAL_TxMessage, SHOOT_TxMessage;
CanRxMsg rx1_message, rx2_message;
MotorMeasureTypeDef Myaw, Mpitch, Mchassis[4], Mfricl[2], Mplate;

CapMeasureTypeDef Super_Stu;
pm01_od_t pm01_od;
volatile uint16_t  pm01_access_id;    
volatile uint16_t  pm01_response_flg; 

uint16_t g_cmd_set   = 2;
uint16_t g_power_set = 5000;
uint16_t g_vout_set  = 2400;
uint16_t g_iout_set  = 500;

void get_chassismotor_measure(MotorMeasureTypeDef *ptr, CanRxMsg *RxMessage)
{
	ptr->last_ecd = ptr->ecd;
	ptr->speed_rpm_last = ptr->speed_rpm;
	ptr->ecd = (uint16_t)((RxMessage)->Data[0] << 8 | (RxMessage)->Data[1]);
	ptr->speed_rpm = (uint16_t)((RxMessage)->Data[2] << 8 | (RxMessage)->Data[3]);
	ptr->given_current = (uint16_t)((RxMessage)->Data[4] << 8 | (RxMessage)->Data[5]);
	ptr->temperate = (RxMessage)->Data[6];
	ptr->speed_rpm_filter = ptr->speed_rpm_last * K_SpdFilter + ptr->speed_rpm * (1 - K_SpdFilter);

	if (ptr->ecd - ptr->last_ecd > 4095)
		ptr->round_cnt--;
	else if (ptr->ecd - ptr->last_ecd < -4095)
		ptr->round_cnt++;
	// cir_limit_fp32(ptr->round_cnt,0,36);

	ptr->total_ecd = ptr->round_cnt * 8191 + ptr->ecd - ptr->offset_ecd;

	ptr->total_ecd_out_2006 = (ptr->round_cnt * 8191 + ptr->ecd - ptr->offset_ecd);
	ptr->total_ecd_out_3508 = (ptr->round_cnt * 8191 + ptr->ecd - ptr->offset_ecd);
	ptr->total_ecd_out_6020 = ptr->round_cnt * 8191 + ptr->ecd - ptr->offset_ecd;
}

void get_gimbalmotor_measure(MotorMeasureTypeDef *ptr, CanRxMsg *RxMessage)
{
	ptr->last_ecd = ptr->ecd;
	ptr->speed_rpm_last = ptr->speed_rpm;
	ptr->ecd = (uint16_t)((RxMessage)->Data[0] << 8 | (RxMessage)->Data[1]);
	ptr->speed_rpm = (uint16_t)((RxMessage)->Data[2] << 8 | (RxMessage)->Data[3]);
	ptr->given_current = (uint16_t)((RxMessage)->Data[4] << 8 | (RxMessage)->Data[5]);
	ptr->temperate = (RxMessage)->Data[6];

	if (ptr->ecd - ptr->last_ecd > 4095)
		ptr->round_cnt--;
	else if (ptr->ecd - ptr->last_ecd < -4095)
		ptr->round_cnt++;
	if (ptr->round_cnt > 37)
		ptr->round_cnt -= 36;
	if (ptr->round_cnt < 0)
		ptr->round_cnt += 37;

	ptr->total_ecd = ptr->round_cnt * 8191 + ptr->ecd - ptr->offset_ecd;

	ptr->total_ecd_out_2006 = (ptr->round_cnt * 8191 + ptr->ecd - ptr->offset_ecd);
	ptr->total_ecd_out_3508 = (ptr->round_cnt * 8191 + ptr->ecd - ptr->offset_ecd);
	ptr->total_ecd_out_6020 = ptr->round_cnt * 8191 + ptr->ecd - ptr->offset_ecd;
	ptr->angle_6020 = ptr->ecd * 360.f / 8191.f;
}
void get_pm01_measure(CanRxMsg *rx_message, uint8_t *can_rx_data )
{

	uint16_t m_tmp;
	
	pm01_response_flg = ( pm01_access_id == rx_message->StdId );
	
	if( rx_message->RTR == CAN_RTR_REMOTE )return;
	
	switch( rx_message->StdId )
	{
		case 0x600:
		
      m_tmp = (uint16_t)can_rx_data[0] << 8 | can_rx_data[1];	
		
		  pm01_od.ccr = m_tmp;
		
			break;
		case 0x601:
			
		  m_tmp = (uint16_t)can_rx_data[0] << 8 | can_rx_data[1];	

			pm01_od.p_set = m_tmp;
		
			break;
		case 0x602:
			
		  m_tmp = (uint16_t)can_rx_data[0] << 8 | can_rx_data[1];	

			pm01_od.v_set = m_tmp;
					
			break;
		case 0x603:
			
		  m_tmp = (uint16_t)can_rx_data[0] << 8 | can_rx_data[1];	

			pm01_od.i_set = m_tmp;
					
			break;
		case 0x610:
			
		  m_tmp = (uint16_t)can_rx_data[0] << 8 | can_rx_data[1];	

			pm01_od.sta_code.all = m_tmp;
		
		  m_tmp = (uint16_t)can_rx_data[2] << 8 | can_rx_data[3];	

			pm01_od.err_code = m_tmp;
		
			break;
		case 0x611:
			
		  m_tmp = (uint16_t)can_rx_data[0] << 8 | can_rx_data[1];	

			pm01_od.p_in = m_tmp;
		
		  m_tmp = (uint16_t)can_rx_data[2] << 8 | can_rx_data[3];	

			pm01_od.v_in = m_tmp;	
		
		  m_tmp = (uint16_t)can_rx_data[4] << 8 | can_rx_data[5];	

			pm01_od.i_in = m_tmp;
		
			break;
		case 0x612:
			
		  m_tmp = (uint16_t)can_rx_data[0] << 8 | can_rx_data[1];	

			pm01_od.p_out = m_tmp;
		
		  m_tmp = (uint16_t)can_rx_data[2] << 8 | can_rx_data[3];	

			pm01_od.v_out = m_tmp;	
		
		  m_tmp = (uint16_t)can_rx_data[4] << 8 | can_rx_data[5];	

			pm01_od.i_out = m_tmp;
					
			break;
		case 0x613:
			
		  m_tmp = (uint16_t)can_rx_data[0] << 8 | can_rx_data[1];	

			pm01_od.temp = m_tmp;
		
		  m_tmp = (uint16_t)can_rx_data[2] << 8 | can_rx_data[3];	

			pm01_od.total_time = m_tmp;	
		
		  m_tmp = (uint16_t)can_rx_data[4] << 8 | can_rx_data[5];	

			pm01_od.run_time = m_tmp;
					
			break;
	}


}
static void CAN1_hook(CanRxMsg *rx_message)
{
	  uint16_t m_tmp;
	if (rx_message->RTR == CAN_RTR_REMOTE)
		return;
	switch (rx_message->StdId)
	{
	case CAN_3508_M1_ID:
	case CAN_3508_M2_ID:
	case CAN_3508_M3_ID:
	case CAN_3508_M4_ID:
	{
		static uint8_t i = 0;
		i = rx_message->StdId - CAN_3508_M1_ID;
		get_chassismotor_measure(&Mchassis[i], rx_message);
		break;
	}
	case CAN_YAW_MOTOR_ID:
	{

		get_gimbalmotor_measure(&Myaw, rx_message);
		break;
	}
	 case 0x600:
      m_tmp = (uint16_t)rx_message->Data[0] << 8 | rx_message->Data[1];	
		  pm01_od.ccr = m_tmp;
			break;
		case 0x601:
		  m_tmp = (uint16_t)rx_message->Data[0] << 8 | rx_message->Data[1];	
			pm01_od.p_set = m_tmp;
			break;
		case 0x602:
		  m_tmp = (uint16_t)rx_message->Data[0] << 8 | rx_message->Data[1];
			pm01_od.v_set = m_tmp;
			break;
		case 0x603:
		  m_tmp = (uint16_t)rx_message->Data[0] << 8 | rx_message->Data[1];	
			pm01_od.i_set = m_tmp;					
			break;
		case 0x610:			
		  m_tmp = (uint16_t)rx_message->Data[0] << 8 | rx_message->Data[1];	
			pm01_od.sta_code.all = m_tmp;	
		  m_tmp = (uint16_t)rx_message->Data[2] << 8 | rx_message->Data[3];	
			pm01_od.err_code = m_tmp;	
			break;
		case 0x611:			
		  m_tmp = (uint16_t)rx_message->Data[0] << 8 | rx_message->Data[1];	
			pm01_od.p_in = m_tmp;	
		  m_tmp = (uint16_t)rx_message->Data[2] << 8 | rx_message->Data[3];	
			pm01_od.v_in = m_tmp;	
		  m_tmp = (uint16_t)rx_message->Data[4] << 8 | rx_message->Data[5];	
			pm01_od.i_in = m_tmp;
			break;
		case 0x612:
		  m_tmp = (uint16_t)rx_message->Data[0] << 8 | rx_message->Data[1];
			pm01_od.p_out = m_tmp;
		  m_tmp = (uint16_t)rx_message->Data[2] << 8 | rx_message->Data[3];	
			pm01_od.v_out = m_tmp;	
		  m_tmp = (uint16_t)rx_message->Data[4] << 8 | rx_message->Data[5];	
			pm01_od.i_out = m_tmp;	
			break;
		case 0x613:
		  m_tmp = (uint16_t)rx_message->Data[0] << 8 | rx_message->Data[1];	
			pm01_od.temp = m_tmp;		
		  m_tmp = (uint16_t)rx_message->Data[2] << 8 | rx_message->Data[3];	
			pm01_od.total_time = m_tmp;		
		  m_tmp = (uint16_t)rx_message->Data[4] << 8 | rx_message->Data[5];	
      pm01_od.run_time = m_tmp;
			break;
	}
}

static void CAN2_hook(CanRxMsg *rx_message)
{
	switch (rx_message->StdId)
	{
	case CAN_PIT_MOTOR_ID:
	{

		get_gimbalmotor_measure(&Mpitch, rx_message);
		break;
	}
	case CAN_FRICL_ID:
	{

		get_gimbalmotor_measure(&Mfricl[FRIC_L], rx_message);
		break;
	}
	case CAN_FRICR_ID:
	{

		get_gimbalmotor_measure(&Mfricl[FRIC_R], rx_message);
		break;
	}
	case CAN_PLATE_MOTOR_ID:
	{

		get_gimbalmotor_measure(&Mplate, rx_message);
		break;
	}
	default:
	{
		break;
	}
	}
}

void CAN1_RX0_IRQHandler(void)
{
	if (CAN_GetITStatus(CAN1, CAN_IT_FMP0) != RESET)
	{
		CAN_ClearITPendingBit(CAN1, CAN_IT_FMP0);
		CAN_Receive(CAN1, CAN_FIFO0, &rx1_message);
		CAN1_hook(&rx1_message);
	}
}

void CAN2_RX1_IRQHandler(void)
{
	if (CAN_GetITStatus(CAN2, CAN_IT_FMP1) != RESET)
	{
		CAN_ClearITPendingBit(CAN2, CAN_IT_FMP1);
		CAN_Receive(CAN2, CAN_FIFO1, &rx2_message);

		CAN2_hook(&rx2_message);
	}
}

void can_cmd_gimbal(int16_t yaw, int16_t pitch, int16_t rev1, int16_t rev2)
{
	GIMBAL_TxMessage.StdId = CAN_GIMBAL_ALL_ID;
	GIMBAL_TxMessage.IDE = CAN_ID_STD;
	GIMBAL_TxMessage.RTR = CAN_RTR_DATA;
	GIMBAL_TxMessage.DLC = 0x08;
	GIMBAL_TxMessage.Data[0] = (yaw >> 8);
	GIMBAL_TxMessage.Data[1] = yaw;
	GIMBAL_TxMessage.Data[2] = (pitch >> 8);
	GIMBAL_TxMessage.Data[3] = pitch;
	GIMBAL_TxMessage.Data[4] = (rev1 >> 8);
	GIMBAL_TxMessage.Data[5] = rev1;
	GIMBAL_TxMessage.Data[6] = (rev2 >> 8);
	GIMBAL_TxMessage.Data[7] = rev2;

	CAN_Transmit(CAN_PITCH, &GIMBAL_TxMessage);
	CAN_Transmit(CAN_YAW, &GIMBAL_TxMessage);
}

void can_cmd_chassis(int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4)
{
	CanTxMsg TxMessage;
	TxMessage.StdId = CAN_CHASSIS_ALL_ID;
	TxMessage.IDE = CAN_ID_STD;
	TxMessage.RTR = CAN_RTR_DATA;
	TxMessage.DLC = 0x08;
	TxMessage.Data[0] = motor1 >> 8;
	TxMessage.Data[1] = motor1;
	TxMessage.Data[2] = motor2 >> 8;
	TxMessage.Data[3] = motor2;
	TxMessage.Data[4] = motor3 >> 8;
	TxMessage.Data[5] = motor3;
	TxMessage.Data[6] = motor4 >> 8;
	TxMessage.Data[7] = motor4;

	CAN_Transmit(CAN_CHASSIS, &TxMessage);
}

void can_cmd_shoot(int16_t User_Motor1, int16_t User_Motor2, int16_t User_Motor3, int16_t User_Motor4)
{
	SHOOT_TxMessage.StdId = 0x200;
	SHOOT_TxMessage.IDE = CAN_ID_STD;
	SHOOT_TxMessage.RTR = CAN_RTR_DATA;
	SHOOT_TxMessage.DLC = 0x08;
	SHOOT_TxMessage.Data[0] = User_Motor1 >> 8;
	SHOOT_TxMessage.Data[1] = User_Motor1;
	SHOOT_TxMessage.Data[2] = User_Motor2 >> 8;
	SHOOT_TxMessage.Data[3] = User_Motor2;
	SHOOT_TxMessage.Data[4] = User_Motor3 >> 8;
	SHOOT_TxMessage.Data[5] = User_Motor3;
	SHOOT_TxMessage.Data[6] = User_Motor4 >> 8;
	SHOOT_TxMessage.Data[7] = User_Motor4;

	CAN_Transmit(CAN_SHOOT, &SHOOT_TxMessage);
}

//CAN ���� 0x700��ID�����ݣ�������M3508�����������IDģʽ
void CAN_CMD_CHASSIS_RESET_ID(void)
{
	CanTxMsg TxMessage;
	TxMessage.StdId = 0x700;
	TxMessage.IDE = CAN_ID_STD;
	TxMessage.RTR = CAN_RTR_DATA;
	TxMessage.DLC = 0x08;
	TxMessage.Data[0] = 0;
	TxMessage.Data[1] = 0;
	TxMessage.Data[2] = 0;
	TxMessage.Data[3] = 0;
	TxMessage.Data[4] = 0;
	TxMessage.Data[5] = 0;
	TxMessage.Data[6] = 0;
	TxMessage.Data[7] = 0;

	CAN_Transmit(CAN1, &TxMessage);
}

void pm01_cmd_send(uint16_t new_cmd, uint8_t save_flg)
{
	CanTxMsg power_tx_message;
	power_tx_message.StdId = 0x600;
	power_tx_message.IDE = CAN_ID_STD;
	power_tx_message.RTR = CAN_RTR_DATA;
	power_tx_message.DLC = 0x04;
	power_tx_message.Data[0] = new_cmd >> 8;
	power_tx_message.Data[1] = new_cmd & 0xFF;
	power_tx_message.Data[2] = 0x00;
	power_tx_message.Data[3] = (save_flg == 0x01 ? 0x01 : 0x00);

	CAN_Transmit(CAN1, &power_tx_message);
}

void pm01_power_set(uint16_t new_power, uint8_t save_flg)
{
	CanTxMsg power_tx_message;
	power_tx_message.StdId = 0x601;
	power_tx_message.IDE = CAN_ID_STD;
	power_tx_message.RTR = CAN_RTR_DATA;
	power_tx_message.DLC = 0x04;
	power_tx_message.Data[0] = new_power >> 8;
	power_tx_message.Data[1] = new_power;
	power_tx_message.Data[2] = 0x00;
	power_tx_message.Data[3] = (save_flg == 0x01 ? 0x01 : 0x00);

	CAN_Transmit(CAN1, &power_tx_message);
}

void pm01_voltage_set(uint16_t new_voltage, uint8_t save_flg)
{
	CanTxMsg power_tx_message;
	power_tx_message.StdId = 0x602;
	power_tx_message.IDE = CAN_ID_STD;
	power_tx_message.RTR = CAN_RTR_DATA;
	power_tx_message.DLC = 0x04;
	power_tx_message.Data[0] = (new_voltage >> 8);
	power_tx_message.Data[1] = (new_voltage & 0xFF);
	power_tx_message.Data[2] = 0x00;
	power_tx_message.Data[3] = (save_flg == 0x01 ? 0x01 : 0x00);

	CAN_Transmit(CAN1, &power_tx_message);
}

void pm01_current_set(uint16_t new_current, uint8_t save_flg)
{
	CanTxMsg power_tx_message;
	power_tx_message.StdId = 0x603;
	power_tx_message.IDE = CAN_ID_STD;
	power_tx_message.RTR = CAN_RTR_DATA;
	power_tx_message.DLC = 0x04;
	power_tx_message.Data[0] = (new_current >> 8);
	power_tx_message.Data[1] = (new_current & 0xFF);
	power_tx_message.Data[2] = 0x00;
	power_tx_message.Data[3] = (save_flg == 0x01 ? 0x01 : 0x00);

	CAN_Transmit(CAN1, &power_tx_message);
}



void pm01_read(uint32_t id)
{
	CanTxMsg  power_tx_message;
	power_tx_message.StdId = id;     
	power_tx_message.IDE   = CAN_ID_STD;   
	power_tx_message.RTR   = CAN_RTR_REMOTE; 
	power_tx_message.DLC   = 0x08;

	CAN_Transmit(CAN1, &power_tx_message);
}


