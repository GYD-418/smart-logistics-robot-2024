#include "main.h"



PID_Zi_Zhuan PID_all_par;
PID_X PID_X_par;
PID_Y PID_Y_par;



/**				
  * 函    数：1为启动,0为停止
  * 参    数：无
  * 返 回 值：无
  */

void PID_Turn_X(uint8_t STATE)
{
      if (STATE)
            PID_X_par.ENABLE_X = 1;
      else
            PID_X_par.ENABLE_X = 0;
}

void PID_Turn_Y(uint8_t STATE)
{
      if (STATE)
            PID_Y_par.ENABLE_Y = 1;
      else
            PID_Y_par.ENABLE_Y = 0;
}



/**				
  * 函    数：转弯参数初始化
  * 参    数：无
  * 返 回 值：无
  */

void PID_ZI_Para(void)
{								
	PID_all_par.kd=100;			//原来400		
	PID_all_par.ki=0.3;
	PID_all_par.kp=7;			//原来7
	//积分限幅
	PID_all_par.Integral_Max=20;
	PID_all_par.Integral_Min=-20;
	
	PID_all_par.error_sum=0;//累计误差
	PID_all_par.error=0;//误差值
	PID_all_par.last_error=0;//上次误差
	
	PID_all_par.Setpoint=0;//目标值

    PID_all_par.Output_Min=-200;//输出限幅
	PID_all_par.Output_Max=200;
	
	PID_all_par.output=0;

}

/**				
  * 函    数：X轴
  * 参    数：无
  * 返 回 值：无						//近距离好使:0.25,0.05,0
  */									//			 0.22,0.05,0.11

void PID_X_Para(void)
{								
	PID_X_par.ENABLE_X = 0;				//状态
	
	PID_X_par.kp_X =0.35;				//0.16(远距离)
	PID_X_par.ki_X =0.0016;				//0.04(远距离)
	PID_X_par.kd_X =0.06;					//
	
	PID_X_par.error_sum_X =0;
	PID_X_par.error_X =0;
	PID_X_par.last_error_X =0;
	
	PID_X_par.Integral_Max_X =50;
	PID_X_par.Integral_Min_X =-50;
	
	PID_X_par.Output_Max_X =6;
	PID_X_par.output_X =0;
	PID_X_par.Output_Min_X =-6;
	
	PID_X_par.Setpoint_X =X_Value_goal;		//目标值
}


/**				
  * 函    数：Y轴
  * 参    数：无
  * 返 回 值：无
  */

void PID_Y_Para(void)
{								
	PID_Y_par.ENABLE_Y = 0;				//状态
		
	PID_Y_par.kp_Y =0.22;				//0.12(远距离)	0.22,0.05,0.15
	PID_Y_par.ki_Y =0.016;				//0.02(远距离)
	PID_Y_par.kd_Y =0.02;
		
	PID_Y_par.error_sum_Y =0;
	PID_Y_par.error_Y =0;
	PID_Y_par.last_error_Y =0;
		
	PID_Y_par.Integral_Max_Y =20;
	PID_Y_par.Integral_Min_Y =-20;
		
	PID_Y_par.Output_Max_Y =4;
	PID_Y_par.output_Y =0;
	PID_Y_par.Output_Min_Y =-4;
		
	PID_Y_par.Setpoint_Y =Y_Value_goal;			//目标值255,301

}

/*********************************************************************************
**********************************************************************************
									下面为PID的计算
**********************************************************************************
**********************************************************************************/



/**					调用会立即执行
  * 函    数：原地旋转,用于PID算法
  * 参    数：方向：0X01/0X00
  * 返 回 值：无
  */

