#include "main.h"
#include "stm32f4xx.h"
#include "usart.h"
#include "adc.h"
#include "buzzer.h"
#include "can.h"
#include "delay.h"
#include "laser.h"
#include "led.h"
#include "power_ctrl.h"
#include "sys.h"
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "Remote_Control.h"
#include "start_task.h"
#include "usart2.h"
#include "uart_vofa.h"
#include "vofa_task.h"
#include "mpu6500driver.h"
#include "dac.h"
#include "icm20602.h"
#include "usart_judge.h"
#include "usart6_vision.h"
#include "mag_pwm.h"
#include "can_rec.h"
void BSP_init(void);

int main(void)
{
	BSP_init();
	delay_ms(100);
	pm01_cmd_send(1,1); //��������ʹ��
	delay_ms(100);
	startTast();
	vTaskStartScheduler();
	while (1)
	{
		//ѭ����ֹ�˳�
	}
}

void BSP_init(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4); //�ж��� 4

	delay_init(configTICK_RATE_HZ); //��ʼ���δ�ʱ��

	led_configuration(); //��ˮ�ƣ����̵Ƴ�ʼ��

	laser_configuration(); // stm32 �����¶ȴ�����ADC��ʼ��

	temperature_ADC_init(); //��Դ���ʿ���DAC��ʼ��

	Dac1_Init(); 		
	buzzer_init(30000, 90); //ң������ʼ��
	mpu6500_init();
	remote_control_init(); // CAN�ӿڳ�ʼ��

	CAN1_mode_init(CAN_SJW_1tq, CAN_BS2_2tq, CAN_BS1_6tq, 5, CAN_Mode_Normal);
	CAN2_mode_init(CAN_SJW_1tq, CAN_BS2_2tq, CAN_BS1_6tq, 5, CAN_Mode_Normal);
  SPI4_Init();
	// 24������ƿڳ�ʼ��
	power_ctrl_configuration(); //ӫ�����
	
	USART3_Judge_Config();  // ����3 ���ڶ�ȡ����ϵͳ								
	MAG_PWM_Configuration();											//�����ʼ��,����
	USART6_vision_Configuration(115200);   //�Ӿ� 

	Init_Uart2(115200); // USART2��ʼ�� ����Vofa+��λ��
	Init_Dma_Tx_Config(USART2, (u32)(&USART2->DR), DMA1_Stream6, DMA_Channel_4, (u32)Vofa.message, sizeof(Vofa.message), DMA1_Stream6_IRQn);
}
