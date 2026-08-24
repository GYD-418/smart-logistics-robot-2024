#include "stm32f10x.h"                  // Device header
#include "PWM.h"
void Motor_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_4 | GPIO_Pin_5);
}
//void Motor_Init1(void)
//{
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
//	
//	GPIO_InitTypeDef GPIO_InitStructure;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOD, &GPIO_InitStructure);
//	GPIO_SetBits(GPIOD,GPIO_Pin_0 | GPIO_Pin_1);
//}

void Motor_SetSpeed(int8_t Speed)
{
	if (Speed==1)
	{
		GPIO_SetBits(GPIOA, GPIO_Pin_4);
	}
	else if(Speed == 0)
	{
		GPIO_ResetBits(GPIOA, GPIO_Pin_4);
	}
}
void Motor_SetSpeed1(int8_t Speed)
{
	if (Speed==1)
	{
		GPIO_SetBits(GPIOA, GPIO_Pin_5);
	}
	else if(Speed == 0)
	{
		GPIO_ResetBits(GPIOA, GPIO_Pin_5);
	}
}

//void Motor_SetSpeed2(int8_t Speed)
//{
//	if (Speed==1)
//	{
//		GPIO_SetBits(GPIOD, GPIO_Pin_0);
//	}
//	else if(Speed == 0)
//	{
//		GPIO_ResetBits(GPIOD, GPIO_Pin_0);
//	}
//}
//void Motor_SetSpeed3(int8_t Speed)
//{
//	if (Speed==1)
//	{
//		GPIO_SetBits(GPIOD, GPIO_Pin_1);
//	}
//	else if(Speed == 0)
//	{
//		GPIO_ResetBits(GPIOD, GPIO_Pin_1);
//	}
//}
