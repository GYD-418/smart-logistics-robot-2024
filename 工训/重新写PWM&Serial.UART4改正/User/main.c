#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Motor.h"
#include "Key.h"
#include "PWM.h"
#include "Delay.h"
#include "Timer.h"
#include "Serial1.h"
#include "Serial2.h"
#include "Serial3.h"
#include "Serial4.h"

int ARR,PSC ,CCR;

int main(void)
{
	ARR = 100;
	PSC = 120;
	CCR = 50;
	PWM_Init(ARR,PSC,CCR);    //ARR  PSC  CCR   arr过高会堵转  f越高脉冲越多
	PWM_Init1(ARR,PSC,CCR);

	while (1)
	{
		
	}
}

void TIM2_IRQHandler(void)         //定时器6，处理标志位响应逻辑
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)  //获取中断标志位
	{
		
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);  //清除中断标志位
	}
}
