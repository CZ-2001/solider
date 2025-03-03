#include "vision_behaviour.h"

VisionTypedef VisionStructure;

void vision_updata(VisionTypedef *Structure, uint16_t *buff)
{
	static uint16_t xrr,yrr;
	static uint16_t Xrr,Yrr;
	
//BUFF[5]  0000   0000    最低为 Y的符号位
//  第二位 为X的符号位
	xrr=(buff[1]<<8)| buff[2] ;
	yrr=(buff[3]<<8)| buff[4] ;
	
	if(buff[5]&0x02)   //0000  0010 
		Xrr=(xrr*1.0)+360.0;  // 0-720
	else if((buff[5]&0x02)==0)
		Xrr=-(xrr*1.0)+360.0; 
	if(buff[5]&0x01)
		Yrr=(yrr*1.0)+360;
	else if((buff[5]&0x01)==0)
		Yrr=-(yrr*1.0)+360;
	Structure->x = Xrr/4.0 ;
	Structure->y = Yrr/4.0 ;
}
