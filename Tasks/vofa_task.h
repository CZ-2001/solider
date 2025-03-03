#ifndef VOFA_TASK_H
#define VOFA_TASK_H
#define channel 6
#include "main.h"

typedef struct
{
	uint8_t message[(channel + 1) * 4];  // 数据 = 通道数 * （每组数据的字节数（4） +  识别位（4））
	uint8_t temp[4];  //用于缓存数据
	uint8_t tail[4];  //识别位

}Vofa_t;

typedef union  
{  
    float float_data;  
    unsigned long long_data;  

}FloatLongType;


void Vofa_tail_init(void);
void VofaTask(void *pvParameters);
extern Vofa_t Vofa;
#endif






