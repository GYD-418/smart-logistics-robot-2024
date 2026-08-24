#ifndef __TIM_PID_XUAN_H
#define __TIM_PID_XUAN_H
#include "stm32f4xx.h"               







extern uint8_t TIM1_Flag;

void TIM_PID_XUAN(void);
void delay(u16 num);
void TIM2_PID_X(void);
void TIM3_PID_Y(void);

#endif
