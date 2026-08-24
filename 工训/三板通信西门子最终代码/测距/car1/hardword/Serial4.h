#ifndef __SERIALR3_H
#define __SERIALR3_H

void Serial_Init3(void);

void Serial_Init3(void);
extern uint8_t uart_rxpb[];
extern uint8_t uart_txpb[];
extern uint8_t Serial_RxData222;
extern uint8_t Serial_RxFlag222;
//void send_hex();
//void Serial_SendByte();
//void Serial_SendArray();
uint8_t Serial_GetRxFlag3(void);

#endif
