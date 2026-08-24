#include "control2.h"
#include "pid.h"
#include "HWT101.h"
#include "Motor_Init.h"
#include "niming.h"
#include <math.h>
#include "control.h"

#define frame_period 10  
int ang_flag;//速度闭环全局变量
Target target;
void motor_init() //第一次调用需要初始化  最好放在轮子输出的后面 防止系统不稳定
{
	int now_conter=0;
	now_conter = TIM_GetCounter(TIM3);																				//获取计数值		
	
				TIM_SetCompare1(TIM3,now_conter+10);												//设置定时器比较值 
				TIM_ITConfig(TIM3,TIM_IT_CC1,ENABLE);																		//使能定时器通道 
				TIM_CCxCmd(TIM3,TIM_Channel_1,TIM_CCx_Enable);

				TIM_SetCompare2(TIM3,now_conter+10);												//设置定时器比较值 
				TIM_ITConfig(TIM3,TIM_IT_CC2,ENABLE);																		//使能定时器通道 
				TIM_CCxCmd(TIM3,TIM_Channel_2,TIM_CCx_Enable);

				TIM_SetCompare3(TIM3,now_conter+10);												//设置定时器比较值 
				TIM_ITConfig(TIM3,TIM_IT_CC3,ENABLE);																		//使能定时器通道 
				TIM_CCxCmd(TIM3,TIM_Channel_3,TIM_CCx_Enable);

				TIM_SetCompare4(TIM3,now_conter+10);												//设置定时器比较值 
				TIM_ITConfig(TIM3,TIM_IT_CC4,ENABLE);																		//使能定时器通道 
				TIM_CCxCmd(TIM3,TIM_Channel_4,TIM_CCx_Enable);
	
	
				TIM_Cmd(TIM3, ENABLE);	
}
int delay_11 = 0;//轮子的延迟值
int delay_12 = 0;
int delay_13 = 0;
int delay_14 = 0;
/**
  * @brief          第一个轮子的控制，通过速度转化为延迟值，之后中断中调用对比值，速度越大延迟值越小，频率越快。
  * @author         Nirvana
  * @param[in]      速度大小（直接输入你的PID值即可）
  * @retval         返回空
  */
