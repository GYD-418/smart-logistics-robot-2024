#include "stm32f10x.h"                  // Device header
#include "Motor.h"

void MOTOR_GPIO_Init(void) 
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 				//开启PA端口时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);					//开启PB端口时钟
	
	/* 步进电机驱动器：脉冲输出PA6 */
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;									//复用输出
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;									//复用输出
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;									//复用输出
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;									//复用输出
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}
 
void PWM_Init(void)  //PWM初始化
{
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);  //开启时钟
	
	
/*	引脚重定义需要开启AFIO的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);    //初始化PA0 让PWM波形从PA0引脚输出
	GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2,ENABLE);     //PA0重映射到PA15 重映射方式1  PA15默认使用JTDI调试端口 先关闭调试端口的复用才能将PA15作为普通的GPIO端口使用
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);  //解除引脚PA15的JTDI的复用
*/
	TIM_InternalClockConfig(TIM4);     //选择内部时钟
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;    //结构体
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;  //指定时钟分频 采样频率 一分频
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; //计数器模式 向上计数
	TIM_TimeBaseInitStructure.TIM_Period = 65536-1 - 1;   //周期 ARR自动重装器的值  0——65535         定时频率 = 72M / (PSC+1) / (ARR+1)  10k频率计10000个数
	TIM_TimeBaseInitStructure.TIM_Prescaler = TIM_PRESCALER - 1;  //PSC预分频器的值         0——65535      预分频多 自动重装少 频率低计较少的数
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;  //重复计数器的值
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStructure);    //初始化时基单元
	
/* 初始化输出比较单元	*/
	TIM_OCInitTypeDef TIM_OCInitStructure;     //在改换配置高级定时器时，需要将所有参数都配置或者将所有参数设置一个初始值再配置结构体成员、
	TIM_OCStructInit(&TIM_OCInitStructure);    //                       结构体配置初始值

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Toggle;   //设置输出比较模式
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //有效电平是高电平   设置输出比较的极性
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;  //设置输出使能
	TIM_OCInitStructure.TIM_Pulse = 0;  //设置CCR初始值给0 占空比控制LED灯亮度  占空比=ccr/arr+1  频率1KHz 占空比50% 分辨率1% 
	TIM_OC2Init(TIM4,&TIM_OCInitStructure);   //PA0   初始化结构体	
//	TIM_CCxCmd(TIM	_Channel_2,TIM_CCx_Enable);
	TIM_OC2PreloadConfig(TIM3,TIM_OCPreload_Disable);
  TIM_CCxCmd(TIM4,TIM_Channel_2,TIM_CCx_Disable);	
	TIM_ClearFlag(TIM4,TIM_FLAG_CC2);																//清除定时器3的通道1的标志位
	TIM_ITConfig(TIM4,TIM_IT_CC2,ENABLE);					
}
 
//void PWM_SetCompare(int Compare)
//{
//	TIM_SetCompare1(TIM3,Compare);
//}

 
void PWM_Init1(void)  //PWM初始化
{
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);  //开启时钟
	
	
/*	引脚重定义需要开启AFIO的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);    //初始化PA0 让PWM波形从PA0引脚输出
	GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2,ENABLE);     //PA0重映射到PA15 重映射方式1  PA15默认使用JTDI调试端口 先关闭调试端口的复用才能将PA15作为普通的GPIO端口使用
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);  //解除引脚PA15的JTDI的复用
*/
	TIM_InternalClockConfig(TIM4);     //选择内部时钟
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;    //结构体
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;  //指定时钟分频 采样频率 一分频
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; //计数器模式 向上计数
	TIM_TimeBaseInitStructure.TIM_Period = 65536 - 1;   //周期 ARR自动重装器的值  0——65535         定时频率 = 72M / (PSC+1) / (ARR+1)  10k频率计10000个数
	TIM_TimeBaseInitStructure.TIM_Prescaler = TIM_PRESCALER - 1;  //PSC预分频器的值         0——65535      预分频多 自动重装少 频率低计较少的数
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;  //重复计数器的值
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStructure);    //初始化时基单元
	
