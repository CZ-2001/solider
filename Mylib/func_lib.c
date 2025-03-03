#include "func_lib.h"

void limit_fp32(fp32* value,fp32 max,fp32 min)
{
	if			(*value>max)*value=max;
	else if	(*value<min)*value=min;
}

void limit_uint16_t(uint16_t* value,uint16_t max,uint16_t min)
{
	if			(*value>max)*value=max;
	else if	(*value<min)*value=min;
}

void limit_int16_t(int16_t* value,int16_t max,int16_t min)
{
	if			(*value>max)*value=max;
	else if	(*value<min)*value=min;
}

void cir_limit_fp32(fp32* value,fp32 cir_begin,fp32 cir_end)
{
	const fp32 T = cir_end-cir_begin;
	if(*value > (cir_end+1)   )	*value -= T;
	if(*value < cir_begin ) *value += (T+1);
}






