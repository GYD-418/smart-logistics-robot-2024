#ifndef __HWT101_H
#define __HWT101_H

#include "stm32f4xx.h"                  // Device header


void Serial_SendByte_USART3(uint8_t Byte);
void USART3_Init(unsigned long baudrate);
void UART3_Put_Char(unsigned char DataToSend);
void UART3_Put_String(unsigned char *Str);

struct SAcc
{
	short a[3];
	short T;
};
struct SGyro
{
	short w[3];
	short T;
};
struct SAngle
{
	short Angle[3];
	short T;
};

//struct SAcc 		stcAcc;
//struct SGyro 		stcGyro;
//struct SAngle 	stcAngle;
void Usart_SendHalfWord(USART_TypeDef * pUSARTx, uint16_t ch);
void MyUSART_SendArr(uint16_t *str,uint8_t count);
void sendcmd(char cmd[]);

#endif