void MOTOR_Move_rb(float speed)
{
    if(speed < 0)   																													 //逆时针
			{
				GPIO_SetBits(GPIOC,GPIO_Pin_0);																		 //PA13为方向引脚，输出高电平,得试
				speed = -speed;      
			}		
		else if(speed > 0)   																																	 //顺时针
			{
				GPIO_ResetBits(GPIOC,GPIO_Pin_0);																	 //PA13为方向引脚，输出低电平		
			}
		if(speed==0)
		{
				delay_11=-2;//最大周期的旋转，后面可以让他倒转进一步降低误差
				if(GPIO_ReadOutputDataBit(GPIOC,GPIO_Pin_0))//nir:翻转 下同
				GPIO_ResetBits(GPIOC,GPIO_Pin_0);	
				else
				GPIO_SetBits(GPIOC,GPIO_Pin_0);										
		}
		else delay_11 = (int32_t)(A_T_x10/speed);//速度归一化 把速度转为延时的时间

}
void MOTOR_Move_rf(float speed)
{
        if(speed < 0)   																													 //逆时针
			{
				speed = -speed;      
				GPIO_SetBits(GPIOC,GPIO_Pin_1);																		 //PA13为方向引脚，输出高电平,得试
			}		
		else if(speed > 0)   																																	 //顺时针
			{
				GPIO_ResetBits(GPIOC,GPIO_Pin_1);																	 //PA13为方向引脚，输出低电平		
			}
            if(speed==0)
            {
                delay_12=-2;//最大周期的旋转，后面可以让他倒转进一步降低误差
														if(GPIO_ReadOutputDataBit(GPIOC,GPIO_Pin_1))
							GPIO_ResetBits(GPIOC,GPIO_Pin_1);	
							else
							GPIO_SetBits(GPIOC,GPIO_Pin_1);	

            }
            else
            delay_12 = (int32_t)(A_T_x10/speed);//速度归一化 把速度转为延时的时间

}
void MOTOR_Move_lb(float speed)
{
        if(speed < 0)   																													 //逆时针
			{
				speed = -speed;      
				GPIO_ResetBits(GPIOC,GPIO_Pin_2);																	 //PA13为方向引脚，输出低电平		
			}		
		else if(speed > 0)   																																	 //顺时针
			{
				GPIO_SetBits(GPIOC,GPIO_Pin_2);																		 //PA13为方向引脚，输出高电平,得试
			}
            if(speed==0)
            {
                delay_13=-2;//最大周期的旋转，后面可以让他倒转进一步降低误差
							if(GPIO_ReadOutputDataBit(GPIOC,GPIO_Pin_2))
							GPIO_ResetBits(GPIOC,GPIO_Pin_2);	
							else
							GPIO_SetBits(GPIOC,GPIO_Pin_2);	
							
            }
            else
            delay_13 = (int32_t)(A_T_x10/speed);//速度归一化 把速度转为延时的时间

}
void MOTOR_Move_lf(float speed)
{
        if(speed < 0)   																													 //逆时针
			{
				GPIO_ResetBits(GPIOC,GPIO_Pin_3);																	 //PA13为方向引脚，输出低电平		
				speed = -speed;      
			}		
		else if(speed > 0)  																																	 //顺时针
			{
				GPIO_SetBits(GPIOC,GPIO_Pin_3);																		 //PA13为方向引脚，输出高电平,得试
			}
            if(speed==0)
            {
                delay_14=-2;//最大周期的旋转，后面可以让他倒转进一步降低误差
							if(GPIO_ReadOutputDataBit(GPIOC,GPIO_Pin_3))
							GPIO_ResetBits(GPIOC,GPIO_Pin_3);	
							else
							GPIO_SetBits(GPIOC,GPIO_Pin_3);	
            }
            else
            delay_14 = (int32_t)(A_T_x10/speed);//速度归一化 把速度转为延时的时间

}
/**
  * @brief          速度解算函数，只写了一个轮子，其他参照即可，修改对应的tim_count_1与step_11即可
  * @author         Nirvana
  * @param[in]      速度大小
  * @retval         返回空
  */
__IO uint32_t tim_count_10=0,tim_count_20=0,tim_count_30=0,tim_count_40=0;//定时器中读取的值
void my_speed_decision()                                                         //中断执行函数
{
	uint32_t tmp_10,tmp_20,tmp_30,tmp_40 = 0;  //之后对比值
     if (TIM_GetITStatus(TIM3, TIM_IT_CC1)== SET)
  {	  
        TIM_ClearITPendingBit(TIM3, TIM_IT_CC1);																	// 清楚定时器中断		
	  tim_count_10 = TIM_GetCounter(TIM3);																					//获取计数值
		if(delay_11 == 0)
		{
			tim_count_10 = 0; 
			
		}
		tmp_10 = tim_count_10+delay_11/2;
	  TIM_SetCompare1(TIM3,tmp_10);																								// 设置比较值
  }
	if (TIM_GetITStatus(TIM3, TIM_IT_CC2) == SET)
	{
		 TIM_ClearITPendingBit(TIM3, TIM_IT_CC2);																	// 清楚定时器中断		
	tim_count_20 = TIM_GetCounter(TIM3);																					//获取计数值
	if(delay_12 == 0)
	{
		tim_count_20 = 0; 
		
	}
	tmp_20 = tim_count_20+delay_12/2;
	TIM_SetCompare2(TIM3,tmp_20);																								// 设置比较值
		
	}
		
	if (TIM_GetITStatus(TIM3, TIM_IT_CC3)== SET)
  {	  
    TIM_ClearITPendingBit(TIM3, TIM_IT_CC3);																	// 清楚定时器中断		
	  tim_count_30 = TIM_GetCounter(TIM3);																					//获取计数值
		if(delay_13 == 0)
		{
			tim_count_30 = 0; 
			
		}
		tmp_30 = tim_count_30+delay_13/2;
	  TIM_SetCompare3(TIM3,tmp_30);																								// 设置比较值
  }
	
	if (TIM_GetITStatus(TIM3, TIM_IT_CC4)== SET)
  {	  
        TIM_ClearITPendingBit(TIM3, TIM_IT_CC4);																	// 清楚定时器中断		
	  tim_count_40 = TIM_GetCounter(TIM3);																					//获取计数值
		if(delay_14 == 0)
		{
			tim_count_40 = 0; 
			
		}
		tmp_40 = tim_count_40+delay_14/2;
	  TIM_SetCompare4(TIM3,tmp_40);																								// 设置比较值
  }	
} 
//void TIM3_IRQHandler(void)
//{
//	my_speed_decision();
//}


