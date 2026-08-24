#ifndef __SERIAL4_H
#define __SERIAL4_H

void Serial4_Init(void);
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
