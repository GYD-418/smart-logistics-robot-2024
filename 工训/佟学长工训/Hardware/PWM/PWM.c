#include "stm32f10x.h"                  // Device header

uint16_t CCR1=0;

void PWM_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
		
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;		//PA6->TIM3_CH1  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
		

	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_1); //PA1为方向引脚
	
	TIM_InternalClockConfig(TIM3);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;//时钟分频因子，数字滤波采样频率的参数
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 65535;		//ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = 1 - 1;		//PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0; //高级定时器功能
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
	
	TIM_OCInitTypeDef TIM_OCInitStructure;	
	TIM_OCStructInit(&TIM_OCInitStructure);	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Toggle;  //翻转模式
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;		//CCR 比较值
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM3,TIM_OCPreload_Disable);    //关闭预装载
	
	
	
	TIM_ClearFlag(TIM3,TIM_FLAG_CC1);
	TIM_ITConfig(TIM3,TIM_IT_CC1,ENABLE); //开启定时器3比较中断
	
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_Cmd(TIM3, ENABLE);
}


void TIM3_IRQHandler(void)
{
	uint16_t num = 0;
	if (TIM_GetITStatus(TIM3, TIM_IT_CC1) == SET)
	{		 
		TIM_ClearITPendingBit(TIM3, TIM_IT_CC1);
		num = TIM_GetCapture1(TIM3);
		TIM_SetCompare1(TIM3,num+CCR1);
	}

}