/* 初始化输出比较单元	*/
	TIM_OCInitTypeDef TIM_OCInitStructure;     //在改换配置高级定时器时，需要将所有参数都配置或者将所有参数设置一个初始值再配置结构体成员、
	TIM_OCStructInit(&TIM_OCInitStructure);    //                       结构体配置初始值

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Toggle;   //设置输出比较模式
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //有效电平是高电平   设置输出比较的极性
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;  //设置输出使能
	TIM_OCInitStructure.TIM_Pulse = 0;  //设置CCR初始值给0 占空比控制LED灯亮度  占空比=ccr/arr+1  频率1KHz 占空比50% 分辨率1% 
	TIM_OC1Init(TIM4,&TIM_OCInitStructure);   //PA0   初始化结构体
		
	TIM_OC1PreloadConfig(TIM4,TIM_OCPreload_Disable);
  TIM_CCxCmd(TIM4,TIM_Channel_1,TIM_CCx_Disable);	
	TIM_ClearFlag(TIM4,TIM_FLAG_CC1);																//清除定时器3的通道1的标志位
	TIM_ITConfig(TIM4,TIM_IT_CC1,ENABLE);					
				
}

void PWM_Init2(void)  //PWM初始化
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
	TIM_OC4Init(TIM3, &TIM_OCInitStructure);
	TIM_OC4PreloadConfig(TIM3,TIM_OCPreload_Disable);    						//关闭预装载
	
	
	TIM_CCxCmd(TIM3,TIM_Channel_4,TIM_CCx_Disable);
	
	
	TIM_ClearFlag(TIM3,TIM_FLAG_CC4);																//清除定时器3的通道1的标志位
	TIM_ITConfig(TIM3,TIM_IT_CC4,ENABLE);														//开启定时器3的通道1中断
 
	
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);									//分组
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStructure);
}
void PWM_Init3(void)  //PWM初始化
{
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);  //开启时钟
	
	
/*	引脚重定义需要开启AFIO的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);    //初始化PA0 让PWM波形从PA0引脚输出
	GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2,ENABLE);     //PA0重映射到PA15 重映射方式1  PA15默认使用JTDI调试端口 先关闭调试端口的复用才能将PA15作为普通的GPIO端口使用
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);  //解除引脚PA15的JTDI的复用
*/
	TIM_InternalClockConfig(TIM3);     //选择内部时钟
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;    //结构体
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;  //指定时钟分频 采样频率 一分频
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; //计数器模式 向上计数
	TIM_TimeBaseInitStructure.TIM_Period = 65536 - 1;   //周期 ARR自动重装器的值  0——65535         定时频率 = 72M / (PSC+1) / (ARR+1)  10k频率计10000个数
	TIM_TimeBaseInitStructure.TIM_Prescaler = TIM_PRESCALER - 1;  //PSC预分频器的值         0——65535      预分频多 自动重装少 频率低计较少的数
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;  //重复计数器的值
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);    //初始化时基单元
	
/* 初始化输出比较单元	*/
	TIM_OCInitTypeDef TIM_OCInitStructure;     //在改换配置高级定时器时，需要将所有参数都配置或者将所有参数设置一个初始值再配置结构体成员、
	TIM_OCStructInit(&TIM_OCInitStructure);    //                       结构体配置初始值

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Toggle;   //设置输出比较模式
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //有效电平是高电平   设置输出比较的极性
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;  //设置输出使能
	TIM_OCInitStructure.TIM_Pulse = 0;  //设置CCR初始值给0 占空比控制LED灯亮度  占空比=ccr/arr+1  频率1KHz 占空比50% 分辨率1% 
	TIM_OC3Init(TIM3,&TIM_OCInitStructure);   //PA0   初始化结构体	
	TIM_OC3PreloadConfig(TIM3,TIM_OCPreload_Disable);
  TIM_CCxCmd(TIM3,TIM_Channel_3,TIM_CCx_Disable);	
	TIM_ClearFlag(TIM3,TIM_FLAG_CC3);																//清除定时器3的通道1的标志位
	TIM_ITConfig(TIM3,TIM_IT_CC3,ENABLE);					
}

