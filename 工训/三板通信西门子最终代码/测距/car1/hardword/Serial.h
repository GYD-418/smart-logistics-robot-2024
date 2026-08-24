#ifndef __SERIALR1_H
#define __SERIALR1_H

#include <stdio.h>

void Serial_InitR(void);

uint8_t Serial_GetRxFlag(void);

uint8_t Serial_GetRxData(void);
#endif