/**
 * @file        速度解算，由逆矩阵公式解算
 * @note        参照论文《Mecanum轮逆矩阵研究》
 * @date        2021
 */
void Speed_Calculation(Target *settarget,float X_speed, float Y_speed, float Z_speed)
{
    if(Z_speed>1200) Z_speed = 1200;
    else if(Z_speed < -1200) Z_speed = -1200;   // 对转向速度限幅

    settarget->Target_HL= X_speed - 0.3*Z_speed- Y_speed;
    settarget->Target_HR= X_speed + 0.3*Z_speed+ Y_speed;
    settarget->Target_BL= X_speed - 0.3*Z_speed+ Y_speed;
    settarget->Target_BR= X_speed + 0.3*Z_speed- Y_speed;
}

/**
  * @brief          T型速度计算，输入需要达到的目标值,调整周期值即可。（该函数会让系统响应延迟，当一些需要急停的场景请直接调用Set_Pwm()函数调用后注意修改静态值）。
  *                 该函数调用了Set_Pwm与fabs注意添加库。
  *                 使用时将该函数放到15ms（时间可修改）的中断中即可，
  * @author         Nirvana
  * @param[in]      4个轮子目标速度
  * @retval         返回空
  */
void T_Speed_calc(float hl,float hr,float bl,float br)
{
	static float hln=0;
	static float hrn=0;
	static float bln=0;
	static float brn=0;

		if (fabs(hln-hl)>frame_period)
		{
			if(hln<hl)
			hln+=frame_period;
			else if(hln>hl)	hln-=frame_period;
			else 	hln=hl;//减速急停应该可以
		}
		else   hln=hl;
		
		if (fabs(hrn-hr)>frame_period)
		{
			if(hrn<hr)
			hrn+=frame_period;
			else if(hrn>hr)
			hrn-=frame_period;
			else  hrn=hr;
		}
		else hrn=hr;

		if (fabs(bln-bl)>frame_period)
		{
			if(bln<bl)	bln+=frame_period;
			else if(bln>bl) bln-=frame_period;
			else  bln=bl;
		}
		else bln=bl;
        
		if (fabs(brn-br)>frame_period)
		{
			if(brn<br)
			brn+=frame_period;
			else if(brn>br)
			brn-=frame_period;
			else
			brn=br;
		}
		else brn=br;
		
			MOTOR_Move_lf(hln);
			MOTOR_Move_lb(bln);
			MOTOR_Move_rf(hrn);
			MOTOR_Move_rb(brn);				

}

void Set_ang()//开启速度闭环 使用前记着开启
{
	int32_t movez;
	ang_flag=1;
	yaw=(float)stcAngle.Angle[2]/32768*180;	
	movez = PID_Calculate_Ang(&Angle_PID,yaw);
	Speed_Calculation(&target,0,0,movez);
	T_Speed_calc(target.Target_HL,target.Target_HR,target.Target_BL,target.Target_BR);
	motor_init();
	
}