//void PWM_SetCompare(int Compare)
//{
//	TIM_SetCompare1(TIM3,Compare);
//}

void PWM_ENABLE(void)
{	
	TIM_Cmd(TIM3, ENABLE);  //使能TIM3
	TIM_Cmd(TIM4, ENABLE);  //使能TIM3	
}
void PWM_DISABLE(void)
{
	TIM_Cmd(TIM3, DISABLE);
	TIM_Cmd(TIM4, DISABLE);
}
void PWM_Init1_SED(int ARR , int PSC , int CCR)  //PWM初始化
{
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);  //开启时钟
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  //初始化PA0 让PWM波形从PA0引脚输出
	GPIO_InitTypeDef GPIO_InitStructure;    
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出可以控制引脚，输出控制权交给片上外设，让PWM波形从PA0引脚输出    普通开漏输出，推挽输出 引脚控制权来自于输出寄存器
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
/*	引脚重定义需要开启AFIO的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);    //初始化PA0 让PWM波形从PA0引脚输出
	GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2,ENABLE);     //PA0重映射到PA15 重映射方式1  PA15默认使用JTDI调试端口 先关闭调试端口的复用才能将PA15作为普通的GPIO端口使用
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);  //解除引脚PA15的JTDI的复用
*/
	TIM_InternalClockConfig(TIM4);     //选择内部时钟
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;    //结构体
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;  //指定时钟分频 采样频率 一分频
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; //计数器模式 向上计数
	TIM_TimeBaseInitStructure.TIM_Period = ARR - 1;   //周期 ARR自动重装器的值  0——65535         定时频率 = 72M / (PSC+1) / (ARR+1)  10k频率计10000个数
	TIM_TimeBaseInitStructure.TIM_Prescaler = PSC - 1;  //PSC预分频器的值         0——65535      预分频多 自动重装少 频率低计较少的数
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;  //重复计数器的值
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStructure);    //初始化时基单元
	
/* 初始化输出比较单元	*/
	TIM_OCInitTypeDef TIM_OCInitStructure;     //在改换配置高级定时器时，需要将所有参数都配置或者将所有参数设置一个初始值再配置结构体成员、
	TIM_OCStructInit(&TIM_OCInitStructure);    //                       结构体配置初始值

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;   //设置输出比较模式
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //有效电平是高电平   设置输出比较的极性
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;  //设置输出使能
	TIM_OCInitStructure.TIM_Pulse = CCR;  //设置CCR初始值给0 占空比控制LED灯亮度  占空比=ccr/arr+1  频率1KHz 占空比50% 分辨率1% 
	TIM_OC2Init(TIM4,&TIM_OCInitStructure);   //PA0   初始化结构体	
//	TIM_CCxCmd(TIM	_Channel_2,TIM_CCx_Enable);
}
 
void PWM_Init2_SED(int ARR , int PSC , int CCR)  //PWM初始化
{
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);  //开启时钟
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  //初始化PA0 让PWM波形从PA0引脚输出
	GPIO_InitTypeDef GPIO_InitStructure;    
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出可以控制引脚，输出控制权交给片上外设，让PWM波形从PA0引脚输出    普通开漏输出，推挽输出 引脚控制权来自于输出寄存器
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
/*	引脚重定义需要开启AFIO的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);    //初始化PA0 让PWM波形从PA0引脚输出
	GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2,ENABLE);     //PA0重映射到PA15 重映射方式1  PA15默认使用JTDI调试端口 先关闭调试端口的复用才能将PA15作为普通的GPIO端口使用
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);  //解除引脚PA15的JTDI的复用
*/
	TIM_InternalClockConfig(TIM4);     //选择内部时钟
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;    //结构体
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;  //指定时钟分频 采样频率 一分频
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; //计数器模式 向上计数
	TIM_TimeBaseInitStructure.TIM_Period = ARR - 1;   //周期 ARR自动重装器的值  0——65535         定时频率 = 72M / (PSC+1) / (ARR+1)  10k频率计10000个数
	TIM_TimeBaseInitStructure.TIM_Prescaler = PSC - 1;  //PSC预分频器的值         0——65535      预分频多 自动重装少 频率低计较少的数
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;  //重复计数器的值
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStructure);    //初始化时基单元
	
