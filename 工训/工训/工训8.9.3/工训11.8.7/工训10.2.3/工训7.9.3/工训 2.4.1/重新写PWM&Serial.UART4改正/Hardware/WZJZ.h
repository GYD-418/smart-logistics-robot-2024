#ifndef __WZJZ_H
#define __WZJZ_H

//float WeiZhi_Heng_PID(float MUB1 ,float SHJ1);    //目标值是实际点与中心点误差为0
float WeiZhi_Zong_PIDL(float MUB2 ,float SHJ2);    //目标值是实际点与中心点误差为0
void Key_Init(void);
uint8_t Key_GetNum(void);
float WeiZhi_Zong_PIDR(float MUB2 ,float SHJ2);    //目标值是实际点与中心点误差为0

#endif

