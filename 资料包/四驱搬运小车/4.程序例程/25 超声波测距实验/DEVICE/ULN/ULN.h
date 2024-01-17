
#ifndef __ULN_H
#define __ULN_H	 
#include "sys.h"

#define Trig PCout(1)

extern unsigned int Distance1;
extern unsigned int Distance2;

void Uln_init(void);//外部中断初始化
void Uln_Trig(void);
#endif







