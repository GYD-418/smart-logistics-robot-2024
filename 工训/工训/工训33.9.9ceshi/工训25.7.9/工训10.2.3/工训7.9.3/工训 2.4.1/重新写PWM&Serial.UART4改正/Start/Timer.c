/* RCC开启时钟，
   选择时基单元时钟源
	 配置时基单元，包括这里的预分频器，自动重装器，计数模式 结构体
	 配置输出终端控制，允许更新中断输出到NVIC
	 配置NVIC，在NVIC中打开定时器中断的通道，并分配一个优先级
	 运行控制  使能计数器 计数器计数，触发中断
*/

/*
    void TIM_DeInit(TIM_TypeDef* TIMx);   恢复初始配置
    void TIM_TimeBaseInit(TIM_TypeDef* TIMx, TIM_TimeBaseInitTypeDef* TIM_TimeBaseInitStruct); 实基单元初始化
		void TIM_TimeBaseStructInit(TIM_TimeBaseInitTypeDef* TIM_TimeBaseInitStruct);   结构体变量的赋默认值
		void TIM_Cmd(TIM_TypeDef* TIMx, FunctionalState NewState); 使能计数器
		void TIM_ITConfig(TIM_TypeDef* TIMx, uint16_t TIM_IT, FunctionalState NewState);   使能中断信号   笔记中断输出控制
		
		void TIM_InternalClockConfig(TIM_TypeDef* TIMx);     选择内部时钟
		void TIM_ITRxExternalClockConfig(TIM_TypeDef* TIMx, uint16_t TIM_InputTriggerSource);   选择ITRx其他定时器的时钟  
		void TIM_TIxExternalClockConfig(TIM_TypeDef* TIMx, uint16_t TIM_TIxExternalCLKSource,uint16_t TIM_ICPolarity, uint16_t ICFilter);      选择TIx捕获通道的时钟
		void TIM_ETRClockMode1Config(TIM_TypeDef* TIMx, uint16_t TIM_ExtTRGPrescaler, uint16_t TIM_ExtTRGPolarity,uint16_t ExtTRGFilter);      选择ETR通过外部时钟模式1输入的时钟
		void TIM_ETRClockMode2Config(TIM_TypeDef* TIMx, uint16_t TIM_ExtTRGPrescaler,uint16_t TIM_ExtTRGPolarity, uint16_t ExtTRGFilter);      选择ETR通过外部时钟模式2输入的时钟
		void TIM_ETRConfig(TIM_TypeDef* TIMx, uint16_t TIM_ExtTRGPrescaler, uint16_t TIM_ExtTRGPolarity,uint16_t ExtTRGFilter);                单独配置ETR引脚的预分频器，极性，滤波器
    void TIM_PrescalerConfig(TIM_TypeDef* TIMx, uint16_t Prescaler, uint16_t TIM_PSCReloadMode);         预分频值 写入模式
		void TIM_CounterModeConfig(TIM_TypeDef* TIMx, uint16_t TIM_CounterMode);       改变计数器计数模式
		void TIM_ARRPreloadConfig(TIM_TypeDef* TIMx, FunctionalState NewState);        自动重装器预装功能配置
		void TIM_SetCounter(TIM_TypeDef* TIMx, uint16_t Counter);         给计数器写入一个值    手动给计数值
		void TIM_SetAutoreload(TIM_TypeDef* TIMx, uint16_t Autoreload);   给自动重装器写一个值
		uint16_t TIM_GetCounter(TIM_TypeDef* TIMx);  获取当前计数器的值
		uint16_t TIM_GetPrescaler(TIM_TypeDef* TIMx);  获取当前预分频器的值
		
		获取标志位清除标志位
		FlagStatus TIM_GetFlagStatus(TIM_TypeDef* TIMx, uint16_t TIM_FLAG);  
		void TIM_ClearFlag(TIM_TypeDef* TIMx, uint16_t TIM_FLAG);
		ITStatus TIM_GetITStatus(TIM_TypeDef* TIMx, uint16_t TIM_IT);
		void TIM_ClearITPendingBit(TIM_TypeDef* TIMx, uint16_t TIM_IT);
		
*/
#include "stm32f10x.h"                  // Device header
// extern uint16_t Num;
void Timer_init_1(int AR , int PS)
{
	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
//	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //初始化PA0 让PWM波形从PA0引脚输出
//	GPIO_InitTypeDef GPIO_InitStructure;    
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出可以控制引脚，输出控制权交给片上外设，让PWM波形从PA0引脚输出    普通开漏输出，推挽输出 引脚控制权来自于输出寄存器
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);

