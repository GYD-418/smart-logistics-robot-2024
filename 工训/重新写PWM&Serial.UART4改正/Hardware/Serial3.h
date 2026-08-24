#ifndef __SERIAL3_H
#define __SERIAL3_H

#include <stdio.h>

void Serial3_Init(void);
void Serial3_SendByte(uint8_t Byte);
uint8_t Serial3_GetRxFlag(void);
uint8_t Serial3_GetRxData(void);
void USART3_IRQHandler(void);

#endif
