#include "stm32f10x.h"                  // Device header
#include "Motor.h"  
#include "Motor_Init.h"  
#include <math.h>
#include "control2.h"
extern int ang_flag;//速度闭环全局变量
State Speed_Test_State;
State ZhuaQu_State;
speedRampData srd_1= {STOP,0,0,0,0,0,0}; //加减速变量    
speedRampData srd_2= {STOP,0,0,0,0,0,0}; //加减速变量    
speedRampData srd_3= {STOP,0,0,0,0,0,0}; //加减速变量    
speedRampData srd_4= {STOP,0,0,0,0,0,0}; //加减速变量    
speedRampData srd_5= {STOP,0,0,0,0,0,0}; //加减速变量    

uint8_t  motor_sta_1        = 0;//电机状态
uint8_t  motor_sta_2        = 0;//电机状态
uint8_t  motor_sta_3        = 0;//电机状态
uint8_t  motor_sta_4        = 0;//电机状态
uint8_t  motor_sta_5        = 0;//电机状态

int step_11 = 0;
int step_12 = 0;
int step_13 = 0;
int step_14 = 0;
int step_15 = 0;


/*
step   移动步数（正数为正转，负数为逆时针）
accel  加速度,实际值为accel*0.1*rad/sec^2  10倍并且2个脉冲算一个完整的周期
decel  减速度,实际值为decel*0.1*rad/sec^2
speed  最大速度,实际值为speed*0.1*rad/sec
 */

uint16_t tim_count_11; 																										 //存放中断时刻的计数值
uint16_t tim_count_12; 			//nir:这几个变量之前的程序中后续没幅值，检查下为什么																							 //存放中断时刻的计数值
uint16_t tim_count_13; 																										 //存放中断时刻的计数值
uint16_t tim_count_14; 																										 //存放中断时刻的计数值
uint16_t tim_count_15; 																										 //存放中断时刻的计数值