/* 初始化输出比较单元	*/
	TIM_OCInitTypeDef TIM_OCInitStructure;     //在改换配置高级定时器时，需要将所有参数都配置或者将所有参数设置一个初始值再配置结构体成员、
	TIM_OCStructInit(&TIM_OCInitStructure);    //                       结构体配置初始值

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;   //设置输出比较模式
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //有效电平是高电平   设置输出比较的极性
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;  //设置输出使能
	TIM_OCInitStructure.TIM_Pulse = CCR;  //设置CCR初始值给0 占空比控制LED灯亮度  占空比=ccr/arr+1  频率1KHz 占空比50% 分辨率1% 
	TIM_OC1Init(TIM4,&TIM_OCInitStructure);   //PA0   初始化结构体	
}

void PWM_Init3_SED(int ARR , int PSC , int CCR)  //PWM初始化
{
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);  //开启时钟
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  //初始化PA0 让PWM波形从PA0引脚输出
	GPIO_InitTypeDef GPIO_InitStructure;    
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出可以控制引脚，输出控制权交给片上外设，让PWM波形从PA0引脚输出    普通开漏输出，推挽输出 引脚控制权来自于输出寄存器
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
/*	引脚重定义需要开启AFIO的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);    //初始化PA0 让PWM波形从PA0引脚输出
	GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2,ENABLE);     //PA0重映射到PA15 重映射方式1  PA15默认使用JTDI调试端口 先关闭调试端口的复用才能将PA15作为普通的GPIO端口使用
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);  //解除引脚PA15的JTDI的复用
*/
	TIM_InternalClockConfig(TIM3);     //选择内部时钟
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;    //结构体
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;  //指定时钟分频 采样频率 一分频
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; //计数器模式 向上计数
	TIM_TimeBaseInitStructure.TIM_Period = ARR - 1;   //周期 ARR自动重装器的值  0——65535         定时频率 = 72M / (PSC+1) / (ARR+1)  10k频率计10000个数
	TIM_TimeBaseInitStructure.TIM_Prescaler = PSC - 1;  //PSC预分频器的值         0——65535      预分频多 自动重装少 频率低计较少的数
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;  //重复计数器的值
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);    //初始化时基单元
	
/* 初始化输出比较单元	*/
	TIM_OCInitTypeDef TIM_OCInitStructure;     //在改换配置高级定时器时，需要将所有参数都配置或者将所有参数设置一个初始值再配置结构体成员、
	TIM_OCStructInit(&TIM_OCInitStructure);    //                       结构体配置初始值

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;   //设置输出比较模式
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //有效电平是高电平   设置输出比较的极性
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;  //设置输出使能
	TIM_OCInitStructure.TIM_Pulse = CCR;  //设置CCR初始值给0 占空比控制LED灯亮度  占空比=ccr/arr+1  频率1KHz 占空比50% 分辨率1% 
	TIM_OC3Init(TIM3,&TIM_OCInitStructure);   //PA0   初始化结构体	
}
void PWM_Init4_SED(int ARR , int PSC , int CCR)  //PWM初始化
{
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);  //开启时钟
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  //初始化PA0 让PWM波形从PA0引脚输出
	GPIO_InitTypeDef GPIO_InitStructure;    
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出可以控制引脚，输出控制权交给片上外设，让PWM波形从PA0引脚输出    普通开漏输出，推挽输出 引脚控制权来自于输出寄存器
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
/*	引脚重定义需要开启AFIO的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);    //初始化PA0 让PWM波形从PA0引脚输出
	GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2,ENABLE);     //PA0重映射到PA15 重映射方式1  PA15默认使用JTDI调试端口 先关闭调试端口的复用才能将PA15作为普通的GPIO端口使用
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);  //解除引脚PA15的JTDI的复用
*/
	TIM_InternalClockConfig(TIM3);     //选择内部时钟
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;    //结构体
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;  //指定时钟分频 采样频率 一分频
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; //计数器模式 向上计数
	TIM_TimeBaseInitStructure.TIM_Period = ARR - 1;   //周期 ARR自动重装器的值  0——65535         定时频率 = 72M / (PSC+1) / (ARR+1)  10k频率计10000个数
	TIM_TimeBaseInitStructure.TIM_Prescaler = PSC - 1;  //PSC预分频器的值         0——65535      预分频多 自动重装少 频率低计较少的数
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;  //重复计数器的值
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);    //初始化时基单元
	
