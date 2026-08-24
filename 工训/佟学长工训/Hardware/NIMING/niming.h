#ifndef _NIMING_H_
#define _NIMING_H_


#include "stm32f10x.h"


#define BYTE0(dwTemp)   (*(char*)(&dwTemp))
#define BYTE1(dwTemp)   (*((char*)(&dwTemp)+1))
#define BYTE2(dwTemp)   (*((char*)(&dwTemp)+2))
#define BYTE3(dwTemp)   (*((char*)(&dwTemp)+3))	

void ANODT_SendF1_1(int a);
void ANODT_SendF1_2(int a);
void ANODT_SendF1_3(int a);
void ANODT_SendF1_4(int a);
void ANODT_SendF1_5(int a);
void ANODT_SendF1_6(int a);
void ANODT_SendF1_7(int a);
void ANODT_SendF1_8(int a);
void ANODT_SendF1_9(float a);
void ANODT_SendF1_10(float a);



#endif //_NIMING_H_