void MOTOR_Move_1(int32_t step, uint32_t accel, uint32_t decel, uint32_t speed)//nir:开启速度闭环 1开启0关闭
{
		
	step_11 = step;
    unsigned int max_s_lim;                                     					 	 //达到最大速度时的步数    
    unsigned int accel_lim;																									 //必须开始减速的步数（如果还没有加速度到最大速度时）

		if(motor_sta_1!= STOP)  																									 //只允许步进电机在停止的时候才继续
			return;			
    if(step < 0)   																													 //逆时针
			{
				GPIO_ResetBits(GPIOC,GPIO_Pin_0);																	 //PA13为方向引脚，输出低电平		
				step = -step;      
			}		
		else if(step > 0)  																																	 //顺时针
			{
				GPIO_SetBits(GPIOC,GPIO_Pin_0);																		 //PA13为方向引脚，输出高电平,得试
			}        
    if(step == 1)   																											   // 如果只移动一步
    {       
      srd_1.accel_count = -1; 																								 // 只移动一步
        
      srd_1.run_state = DECEL;																								 // 减速状态
        
      srd_1.step_delay = 1000;																								 // 短延时

     }
    
    else if(step != 0)  																			 								// 步数不为零才移动
    {					
				srd_1.min_delay = (int32_t)(A_T_x10/speed);															// 设置最大速度极限, 计算min_delay用于定时器的计数器的值min_delay = (alpha / tt)/ w   
				srd_1.step_delay = (int32_t)((T1_FREQ_148 * sqrt(A_SQ / accel))/10);		// 通过计算第一个(c0) 的步进延时来设定加速度,其中accel单位为0.01rad/sec^2
																																							// step_delay = 1/tt * sqrt(2*alpha/accel)
																																							// step_delay = ( tfreq*0.69/10 )*10 * sqrt( (2*alpha*100000) / (accel*10) )/100 
			  max_s_lim = (uint32_t)(speed*speed/(A_x200*accel/10));								//计算多少步之后达到最大速度的限制 max_s_lim = speed^2 / (2*alpha*accel)
    
				if(max_s_lim == 0)																										//如果达到最大速度小于0.5步，我们将四舍五入为0,但实际我们必须移动至少一步才能达到想要的速度 
				{
					max_s_lim = 1;
				}    
				accel_lim = (uint32_t)(step*decel/(accel+decel)); 										// 计算多少步之后我们必须开始减速,n1 = (n1+n2)decel / (accel + decel)
   
				if(accel_lim == 0) 																										// 我们必须加速至少1步才能开始减速
				{
					accel_lim = 1;
				}
   
				if(accel_lim <= max_s_lim)																						//加速阶段到不了最大速度就得减速。。。使用限制条件我们可以计算出减速阶段步数 
				{
					srd_1.decel_val = accel_lim - step;																		//减速段的步数
				}
				else
				{
					srd_1.decel_val = -(max_s_lim*accel/decel);														//减速段的步数 
				}
   
				if(srd_1.decel_val == 0) 																								// 不足一步 按一步处理 
				{
					srd_1.decel_val = -1;
				}    
				srd_1.decel_start = step + srd_1.decel_val;																//计算开始减速时的步数
 
    
				if(srd_1.step_delay <= srd_1.min_delay)																		// 如果一开始c0的速度比匀速段速度还大，就不需要进行加速运动，直接进入匀速
				{
					srd_1.step_delay = srd_1.min_delay;
					srd_1.run_state = RUN;
				}
				else
				{
					srd_1.run_state = ACCEL;
				}
    
				srd_1.accel_count = 0;																									// 复位加速度计数值
		
			}
			motor_sta_1 = 1;  			// 电机为运动状态
			if(step != 0)
			{
				tim_count_11 = TIM_GetCounter(TIM3);																				//获取计数值				
			}
			else 
			{
//				motor_sta_1 = 5;
				tim_count_11 = 0;
				if(GPIO_ReadOutputDataBit(GPIOC,GPIO_Pin_0))//nir:翻转 下同
					GPIO_ResetBits(GPIOC,GPIO_Pin_0);	
				else
					GPIO_SetBits(GPIOC,GPIO_Pin_0);	
			}
			TIM_SetCompare1(TIM3,tim_count_11+srd_1.step_delay/2);												//设置定时器比较值 
			TIM_ITConfig(TIM3,TIM_IT_CC1,ENABLE);																		//使能定时器通道 
			TIM_CCxCmd(TIM3,TIM_Channel_1,TIM_CCx_Enable);
			TIM_Cmd(TIM3, ENABLE);																									//开启定时器
}

 
void MOTOR_Move_2(int32_t step, uint32_t accel, uint32_t decel, uint32_t speed)
{
			step_12 = step;
    unsigned int max_s_lim;                                     					 	 //达到最大速度时的步数    
    unsigned int accel_lim;																									 //必须开始减速的步数（如果还没有加速度到最大速度时）

		if(motor_sta_2!= STOP)  																									 //只允许步进电机在停止的时候才继续
			return;			
    if(step < 0)   																													 //逆时针
			{
				GPIO_ResetBits(GPIOC,GPIO_Pin_1);																	 //PA13为方向引脚，输出低电平		
				step = -step;      
			}		
		else if(step > 0)   																																	 //顺时针
			{
				GPIO_SetBits(GPIOC,GPIO_Pin_1);																		 //PA13为方向引脚，输出高电平
			}        
    if(step == 1)   																											   // 如果只移动一步
    {       
      srd_2.accel_count = -1; 																								 // 只移动一步
        
      srd_2.run_state = DECEL;																								 // 减速状态
        
      srd_2.step_delay = 1000;																								 // 短延时

     }
    
    else if(step != 0)  																			 								// 步数不为零才移动
    {					
				srd_2.min_delay = (int32_t)(A_T_x10/speed);															// 设置最大速度极限, 计算min_delay用于定时器的计数器的值min_delay = (alpha / tt)/ w   
				srd_2.step_delay = (int32_t)((T1_FREQ_148 * sqrt(A_SQ / accel))/10);		// 通过计算第一个(c0) 的步进延时来设定加速度,其中accel单位为0.01rad/sec^2
																																							// step_delay = 1/tt * sqrt(2*alpha/accel)
																																							// step_delay = ( tfreq*0.69/10 )*10 * sqrt( (2*alpha*100000) / (accel*10) )/100 
			  max_s_lim = (uint32_t)(speed*speed/(A_x200*accel/10));								//计算多少步之后达到最大速度的限制 max_s_lim = speed^2 / (2*alpha*accel)
    
				if(max_s_lim == 0)																										//如果达到最大速度小于0.5步，我们将四舍五入为0,但实际我们必须移动至少一步才能达到想要的速度 
				{
					max_s_lim = 1;
				}    
				accel_lim = (uint32_t)(step*decel/(accel+decel)); 										// 计算多少步之后我们必须开始减速,n1 = (n1+n2)decel / (accel + decel)
   
				if(accel_lim == 0) 																										// 我们必须加速至少1步才能开始减速
				{
					accel_lim = 1;
				}
   
				if(accel_lim <= max_s_lim)																						//加速阶段到不了最大速度就得减速。。。使用限制条件我们可以计算出减速阶段步数 
				{
					srd_2.decel_val = accel_lim - step;																		//减速段的步数
				}
				else
				{
					srd_2.decel_val = -(max_s_lim*accel/decel);														//减速段的步数 
				}
   
				if(srd_2.decel_val == 0) 																								// 不足一步 按一步处理 
				{
					srd_2.decel_val = -1;
				}    
				srd_2.decel_start = step + srd_2.decel_val;																//计算开始减速时的步数
 
    
				if(srd_2.step_delay <= srd_2.min_delay)																		// 如果一开始c0的速度比匀速段速度还大，就不需要进行加速运动，直接进入匀速
				{
					srd_2.step_delay = srd_2.min_delay;
					srd_2.run_state = RUN;
				}
				else
				{
					srd_2.run_state = ACCEL;
				}
    
				srd_2.accel_count = 0;																									// 复位加速度计数值
		
			}
			motor_sta_2 = 1;  																												// 电机为运动状态
			if(step != 0)
			{
				tim_count_12 = TIM_GetCounter(TIM3);																				//获取计数值
						
			}
			else 
			{
//				motor_sta_2 = 5;				
				tim_count_12 = 0;
				if(GPIO_ReadOutputDataBit(GPIOC,GPIO_Pin_1))
					GPIO_ResetBits(GPIOC,GPIO_Pin_1);	
				else
					GPIO_SetBits(GPIOC,GPIO_Pin_1);		
			}
			TIM_SetCompare2(TIM3,tim_count_12+srd_2.step_delay/2);												//设置定时器比较值 
			TIM_ITConfig(TIM3,TIM_IT_CC2,ENABLE);																		//使能定时器通道 
			TIM_CCxCmd(TIM3,TIM_Channel_2,TIM_CCx_Enable);
			TIM_Cmd(TIM3, ENABLE);																									//开启定时器
}
 
