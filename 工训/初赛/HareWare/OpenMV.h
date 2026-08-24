#ifndef __OPENMV_H
#define __OPENMV_H
#include "stm32f4xx.h"              


extern uint8_t Wu_Liao_Color;
extern uint16_t X ;
extern uint16_t Y ;




void Serial_SendByte_UART4(uint8_t Byte);

void OpenMV_Init(void);
#endif
