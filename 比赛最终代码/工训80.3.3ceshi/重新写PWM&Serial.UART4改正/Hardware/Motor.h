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
void Motor_go_HQ(int count);
void Motor_go_ZY(int count);
void Motor_conter(float conter,float mumax,float mumin,int a,int b,int c,int d,int e);
void xiansgu1(void);    //分段矫正
void xiansgu2(void);
extern uint8_t HV_DE[6];
extern int STS;
void xiansgu3(void);    //分段矫正  左小
extern int Cunt1,Cunt2;
extern int Cunt;
extern int open_CD;
extern int ard;
extern int aa;
extern int HUYNT;
extern int av,bv,cv,dv;
extern int aakk;
#endif