void MOTOR_Move_3(int32_t step, uint32_t accel, uint32_t decel, uint32_t speed)
{
		step_13 = step;
    unsigned int max_s_lim;                                     					 	 //达到最大速度时的步数    
    unsigned int accel_lim;																									 //必须开始减速的步数（如果还没有加速度到最大速度时）

		if(motor_sta_3!= STOP)  																									 //只允许步进电机在停止的时候才继续
			return;			
    if(step < 0)   																													 //逆时针
			{
				GPIO_SetBits(GPIOC,GPIO_Pin_2);																		 //PA13为方向引脚，输出高电平
				step = -step;      
			}		
		else if(step > 0)  																																	 //顺时针
			{
				GPIO_ResetBits(GPIOC,GPIO_Pin_2);																	 //PA13为方向引脚，输出低电平		
			}        
    if(step == 1)   																											   // 如果只移动一步
    {       
      srd_3.accel_count = -1; 																								 // 只移动一步
        
      srd_3.run_state = DECEL;																								 // 减速状态
        
      srd_3.step_delay = 1000;																								 // 短延时

     }
    
    else if(step != 0)  																			 								// 步数不为零才移动
    {					
				srd_3.min_delay = (int32_t)(A_T_x10/speed);															// 设置最大速度极限, 计算min_delay用于定时器的计数器的值min_delay = (alpha / tt)/ w   
				srd_3.step_delay = (int32_t)((T1_FREQ_148 * sqrt(A_SQ / accel))/10);		// 通过计算第一个(c0) 的步进延时来设定加速度,其中accel单位为0.01rad/sec^2
																																							// step_delay = 1/tt * sqrt(2*alpha/accel)
																																							// step_delay = ( tfreq*0.69/10 )*10 * sqrt( (2*alpha*100000) / (accel*10) )/100 
			  max_s_lim = (uint32_t)(speed*speed/(A_x200*accel/10));								//计算多少步之后达到最大速度的限制 max_s_lim = speed^2 / (2*alpha*accel)
    
				if(max_s_lim == 0)																										//如果达到最大速度小于0.5步，我们将四舍五入为0,但实际我们必须移动至少一步才能达到想要的速度 
				{
					max_s_lim = 1;
				}    
				accel_lim = (uint32_t)(step*decel/(accel+decel)); 										// 计算多少步之后我们必须开始减速,n1 = (n1+n2)decel / (accel + decel)
   
				if(accel_lim == 0) 																										// 我们必须加速至少1步才能开始减速
				{
					accel_lim = 1;
				}
   
				if(accel_lim <= max_s_lim)																						//加速阶段到不了最大速度就得减速。。。使用限制条件我们可以计算出减速阶段步数 
				{
					srd_3.decel_val = accel_lim - step;																		//减速段的步数
				}
				else
				{
					srd_3.decel_val = -(max_s_lim*accel/decel);														//减速段的步数 
				}
   
				if(srd_3.decel_val == 0) 																								// 不足一步 按一步处理 
				{
					srd_3.decel_val = -1;
				}    
				srd_3.decel_start = step + srd_3.decel_val;																//计算开始减速时的步数
 
    
				if(srd_3.step_delay <= srd_3.min_delay)																		// 如果一开始c0的速度比匀速段速度还大，就不需要进行加速运动，直接进入匀速
				{
					srd_3.step_delay = srd_3.min_delay;
					srd_3.run_state = RUN;
				}
				else
				{
					srd_3.run_state = ACCEL;
				}
    
				srd_3.accel_count = 0;																									// 复位加速度计数值
		
			}
			motor_sta_3 = 1;  																												// 电机为运动状态
			if(step != 0)
			{
				tim_count_13 = TIM_GetCounter(TIM3);																				//获取计数值			
							
			}
			else 
			{
//				motor_sta_3 = 5;				
				tim_count_13 = 0;
				if(GPIO_ReadOutputDataBit(GPIOC,GPIO_Pin_2))
				GPIO_ResetBits(GPIOC,GPIO_Pin_2);	
			else
				GPIO_SetBits(GPIOC,GPIO_Pin_2);	
			}
			TIM_SetCompare3(TIM3,tim_count_13+srd_3.step_delay/2);												//设置定时器比较值 
			TIM_ITConfig(TIM3,TIM_IT_CC3,ENABLE);																		//使能定时器通道 
			TIM_CCxCmd(TIM3,TIM_Channel_3,TIM_CCx_Enable);
			TIM_Cmd(TIM3, ENABLE);																									//开启定时器
}
 
