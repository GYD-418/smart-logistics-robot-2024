#include "stm32f10x.h"                  // Device header

uint16_t num;

void timer_Init(void)
{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);   //使能时钟

			//配置中断
		TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
		TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;
		TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;
		TIM_TimeBaseInitStruct.TIM_Period=100-1;  //调节此可以让其位数开始计时
		TIM_TimeBaseInitStruct.TIM_Prescaler=720-1;
		TIM_TimeBaseInitStruct.TIM_RepetitionCounter=0;
		TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStruct);

		TIM_ClearFlag (TIM2,TIM_IT_Update);          //手动把中断开始标志位清除，避免其一初始化就进中断。使其每次开始不是从0而是从1开始
		TIM_ITConfig(TIM2 ,TIM_IT_Update,ENABLE );   //使能更新中断开启
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

		NVIC_InitTypeDef NVIC_InitStruct;
		NVIC_InitStruct.NVIC_IRQChannel=TIM2_IRQn;
		NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
		NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=2;
		NVIC_InitStruct.NVIC_IRQChannelSubPriority=1;
		NVIC_Init(&NVIC_InitStruct);

		TIM_Cmd(TIM2,ENABLE);
}

//void TIM2_IRQHandler(void)     //函数定义错
//	{
//	   if(TIM_GetFlagStatus(TIM2,TIM_IT_Update)==SET)
//		 {

//		 } 
//		 TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
//	}