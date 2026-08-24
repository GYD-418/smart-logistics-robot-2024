#ifndef __MORTOR_H
#define __MORTOR_H
#include "stm32f4xx.h"                




extern uint8_t RX_Flag;

void Deng_Dai_Init(void);


void Mortor_Zhi_Xing_Speed_1(int8_t speed);
void Mortor_Straight_slow(int16_t Mai_Chong);
void Mortor_5_Stop(void);				
void Mortor_5_Start(void);
void Mortor_Heng_Yi_Speed(int8_t speed);
void Mortor_Zhi_Xing_Speed(int8_t speed_x,int8_t speed_y);
void Mortor_Heng_Yi(int16_t Mai_Chong);
void Mortor_5(uint8_t dir,uint16_t Mai_Chong);

void Mortor_Init(void);
void Serial_SendByte_USART1(uint8_t Byte);
void Mortor_Start(void);
void Mortor_Stop(void);											//停止

void Mortor_Zhuan_PID(uint8_t dir);								//转90度，带PID算法
void Mortor_Zi_Zhuan_dir(uint8_t dir);							//自转90°
void Mortor_Zi_Zhuan_speed(int16_t speed);						//自转
void Mortor_Straight(int16_t Mai_Chong);						//直走


#endif
