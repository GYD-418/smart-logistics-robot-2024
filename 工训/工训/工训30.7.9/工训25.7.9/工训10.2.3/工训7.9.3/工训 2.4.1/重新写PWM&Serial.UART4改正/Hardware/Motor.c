#include "stm32f10x.h"                  // Device header
#include "PWM.h"
#include "LED.h"
#include "Motor.h"
#include "stdlib.h" 
#include "Serial3.h" 
#include "Delay.h"

extern int GG;
int F_a,F_b,F_c,F_d,F_Yaw,F_Ybw,F_e;
extern float yaw;
int STS,xzq;
uint8_t getx,gety,goalx,goaly;
double adjust_x,adjust_y,adjust_ratio;

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
		 if((conter > mumin)&&(conter < mumax)&&(e == 0))
		 {				
				SPEET_BUSHU_V = 0;
				STS = 10;
				Motor_SetSpeed(a);   //右上       前
				Motor_SetSpeed1(b);  //左上   
				Motor_SetSpeed2(c);  //右下		
				Motor_SetSpeed3(d);  //左下			 
	//	 		PWM_ENABLE();
		 }
		 if(conter <= mumin && (e == 0))   //左转
		 {	  
				STS = 50;		
				if((SPEET_BUSHU_V == 0) && (e == 0))
				{						 
					Motor_SetSpeed(a);   //右上       前
					Motor_SetSpeed1(b);  //左上   
					Motor_SetSpeed2(c);  //右下		
					Motor_SetSpeed3(d);  //左下			 
					MOROR_MOVE_MAX(-200*1,50,STS);
				}				
				if((SPEET_BUSHU_V >= 1) && (e == 0))
				{
					Motor_SetSpeed(1);   //右下
					Motor_SetSpeed1(1);  //左下
					Motor_SetSpeed2(1);  //右上
					Motor_SetSpeed3(1);  //左上
					MOROR_MOVE_MAX(-200*1,50,STS); 
				}
				PWM_ENABLE();							
		 }
		 if((conter >= mumax) && (e == 0))   //右转
		 {	
				STS = 50;		
				if((SPEET_BUSHU_V == 0) && (e == 0))
				{				
					Motor_SetSpeed(a);   //右上       前
					Motor_SetSpeed1(b);  //左上   
					Motor_SetSpeed2(c);  //右下		
					Motor_SetSpeed3(d);  //左下			 
					MOROR_MOVE_MAX(-200*1,50,STS); 
				}
				if((SPEET_BUSHU_V >= 1) && (e == 0))
				{
					Motor_SetSpeed(2);   //右下
					Motor_SetSpeed1(2);  //左下
					Motor_SetSpeed2(2);  //右上
					Motor_SetSpeed3(2);  //左上
					MOROR_MOVE_MAX(-200*1,50,STS);	
				}				
				PWM_ENABLE();							
		 }
		 if((conter > mumin)&&(conter < mumax)&&(e == 91))
		 {				
				SPEET_BUSHU_V = 0;
				STS = 10;
				Motor_SetSpeed(a);   //右上       前
				Motor_SetSpeed1(b);  //左上   
				Motor_SetSpeed2(c);  //右下		
				Motor_SetSpeed3(d);  //左下			 
	//	 		PWM_ENABLE();
		 }
		 if(conter <= mumin && (e == 91))   //左转
		 {	  
				STS = 50;		
				if((SPEET_BUSHU_V == 0) && (e == 91))
				{						 
					Motor_SetSpeed(a);   //右上       前
					Motor_SetSpeed1(b);  //左上   
					Motor_SetSpeed2(c);  //右下		
					Motor_SetSpeed3(d);  //左下			 
					MOROR_MOVE_MAX(-200*1,50,STS);
				}				
				if((SPEET_BUSHU_V >= 1) && (e == 91) && (conter >= 0))
				{
					Motor_SetSpeed(1);   //右下
					Motor_SetSpeed1(1);  //左下
					Motor_SetSpeed2(1);  //右上
					Motor_SetSpeed3(1);  //左上
					MOROR_MOVE_MAX(-200*1,50,STS); 
				}
				if((SPEET_BUSHU_V >= 1) && (e == 91) && (conter <= 0))
				{
					Motor_SetSpeed(2);   //右下
					Motor_SetSpeed1(2);  //左下
					Motor_SetSpeed2(2);  //右上
					Motor_SetSpeed3(2);  //左上
					MOROR_MOVE_MAX(-200*1,50,STS); 
				}
				PWM_ENABLE();							
		 }
		 if((conter >= mumax) && (e == 91))   //右转
		 {	
				STS = 50;		
				if((SPEET_BUSHU_V == 0) && (e == 91))
				{				
					Motor_SetSpeed(a);   //右上       前
					Motor_SetSpeed1(b);  //左上   
					Motor_SetSpeed2(c);  //右下		
					Motor_SetSpeed3(d);  //左下			 
					MOROR_MOVE_MAX(-200*1,50,STS); 
				}
				if((SPEET_BUSHU_V >= 1) && (e == 91))
				{
					Motor_SetSpeed(2);   //右下
					Motor_SetSpeed1(2);  //左下
					Motor_SetSpeed2(2);  //右上
					Motor_SetSpeed3(2);  //左上
					MOROR_MOVE_MAX(-200*1,50,STS);	
				}				
				PWM_ENABLE();							
		 }
		 if((conter < -179 || conter > 179) && (e == 55))   //直走
		 {	
        SPEET_BUSHU_V = 0;
		    STS = 10;			 
				Motor_SetSpeed(a);   //右下
				Motor_SetSpeed1(b);  //左下
				Motor_SetSpeed2(c);  //右上
				Motor_SetSpeed3(d);  //左上			
		 }
		 if((conter >= -179 && conter <= 0) && (e == 55))   //右转
		 {		
			  STS = 50;		
				if((SPEET_BUSHU_V == 0) && (e == 55))
				{						 
					Motor_SetSpeed(a);   //右上       前
					Motor_SetSpeed1(b);  //左上   
					Motor_SetSpeed2(c);  //右下		
					Motor_SetSpeed3(d);  //左下			 
					MOROR_MOVE_MAX(-200*1,50,STS);
				}		
				if((SPEET_BUSHU_V >= 1) && (e == 55))
				{				
					Motor_SetSpeed(2);   //右下
					Motor_SetSpeed1(2);  //左下
					Motor_SetSpeed2(2);  //右上
					Motor_SetSpeed3(2);  //左上
					MOROR_MOVE_MAX(-200*1,50,STS);					
				}					
		 		PWM_ENABLE();
			}
		 if((conter > 0 && conter <= 179) && (e == 55))   //左转
		 {
				STS = 50;		
				if((SPEET_BUSHU_V == 0) && (e == 55))
				{						 
					Motor_SetSpeed(a);   //右上       前
					Motor_SetSpeed1(b);  //左上   
					Motor_SetSpeed2(c);  //右下		
					Motor_SetSpeed3(d);  //左下			 
					MOROR_MOVE_MAX(-200*1,50,STS);
				}				
				if((SPEET_BUSHU_V >= 1) && (e == 55))
				{					
					Motor_SetSpeed(1);   //右下
					Motor_SetSpeed1(1);  //左下
					Motor_SetSpeed2(1);  //右上
					Motor_SetSpeed3(1);  //左上		
					MOROR_MOVE_MAX(-200*1,50,STS);					
				}				
		 		PWM_ENABLE();		
		}
	 
}
//int x,y,goalx,goaly,adjust_x,adjust_y,adjust_ratio;
void kk_adjust(void)
{
	if(GG == 21)
	{
		adjust_x = 273;
		adjust_y = 248;
		adjust_ratio = 5;
		getx=(Serial_RxPacket_2[1]*100)+(Serial_RxPacket_2[4]*10)+(Serial_RxPacket_2[7]);
		gety=(Serial_RxPacket_2[10]*100)+(Serial_RxPacket_2[13]*10)+(Serial_RxPacket_2[16]);
		if((goalx-2) < getx < (goalx+2))		//x矫正完
		{
			xzq = 17;
		}
		if((goaly-2) < gety < (goaly-2))		//y矫正完
		{
			xzq = 717;
		}
		if(xzq == 17 && xzq == 717)						//x，y都矫正完GG=50
		{
			GG = 50;
		}
		adjust_x = (getx-goalx)*adjust_ratio;
		adjust_y = (gety-goaly)*adjust_ratio;
		if(adjust_x < 0)  //在x左边，右大
		{
			LED1_ON();
			F_a = 1;
			F_b = 2;
			F_c = 2;
			F_d = 1;
			adjust_x = -adjust_x;
			Motor_conter(yaw,F_Yaw,F_Ybw,F_a,F_b,F_c,F_d,F_e);
			MOROR_MOVE_MAX(-adjust_x*1,100,100);
			PWM_ENABLE();	
		}
		if(adjust_x > 0)		//在x右边，左小
		{
			 LED1_ON();
			 F_a = 2;
			 F_b = 1;
			 F_c = 1;
			 F_d = 2;	
			 Motor_conter(yaw,F_Yaw,F_Ybw,F_a,F_b,F_c,F_d,F_e);
			 MOROR_MOVE_MAX(-adjust_x*1,100,100);
			 PWM_ENABLE();						
		}
		if(adjust_y < 0)		//在y前边，后大
		{
			LED1_ON();
			F_a = 1;
			F_b = 1;
			F_c = 2;
			F_d = 2;	
			adjust_y = -adjust_y;
			Motor_conter(yaw,F_Yaw,F_Ybw,F_a,F_b,F_c,F_d,F_e);
			MOROR_MOVE_MAX(-adjust_y*1,100,100);		
			PWM_ENABLE();					
		}
		if(adjust_y > 0)		//在y后边，前小
		{
			 LED1_ON();
			 F_a = 2;
			 F_b = 2;
			 F_c = 1;
			 F_d = 1;	
			 Motor_conter(yaw,F_Yaw,F_Ybw,F_a,F_b,F_c,F_d,F_e);
			 MOROR_MOVE_MAX(-adjust_y*1,100,100);		
			 PWM_ENABLE();			
		}
		Delay_ms(200);
	}
	
}

