#include "function.h"

void Limit_Max(uint16_t *x, fp32 Max)
{
	if (*x >= Max)
		*x = Max;
}

void Limit_Max_Min(fp32 *x, fp32 Max, fp32 Min)
{
	if (*x >= Max)
		*x = Max;
	if (*x <= Min)
		*x = Min;
}

void Limit_Slove_Zero(fp32 *x, fp32 Max, fp32 Min)
{

	const fp32 T = Max - Min; // 8191

	if (*x > (Max + 1))
		*x -= T;
	if (*x < Min)
		*x += (T + 1);
	//
	//		if(*x>Max) *x-=Max;
	//	if(*x<Min)*x+=Max+1;
}
