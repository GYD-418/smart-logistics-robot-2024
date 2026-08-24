#ifndef __SERIAL3_H
#define __SERIAL3_H

#include <stdio.h>

void Serial_Init3(void);
void Serial_SendByte3(uint8_t Byte);
uint8_t Serial_GetRxFlag3(void);
uint8_t Serial_GetRxData3(void);
void USART3_IRQHandler(void);

#endif
