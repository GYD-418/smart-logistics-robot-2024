#ifndef __SERIAL_H
#define __SERIAL_H

#include <stdio.h>


extern uint8_t Serial_RxPacket_2[];
void Serial_Init22(void);
uint8_t RXDATA_X(void);
uint8_t Serial_GetRxFlag_2(void);

void USART2_IRQHandler(void);




#endif