int PID_speed_count(float now_value)
{
	
		PID_all_par.error=PID_all_par.Setpoint-now_value;//误差值
		PID_all_par.error_sum+=PID_all_par.error;//累计误差
		
		if(PID_all_par.error_sum>PID_all_par.Integral_Max)
		PID_all_par.error_sum=PID_all_par.Integral_Max;
													//积分限幅
		else
			if(PID_all_par.error_sum<PID_all_par.Integral_Min)
		  PID_all_par.error_sum=PID_all_par.Integral_Min;
		
		PID_all_par.output=							//计算
						PID_all_par.kp*PID_all_par.error
								+PID_all_par.ki*PID_all_par.error_sum
										+PID_all_par.kd*(PID_all_par.error-PID_all_par.last_error);

		
			if(PID_all_par.error < 0.12 && PID_all_par.error > -0.12)
			{
				Mortor_Stop();
				Temp[5] = 1;
				return 0;
			}

		PID_all_par.last_error=PID_all_par.error;
			
		return PID_all_par.output;

}

float Error_Max = 1.1;
float Error_Min = -1.1;

/**				
  * 函    数：X轴方向
  * 参    数：
  * 返 回 值：无
  */

int PID_X_count(uint16_t now_value)
{
		PID_X_par.error_X=PID_X_par.Setpoint_X-now_value;//误差值
		PID_X_par.error_sum_X+=PID_X_par.error_X;//累计误差
		
	
		
		if(PID_X_par.error_sum_X>PID_X_par.Integral_Max_X)
		PID_X_par.error_sum_X=PID_X_par.Integral_Max_X;
													//积分限幅
		else
			if(PID_X_par.error_sum_X<PID_X_par.Integral_Min_X)
		  PID_X_par.error_sum_X=PID_X_par.Integral_Min_X;
		
		PID_X_par.output_X=							//计算
						PID_X_par.kp_X*PID_X_par.error_X
								+PID_X_par.ki_X*PID_X_par.error_sum_X
										+PID_X_par.kd_X*(PID_X_par.error_X-PID_X_par.last_error_X);

//		if(PID_X_par.output_X>PID_all_par.Output_Max)
//		PID_X_par.output_X=PID_all_par.Output_Max;
//		else if(PID_X_par.output_X<PID_X_par.Output_Min_X)//输出限幅
//		PID_X_par.output_X=PID_X_par.Output_Min_X;

		PID_X_par.last_error_X=PID_X_par.error_X;
		
		
		if(PID_X_par.error_X < 3.1 && PID_X_par.error_X > -3.1)
		{
			Mortor_Stop();
			Temp[8]++;
			return 0;
		}

			
		return PID_X_par.output_X;
}

/**					调用会立即执行
  * 函    数：原地旋转,用于PID算法
  * 参    数：方向：0X01/0X00
  * 返 回 值：无
  */

int PID_Y_count(uint16_t now_value)
{
		PID_Y_par.error_Y=PID_Y_par.Setpoint_Y-now_value;//误差值
		PID_Y_par.error_sum_Y+=PID_Y_par.error_Y;//累计误差
		
		if(PID_Y_par.error_sum_Y>PID_Y_par.Integral_Max_Y)
		PID_Y_par.error_sum_Y=PID_Y_par.Integral_Max_Y;
													//积分限幅
		else
			if(PID_Y_par.error_sum_Y<PID_Y_par.Integral_Min_Y)
		  PID_Y_par.error_sum_Y=PID_Y_par.Integral_Min_Y;
		
		PID_Y_par.output_Y=							//计算
						PID_Y_par.kp_Y*PID_Y_par.error_Y
								+PID_Y_par.ki_Y*PID_Y_par.error_sum_Y
										+PID_Y_par.kd_Y*(PID_Y_par.error_Y-PID_Y_par.last_error_Y);

		
		PID_Y_par.last_error_Y=PID_Y_par.error_Y;
		
		if(PID_Y_par.error_Y < 3.1 && PID_Y_par.error_Y > -3.1)
		{
//			Mortor_Stop();
			Temp[10]++;
			return 0;
		}

		return PID_Y_par.output_Y;
	
}