void MOTOR_Move_4(int32_t step, uint32_t accel, uint32_t decel, uint32_t speed)
{
		step_14 = step;
    unsigned int max_s_lim;                                     					 	 //达到最大速度时的步数    
    unsigned int accel_lim;																									 //必须开始减速的步数（如果还没有加速度到最大速度时）

		if(motor_sta_4!= STOP)  																									 //只允许步进电机在停止的时候才继续
			return;			
    if(step < 0)   																													 //逆时针
			{
				GPIO_SetBits(GPIOC,GPIO_Pin_3);																		 //PA13为方向引脚，输出高电平
				step = -step;      
			}		
		else if(step > 0)   																																	 //顺时针
			{
				GPIO_ResetBits(GPIOC,GPIO_Pin_3);																	 //PA13为方向引脚，输出低电平		
			}        
    if(step == 1)   																											   // 如果只移动一步
    {       
      srd_4.accel_count = -1; 																								 // 只移动一步
        
      srd_4.run_state = DECEL;																								 // 减速状态
        
      srd_4.step_delay = 1000;																								 // 短延时

     }
    
    else if(step != 0)  																			 								// 步数不为零才移动
    {					
				srd_4.min_delay = (int32_t)(A_T_x10/speed);															// 设置最大速度极限, 计算min_delay用于定时器的计数器的值min_delay = (alpha / tt)/ w   
				srd_4.step_delay = (int32_t)((T1_FREQ_148 * sqrt(A_SQ / accel))/10);		// 通过计算第一个(c0) 的步进延时来设定加速度,其中accel单位为0.01rad/sec^2
																																							// step_delay = 1/tt * sqrt(2*alpha/accel)
																																							// step_delay = ( tfreq*0.69/10 )*10 * sqrt( (2*alpha*100000) / (accel*10) )/100 
			  max_s_lim = (uint32_t)(speed*speed/(A_x200*accel/10));								//计算多少步之后达到最大速度的限制 max_s_lim = speed^2 / (2*alpha*accel)
    
				if(max_s_lim == 0)																										//如果达到最大速度小于0.5步，我们将四舍五入为0,但实际我们必须移动至少一步才能达到想要的速度 
				{
					max_s_lim = 1;
				}    
				accel_lim = (uint32_t)(step*decel/(accel+decel)); 										// 计算多少步之后我们必须开始减速,n1 = (n1+n2)decel / (accel + decel)
   
				if(accel_lim == 0) 																										// 我们必须加速至少1步才能开始减速
				{
					accel_lim = 1;
				}
   
				if(accel_lim <= max_s_lim)																						//加速阶段到不了最大速度就得减速。。。使用限制条件我们可以计算出减速阶段步数 
				{
					srd_4.decel_val = accel_lim - step;																		//减速段的步数
				}
				else
				{
					srd_4.decel_val = -(max_s_lim*accel/decel);														//减速段的步数 
				}
   
				if(srd_4.decel_val == 0) 																								// 不足一步 按一步处理 
				{
					srd_4.decel_val = -1;
				}    
				srd_4.decel_start = step + srd_4.decel_val;																//计算开始减速时的步数
 
    
				if(srd_4.step_delay <= srd_4.min_delay)																		// 如果一开始c0的速度比匀速段速度还大，就不需要进行加速运动，直接进入匀速
				{
					srd_4.step_delay = srd_4.min_delay;
					srd_4.run_state = RUN;
				}
				else
				{
					srd_4.run_state = ACCEL;
				}
    
				srd_4.accel_count = 0;																									// 复位加速度计数值
		
			}
			motor_sta_4 = 1;  																												// 电机为运动状态
			if(step != 0)
			{
				tim_count_14 = TIM_GetCounter(TIM3);																				//获取计数值				
			}
			else 
			{
//				motor_sta_4 = 5;
				tim_count_14 = 0;
				if(GPIO_ReadOutputDataBit(GPIOC,GPIO_Pin_3))
				GPIO_ResetBits(GPIOC,GPIO_Pin_3);	
			else
				GPIO_SetBits(GPIOC,GPIO_Pin_3);	
			}
			TIM_SetCompare4(TIM3,tim_count_14+srd_4.step_delay/2);												//设置定时器比较值 
			TIM_ITConfig(TIM3,TIM_IT_CC4,ENABLE);																		//使能定时器通道 
			TIM_CCxCmd(TIM3,TIM_Channel_4,TIM_CCx_Enable);
			TIM_Cmd(TIM3, ENABLE);																									//开启定时器
}
 


	


