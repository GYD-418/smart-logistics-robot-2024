#ifndef __PWM_H
#define __PWM_H
#include "math.h"
#define TIM_PERIOD                   0xFFFF
//void PWM_Init(int ARR , int PSC , int CCR);
void PWM_Init(void);
void PWM_Init1(void);
void PWM_Init2(void);
void PWM_Init3(void);

void PWM_ENABLE(void);
void PWM_DISABLE(void);
void MOTOR_GPIO_Init(void);

#endif