/* 初始化输出比较单元	*/
	TIM_OCInitTypeDef TIM_OCInitStructure;     //在改换配置高级定时器时，需要将所有参数都配置或者将所有参数设置一个初始值再配置结构体成员、
	TIM_OCStructInit(&TIM_OCInitStructure);    //                       结构体配置初始值

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;   //设置输出比较模式
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //有效电平是高电平   设置输出比较的极性
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;  //设置输出使能
	TIM_OCInitStructure.TIM_Pulse = CCR;  //设置CCR初始值给0 占空比控制LED灯亮度  占空比=ccr/arr+1  频率1KHz 占空比50% 分辨率1% 
	TIM_OC4Init(TIM3,&TIM_OCInitStructure);   //PA0   初始化结构体	
}
void PWM_SED_INIT(void)
{
	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, DISABLE);  //开启时钟
	
	
/*	引脚重定义需要开启AFIO的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);    //初始化PA0 让PWM波形从PA0引脚输出
	GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2,ENABLE);     //PA0重映射到PA15 重映射方式1  PA15默认使用JTDI调试端口 先关闭调试端口的复用才能将PA15作为普通的GPIO端口使用
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);  //解除引脚PA15的JTDI的复用
*/
	TIM_InternalClockConfig(TIM3);     //选择内部时钟
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure1;    //结构体
	TIM_TimeBaseInitStructure1.TIM_ClockDivision = TIM_CKD_DIV1;  //指定时钟分频 采样频率 一分频
	TIM_TimeBaseInitStructure1.TIM_CounterMode = TIM_CounterMode_Up; //计数器模式 向上计数
	TIM_TimeBaseInitStructure1.TIM_Period = 65536 - 1;   //周期 ARR自动重装器的值  0——65535         定时频率 = 72M / (PSC+1) / (ARR+1)  10k频率计10000个数
	TIM_TimeBaseInitStructure1.TIM_Prescaler = TIM_PRESCALER - 1;  //PSC预分频器的值         0——65535      预分频多 自动重装少 频率低计较少的数
	TIM_TimeBaseInitStructure1.TIM_RepetitionCounter = 0;  //重复计数器的值
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure1);    //初始化时基单元
	
