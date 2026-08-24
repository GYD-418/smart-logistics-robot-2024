#ifndef _TIM_H_
#define _TIM_H_

#include "stm32f10x.h"

void Basic_TIM_Init(int arr, int psc);
void TIM5_Init(int arr, int psc);
extern int test_count;
extern int c;

#endif //_TIM_H_


