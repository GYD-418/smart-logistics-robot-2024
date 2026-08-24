#ifndef __SERIAL2_H
#define __SERIAL2_H

#include <stdio.h>


extern uint8_t Serial2_RxPacket_2[];
void Serial2_Init(void);
uint8_t Serial2_GetRxFlag(void);

void USART2_IRQHandler(void);




#endif
