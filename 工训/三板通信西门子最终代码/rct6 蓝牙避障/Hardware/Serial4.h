#ifndef __SERIAL4_H
#define __SERIAL4_H

#include <stdio.h>

extern uint8_t Serial_TxPacket_4[];				//FF 01 02 03 04 FE

extern uint8_t Serial_RxPacket_4[];
void Serial_Init4(void);
void Serial_SendByte4(uint8_t Byte);

void Serial_SendArray4(uint8_t *Array, uint16_t Length);  //串口发送数组  数组发送需要指针

uint8_t Serial_GetRxFlag_4(void);
void Serial_SendPacket4(void);



#endif
