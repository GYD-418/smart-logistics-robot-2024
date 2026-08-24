#include "stm32f10x.h"                  // Device header
#include "LED.h"                  // Device header
#include "Serial1.h"                  // Device header

int bb =11,GG,YUY,next,Pick_Up,rnt,nine = 940,nine_2 = 380,nine_3 = 560;
long int crt;
void PWM_Initkk(int ARR , int PSC , int CCR)  //PWM初始化
{
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);  //开启时钟
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //初始化PA0 让PWM波形从PA0引脚输出
	GPIO_InitTypeDef GPIO_InitStructure;    
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出可以控制引脚，输出控制权交给片上外设，让PWM波形从PA0引脚输出    普通开漏输出，推挽输出 引脚控制权来自于输出寄存器
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
/*	引脚重定义需要开启AFIO的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);    //初始化PA0 让PWM波形从PA0引脚输出
	GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2,ENABLE);     //PA0重映射到PA15 重映射方式1  PA15默认使用JTDI调试端口 先关闭调试端口的复用才能将PA15作为普通的GPIO端口使用
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);  //解除引脚PA15的JTDI的复用
*/
	TIM_InternalClockConfig(TIM5);     //选择内部时钟
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;    //结构体
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;  //指定时钟分频 采样频率 一分频
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; //计数器模式 向上计数
	TIM_TimeBaseInitStructure.TIM_Period = ARR - 1;   //周期 ARR自动重装器的值  0——65535         定时频率 = 72M / (PSC+1) / (ARR+1)  10k频率计10000个数
	TIM_TimeBaseInitStructure.TIM_Prescaler = PSC - 1;  //PSC预分频器的值         0——65535      预分频多 自动重装少 频率低计较少的数
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;  //重复计数器的值
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseInitStructure);    //初始化时基单元
	
/* 初始化输出比较单元	*/
	TIM_OCInitTypeDef TIM_OCInitStructure;     //在改换配置高级定时器时，需要将所有参数都配置或者将所有参数设置一个初始值再配置结构体成员、
	TIM_OCStructInit(&TIM_OCInitStructure);    //                       结构体配置初始值

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;   //设置输出比较模式
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //有效电平是高电平   设置输出比较的极性
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;  //设置输出使能
	TIM_OCInitStructure.TIM_Pulse = CCR;  //设置CCR初始值给0 占空比控制LED灯亮度  占空比=ccr/arr+1  频率1KHz 占空比50% 分辨率1% 
	TIM_OC1Init(TIM5,&TIM_OCInitStructure);   //PA0   初始化结构体	
//	TIM_CCxCmd(TIM	_Channel_2,TIM_CCx_Enable);
}

void Servo1_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	TIM_InternalClockConfig(TIM2);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 5000 - 1;		//ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;		//PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
	
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;		//CCR
	TIM_OC3Init(TIM2, &TIM_OCInitStructure);
	
	TIM_Cmd(TIM2, ENABLE);
	
}

void Servo2_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	TIM_InternalClockConfig(TIM2);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 5000 - 1;		//ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;		//PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
	
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;		//CCR
	TIM_OC4Init(TIM2, &TIM_OCInitStructure);
	
	TIM_Cmd(TIM2, ENABLE);
	
}

