#ifndef _CONTROL2_H_
#define _CONTROL2_H_

#include "stm32f10x.h"
#include "control2.h"
#include "pid.h"
#include "HWT101.h"
#include "Motor_Init.h"
#include "niming.h"

extern int ang_flag;//速度闭环全局变量
void Set_ang(void);//开启速度闭环 使用前记着开启
void Ang_cal(void);//循环调用该函数即可闭环 到达固定角度 建议周期15ms以内（陀螺仪串口通信速度也要快） 如果反应不够快速 可增加frame_period
void Ang_cal_1(void);//循环调用该函数即可闭环 到达固定角度 建议周期15ms以内（陀螺仪串口通信速度也要快） 如果反应不够快速 可增加frame_period
void Reset_ang(void);//关闭速度闭环 关闭后即可使用MOTOR_Move_x  一般情况不管定时器应该不会自转
void xy_speed_cal(float x_speeds,float y_speeds);
void xy_speed_cal_1(float x_speeds,float y_speeds);

typedef struct {

	float Target_HL;
	float Target_HR;
	float Target_BL;
	float Target_BR;

}Target;     //电机目标值 归一化量程 0-100


void my_speed_decision(void);                                                         //中断执行函数

void MOTOR_Move_lf(float speed);
void MOTOR_Move_rb(float speed);
void MOTOR_Move_lb(float speed);
void MOTOR_Move_rf(float speed);



#endif //_CONTROL2_H_

