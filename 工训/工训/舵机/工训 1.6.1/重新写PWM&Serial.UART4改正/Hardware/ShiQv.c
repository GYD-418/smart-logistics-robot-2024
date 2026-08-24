#include "stm32f10x.h"                  // Device header

int aa,bb,kk;

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
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);
	
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
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);
	
	TIM_Cmd(TIM2, ENABLE);
	
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


void PWMkk_ENABLE(void)
{	
	TIM_Cmd(TIM5, ENABLE);  //使能TIM5
}
void PWMkk_DISABLE(void)
{
	TIM_Cmd(TIM5, DISABLE);
}


void ShiQv(void)
{
	PWM_Initkk(72,100,50); 
	aa++;
	if(aa>=100 && aa<=400)
	{
		PWMkk_ENABLE();
		Motor_SetSpeedkk(1);	//向下
	}
	if(aa>400 && aa<=500)
	{
	    PWMkk_DISABLE();
		Servo1_SetAngle(30);
	}
	if(aa>500 && aa<760)
	{
		PWMkk_ENABLE();
	    Motor_SetSpeedkk(2);	//向上
	}
	if(aa>=760 && aa<1400)
	{
		PWMkk_DISABLE();
		Servo2_SetAngle(159);
	}
	if(aa>=1580 && aa<1680)
	{
	  Servo1_SetAngle(55);
	}
	if(aa>1680 && aa<1690)
	{
	  Servo2_SetAngle(45);
	}
	if(aa>1690)
	{ 
	  PWMkk_DISABLE(); 
	  bb=0;
	  aa=0;
	}
}
void ShiQv2(void)
{
	kk++;
	if(kk>=100 && kk<=740)
	{
		Servo2_SetAngle(159);
	}
	if(kk>740 && kk<=840)
	{
		Servo1_SetAngle(30);
	}
	if(kk>840 && kk<=1080)
	{
		Servo2_SetAngle(40);
	}
	if(kk>1080 && kk<=1380)
	{
		PWM_Initkk(72,100,50);
		PWMkk_ENABLE();
		Motor_SetSpeedkk(1);	//向下
	}
	if(kk>1380 && kk<=1480)
	{
		PWMkk_DISABLE();
	}
	if(kk>1620 && kk<=1680)
	{
		Servo1_SetAngle(55);
	}
	if(kk>1680 && kk<=1980)
	{
		
		PWM_Initkk(72,100,50);
		PWMkk_ENABLE();
	    Motor_SetSpeedkk(2);	//向上
	}
	if(kk>1980)
	{
		bb=0;
		kk=0;
	}
}
