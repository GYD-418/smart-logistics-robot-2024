#ifndef __DELAY_H_
#define __DELAY_H_
#include "sys.h"  

void delay_init(void);
void delay_ms(__IO uint32_t  nTime);
void delay(u16 num);
void TimingDelay_Decrement(void);

#endif





