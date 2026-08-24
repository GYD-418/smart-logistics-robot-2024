#ifndef __USART2_H
#define __USART2_H
#include "stm32f4xx.h"                  // Device header





extern uint8_t Flag_OpenMV;
extern uint8_t Receive_OpenMV[6];





void USART2_send(void);
extern uint8_t Recevie[10];
void open_mv_send(void);
void USART2_Init(void);


#endif
