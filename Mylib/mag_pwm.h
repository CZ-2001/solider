#ifndef __MAG__PWM_H__
#define __MAG__PWM_H__

extern void MAG_PWM_Configuration(void);

#define LASER_ON()   GPIO_SetBits(GPIOG, GPIO_Pin_13)
#define LASER_OFF()  GPIO_ResetBits(GPIOG, GPIO_Pin_13)

#endif





