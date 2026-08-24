#ifndef __SERIAL2_H
#define __SERIAL2_H

#include <stdio.h>

void Serial_Init2(void);
extern uint8_t uart_rxpa2[];
extern uint8_t uart_txpa2[];
extern uint8_t Serial_RxData2;
extern uint8_t Serial_RxFlag2;
uint8_t Serial_GetRxFlag2(void);

void Serial_SendByte2(uint8_t Byte);
void Serial_SendArray2(uint8_t *Array, uint16_t Length);

void send_hex2(void);
#endif
