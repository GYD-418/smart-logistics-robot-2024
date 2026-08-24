#ifndef __SERIAL_H
#define __SERIAL_H

#include <stdio.h>

extern uint8_t Serial_hinght_xy[];
extern uint8_t Serial_hinght_xy_last[];

extern uint8_t Serial_RxPacket_3[];
void Serial_Init22(void);
uint8_t RXDATA_X(void);
uint8_t Serial_GetRxFlag_2(void);
extern uint8_t Serial_hinght_erweima[6];

void USART2_IRQHandler(void);




#endif
