#ifndef __ACTION_H
#define __ACTION_H
#include "stm32f4xx.h"               





extern uint16_t xuan_xiao;
extern uint16_t xuan_da ;
extern uint8_t Last_color;
extern uint8_t color;
extern uint8_t Main_Mission;

void Jiao_Zheng2(void);
void Jia_QU_shou(void);
void Jiao_Zheng(void);				//调用，即可进行XY轴的校正
void Jia_QU_fang(void);	
void Jia_QU_Pan(void);				//夹取的动作组，是载物盘那块的动作
void Define_Wu_Liao(void);			//转盘夹取物块时判断
void Main_Action(void);
void Jia_QU(void);					//夹取的动作组，平地上的动作




#endif