__IO uint32_t tim_count_1=0,tim_count_2=0,tim_count_3=0,tim_count_4=0;
__IO uint32_t tmp_1,tmp_2,tmp_3,tmp_4 = 0;  
void speed_decision()                                                         //中断执行函数
{
  uint16_t new_step_delay_1=0,new_step_delay_2=0,new_step_delay_3=0,new_step_delay_4=0;                                                  // 保存新（下）一个延时周期  
  __IO static uint16_t last_accel_delay_1,last_accel_delay_2,last_accel_delay_3,last_accel_delay_4=0;                                    // 加速过程中最后一次延时（脉冲周期）. 
  __IO static uint32_t step_count_1 = 0,step_count_2 = 0,step_count_3 = 0,step_count_4 = 0; 																			  // 总移动步数计数器  
  __IO static int32_t rest_1,rest_2,rest_3,rest_4 = 0;																								// 记录new_step_delay中的余数，提高下一步计算的精度  
  __IO static uint8_t i_1=0,i_2=0,i_3=0,i_4=0;																										//定时器使用翻转模式，需要进入两次中断才输出一个完整脉冲
 
  if (TIM_GetITStatus(TIM3, TIM_IT_CC1)== SET)
  {	  
		
    TIM_ClearITPendingBit(TIM3, TIM_IT_CC1);																	// 清楚定时器中断		
	  tim_count_1 = TIM_GetCounter(TIM3);																					//获取计数值
		if(step_11 == 0)
		{
			tim_count_1 = 0; 
		}
		tmp_1 = tim_count_1+srd_1.step_delay/2;
	  TIM_SetCompare1(TIM3,tmp_1);																								// 设置比较值
		i_1++; 
		if(i_1==2)																																	//中断两次为一个脉冲
		{
			i_1=0; 
			switch(srd_1.run_state)
			{
				case STOP:						//停止状态
				{
					step_count_1 = 0;
					rest_1 = 0;
					
					TIM_ClearITPendingBit(TIM3, TIM_IT_CC1);
				  TIM_CCxCmd(TIM3,TIM_Channel_1,TIM_CCx_Disable);
//				  TIM_Cmd(TIM3, DISABLE);					                                  //单个电机可以关闭定时器，多个电机只关闭通道即可
					motor_sta_1 = 0;  			
				}
					break;
				
				case ACCEL:									//加速状态
				{
					step_count_1++;
					srd_1.accel_count++;
				
					new_step_delay_1 = srd_1.step_delay - (((2 *srd_1.step_delay) + rest_1)/(4 * srd_1.accel_count + 1));//计算新(下)一步脉冲周期(时间间隔)
					rest_1 = ((2 * srd_1.step_delay)+rest_1)%(4 * srd_1.accel_count + 1);					// 计算余数，下次计算补上余数，减少误差
					
						if(step_count_1 >= srd_1.decel_start) 																	//检查是够应该开始减速
						{
							srd_1.accel_count = srd_1.decel_val;																	//加速计数值为减速阶段计数值的初始值
							srd_1.run_state = DECEL;																						//下个脉冲进入减速阶段 
						}
						
						else if(new_step_delay_1 <= srd_1.min_delay)														//检查是否到达期望的最大速度
						{
							last_accel_delay_1 = new_step_delay_1;																//保存加速过程中最后一次延时（脉冲周期）
							new_step_delay_1 = srd_1.min_delay;   																// 使用min_delay（对应最大速度speed） 
							rest_1 = 0;            																							//清零余值               
							srd_1.run_state = RUN;																							//设置为匀速运行状态 
						}					
				}
					break;
					
				case RUN:
				{
          step_count_1++;  																											// 步数加1				  
          new_step_delay_1 = srd_1.min_delay;   																  // 使用min_delay（对应最大速度speed）				 
          if(step_count_1 >= srd_1.decel_start)   																// 需要开始减速
					{
            srd_1.accel_count = srd_1.decel_val;  																// 减速步数做为加速计数值
            new_step_delay_1 = last_accel_delay_1;																// 加阶段最后的延时做为减速阶段的起始延时(脉冲周期)
            srd_1.run_state = DECEL;           																  // 状态改变为减速
          }					
				}
          break;
					
				case DECEL:
				{
          step_count_1++;  																											// 步数加1

          srd_1.accel_count++; 																									// 是个负数
          new_step_delay_1 = srd_1.step_delay - (((2 * srd_1.step_delay) + rest_1)/(4 * srd_1.accel_count + 1)); //计算新(下)一步脉冲周期(时间间隔)
          rest_1 = ((2 * srd_1.step_delay)+rest_1)%(4 * srd_1.accel_count + 1);				// 计算余数，下次计算补上余数，减少误差
          if(srd_1.accel_count >= 0) 																						//检查是否为最后一步  是个负数所以要判断 大于等于零时 应该就是减速结束
          {
            srd_1.run_state = STOP;
          }				
				}
          break;
				default:
					break;
			}
			 srd_1.step_delay = new_step_delay_1; 																			// 为下个(新的)延时(脉冲周期)赋值
		}
	}
  if (TIM_GetITStatus(TIM3, TIM_IT_CC2)== SET)
  {	  
		
    TIM_ClearITPendingBit(TIM3, TIM_IT_CC2);																	// 清楚定时器中断		
	  tim_count_2 = TIM_GetCounter(TIM3);																					//获取计数值
		if(step_12 == 0)
		{
			tim_count_2 = 0; 
		}		
		tmp_2 = tim_count_2+srd_2.step_delay/2;
	  TIM_SetCompare2(TIM3,tmp_2);																								// 设置比较值
		i_2++; 
		if(i_2==2)																																	//中断两次为一个脉冲
		{
			i_2=0; 
			switch(srd_2.run_state)
			{
				case STOP:																														//停止状态
					step_count_2 = 0;
					rest_2 = 0;
					
					TIM_ClearITPendingBit(TIM3, TIM_IT_CC2);
				  TIM_CCxCmd(TIM3,TIM_Channel_2,TIM_CCx_Disable);
//				  TIM_Cmd(TIM3, DISABLE);					                                  //单个电机可以关闭定时器，多个电机只关闭通道即可
					motor_sta_2 = 0;  
					break;
				
				case ACCEL:																														//加速状态
				step_count_2++;
				srd_2.accel_count++;
				new_step_delay_2 = srd_2.step_delay - (((2 *srd_2.step_delay) + rest_2)/(4 * srd_2.accel_count + 1));//计算新(下)一步脉冲周期(时间间隔)
				rest_2 = ((2 * srd_2.step_delay)+rest_2)%(4 * srd_2.accel_count + 1);					// 计算余数，下次计算补上余数，减少误差
				
					if(step_count_2 >= srd_2.decel_start) 																	//检查是够应该开始减速
					{
						srd_2.accel_count = srd_2.decel_val;																	//加速计数值为减速阶段计数值的初始值
						srd_2.run_state = DECEL;																						//下个脉冲进入减速阶段 
					}
					
					else if(new_step_delay_2 <= srd_2.min_delay)														//检查是否到达期望的最大速度
					{
						last_accel_delay_2 = new_step_delay_2;																//保存加速过程中最后一次延时（脉冲周期）
						new_step_delay_2 = srd_2.min_delay;   																// 使用min_delay（对应最大速度speed） 
						rest_2 = 0;            																							//清零余值               
						srd_2.run_state = RUN;																							//设置为匀速运行状态 
					}
					break;
					
				case RUN:
          step_count_2++;  																											// 步数加1				  
          new_step_delay_2 = srd_2.min_delay;   																  // 使用min_delay（对应最大速度speed）				 
          if(step_count_2 >= srd_2.decel_start)   																// 需要开始减速
					{
            srd_2.accel_count = srd_2.decel_val;  																// 减速步数做为加速计数值
            new_step_delay_2= last_accel_delay_2;																// 加阶段最后的延时做为减速阶段的起始延时(脉冲周期)
            srd_2.run_state = DECEL;           																  // 状态改变为减速
          }
          break;
					
				case DECEL:
          step_count_2++;  																											// 步数加1

          srd_2.accel_count++; 																									// 是个负数
          new_step_delay_2 = srd_2.step_delay - (((2 * srd_2.step_delay) + rest_2)/(4 * srd_2.accel_count + 1)); //计算新(下)一步脉冲周期(时间间隔)
          rest_2 = ((2 * srd_2.step_delay)+rest_2)%(4 * srd_2.accel_count + 1);				// 计算余数，下次计算补上余数，减少误差
          if(srd_2.accel_count >= 0) 																						//检查是否为最后一步  是个负数所以要判断 大于等于零时 应该就是减速结束
          {
            srd_2.run_state = STOP;
          }
          break;
			}
			 srd_2.step_delay = new_step_delay_2; 																			// 为下个(新的)延时(脉冲周期)赋值
		}
	}
  if (TIM_GetITStatus(TIM3, TIM_IT_CC3)== SET)
{	  
		
    TIM_ClearITPendingBit(TIM3, TIM_IT_CC3);																	// 清楚定时器中断		
	  tim_count_3 = TIM_GetCounter(TIM3);																					//获取计数值
		if(step_13 == 0)
		{
			tim_count_3 = 0; 
		}
	
		tmp_3 = tim_count_3+srd_3.step_delay/2;
	  TIM_SetCompare3(TIM3,tmp_3);																								// 设置比较值
		i_3++; 
		if(i_3==2)																																	//中断两次为一个脉冲
		{
			i_3=0; 
			switch(srd_3.run_state)
			{
				case STOP:																														//停止状态
					step_count_3 = 0;
					rest_3 = 0;
					
					TIM_ClearITPendingBit(TIM3, TIM_IT_CC3);
				  TIM_CCxCmd(TIM3,TIM_Channel_3,TIM_CCx_Disable);
//				  TIM_Cmd(TIM3, DISABLE);					                                  //单个电机可以关闭定时器，多个电机只关闭通道即可
					motor_sta_3 = 0;  
					break;
				
				case ACCEL:																														//加速状态
				step_count_3++;
				srd_3.accel_count++;
				new_step_delay_3 = srd_3.step_delay - (((2 *srd_3.step_delay) + rest_3)/(4 * srd_3.accel_count + 1));//计算新(下)一步脉冲周期(时间间隔)
				rest_3 = ((2 * srd_3.step_delay)+rest_3)%(4 * srd_3.accel_count + 1);					// 计算余数，下次计算补上余数，减少误差
				
					if(step_count_3 >= srd_3.decel_start) 																	//检查是够应该开始减速
					{
						srd_3.accel_count = srd_3.decel_val;																	//加速计数值为减速阶段计数值的初始值
						srd_3.run_state = DECEL;																						//下个脉冲进入减速阶段 
					}
					
					else if(new_step_delay_3 <= srd_3.min_delay)														//检查是否到达期望的最大速度
					{
						last_accel_delay_3 = new_step_delay_3;																//保存加速过程中最后一次延时（脉冲周期）
						new_step_delay_3 = srd_3.min_delay;   																// 使用min_delay（对应最大速度speed） 
						rest_3 = 0;            																							//清零余值               
						srd_3.run_state = RUN;																							//设置为匀速运行状态 
					}
					break;
					
				case RUN:
          step_count_3++;  																											// 步数加1				  
          new_step_delay_3= srd_3.min_delay;   																  // 使用min_delay（对应最大速度speed）				 
          if(step_count_3 >= srd_3.decel_start)   																// 需要开始减速
					{
            srd_3.accel_count = srd_3.decel_val;  																// 减速步数做为加速计数值
            new_step_delay_3 = last_accel_delay_3;																// 加阶段最后的延时做为减速阶段的起始延时(脉冲周期)
            srd_3.run_state = DECEL;           																  // 状态改变为减速
          }
          break;
					
				case DECEL:
          step_count_3++;  																											// 步数加1

          srd_3.accel_count++; 																									// 是个负数
          new_step_delay_3 = srd_3.step_delay - (((2 * srd_3.step_delay) + rest_3)/(4 * srd_3.accel_count + 1)); //计算新(下)一步脉冲周期(时间间隔)
          rest_3 = ((2 * srd_3.step_delay)+rest_3)%(4 * srd_3.accel_count + 1);				// 计算余数，下次计算补上余数，减少误差
          if(srd_3.accel_count >= 0) 																						//检查是否为最后一步  是个负数所以要判断 大于等于零时 应该就是减速结束
          {
            srd_3.run_state = STOP;
          }
          break;
			}
			 srd_3.step_delay = new_step_delay_3; 																			// 为下个(新的)延时(脉冲周期)赋值
		}
	}
  if (TIM_GetITStatus(TIM3, TIM_IT_CC4)== SET)
  {	  
		
    TIM_ClearITPendingBit(TIM3, TIM_IT_CC4);																	// 清楚定时器中断		
	  tim_count_4 = TIM_GetCounter(TIM3);																					//获取计数值
		if(step_14 == 0)
		{
			tim_count_4 = 0; 
		}
		
		tmp_4 = tim_count_4+srd_4.step_delay/2;
	  TIM_SetCompare4(TIM3,tmp_4);																								// 设置比较值
		i_4++; 
		if(i_4==2)																																	//中断两次为一个脉冲
		{
			i_4=0; 
			switch(srd_4.run_state)
			{
				case STOP:																														//停止状态
					step_count_4 = 0;
					rest_4 = 0;
					
					TIM_ClearITPendingBit(TIM3, TIM_IT_CC4);
				  TIM_CCxCmd(TIM3,TIM_Channel_4,TIM_CCx_Disable);
//				  TIM_Cmd(TIM3, DISABLE);					                                  //单个电机可以关闭定时器，多个电机只关闭通道即可
					motor_sta_4 = 0;  
					break;
				
				case ACCEL:																														//加速状态
				step_count_4++;
				srd_4.accel_count++;
				new_step_delay_4 = srd_4.step_delay - (((2 *srd_4.step_delay) + rest_4)/(4 * srd_4.accel_count + 1));//计算新(下)一步脉冲周期(时间间隔)
				rest_4 = ((2 * srd_4.step_delay)+rest_4)%(4 * srd_4.accel_count + 1);					// 计算余数，下次计算补上余数，减少误差
				
					if(step_count_4 >= srd_4.decel_start) 																	//检查是够应该开始减速
					{
						srd_4.accel_count = srd_4.decel_val;																	//加速计数值为减速阶段计数值的初始值
						srd_4.run_state = DECEL;																						//下个脉冲进入减速阶段 
					}
					
					else if(new_step_delay_4<= srd_4.min_delay)														//检查是否到达期望的最大速度
					{
						last_accel_delay_4 = new_step_delay_4;																//保存加速过程中最后一次延时（脉冲周期）
						new_step_delay_4 = srd_4.min_delay;   																// 使用min_delay（对应最大速度speed） 
						rest_4 = 0;            																							//清零余值               
						srd_4.run_state = RUN;																							//设置为匀速运行状态 
					}
					break;
					
				case RUN:
          step_count_4++;  																											// 步数加1				  
          new_step_delay_4 = srd_4.min_delay;   																  // 使用min_delay（对应最大速度speed）				 
          if(step_count_4 >= srd_4.decel_start)   																// 需要开始减速
					{
            srd_4.accel_count = srd_4.decel_val;  																// 减速步数做为加速计数值
            new_step_delay_4 = last_accel_delay_4;																// 加阶段最后的延时做为减速阶段的起始延时(脉冲周期)
            srd_4.run_state = DECEL;           																  // 状态改变为减速
          }
          break;
					
				case DECEL:
          step_count_4++;  																											// 步数加1

          srd_4.accel_count++; 																									// 是个负数
          new_step_delay_4 = srd_4.step_delay - (((2 * srd_4.step_delay) + rest_4)/(4 * srd_4.accel_count + 1)); //计算新(下)一步脉冲周期(时间间隔)
          rest_4 = ((2 * srd_4.step_delay)+rest_4)%(4 * srd_4.accel_count + 1);				// 计算余数，下次计算补上余数，减少误差
          if(srd_4.accel_count >= 0) 																						//检查是否为最后一步  是个负数所以要判断 大于等于零时 应该就是减速结束
          {
            srd_4.run_state = STOP;
          }
          break;
			}
			 srd_4.step_delay = new_step_delay_4; 																			// 为下个(新的)延时(脉冲周期)赋值
		}
	}
	
}
	

