#ifndef __MOTOR_H
#define __MOTOR_H
#include "T_BuJin_Sp.h"
#include "math.h"

void Motor_Init(void);
void Motor_InitD(void);
void Motor_SetSpeed(int8_t Speed);
void Motor_SetSpeed1(int8_t Speed);
void Motor_SetSpeed2(int8_t Speed);
void Motor_SetSpeed3(int8_t Speed);
void Motor_Enable_1(void);
void Motor_Enable_2(void);
void Motor_go(int count);
void Motor_conter(float conter,float mumax,float mumin,int x,int a,int b,int c,int d);
void xiansgu1(void);    //·Ö¶Î½ÃÕý
void xiansgu2(void);
extern int BCCTR;
extern int BCDF;
extern uint8_t HV_DE[6];
void PCS(int art);

#endif
