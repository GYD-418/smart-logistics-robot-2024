#include "main.h"

uint16_t Chan_Shi_=1;

/*
 * 说明：此函数为舵机的PWM
 *
 */
 
void PWM_Init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE); //时钟使能
	
	//定时器TIM3初始化				1S=1000ms;180度;需900ms完成;进一次中断50ms;需要进18次
	TIM_TimeBaseStructure.TIM_Period = 300; //设置自动重装载的值	
	TIM_TimeBaseStructure.TIM_Prescaler = 84-1 ; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
	
	TIM_ClearFlag(TIM5,TIM_IT_Update);
	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断
	
	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器


	TIM_Cmd(TIM5, DISABLE );  //使能TIMx	
	
	
}

void TIM5_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM5,TIM_IT_Update)==SET) //溢出中断
	{
		xuan_xiao++;
		TIM_SetCompare2(TIM4,xuan_xiao);
	}
	TIM_ClearITPendingBit(TIM5,TIM_IT_Update);  //清除中断标志位
}

void zhuan_da(void)
{
	TIM_Cmd(TIM5, ENABLE );
	while(xuan_xiao<xuan_da);
	xuan_xiao=685;
	TIM_Cmd(TIM5, DISABLE );
}


void PWM_Init_2(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE); //时钟使能
	
	//定时器TIM3初始化				1S=1000ms;180度;需900ms完成;进一次中断50ms;需要进18次
	TIM_TimeBaseStructure.TIM_Period = 300; //设置自动重装载的值	
	TIM_TimeBaseStructure.TIM_Prescaler = 84-1 ; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
	
	TIM_ClearFlag(TIM7,TIM_IT_Update);
	TIM_ITConfig(TIM7,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断
	
	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器


	TIM_Cmd(TIM7, DISABLE );  //使能TIMx	
	
	
}

void TIM7_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM7,TIM_IT_Update)==SET) //溢出中断
	{
		xuan_da--;
		TIM_SetCompare2(TIM4,xuan_da);
	}
	TIM_ClearITPendingBit(TIM7,TIM_IT_Update);  //清除中断标志位
}

void zhuan_xiao(void)
{
	TIM_Cmd(TIM7, ENABLE );
	while(xuan_xiao<xuan_da);
	xuan_da=2025;
	TIM_Cmd(TIM7, DISABLE );
}
