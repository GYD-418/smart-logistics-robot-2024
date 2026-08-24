#ifndef __SERIAL2_H
#define __SERIAL2_H

#include <stdio.h>

void Serial_Init(void);
extern uint8_t uart_rxpa[];
extern uint8_t uart_txpa[];
extern uint8_t Serial_RxData;
extern uint8_t Serial_RxFlag;
//void send_hex();
//void Serial_SendByte();
//void Serial_SendArray();
uint8_t Serial_GetRxFlag(void);
#endif
