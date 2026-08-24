#ifndef __SERIAL1_H
#define __SERIAL1_H

#include <stdio.h>

extern uint16_t Serial_TxPacket[];   //数组声明时 数量可以不要
extern uint16_t Serial_RxPacket[];
extern uint8_t Cx,Cy;
extern uint8_t RxData;

void Serial1_Init(void);
void Serial1_SendByte(uint8_t Byte);
void Serial1_SendArray(uint8_t *Array, uint16_t Length);
void Serial1_SendString(char *String);
void Serial1_SendNumber(uint32_t Number, uint8_t Length);
void Serial1_Printf(char *format, ...);

void Serial_SendPacket(void);
uint8_t Serial_GetRxFlag(void);

#endif
