#include "main.h"




/**				
  * 函    数：该函数用于旋转时的PID计算，30ms进入一次中断
  * 参    数：无
  * 返 回 值：无
  */
void TIM_PID_XUAN(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
	
	TIM_TimeBaseInitTypeDef TIM_BASE_Init;
	
	TIM_BASE_Init.TIM_ClockDivision = TIM_CounterMode_Up;
	TIM_BASE_Init.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_BASE_Init.TIM_Period = 30000;
	TIM_BASE_Init.TIM_Prescaler = 71;
	TIM_BASE_Init.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM1,&TIM_BASE_Init);
	
	TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);
	TIM_ClearITPendingBit(TIM1,TIM_IT_Update);	
	
	NVIC_InitTypeDef NVIC_Inital;
	NVIC_Inital.NVIC_IRQChannel=TIM1_UP_TIM10_IRQn;           //定时器2中断
	NVIC_Inital.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Inital.NVIC_IRQChannelPreemptionPriority=2;//最高优先级
	NVIC_Inital.NVIC_IRQChannelSubPriority=1;
	NVIC_Init(&NVIC_Inital);
	
	
	TIM_Cmd(TIM1,DISABLE);
	
}

/**				
  * 函    数：中断服务函数，用于计算PID
  * 参    数：无
  * 返 回 值：无
  */
void TIM1_UP_TIM10_IRQHandler()
{
	if (TIM_GetITStatus(TIM1, TIM_IT_Update) == SET)
	{	
		int16_t speed;
		speed = (int16_t)PID_speed_count(yaw);
		Mortor_Zi_Zhuan_speed(speed);

		
		TIM_ClearITPendingBit(TIM1,TIM_IT_Update);

	}
	
	
}


/**				
  * 函    数：该函数用于旋转时的PID计算，30ms进入一次中断
  * 参    数：无
  * 返 回 值：无
  */
void TIM2_PID_X(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	
	TIM_TimeBaseInitTypeDef TIM_BASE_Init;
	
	TIM_BASE_Init.TIM_ClockDivision = TIM_CounterMode_Up;
	TIM_BASE_Init.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_BASE_Init.TIM_Period = 40000;
	TIM_BASE_Init.TIM_Prescaler = 72-1;				
	TIM_BASE_Init.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2,&TIM_BASE_Init);
	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);	
	
	NVIC_InitTypeDef NVIC_Inital;
	NVIC_Inital.NVIC_IRQChannel=TIM2_IRQn;           //定时器2中断
	NVIC_Inital.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Inital.NVIC_IRQChannelPreemptionPriority=2;//最高优先级
	NVIC_Inital.NVIC_IRQChannelSubPriority=2;
	NVIC_Init(&NVIC_Inital);
	
	
	TIM_Cmd(TIM2,DISABLE);
	
}


void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{	
			int8_t speed;
			speed = PID_X_count(X);
			Mortor_Zhi_Xing_Speed_1(speed);

		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
	}
	
}


/**				
  * 函    数：该函数用于旋转时的PID计算，30ms进入一次中断
  * 参    数：无
  * 返 回 值：无
  */
void TIM3_PID_Y(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	
	TIM_TimeBaseInitTypeDef TIM_BASE_Init;
	
	TIM_BASE_Init.TIM_ClockDivision = TIM_CounterMode_Up;
	TIM_BASE_Init.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_BASE_Init.TIM_Period = 40000;
	TIM_BASE_Init.TIM_Prescaler = 72-1;
	TIM_BASE_Init.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3,&TIM_BASE_Init);
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);	
	
	NVIC_InitTypeDef NVIC_Inital;
	NVIC_Inital.NVIC_IRQChannel=TIM3_IRQn;           //定时器2中断
	NVIC_Inital.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Inital.NVIC_IRQChannelPreemptionPriority=2;//最高优先级
	NVIC_Inital.NVIC_IRQChannelSubPriority=3;
	NVIC_Init(&NVIC_Inital);
	
	
	TIM_Cmd(TIM3,DISABLE);
	
}


void TIM3_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) == SET)
	{	
		int8_t speed;
		speed = PID_Y_count(Y);
		Mortor_Heng_Yi_Speed(speed);

		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);

	}
	
}


