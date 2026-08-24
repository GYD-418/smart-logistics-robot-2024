#ifndef __SERIAL5_H
#define __SERIAL5_H

#include <stdio.h>

void Serial_Init5(void);
void Serial_SendByte5(uint8_t Byte);
void Serial_SendArray5(uint8_t *Array, uint16_t Length);
void Serial_SendString5(char *String);
void Serial_SendNumber5(uint32_t Number, uint8_t Length);
void Serial_Printf5(char *format, ...);

#endif
