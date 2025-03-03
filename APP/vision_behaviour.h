#ifndef _VISION_BEHAVIOUR__H_
#define _VISION_BEHAVIOUR__H_

#include "main.h"


#include "gimbal_behaviour.h"
typedef struct
{
	fp32 x;
	fp32 y;
//	uint8_t x_integer;
//	uint8_t x_float;
//	uint8_t y_integer;
//	uint8_t y_float;
//	uint16_t fire_flag;
	
}VisionTypedef;

extern VisionTypedef VisionStructure;

extern void vision_updata(VisionTypedef *Structure, uint16_t *buff);

#endif




