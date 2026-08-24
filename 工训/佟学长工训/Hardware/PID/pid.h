#ifndef _PID_H_
#define _PID_H_

#include "stm32f10x.h"


#define kp_a_1    5.0f
#define ki_a_1    0.0f
#define kd_a_1    0.0f

#define kp_a_2    5.5f
#define ki_a_2    0.008f
#define kd_a_2    3.0f

//结构体变量
typedef struct
{
	float KP;
	float KI;
	float KD;
	
	float Setpoint;      //目标值
//	float ActulValue;    //实际值
	
	int error;         //Error[0]
	int last_error;    //Error[-1]
	int prev_error;    //Error[-2]
	
	int Sumerror;      //误差累积
	int Integral_Max;  //积分限幅
	int Output;
	int Output_Max;    //输出限幅
	
}PID_TypeDef;


typedef struct {
	volatile double SetPoint;            //设定值
	double Kp;                  //比例系数
	double Ki;                  //积分系数
	double Kd;                  //微分系数
	double LastError;           //最后一次误差数Er[-1]
	double PrevError;           //最后第二次误差数er[-2]
	double SumError;            //误差积分  

    double error;
    float i_max; //integrator_max
    float p_max; //integrator_max
    float d_max; //integrator_max
    float low_pass;
    float out_p;
    float out_i;
    float out_d;
}PID;


void PID_CREATE(PID *pp, double _kp, double _ki, double _kd, double _low_pass, float max_p, float max_i, float max_d);
float pid_solve(PID *pid, float error);
extern PID K210x_pid;//K210循迹
extern PID K210y_pid;//K210循迹


extern PID_TypeDef Angle_PID;
void PID_Init_Angle(void);
void PID_Init_Angle_1(void);
int32_t PID_Calculate_Ang(PID_TypeDef*PID,int FeedBack_Value);


#endif // _PID_H_
