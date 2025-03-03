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
	pm01_cmd_send(1,1); //超级电容使能
	delay_ms(100);
	startTast();
	vTaskStartScheduler();
	while (1)
	{
		//循环防止退出
	}
}

void BSP_init(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4); //中断组 4

	delay_init(configTICK_RATE_HZ); //初始化滴答时钟

	led_configuration(); //流水灯，红绿灯初始化

	laser_configuration(); // stm32 板载温度传感器ADC初始化

	temperature_ADC_init(); //电源功率控制DAC初始化

	Dac1_Init(); 		
	buzzer_init(30000, 90); //遥控器初始化
	mpu6500_init();
	remote_control_init(); // CAN接口初始化

	CAN1_mode_init(CAN_SJW_1tq, CAN_BS2_2tq, CAN_BS1_6tq, 5, CAN_Mode_Normal);
	CAN2_mode_init(CAN_SJW_1tq, CAN_BS2_2tq, CAN_BS1_6tq, 5, CAN_Mode_Normal);
  SPI4_Init();
	// 24输出控制口初始化
	power_ctrl_configuration(); //荧光充能
	
	USART3_Judge_Config();  // 串口3 用于读取裁判系统								
	MAG_PWM_Configuration();											//舵机初始化,弹仓
	USART6_vision_Configuration(115200);   //视觉 

	Init_Uart2(115200); // USART2初始化 用于Vofa+上位机
	Init_Dma_Tx_Config(USART2, (u32)(&USART2->DR), DMA1_Stream6, DMA_Channel_4, (u32)Vofa.message, sizeof(Vofa.message), DMA1_Stream6_IRQn);
}
