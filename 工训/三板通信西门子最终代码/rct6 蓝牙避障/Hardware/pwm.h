#ifndef __PWM_H
#define __PWM_H

#include <stdio.h>

void PWM_Init(int ARR , int PSC , int CCR);  //PWM≥ı ºªØ
void Motor_Init(void);
void Motor_SetSpeed(int8_t Speed);
void PWM_ENABLE(void);
void PWM_DISABLE(void);

#endif
