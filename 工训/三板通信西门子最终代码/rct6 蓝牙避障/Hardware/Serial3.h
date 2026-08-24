#ifndef __SERIALR3_H
#define __SERIALR3_H

void Serial_Init3(void);

extern uint8_t uart_rxpb3[];
extern uint8_t uart_txpb3[];
extern uint8_t Serial_RxData3;
extern uint8_t Serial_RxFlag3;
void Serial_SendByte3(uint8_t Byte);
uint8_t Serial_GetRxFlag3(void);
uint8_t Serial_GetRxData3(void);

#endif