void Servo3_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	TIM_InternalClockConfig(TIM2);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 5000 - 1;		//ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;		//PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
	
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;		//CCR
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);
	
	TIM_Cmd(TIM2, ENABLE);
	
}
void Servo4_Init(void)
{
	  GPIO_InitTypeDef GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_TIM1 | RCC_APB2Periph_AFIO,ENABLE);//开启时钟
	
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF_PP;//初始化GPIO--PA8、PA11为复用推挽输出
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_8 |GPIO_Pin_11;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	TIM_TimeBaseStructInit(&TIM_TimeBaseInitStruct);//初始化定时器。
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period=20000-1;
	TIM_TimeBaseInitStruct.TIM_Prescaler=72-1;
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStruct);/*【2】*///TIM2
	
	TIM_OCStructInit(&TIM_OCInitStruct);
	TIM_OCInitStruct.TIM_OCMode=TIM_OCMode_PWM1;//初始化输出比较
	TIM_OCInitStruct.TIM_OCPolarity=TIM_OCPolarity_High;
	TIM_OCInitStruct.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_Pulse=0;
	TIM_OC1Init(TIM1,&TIM_OCInitStruct);
	TIM_OC4Init(TIM1,&TIM_OCInitStruct);
	
	TIM_CtrlPWMOutputs(TIM1,ENABLE);//高级定时器专属--MOE主输出使能
	
	TIM_OC1PreloadConfig(TIM1,TIM_OCPreload_Enable);/*【3】*///ENABLE//OC1预装载寄存器使能
	TIM_OC4PreloadConfig(TIM1,TIM_OCPreload_Enable);//ENABLE//OC4预装载寄存器使能
	TIM_ARRPreloadConfig(TIM1,ENABLE);//TIM1在ARR上预装载寄存器使能
	
	TIM_Cmd(TIM1,ENABLE);//开定时器。}
}
void Motor_InitC(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}

void Motor_SetSpeedkk(int8_t Speed)
{
	if (Speed==1)
	{
		GPIO_SetBits(GPIOC, GPIO_Pin_15);
	}
	else if(Speed == 2)
	{
		GPIO_ResetBits(GPIOC, GPIO_Pin_15);
	}
}

void PWM_SetCompare2(uint16_t Compare)
{
	TIM_SetCompare2(TIM2, Compare);
}

void PWM_SetCompare3(uint16_t Compare)
{
	TIM_SetCompare3(TIM2, Compare);
}

void PWM_SetCompare4(uint16_t Compare)
{
	TIM_SetCompare4(TIM2, Compare);
}

void Servo1_SetAngle(float Angle1)
{
	PWM_SetCompare3(Angle1 / 180 * 2000 + 500);
}

void Servo2_SetAngle(float Angle2)
{
	PWM_SetCompare4(Angle2 / 180 * 2000 + 500);
}

void Servo3_SetAngle(float Angle3)
{
	PWM_SetCompare2(Angle3 / 180 * 2000 + 500);

}
void FK4(uint16_t Compare)
{
	  TIM_SetCompare1(TIM1, Compare);
		TIM_SetCompare2(TIM1, Compare);
		TIM_SetCompare3(TIM1, Compare);
		TIM_SetCompare4(TIM1, Compare);
}
void Servo4_SetAngle(float Angle44)
{
		FK4(Angle44 / 180 * 2000 + 500);
}

void PWMkk_ENABLE(void)
{	
	TIM_Cmd(TIM5, ENABLE);  //使能TIM5
}
void PWMkk_DISABLE(void)
{
	TIM_Cmd(TIM5, DISABLE);
}

void F1(int ob)//放置
{
	if(ob == 1) //先抓蓝再抓红
	{
		ActionGroup11();//机械臂扫完二维码准备抓转盘
	}
	if(ob == 2) 
	{
		ActionGroup12();//机械臂扫完二维码准备抓转盘
	}
	if(ob == 3) 
	{
		ActionGroup13();//机械臂扫完二维码准备抓转盘
	}
}
void F2(int ob)//抓色环
{
	if(ob == 1) //先抓蓝再抓红
	{
		ActionGroup91();//机械臂扫完二维码准备抓转盘
	}
	if(ob == 2) 
	{
		ActionGroup92();//机械臂扫完二维码准备抓转盘
	}
	if(ob == 3) 
	{
		ActionGroup93();//机械臂扫完二维码准备抓转盘
	}
}
void F3(int ob)//码垛
{
	if(ob == 1) //先抓蓝再抓红
	{
		ActionGroup21();//机械臂扫完二维码准备抓转盘
	}
	if(ob == 2) 
	{
		ActionGroup22();//机械臂扫完二维码准备抓转盘
	}
	if(ob == 3) 
	{
		ActionGroup23();//机械臂扫完二维码准备抓转盘
	}
}
