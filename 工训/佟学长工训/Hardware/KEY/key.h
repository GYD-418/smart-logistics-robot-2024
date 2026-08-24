#ifndef _KEY_H_
#define _KEY_H_

#include "stm32f10x.h"

#define KEY1  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13)//¶ÁÈ¡°´¼ü1

extern int k ;


void KEY_Init(void);
void Key_Scan(void);
//void My_delay(unsigned int  m);		//ÑÓÊ±

#endif //_KEY_H_
