#ifndef __SERIAL_H
#define __SERIAL_H

#include <stdio.h>

extern uint16_t Serial_TxPacket[];   //数组声明时 数量可以不要
extern uint16_t Serial_RxPacket[];
extern uint8_t Cx,Cy;
extern uint8_t RxData;

void Serial_Init(void);
void Serial_SendByte(uint8_t Byte);
void Serial_SendArray(uint8_t *Array, uint16_t Length);
void Serial_SendString(char *String);
void Serial_SendNumber(uint32_t Number, uint8_t Length);
void Serial_Printf(char *format, ...);

void Serial_SendPacket(void);
uint8_t Serial_GetRxFlag(void);

#endif