//	
//	TIM_TimeBaseInitTypeDef TIM_InitStructure;
//	TIM_InitStructure.TIM_Period = AR - 1;
//	TIM_InitStructure.TIM_Prescaler = PS - 1;
//	TIM_InitStructure.TIM_ClockDivision = TIM_CKD_DIV1;	//不分割时钟
//	TIM_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;	//向上计数
//	TIM_InitStructure.TIM_RepetitionCounter = 0;	//重复计数次数，就是计数溢出即申请中断
//	TIM_TimeBaseInit(TIM1, &TIM_InitStructure);	//初始化定时器
//	
//	TIM_OCInitTypeDef TIM_OCInitStructure;	
//	TIM_OCStructInit(&TIM_OCInitStructure);	
//	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Toggle;  						//翻转模式
//	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
//	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
//	TIM_OCInitStructure.TIM_Pulse = 0;															//CCR 比较值
//	TIM_OC1Init(TIM1, &TIM_OCInitStructure);
//	TIM_OC1PreloadConfig(TIM1,TIM_OCPreload_Disable);    						//关闭预装载
//	TIM_CCxCmd(TIM1,TIM_Channel_1,TIM_CCx_Disable);

//	
//	TIM_ClearFlag(TIM1,TIM_FLAG_CC1);																//清除定时器3的通道1的标志位
//	TIM_ITConfig(TIM1,TIM_IT_CC1,ENABLE);														//开启定时器3的通道1中断
//	
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);									//分组
//	
//	NVIC_InitTypeDef NVIC_InitStructure;
//	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//	NVIC_Init(&NVIC_InitStructure);

}
void Timer_Init(int AR , int PS)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);  //开启时钟
	
	TIM_InternalClockConfig(TIM6);     //选择内部时钟
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;    //结构体
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;  //指定时钟分频 采样频率 一分频
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; //计数器模式 向上计数
	TIM_TimeBaseInitStructure.TIM_Period = AR - 1;   //周期 ARR自动重装器的值  0——65535         定时频率 = 72M / (PSC+1) / (ARR+1)  10k频率计10000个数
	TIM_TimeBaseInitStructure.TIM_Prescaler = PS - 1;  //PSC预分频器的值         0——65535      预分频多 自动重装少 频率低计较少的数
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;  //重复计数器的值
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseInitStructure);    //初始化时基单元
	
	TIM_ClearFlag(TIM6, TIM_FLAG_Update);     //手动清除更新中断标志位  防止立刻更新中断进入中断从1开始
	TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);//使能中断
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;   //中断通道  定时器2   NVIC通道
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;    //响应优先级
	NVIC_Init(&NVIC_InitStructure);         //初始化结构体
	
	TIM_Cmd(TIM6, ENABLE);
}
void Timer_Init_seven(int AR , int PS)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);  //开启时钟
	
	TIM_InternalClockConfig(TIM7);     //选择内部时钟
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;    //结构体
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;  //指定时钟分频 采样频率 一分频
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; //计数器模式 向上计数
	TIM_TimeBaseInitStructure.TIM_Period = AR - 1;   //周期 ARR自动重装器的值  0——65535         定时频率 = 72M / (PSC+1) / (ARR+1)  10k频率计10000个数
	TIM_TimeBaseInitStructure.TIM_Prescaler = PS - 1;  //PSC预分频器的值         0——65535      预分频多 自动重装少 频率低计较少的数
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;  //重复计数器的值
	TIM_TimeBaseInit(TIM7, &TIM_TimeBaseInitStructure);    //初始化时基单元
	
	TIM_ClearFlag(TIM7, TIM_FLAG_Update);     //手动清除更新中断标志位  防止立刻更新中断进入中断从1开始
	TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);//使能中断
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;   //中断通道  定时器2   NVIC通道
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;    //响应优先级
	NVIC_Init(&NVIC_InitStructure);         //初始化结构体
	
	TIM_Cmd(TIM7, ENABLE);
}

/*
void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)  //获取中断标志位  
	{
		
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);  //清除标志位
	}
}
*/