/* 初始化输出比较单元	*/
	TIM_OCInitTypeDef TIM_OCInitStructure;     //在改换配置高级定时器时，需要将所有参数都配置或者将所有参数设置一个初始值再配置结构体成员、
	TIM_OCStructInit(&TIM_OCInitStructure);    //                       结构体配置初始值

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Toggle;   //设置输出比较模式
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //有效电平是高电平   设置输出比较的极性
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable;  //设置输出使能
	TIM_OCInitStructure.TIM_Pulse = 0;  //设置CCR初始值给0 占空比控制LED灯亮度  占空比=ccr/arr+1  频率1KHz 占空比50% 分辨率1% 
	TIM_OC3Init(TIM3,&TIM_OCInitStructure);   //PA0   初始化结构体	
	TIM_OC3PreloadConfig(TIM3,TIM_OCPreload_Disable);
  TIM_CCxCmd(TIM3,TIM_Channel_3,TIM_CCx_Disable);	
	TIM_ClearFlag(TIM3,TIM_FLAG_CC3);																//清除定时器3的通道1的标志位
	TIM_ITConfig(TIM3,TIM_IT_CC3,DISABLE);					

	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, DISABLE);
	
	TIM_InternalClockConfig(TIM3);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;			//时钟分频因子，数字滤波采样频率的参数
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 65536-1;									//ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = TIM_PRESCALER;				//PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0; 						//高级定时器功能
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
	
	TIM_OCInitTypeDef TIM_OCInitStructure2;	
	TIM_OCStructInit(&TIM_OCInitStructure2);	
	TIM_OCInitStructure2.TIM_OCMode = TIM_OCMode_Toggle;  						//翻转模式
	TIM_OCInitStructure2.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure2.TIM_OutputState = TIM_OutputState_Disable;
	TIM_OCInitStructure2.TIM_Pulse = 0;															//CCR 比较值
	TIM_OC4Init(TIM3, &TIM_OCInitStructure2);
	TIM_OC4PreloadConfig(TIM3,TIM_OCPreload_Disable);    						//关闭预装载
	
	
	TIM_CCxCmd(TIM3,TIM_Channel_4,TIM_CCx_Disable);
	
	
	TIM_ClearFlag(TIM3,TIM_FLAG_CC4);																//清除定时器3的通道1的标志位
	TIM_ITConfig(TIM3,TIM_IT_CC4,DISABLE);														//开启定时器3的通道1中断
 
	
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);									//分组
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStructure);

	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, DISABLE);  //开启时钟
	
	
/*	引脚重定义需要开启AFIO的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);    //初始化PA0 让PWM波形从PA0引脚输出
	GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2,ENABLE);     //PA0重映射到PA15 重映射方式1  PA15默认使用JTDI调试端口 先关闭调试端口的复用才能将PA15作为普通的GPIO端口使用
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);  //解除引脚PA15的JTDI的复用
*/
	TIM_InternalClockConfig(TIM4);     //选择内部时钟
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure3;    //结构体
	TIM_TimeBaseInitStructure3.TIM_ClockDivision = TIM_CKD_DIV1;  //指定时钟分频 采样频率 一分频
	TIM_TimeBaseInitStructure3.TIM_CounterMode = TIM_CounterMode_Up; //计数器模式 向上计数
	TIM_TimeBaseInitStructure3.TIM_Period = 65536 - 1;   //周期 ARR自动重装器的值  0——65535         定时频率 = 72M / (PSC+1) / (ARR+1)  10k频率计10000个数
	TIM_TimeBaseInitStructure3.TIM_Prescaler = TIM_PRESCALER - 1;  //PSC预分频器的值         0——65535      预分频多 自动重装少 频率低计较少的数
	TIM_TimeBaseInitStructure3.TIM_RepetitionCounter = 0;  //重复计数器的值
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStructure3);    //初始化时基单元
	
/* 初始化输出比较单元	*/
	TIM_OCInitTypeDef TIM_OCInitStructure3;     //在改换配置高级定时器时，需要将所有参数都配置或者将所有参数设置一个初始值再配置结构体成员、
	TIM_OCStructInit(&TIM_OCInitStructure3);    //                       结构体配置初始值

	TIM_OCInitStructure3.TIM_OCMode = TIM_OCMode_Toggle;   //设置输出比较模式
	TIM_OCInitStructure3.TIM_OCPolarity = TIM_OCPolarity_High;  //有效电平是高电平   设置输出比较的极性
	TIM_OCInitStructure3.TIM_OutputState = TIM_OutputState_Disable;  //设置输出使能
	TIM_OCInitStructure3.TIM_Pulse = 0;  //设置CCR初始值给0 占空比控制LED灯亮度  占空比=ccr/arr+1  频率1KHz 占空比50% 分辨率1% 
	TIM_OC1Init(TIM4,&TIM_OCInitStructure3);   //PA0   初始化结构体
		
	TIM_OC1PreloadConfig(TIM4,TIM_OCPreload_Disable);
  TIM_CCxCmd(TIM4,TIM_Channel_1,TIM_CCx_Disable);	
	TIM_ClearFlag(TIM4,TIM_FLAG_CC1);																//清除定时器3的通道1的标志位
	TIM_ITConfig(TIM4,TIM_IT_CC1,DISABLE);					

	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, DISABLE);  //开启时钟
	
	