void Ang_cal()//循环调用该函数即可闭环 到达固定角度 建议周期15ms以内（陀螺仪串口通信速度也要快） 如果反应不够快速 可增加frame_period
{
	int32_t movez;	
	yaw=(float)stcAngle.Angle[2]/32768*180;		
	movez = PID_Calculate_Ang(&Angle_PID,yaw);
	Speed_Calculation(&target,0,0,movez);
	T_Speed_calc(target.Target_HL,target.Target_HR,target.Target_BL,target.Target_BR);
}

void Ang_cal_1()//用于特殊角度的旋转
{
	int32_t movez;
	yaw=(float)stcAngle.Angle[2]/32768*180;		
	if(yaw <0) yaw += 360;
	movez = PID_Calculate_Ang(&Angle_PID,yaw);
	Speed_Calculation(&target,0,0,movez);
	T_Speed_calc(target.Target_HL,target.Target_HR,target.Target_BL,target.Target_BR);
}

void xy_speed_cal(float x_speeds,float y_speeds)
{
	int32_t movez=0;
	int32_t movex=0;
	int32_t movey=0;
	yaw=(float)stcAngle.Angle[2]/32768*180;		//如果需要角度换 直接把这段取消注释
	movez = PID_Calculate_Ang(&Angle_PID,yaw);
	movex = pid_solve(&K210x_pid,(K210x_pid.SetPoint-x_speeds));                           //-(K210x_pid.SetPoint-x_speeds); 
	movey = pid_solve(&K210y_pid,-(K210y_pid.SetPoint-y_speeds)); //这里注意正负号 如果正反馈了变个号
	Speed_Calculation(&target,movex,movey,movez);
	T_Speed_calc(target.Target_HL,target.Target_HR,target.Target_BL,target.Target_BR);
}

void xy_speed_cal_1(float x_speeds,float y_speeds)
{
	int32_t movez=0;
	int32_t movex=0;
	int32_t movey=0;
	yaw=(float)stcAngle.Angle[2]/32768*180;		//如果需要角度换 直接把这段取消注释
	if(yaw <0) yaw += 360;	
	movez = PID_Calculate_Ang(&Angle_PID,yaw);
	movex = pid_solve(&K210x_pid,(K210x_pid.SetPoint-x_speeds));                           //-(K210x_pid.SetPoint-x_speeds); 
	movey = pid_solve(&K210y_pid,-(K210y_pid.SetPoint-y_speeds)); //这里注意正负号 如果正反馈了变个号
	Speed_Calculation(&target,movex,movey,movez);
	T_Speed_calc(target.Target_HL,target.Target_HR,target.Target_BL,target.Target_BR);
}

void Reset_ang()//关闭速度闭环 关闭后即可使用MOTOR_Move_x  一般情况不管定时器应该不会自转
{
	ang_flag=0;
					TIM_ClearITPendingBit(TIM3, TIM_IT_CC1);
				  TIM_CCxCmd(TIM3,TIM_Channel_1,TIM_CCx_Disable);
					TIM_ClearITPendingBit(TIM3, TIM_IT_CC2);
				  TIM_CCxCmd(TIM3,TIM_Channel_2,TIM_CCx_Disable);
					TIM_ClearITPendingBit(TIM3, TIM_IT_CC3);
				  TIM_CCxCmd(TIM3,TIM_Channel_3,TIM_CCx_Disable);
					TIM_ClearITPendingBit(TIM3, TIM_IT_CC4);
				  TIM_CCxCmd(TIM3,TIM_Channel_4,TIM_CCx_Disable);
				  TIM_Cmd(TIM3, DISABLE);					                                  //单个电机可以关闭定时器，多个电机只关闭通道即可
}


