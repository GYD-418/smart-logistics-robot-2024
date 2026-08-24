#ifndef __SERIAL_H
#define __SERIAL_H

#include <stdio.h>

extern uint8_t Serial_hinght_xy[];
extern uint8_t Serial_hinght_xy_last[];
extern uint8_t Serial_rbg_wulaio[2];

extern uint8_t Serial_RxPacket_3[];
void Serial_Init22(void);
uint8_t RXDATA_X(void);
uint8_t Serial_GetRxFlag_2(void);
extern uint8_t Serial_hinght_erweima[];
void Serial_SendByte2(uint8_t Byte);

void USART2_IRQHandler(void);
int fputc2(int ch, FILE *f);
void Serial_SendString2(char *String);
void Serial_Printf2(char *format, ...);




#endif
