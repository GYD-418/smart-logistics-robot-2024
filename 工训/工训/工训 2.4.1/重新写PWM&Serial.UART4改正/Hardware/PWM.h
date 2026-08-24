#ifndef __PWM_H
#define __PWM_H
//void PWM_Init(int ARR , int PSC , int CCR);
void PWM_Init(int ARR , int PSC , int CCR);
void PWM_Init1(int ARR , int PSC , int CCR);
void PWM_Init2(int ARR , int PSC , int CCR);
void PWM_Init3(int ARR , int PSC , int CCR);

void PWM_ENABLE(void);
void PWM_DISABLE(void);

#endif