void TIM3_IRQHandler(void)
{
	if(ang_flag)
	my_speed_decision();
	else
	speed_decision();
}


void MOTOR_Move_5(int32_t step, uint32_t accel, uint32_t decel, uint32_t speed)
{
		
    uint16_t tim_count; 																										 //存放中断时刻的计数值
    unsigned int max_s_lim;                                     					 	 //达到最大速度时的步数    
    unsigned int accel_lim;																									 //必须开始减速的步数（如果还没有加速度到最大速度时）

		if(motor_sta_5!= STOP)  																									 //只允许步进电机在停止的时候才继续
			return;			
    if(step < 0)   																													 //逆时针
			{
				GPIO_SetBits(GPIOC,GPIO_Pin_4);																		 //PA13为方向引脚，输出高电平
				step = -step;      
			}		
		else   																																	 //顺时针
			{
				GPIO_ResetBits(GPIOC,GPIO_Pin_4);																	 //PA13为方向引脚，输出低电平		
			}        
    if(step == 1)   																											   // 如果只移动一步
    {       
      srd_5.accel_count = -1; 																								 // 只移动一步
        
      srd_5.run_state = DECEL;																								 // 减速状态
        
      srd_5.step_delay = 1000;																								 // 短延时

     }
    
    else if(step != 0)  																			 								// 步数不为零才移动
    {					
				srd_5.min_delay = (int32_t)(A_T_x10/speed);															// 设置最大速度极限, 计算min_delay用于定时器的计数器的值min_delay = (alpha / tt)/ w   
				srd_5.step_delay = (int32_t)((T1_FREQ_148 * sqrt(A_SQ / accel))/10);		// 通过计算第一个(c0) 的步进延时来设定加速度,其中accel单位为0.01rad/sec^2
																																							// step_delay = 1/tt * sqrt(2*alpha/accel)
																																							// step_delay = ( tfreq*0.69/10 )*10 * sqrt( (2*alpha*100000) / (accel*10) )/100 
			  max_s_lim = (uint32_t)(speed*speed/(A_x200*accel/10));								//计算多少步之后达到最大速度的限制 max_s_lim = speed^2 / (2*alpha*accel)
    
				if(max_s_lim == 0)																										//如果达到最大速度小于0.5步，我们将四舍五入为0,但实际我们必须移动至少一步才能达到想要的速度 
				{
					max_s_lim = 1;
				}    
				accel_lim = (uint32_t)(step*decel/(accel+decel)); 										// 计算多少步之后我们必须开始减速,n1 = (n1+n2)decel / (accel + decel)
   
				if(accel_lim == 0) 																										// 我们必须加速至少1步才能开始减速
				{
					accel_lim = 1;
				}
   
				if(accel_lim <= max_s_lim)																						//加速阶段到不了最大速度就得减速。。。使用限制条件我们可以计算出减速阶段步数 
				{
					srd_5.decel_val = accel_lim - step;																		//减速段的步数
				}
				else
				{
					srd_5.decel_val = -(max_s_lim*accel/decel);														//减速段的步数 
				}
   
				if(srd_5.decel_val == 0) 																								// 不足一步 按一步处理 
				{
					srd_5.decel_val = -1;
				}    
				srd_5.decel_start = step + srd_5.decel_val;																//计算开始减速时的步数
 
    
				if(srd_5.step_delay <= srd_5.min_delay)																		// 如果一开始c0的速度比匀速段速度还大，就不需要进行加速运动，直接进入匀速
				{
					srd_5.step_delay = srd_5.min_delay;
					srd_5.run_state = RUN;
				}
				else
				{
					srd_5.run_state = ACCEL;
				}
    
				srd_5.accel_count = 0;																									// 复位加速度计数值
		
			}
			motor_sta_5 = 1;  																												// 电机为运动状态
			tim_count = TIM_GetCounter(TIM2);																				//获取计数值
			TIM_SetCompare2(TIM2,tim_count+srd_5.step_delay/2);												//设置定时器比较值 
			TIM_ITConfig(TIM2,TIM_IT_CC2,ENABLE);																		//使能定时器通道 
			TIM_CCxCmd(TIM2,TIM_Channel_2,TIM_CCx_Enable);
			TIM_Cmd(TIM2, ENABLE);																									//开启定时器
}
 


