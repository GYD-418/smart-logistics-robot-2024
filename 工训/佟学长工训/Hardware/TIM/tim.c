#include "tim.h"
#include "Motor_Init.h"
#include "control.h"
#include "HWT101.h"
#include "Motor.h"
#include "control2.h"

void Basic_TIM_Init(int arr, int psc)
{

	GPIO_InitTypeDef GPIO_InitTypeStruce;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitTypeStruce;
	TIM_OCInitTypeDef TIM_OCInitTypeStruce;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_TIM4, ENABLE);
//	USART_DeInit(USART3);
	
	GPIO_InitTypeStruce.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitTypeStruce.GPIO_Pin=GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;//这里是我之前的代码，主要是控制到4个舵机，这里大家只用一个输出口即可
	GPIO_InitTypeStruce.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOD,&GPIO_InitTypeStruce);
	
	TIM_TimeBaseInitTypeStruce.TIM_Period=arr;//这里是自动重装值设置
	TIM_TimeBaseInitTypeStruce.TIM_Prescaler=psc;//这里是预分频系数
	TIM_TimeBaseInitTypeStruce.TIM_ClockDivision=0;
	TIM_TimeBaseInitTypeStruce.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitTypeStruce);
	
	TIM_OCInitTypeStruce.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_OCInitTypeStruce.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitTypeStruce.TIM_OCPolarity=TIM_OCPolarity_High;
	
	TIM_OC1Init(TIM4,&TIM_OCInitTypeStruce);
	TIM_OC2Init(TIM4,&TIM_OCInitTypeStruce);//这些是TIM4其他通道初始化
	TIM_OC3Init(TIM4,&TIM_OCInitTypeStruce);
	TIM_OC4Init(TIM4,&TIM_OCInitTypeStruce);
	
	TIM_OC1PreloadConfig(TIM4,TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(TIM4,TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(TIM4,TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(TIM4,TIM_OCPreload_Enable);
	
	TIM_Cmd(TIM4,ENABLE);
	
}
int test_count = 0;
int c = 0;

//void TIM4_IRQHandler(void)
//{
//  if (TIM_GetITStatus(TIM4, TIM_IT_Update)== SET)
//  {	  
//    TIM_ClearITPendingBit(TIM4, TIM_IT_Update);																	// 清楚定时器中断		
//	}
//}



/*
*/
void TIM5_Init(int arr, int psc)
{

	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitTypeStruce;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);
	
	
	TIM_TimeBaseInitTypeStruce.TIM_Period=arr;//这里是自动重装值设置
	TIM_TimeBaseInitTypeStruce.TIM_Prescaler=psc;//这里是预分频系数
	TIM_TimeBaseInitTypeStruce.TIM_ClockDivision=0;
	TIM_TimeBaseInitTypeStruce.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseInitTypeStruce);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_Cmd(TIM5,DISABLE);
	
}

void TIM5_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM5, TIM_IT_Update)== SET)
  {	  
    TIM_ClearITPendingBit(TIM5, TIM_IT_Update);																	// 清楚定时器中断		
		c++;
		if(c > 6999)
		{
			TIM_Cmd(TIM5,DISABLE);
			Mission_State = 2;
			Reset_ang();		
		}
	}

}

