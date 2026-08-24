#ifndef __PID_ZI_ZHUAN_H
#define __PID_ZI_ZHUAN_H
#include "stm32f4xx.h"               

//此结构体用于转90度
typedef struct
{
	float kp;
	float ki;
	float kd;
	
	int Setpoint;//目标值
	
	float error;//误差
	float error_sum;//累计误差
	float last_error;//上次误差
	
	int Integral_Max;  //积分限幅
	int Integral_Min;
	
	int Output_Max;    //输出限幅
	int Output_Min;
	int16_t output;
	
}PID_Zi_Zhuan;


//此结构体用于色环校正X
typedef struct
{
	uint8_t ENABLE_X;
	
	float kp_X;
	float ki_X;
	float kd_X;
	
	float Setpoint_X;//目标值
	
	int error_X;//误差
	int error_sum_X;//累计误差
	int last_error_X;//上次误差
	
	int Integral_Max_X;  //积分限幅
	int Integral_Min_X;
	
	int Output_Max_X;    //输出限幅
	int Output_Min_X;
	int16_t output_X;
	
}PID_X;


//此结构体用于色环校正Y
typedef struct
{
	uint8_t ENABLE_Y;
	
	float kp_Y;
	float ki_Y;
	float kd_Y;
	
	float Setpoint_Y;//目标值
	
	int error_Y;//误差
	int error_sum_Y;//累计误差
	int last_error_Y;//上次误差
	
	int Integral_Max_Y;  //积分限幅
	int Integral_Min_Y;
	
	int Output_Max_Y;    //输出限幅
	int Output_Min_Y;
	int16_t output_Y;
	
}PID_Y;



extern PID_Zi_Zhuan PID_all_par;
extern PID_X PID_X_par;
extern PID_Y PID_Y_par;
extern float Error_Max;
extern float Error_Min;



void PID_Turn_X(uint8_t STATE);
void PID_Turn_Y(uint8_t STATE);

void PID_X_Para(void);
void PID_Y_Para(void);
void PID_ZI_Para(void);
int PID_speed_count(float now_value);
int PID_X_count(uint16_t now_value);
int PID_Y_count(uint16_t now_value);


#endif