void xiansgu1(void)    //分段矫正  左小  266 268     194  198
{
	if(GG == 21)
	{
		 if(((Serial_RxPacket_2[1]*100)+(Serial_RxPacket_2[4]*10)+(Serial_RxPacket_2[7])) >= 274)  //前走xcrc个计数器284 288       200 204 
		 {
			 LED1_ON();
				F_a = 2;
				F_b = 1;
				F_c = 1;
				F_d = 2;	
			  Motor_conter(yaw,F_Yaw,F_Ybw,F_a,F_b,F_c,F_d,F_e);
        MOROR_MOVE_MAX(-20*1,100,100);
				PWM_ENABLE();						
		 }
		 if(((Serial_RxPacket_2[1]*100)+(Serial_RxPacket_2[4]*10)+(Serial_RxPacket_2[7])) <= 271)  //后走xcrc个计数器
		 {
			 LED1_ON();
			  F_a = 1;
				F_b = 2;
				F_c = 2;
				F_d = 1;	
			  Motor_conter(yaw,F_Yaw,F_Ybw,F_a,F_b,F_c,F_d,F_e);
        MOROR_MOVE_MAX(-20*1,100,100);
				PWM_ENABLE();									 
		 }
		 if(((Serial_RxPacket_2[1]*100)+(Serial_RxPacket_2[4]*10)+(Serial_RxPacket_2[7])) == 0)  //后走xcrc个计数器
		 {
			 LED1_ON();
				PWM_DISABLE();									 
		 }
		 if((((Serial_RxPacket_2[1]*100)+(Serial_RxPacket_2[4]*10)+(Serial_RxPacket_2[7])) > 271) && (((Serial_RxPacket_2[1]*100)+(Serial_RxPacket_2[4]*10)+(Serial_RxPacket_2[7])) < 274 ))
		 {
			  MOROR_MOVE_MAX(-1*1,10,30);
				PWM_DISABLE();	
        GG = 29;			 
		 }
	 }
}
void xiansgu2(void)  //前小
{
	if(GG == 29)
	{
		 if(((Serial_RxPacket_2[10]*100)+(Serial_RxPacket_2[13]*10)+(Serial_RxPacket_2[16])) >= 249)  //前走xcrc个计数器
		 {
			 LED1_ON();
			  F_a = 2;
				F_b = 2;
				F_c = 1;
				F_d = 1;	
			  Motor_conter(yaw,F_Yaw,F_Ybw,F_a,F_b,F_c,F_d,F_e);
        MOROR_MOVE_MAX(-20*1,100,100);		
				PWM_ENABLE();									 
		 }
		 if(((Serial_RxPacket_2[10]*100)+(Serial_RxPacket_2[13]*10)+(Serial_RxPacket_2[16])) <= 247)  //前走xcrc个计数器
		 {
			 LED1_ON();
			  F_a = 1;
				F_b = 1;
				F_c = 2;
				F_d = 2;	
			  Motor_conter(yaw,F_Yaw,F_Ybw,F_a,F_b,F_c,F_d,F_e);
        MOROR_MOVE_MAX(-20*1,100,100);		
				PWM_ENABLE();									 
		 }   
		 if(((Serial_RxPacket_2[10]*100)+(Serial_RxPacket_2[13]*10)+(Serial_RxPacket_2[16])) == 0)  //前走xcrc个计数器
		 {
			 LED1_ON();
				PWM_DISABLE();									 
		 }   
		 if((((Serial_RxPacket_2[10]*100)+(Serial_RxPacket_2[13]*10)+(Serial_RxPacket_2[16])) > 247) && (((Serial_RxPacket_2[10]*100)+(Serial_RxPacket_2[13]*10)+(Serial_RxPacket_2[16])) < 249))
		 {
			  MOROR_MOVE_MAX(-1*1,10,30);
				PWM_DISABLE();			 
			  LED1_OFF();
        GG = 50;			 
		 }
	 }		 		 
}
void xiansgu3(void)    //分段矫正  左小
{
//	if(GG == 68)
//	{
//		 if(((Serial_RxPacket_2[10]*100)+(Serial_RxPacket_2[13]*10)+(Serial_RxPacket_2[16])) >= 198)  //前走xcrc个计数器
//		 {
//			 LED1_ON();
//			  F_a = 2;
//				F_b = 2;
//				F_c = 1;
//				F_d = 1;	
//			  Motor_conter(yaw,F_Yaw,F_Ybw,F_a,F_b,F_c,F_d,F_e);
//        MOROR_MOVE_MAX(-200*1,10,20);		
//				PWM_ENABLE();									 
//		 }
//		 if(((Serial_RxPacket_2[10]*100)+(Serial_RxPacket_2[13]*10)+(Serial_RxPacket_2[16])) <= 195)  //前走xcrc个计数器
//		 {
//			 LED1_ON();
//			  F_a = 1;
//				F_b = 1;
//				F_c = 2;
//				F_d = 2;	
//			  Motor_conter(yaw,F_Yaw,F_Ybw,F_a,F_b,F_c,F_d,F_e);
//        MOROR_MOVE_MAX(-200*1,10,20);		
//				PWM_ENABLE();									 
//		 }   
//		 if(((Serial_RxPacket_2[10]*100)+(Serial_RxPacket_2[13]*10)+(Serial_RxPacket_2[16])) == 0)  //前走xcrc个计数器
//		 {
//			 LED1_ON();
//				PWM_DISABLE();									 
//		 }   
//		 if((((Serial_RxPacket_2[10]*100)+(Serial_RxPacket_2[13]*10)+(Serial_RxPacket_2[16])) > 195) && (((Serial_RxPacket_2[10]*100)+(Serial_RxPacket_2[13]*10)+(Serial_RxPacket_2[16])) < 198))
//		 {
//			 LED1_OFF();
//				PWM_DISABLE();	
//        GG = 50;			 
//		 }
//	 }
}
