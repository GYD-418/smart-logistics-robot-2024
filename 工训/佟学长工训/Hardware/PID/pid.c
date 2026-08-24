#include "pid.h"

//角度环pid

PID_TypeDef Angle_PID;
PID K210x_pid;//K210循迹
PID K210y_pid;//K210循迹
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MINMAX(input, low, upper) MIN(MAX(input, low), upper)




void PID_Init_Angle(void)
{
	Angle_PID.KP = kp_a_1;     
	Angle_PID.KI = ki_a_1;   
	Angle_PID.KD = kd_a_1;
	Angle_PID.Setpoint =0;
	Angle_PID.error = 0;
	Angle_PID.last_error = 0;
	Angle_PID.Sumerror = 0;
	Angle_PID.Output = 0;
	Angle_PID.Output_Max = 6000;
	Angle_PID.Integral_Max = 5000;
}
void PID_Init_Angle_1(void)
{
	Angle_PID.KP = kp_a_2;     
	Angle_PID.KI = ki_a_2;   
	Angle_PID.KD = kd_a_2;
	Angle_PID.Setpoint =0;
	Angle_PID.error = 0;
	Angle_PID.last_error = 0;
	Angle_PID.Sumerror = 0;
	Angle_PID.Output = 0;
	Angle_PID.Output_Max = 6000;
	Angle_PID.Integral_Max = 1000;
}
void PID_CREATE(PID *pp, double _kp, double _ki, double _kd, double _low_pass, float max_p, float max_i, float max_d)
{
    pp->Kp = _kp;
    pp->Ki = _ki;
    pp->Kd = _kd;
    pp->low_pass = _low_pass;
    pp->out_p = 0;
    pp->out_i = 0;
    pp->out_d = 0;
    pp->p_max = max_p;
    pp->i_max = max_i;
    pp->d_max = max_d;
}


// 常规PID
float pid_solve(PID *pid, float error)
{
		
    pid->out_d = (error - pid->out_p) * pid->low_pass + pid->out_d * (1 - pid->low_pass);
    pid->out_p = error;
    pid->out_i += error;

    if (pid->Ki != 0) pid->out_i = MINMAX(pid->out_i, -pid->i_max / pid->Ki, pid->i_max / pid->Ki);

    return pid->Kp * pid->out_p + pid->Ki * pid->out_i + pid->Kd * pid->out_d;
}
/* 速度闭环的位置式PID */
double PIDCalc(PID *pp, double NextPoint)   
{
	double dError,                              /* 当前微分 */
				 Error;                               /* 偏差 */
	Error = pp->SetPoint - NextPoint;           /* 偏差值=设定值-输入值（当前值）*/
	
	/* 积分限幅 */
	if(pp->SumError > 4000) pp->SumError = 400;
	if(pp->SumError < -4000) pp->SumError = -400;
	
	pp->SumError += Error;                      /* 积分=积分+偏差  --偏差的累加 */
	dError = pp->LastError - pp->PrevError;     /* 当前微分 = 最后误差 - 之前误差 */
	pp->PrevError = pp->LastError;              /* 更新“之前误差” */
	pp->LastError = Error;                      /* 更新“最后误差” */
	return (pp->Kp * Error                      /* 比例项 = 比例常数 * 偏差 */
			+   pp->Ki *  pp->SumError              /* 积分项 = 积分常数 * 误差积分 */
			+   pp->Kd * dError                     /* 微分项 = 微分常数 * 当前微分 */
				 );
}



int32_t PID_Calculate_Ang(PID_TypeDef*PID,int FeedBack_Value)               
{
	Angle_PID.error = Angle_PID.Setpoint - FeedBack_Value;
//	if(Angle_PID.error == 0) Angle_PID.Sumerror = 0;

	Angle_PID.Sumerror += Angle_PID.error;
	//积分限幅
	if(Angle_PID.Sumerror > Angle_PID.Integral_Max) Angle_PID.Sumerror = Angle_PID.Integral_Max; 
	else if(Angle_PID.Sumerror < (-Angle_PID.Integral_Max)) Angle_PID.Sumerror = (-Angle_PID.Integral_Max);
	//输出计算
	Angle_PID.Output  = Angle_PID.KP * Angle_PID.error                              //比例
										+ Angle_PID.KI * Angle_PID.Sumerror                           //积分
										+ Angle_PID.KD * (Angle_PID.error - Angle_PID.last_error);    //微分
	//输出限幅
	if(Angle_PID.Output > Angle_PID.Output_Max) Angle_PID.Output = Angle_PID.Output_Max;
	else if(Angle_PID.Output < (-Angle_PID.Output_Max)) Angle_PID.Output = (-Angle_PID.Output_Max);
	//误差更新
	Angle_PID.last_error = Angle_PID.error;
	return (int32_t)Angle_PID.Output;
}
