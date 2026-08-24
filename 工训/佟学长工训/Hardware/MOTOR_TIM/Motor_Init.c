#include "stm32f10x.h"                  // Device header
#include "Motor_Init.h"  
#include "Motor.h"

int test = 0;
//GPIO口初始化
void MOTOR_GPIO_Init(void) 
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 				//开启PA端口时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);					//开启PB端口时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);					//开启PB端口时钟
	
	/* 步进电机驱动器：脉冲输出PA6,7,PB0,1 */
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;									//复用输出
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6|GPIO_Pin_7;			//PA6->TIM3_CH1  PA7->TIM3_CH2
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;   //PB0->TIM3_CH3  PB1->TIM3_CH4
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
	
	/* 步进电机驱动器：方向控制 PB13*/
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	 /* 步进电机驱动器：使能控制PB14 */
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

//定时器初始化
void MOTOR_TIM_Init(void)
{
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	TIM_InternalClockConfig(TIM3);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;			//时钟分频因子，数字滤波采样频率的参数
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 65536-1;									//ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = TIM_PRESCALER;				//PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0; 						//高级定时器功能
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
	
	TIM_OCInitTypeDef TIM_OCInitStructure;	
	TIM_OCStructInit(&TIM_OCInitStructure);	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Toggle;  						//翻转模式
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;															//CCR 比较值
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM3,TIM_OCPreload_Disable);    						//关闭预装载
	
	TIM_OCInitStructure.TIM_Pulse = 0;		//CCR 比较值
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM3,TIM_OCPreload_Disable);    //关闭预装载
	
	TIM_OCInitStructure.TIM_Pulse = 0;		//CCR 比较值
	TIM_OC3Init(TIM3, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(TIM3,TIM_OCPreload_Disable);    //关闭预装载
	
	TIM_OCInitStructure.TIM_Pulse = 0;		//CCR 比较值
	TIM_OC4Init(TIM3, &TIM_OCInitStructure);
	TIM_OC4PreloadConfig(TIM3,TIM_OCPreload_Disable);    //关闭预装载
	
	TIM_CCxCmd(TIM3,TIM_Channel_1,TIM_CCx_Disable);
	TIM_CCxCmd(TIM3,TIM_Channel_2,TIM_CCx_Disable);
	TIM_CCxCmd(TIM3,TIM_Channel_3,TIM_CCx_Disable);
	TIM_CCxCmd(TIM3,TIM_Channel_4,TIM_CCx_Disable);
	
	
	TIM_ClearFlag(TIM3,TIM_FLAG_CC1);																//清除定时器3的通道1的标志位
	TIM_ClearFlag(TIM3,TIM_FLAG_CC2);																//清除定时器3的通道1的标志位
	TIM_ClearFlag(TIM3,TIM_FLAG_CC3);																//清除定时器3的通道1的标志位
	TIM_ClearFlag(TIM3,TIM_FLAG_CC4);																//清除定时器3的通道1的标志位
	
	
	TIM_ITConfig(TIM3,TIM_IT_CC1,ENABLE); //开启定时器3比较中断
	TIM_ITConfig(TIM3,TIM_IT_CC2,ENABLE); //开启定时器3比较中断
	TIM_ITConfig(TIM3,TIM_IT_CC3,ENABLE); //开启定时器3比较中断
	TIM_ITConfig(TIM3,TIM_IT_CC4,ENABLE); //开启定时器3比较中断

	
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStructure);
	
}



void MOTOR5_GPIO_Init(void) //抓取结构的步进电机
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 				//开启PA端口时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);					//开启PB端口时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);					//开启PB端口时钟
	
	/* 步进电机驱动器：脉冲输出PA1*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;					//复用输出
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1;			          //PA1->TIM2_CH1  
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
		
	/* 步进电机驱动器：方向控制 PB13*/
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	 /* 步进电机驱动器：使能控制PB14 */
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}
//定时器初始化
void MOTOR5_TIM2_Init(void)
{
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	TIM_InternalClockConfig(TIM2);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;			//时钟分频因子，数字滤波采样频率的参数
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 65536-1;									//ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = TIM_PRESCALER;				//PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0; 						//高级定时器功能
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
	
	TIM_OCInitTypeDef TIM_OCInitStructure;	
	TIM_OCStructInit(&TIM_OCInitStructure);	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Toggle;  						//翻转模式
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;															//CCR 比较值
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM2,TIM_OCPreload_Disable);    						//关闭预装载
	TIM_CCxCmd(TIM2,TIM_Channel_2,TIM_CCx_Disable);
	
	
	TIM_ClearFlag(TIM2,TIM_FLAG_CC2);																//清除定时器3的通道1的标志位
	TIM_ITConfig(TIM2,TIM_IT_CC2,ENABLE);														//开启定时器3的通道1中断

	
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);									//分组
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStructure);

}

