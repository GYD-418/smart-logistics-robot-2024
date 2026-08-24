#ifndef __HWT101_H_
#define __HWT101_H_
#include "stm32f10x.h"                  // Device header



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

extern float yaw;            
extern struct SAngle 	stcAngle;




#endif


