#include "stm32f10x.h"                  // Device header
#include "PWM.h"
#include "LED.h"
#include "stdlib.h" 
#include "Motor.h"

int oint;
extern int GG;
int BCCTR;
int BCDF;
int cxcr;
int crct;
int F_a,F_b,F_c,F_d,F_Yaw,F_Ybw,F_e;

uint8_t HV_DE[6];
uint32_t set_speed;
void Motor_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}
void Motor_InitD(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
}


void Motor_SetSpeed(int8_t Speed)
{
	if (Speed==1)                                //1,2前轮   3，4后轮
	{
		GPIO_SetBits(GPIOA, GPIO_Pin_4);
	}
	else if(Speed == 2)
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
	else if(Speed == 2)
	{
		GPIO_ResetBits(GPIOA, GPIO_Pin_5);
	}
}

void Motor_SetSpeed2(int8_t Speed)
{
	if (Speed==1)
	{
		GPIO_SetBits(GPIOD, GPIO_Pin_0);
	}
	else if(Speed == 2)
	{
		GPIO_ResetBits(GPIOD, GPIO_Pin_0);
	}
}

void Motor_SetSpeed3(int8_t Speed)
{
	if (Speed==1)
	{
		GPIO_SetBits(GPIOD, GPIO_Pin_1);
	}
	else if(Speed == 2)
	{
		GPIO_ResetBits(GPIOD, GPIO_Pin_1);
	}
}
void Motor_go(int count)
{
	if(count >= 0)
	{
		Motor_SetSpeed(2);   //右下
		Motor_SetSpeed1(1);  //左下
		Motor_SetSpeed2(1);  //右上
		Motor_SetSpeed3(2);  //左上			
		PWM_ENABLE();							
	}						 
	if(count <= 0)
	{
		Motor_SetSpeed(1);   //右下
		Motor_SetSpeed1(2);  //左下
		Motor_SetSpeed2(2);  //右上
		Motor_SetSpeed3(1);  //左上					
		PWM_ENABLE();
	}
}
void Motor_conter(float conter,float mumax,float mumin,int a,int b,int c,int d,int e)
{
	 if((conter > mumin)&&(conter < mumax))
	 {				
	 		Motor_SetSpeed(a);   //右上       前
	 		Motor_SetSpeed1(b);  //左上   
	 		Motor_SetSpeed2(c);  //右下		
	 		Motor_SetSpeed3(d);  //左下		
	 		PWM_ENABLE();
	 }
	 if(conter <= mumin)   //左转
	 {		
		  Motor_SetSpeed(1);   //右下
		  Motor_SetSpeed1(1);  //左下
		  Motor_SetSpeed2(1);  //右上
		  Motor_SetSpeed3(1);  //左上			
		  PWM_ENABLE();							

	 }
 	 if(conter >= mumax)   //右转
	 {	
		  Motor_SetSpeed(2);   //右下
	    Motor_SetSpeed1(2);  //左下
	    Motor_SetSpeed2(2);  //右上
	    Motor_SetSpeed3(2);  //左上			
	    PWM_ENABLE();							
	 }
	if(e == 55)
	{
		 if(conter <= -179 || conter >= 179)   //直走
		 {		
				Motor_SetSpeed(a);   //右下
				Motor_SetSpeed1(b);  //左下
				Motor_SetSpeed2(c);  //右上
				Motor_SetSpeed3(d);  //左上			
				PWM_ENABLE();							

		 }
		 if(conter >= -179 && conter <= 0)   //右转
		 {		
				Motor_SetSpeed(2);   //右下
				Motor_SetSpeed1(2);  //左下
				Motor_SetSpeed2(2);  //右上
				Motor_SetSpeed3(2);  //左上			
				PWM_ENABLE();							

		 }
		 if(conter > 0 && conter <= 179)   //左转
		 {	
				Motor_SetSpeed(1);   //右下
				Motor_SetSpeed1(1);  //左下
				Motor_SetSpeed2(1);  //右上
				Motor_SetSpeed3(1);  //左上			
				PWM_ENABLE();							
		 }
	 }
}
void xiansgu1(void)    //分段矫正
{
	cxcr = 212;
	crct = 120;
	BCCTR = 20;
	if(GG == 29)
	{
		BCCTR = 21;
		 if(((oint <= abs(crct*(HV_DE[3]*100+HV_DE[4]*10+HV_DE[5]-cxcr)))) && ((HV_DE[3]*100+HV_DE[4]*10+HV_DE[5]-cxcr) > 1) && ((HV_DE[3]*100+HV_DE[4]*10+HV_DE[5]) != 0))  //前走xcrc个计数器
		 {
			  BCCTR = 22;
				F_a = 1;
				F_b = 1;
				F_c = 2;
				F_d = 2;	
        MOROR_MOVE_MAX(-800*1,50,25);
				PWM_ENABLE();						
		 }
		 if((oint <= abs(crct*(HV_DE[3]*100+HV_DE[4]*10+HV_DE[5]-cxcr))) && ((HV_DE[3]*100+HV_DE[4]*10+HV_DE[5]-cxcr) < -1) && ((HV_DE[3]*100+HV_DE[4]*10+HV_DE[5]) != 0))  //后走xcrc个计数器
		 {
			  BCCTR = 23;
			  F_a = 2;
				F_b = 2;
				F_c = 1;
				F_d = 1;	
        MOROR_MOVE_MAX(-800*1,50,25);
				PWM_ENABLE();									 
		 }
		 if(((HV_DE[3]*100+HV_DE[4]*10+HV_DE[5]-cxcr) >= -1) && ((HV_DE[3]*100+HV_DE[4]*10+HV_DE[5]-cxcr) <= 1))
		 {
			  BCCTR = 24;
			  LED1_OFF();
				PWM_DISABLE();	
        GG = 50;			 
		 }
		 if((HV_DE[3]*100+HV_DE[4]*10+HV_DE[5]) == 0)
		 {
			  BCCTR = 25;
				PWM_DISABLE();									 		 
		 }	
		 if(oint >= abs(crct*(HV_DE[3]*100+HV_DE[4]*10+HV_DE[5]-cxcr)))
		 {	
				 PWM_DISABLE();									 		 
		 }

	 }
}
void xiansgu2(void)
{
	BCCTR = 13;
	cxcr = 384;
	crct = 120;
	if(GG == 21)
	{
		 BCCTR = 14;
		 LED1_ON();
		 if((oint <= abs(crct*(HV_DE[0]*100+HV_DE[1]*10+HV_DE[2]-cxcr))) && ((HV_DE[0]*100+HV_DE[1]*10+HV_DE[2]-cxcr) > 1) && ((HV_DE[0]*100+HV_DE[1]*10+HV_DE[2]) != 0))  //前走xcrc个计数器
		 {
			 BCCTR = 15;
			  F_a = 1;
				F_b = 2;
				F_c = 2;
				F_d = 1;	
        MOROR_MOVE_MAX(-800*1,50,25);		
				PWM_ENABLE();									 
		 }
		 if((oint <= abs((crct*(HV_DE[0]*100+HV_DE[1]*10+HV_DE[2]-cxcr)))) && ((HV_DE[0]*100+HV_DE[1]*10+HV_DE[2]-cxcr) < -1) && ((HV_DE[0]*100+HV_DE[1]*10+HV_DE[2]) != 0))  //后走xcrc个计数器
		 {
			 BCCTR = 16;
			  F_a = 2;
				F_b = 1;
				F_c = 1;
				F_d = 2;	
        MOROR_MOVE_MAX(-800*1,50,25);		
				PWM_ENABLE();									 
		 }   
		 if(((HV_DE[0]*100+HV_DE[1]*10+HV_DE[2]-cxcr) >= -1) && ((HV_DE[0]*100+HV_DE[1]*10+HV_DE[2]-cxcr) <= 1))
		 {
			 BCCTR = 17;
				PWM_DISABLE();	
        GG = 29;			 
		 }
		 if((HV_DE[0]*100+HV_DE[1]*10+HV_DE[2]) == 0)
		 {
			 BCCTR = 18;
				PWM_DISABLE();									 		 
		 }	
		 if(oint >= abs(crct*(HV_DE[0]*100+HV_DE[1]*10+HV_DE[2]-cxcr)))
		 {
			 BCCTR = 19;
				 PWM_DISABLE();									 		 
		 }
//			 BCCTR = 19;
	 }		 
}