/*	引脚重定义需要开启AFIO的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);    //初始化PA0 让PWM波形从PA0引脚输出
	GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2,ENABLE);     //PA0重映射到PA15 重映射方式1  PA15默认使用JTDI调试端口 先关闭调试端口的复用才能将PA15作为普通的GPIO端口使用
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);  //解除引脚PA15的JTDI的复用
*/
	TIM_InternalClockConfig(TIM4);     //选择内部时钟
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure4;    //结构体
	TIM_TimeBaseInitStructure4.TIM_ClockDivision = TIM_CKD_DIV1;  //指定时钟分频 采样频率 一分频
	TIM_TimeBaseInitStructure4.TIM_CounterMode = TIM_CounterMode_Up; //计数器模式 向上计数
	TIM_TimeBaseInitStructure4.TIM_Period = 65536-1 - 1;   //周期 ARR自动重装器的值  0——65535         定时频率 = 72M / (PSC+1) / (ARR+1)  10k频率计10000个数
	TIM_TimeBaseInitStructure4.TIM_Prescaler = TIM_PRESCALER - 1;  //PSC预分频器的值         0——65535      预分频多 自动重装少 频率低计较少的数
	TIM_TimeBaseInitStructure4.TIM_RepetitionCounter = 0;  //重复计数器的值
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStructure4);    //初始化时基单元
	
/* 初始化输出比较单元	*/
	TIM_OCInitTypeDef TIM_OCInitStructure5;     //在改换配置高级定时器时，需要将所有参数都配置或者将所有参数设置一个初始值再配置结构体成员、
	TIM_OCStructInit(&TIM_OCInitStructure5);    //                       结构体配置初始值

	TIM_OCInitStructure5.TIM_OCMode = TIM_OCMode_Toggle;   //设置输出比较模式
	TIM_OCInitStructure5.TIM_OCPolarity = TIM_OCPolarity_High;  //有效电平是高电平   设置输出比较的极性
	TIM_OCInitStructure5.TIM_OutputState = TIM_OutputState_Disable;  //设置输出使能
	TIM_OCInitStructure5.TIM_Pulse = 0;  //设置CCR初始值给0 占空比控制LED灯亮度  占空比=ccr/arr+1  频率1KHz 占空比50% 分辨率1% 
	TIM_OC2Init(TIM4,&TIM_OCInitStructure5);   //PA0   初始化结构体	
//	TIM_CCxCmd(TIM	_Channel_2,TIM_CCx_Enable);
	TIM_OC2PreloadConfig(TIM3,TIM_OCPreload_Disable);
  TIM_CCxCmd(TIM4,TIM_Channel_2,TIM_CCx_Disable);	
	TIM_ClearFlag(TIM4,TIM_FLAG_CC2);																//清除定时器3的通道1的标志位
	TIM_ITConfig(TIM4,TIM_IT_CC2,DISABLE);					

	PWM_Init4_SED(24000 ,150 ,24000/2);  //PWM初始化
	PWM_Init1_SED(24000 ,150 ,24000/2);  //PWM初始化
	PWM_Init2_SED(24000 ,150 ,24000/2);  //PWM初始化
	PWM_Init3_SED(24000 ,150 ,24000/2);  //PWM初始化
	PWM_ENABLE();
}
void PWM_ONE_INIT(void)
{
	PWM_Init();
	PWM_Init1();
	PWM_Init2();
	PWM_Init3();
}

