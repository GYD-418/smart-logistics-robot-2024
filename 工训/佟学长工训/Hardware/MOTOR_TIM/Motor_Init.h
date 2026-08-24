#ifndef __Motor_Init_H
#define __Motor_Init_H
#include "stm32f10x.h"                  // Device header
#include "math.h"
#include "Motor.h"  



//输出比较模式周期设置为0xFFFF
#define TIM_PERIOD                   0xFFFF



void MOTOR_GPIO_Init(void);
void MOTOR_TIM_Init(void);
void MOTOR5_GPIO_Init(void); //抓取结构的步进电机
void MOTOR5_TIM2_Init(void);

extern int test;

#endif