void speed_decision_5()                                                         //中断执行函数
{
	__IO uint32_t tim_count=0;
	__IO uint32_t tmp = 0;  
  uint16_t new_step_delay=0;                                                  // 保存新（下）一个延时周期  
  __IO static uint16_t last_accel_delay=0;                                    // 加速过程中最后一次延时（脉冲周期）. 
  __IO static uint32_t step_count = 0; 																			  // 总移动步数计数器  
  __IO static int32_t rest = 0;																								// 记录new_step_delay中的余数，提高下一步计算的精度  
  __IO static uint8_t i=0;																										//定时器使用翻转模式，需要进入两次中断才输出一个完整脉冲
 
  if (TIM_GetITStatus(TIM2, TIM_IT_CC2)== SET)
  {	  
		
    TIM_ClearITPendingBit(TIM2, TIM_IT_CC2);																	// 清楚定时器中断		
	  tim_count = TIM_GetCounter(TIM2);																					//获取计数值
		tmp = tim_count+srd_5.step_delay/2;
	  TIM_SetCompare2(TIM2,tmp);																								// 设置比较值
		i++; 
		if(i==2)																																	//中断两次为一个脉冲
		{
			i=0; 
			switch(srd_5.run_state)
			{
				case STOP:																														//停止状态
					step_count = 0;
					rest = 0;
					
					TIM_ClearITPendingBit(TIM2, TIM_IT_CC2);
				  TIM_CCxCmd(TIM2,TIM_Channel_2,TIM_CCx_Disable);
				  TIM_Cmd(TIM2, DISABLE);					                                  //单个电机可以关闭定时器，多个电机只关闭通道即可
					motor_sta_5 = 0;  
					break;
				
				case ACCEL:																														//加速状态
				step_count++;
				srd_5.accel_count++;
				new_step_delay = srd_5.step_delay - (((2 *srd_5.step_delay) + rest)/(4 * srd_5.accel_count + 1));//计算新(下)一步脉冲周期(时间间隔)
				rest = ((2 * srd_5.step_delay)+rest)%(4 * srd_5.accel_count + 1);					// 计算余数，下次计算补上余数，减少误差
				
					if(step_count >= srd_5.decel_start) 																	//检查是够应该开始减速
					{
						srd_5.accel_count = srd_5.decel_val;																	//加速计数值为减速阶段计数值的初始值
						srd_5.run_state = DECEL;																						//下个脉冲进入减速阶段 
					}
					
					else if(new_step_delay <= srd_5.min_delay)														//检查是否到达期望的最大速度
					{
						last_accel_delay = new_step_delay;																//保存加速过程中最后一次延时（脉冲周期）
						new_step_delay = srd_5.min_delay;   																// 使用min_delay（对应最大速度speed） 
						rest = 0;            																							//清零余值               
						srd_5.run_state = RUN;																							//设置为匀速运行状态 
					}
					break;
					
				case RUN:
          step_count++;  																											// 步数加1				  
          new_step_delay = srd_5.min_delay;   																  // 使用min_delay（对应最大速度speed）				 
          if(step_count >= srd_5.decel_start)   																// 需要开始减速
					{
            srd_5.accel_count = srd_5.decel_val;  																// 减速步数做为加速计数值
            new_step_delay = last_accel_delay;																// 加阶段最后的延时做为减速阶段的起始延时(脉冲周期)
            srd_5.run_state = DECEL;           																  // 状态改变为减速
          }
          break;
					
				case DECEL:
          step_count++;  																											// 步数加1

          srd_5.accel_count++; 																									// 是个负数
          new_step_delay = srd_5.step_delay - (((2 * srd_5.step_delay) + rest)/(4 * srd_5.accel_count + 1)); //计算新(下)一步脉冲周期(时间间隔)
          rest = ((2 * srd_5.step_delay)+rest)%(4 * srd_5.accel_count + 1);				// 计算余数，下次计算补上余数，减少误差
          if(srd_5.accel_count >= 0) 																						//检查是否为最后一步  是个负数所以要判断 大于等于零时 应该就是减速结束
          {
            srd_5.run_state = STOP;
          }
          break;
			}
			 srd_5.step_delay = new_step_delay; 																			// 为下个(新的)延时(脉冲周期)赋值
		}
	}
}
	

void TIM2_IRQHandler(void)
{
	speed_decision_5();
}
