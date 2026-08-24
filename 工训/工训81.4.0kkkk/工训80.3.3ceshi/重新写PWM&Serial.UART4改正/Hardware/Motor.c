#include "stm32f10x.h"                  // Device header
#include "PWM.h"
#include "LED.h"
#include "Motor.h"
#include "stdlib.h" 
#include "Serial3.h" 
#include "WZJZ.h" 

int Rct;
int aakk;
int ZBX[2];
int ZBW[2];
int Cunt1 = 337,Cunt2 = 326;
int Cunt;
extern int PID_Serial_ZYQH[2];
extern int GG;
float F_a,F_b,F_c,F_d,F_Yaw,F_Ybw,F_e;
extern float yaw;
int open_CD;
int STS;
int jj;
int Three_CnEn_SH = 1;
int ard;
int aa;
int HUYNT = 1;
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
void Motor_go_HQ(int count)
{
		if(count == Cunt)
		{
			PWM_DISABLE();
		}
	if(count >= 0)
	{
		F_a = 1;
		F_b = 1;
		F_c = 2;
		F_d = 2;	
	}						 
	if(count <= 0)
	{
		F_a = 2;
		F_b = 2;
		F_c = 1;
		F_d = 1;	
	}
}
void Motor_go_ZY(int count)
{
	if(count == Cunt)
	{
		PWM_DISABLE();
	}
	if(count < 0)
	{
		jj = 1;
		F_a = 2;
		F_b = 1;
		F_c = 1;
		F_d = 2;	
	}						 
	if(count > 0)
	{
		jj = 2;
		F_a = 1;
		F_b = 2;
		F_c = 2;
		F_d = 1;	
}
}
/*
						if(STS == 10)         //正常走
						{
						 SPEET_BUSHU_STOP++;
						}
						if(STS == 50)         //矫正矫正走
						{
							SPEET_BUSHU_V++;
						}
						if(ENSY == 101)       //颜色矫正走
						{
							SEET_ENSY = 1;
						}
*/
void Motor_conter(float conter,float mumax,float mumin,int a,int b,int c,int d,int e)
{
		 if((conter > mumin)&&(conter < mumax)&&(e == 0)&&(aakk==717))
		 {		
        if(aa>=7)
				{						
					STS = 10;
					aakk=17;
					Motor_SetSpeed(a);   //右上       前
					Motor_SetSpeed1(b);  //左上   
					Motor_SetSpeed2(c);  //右下		
					Motor_SetSpeed3(d);  //左下			 
				}
				SPEET_BUSHU_V = 0;
				
//		 		PWM_DISABLE();
		 }
		 if(conter <= mumin && (e == 0))   //左转
		 {	  
			  aa = 0;
				STS = 50;	
//        HUYNT = 0;			 
				if((SPEET_BUSHU_V == 0) && (e == 0))
				{						 
					Motor_SetSpeed(a);   //右上       前
					Motor_SetSpeed1(b);  //左上   
					Motor_SetSpeed2(c);  //右下		
					Motor_SetSpeed3(d);  //左下			 
					MOROR_MOVE_MAX(-10*1,50,STS);
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
			  aa = 0;
				STS = 50;		
				if((SPEET_BUSHU_V == 0) && (e == 0))
				{				
					Motor_SetSpeed(a);   //右上       前
					Motor_SetSpeed1(b);  //左上   
					Motor_SetSpeed2(c);  //右下		
					Motor_SetSpeed3(d);  //左下			 
					MOROR_MOVE_MAX(-10*1,50,STS); 
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
		 if((conter > 176.9 && conter < 177.1) && (e == 55))   //直走
		 {		    
			  if(aa>=7)
				{					
					STS = 10;
//					PWM_ENABLE();
				}
        SPEET_BUSHU_V = 0;
				Motor_SetSpeed(a);   //右下
				Motor_SetSpeed1(b);  //左下
				Motor_SetSpeed2(c);  //右上
				Motor_SetSpeed3(d);  //左上			
//				PWM_DISABLE();
		 }
		 if(((conter >= -180 && conter <= 0 )||(conter >=177.1 && conter <= 180)) && (e == 55))   //右转
		 {		
			  aa = 0;
			  STS = 50;		
				if((SPEET_BUSHU_V == 0) && (e == 55))
				{						 
					Motor_SetSpeed(a);   //右上       前
					Motor_SetSpeed1(b);  //左上   
					Motor_SetSpeed2(c);  //右下		
					Motor_SetSpeed3(d);  //左下			 
					MOROR_MOVE_MAX(-7*1,50,STS);
				}		
				if((SPEET_BUSHU_V >= 1) && (e == 55))
				{				
					Motor_SetSpeed(2);   //右下
					Motor_SetSpeed1(2);  //左下
					Motor_SetSpeed2(2);  //右上
					Motor_SetSpeed3(2);  //左上
					MOROR_MOVE_MAX(-20*1,50,STS);					
				}					
		 		PWM_ENABLE();
			}
		 if((conter > 0 && conter <= 176.9) && (e == 55))   //左转
		 {
			  aa = 0;
				STS = 50;		
				if((SPEET_BUSHU_V == 0) && (e == 55))
				{						 
					Motor_SetSpeed(a);   //右上       前
					Motor_SetSpeed1(b);  //左上   
					Motor_SetSpeed2(c);  //右下		
					Motor_SetSpeed3(d);  //左下			 
					MOROR_MOVE_MAX(-7*1,50,STS);
				}				
				if((SPEET_BUSHU_V >= 1) && (e == 55))
				{					
					Motor_SetSpeed(1);   //右下
					Motor_SetSpeed1(1);  //左下
					Motor_SetSpeed2(1);  //右上
					Motor_SetSpeed3(1);  //左上		
					MOROR_MOVE_MAX(-20*1,50,STS);					
				}				
		 		PWM_ENABLE();		
		}
	 
}

void xiansgu1(void)    //分段矫正  左小  266 268     194  198
{
	if(GG == 210)
	{
		 if((SEET_ENSY == 1) && (GG == 210))
		 {
			  ENSY = 110;
			  PWM_DISABLE();			  
			  SEET_ENSY = 0;
			  GG = 29;			 
		 }
		 else	if((SEET_ENSY == 0) && (GG == 210))
		 {
			   ENSY = 101;
				 Cunt = Cunt1;
				 LED1_ON();
				 STS = 50;
			   open_CD = 20;
				 Motor_go_ZY(Cunt - ZBX[1]);
				 MOROR_MOVE_MAX(-4.1*(Cunt - ZBX[1]),3*20,20*0.3);
				 PWM_ENABLE();
		 }
	 }
}
void xiansgu2(void)  //前小  400  258
{
	if(GG == 29)
	{		 
		 if((GG == 29) && (SEET_ENSY == 1) && (Three_CnEn_SH >= 2))
		 {
			  ENSY = 110;
				PWM_DISABLE();			 
			  LED1_OFF();
			  Three_CnEn_SH = 1;
			  SEET_ENSY = 0;
        GG = 50;			 
		 }
		 if((Three_CnEn_SH < 2) && (GG == 29) && (SEET_ENSY == 1))
		 {
			 	ENSY = 110;
				PWM_DISABLE();			 
 			  LED1_OFF();
 			  SEET_ENSY = 0;
 			  Three_CnEn_SH ++;
 			  GG = 21;
		 }
		 if((GG == 29) && (SEET_ENSY == 0))  //前走xcrc个计数器
		 {
			   ENSY = 101;
				 Cunt = Cunt2;
				 LED1_ON();
				 STS = 50;
			   open_CD = 20;
				 Motor_go_HQ(Cunt - ZBW[1]);
				 MOROR_MOVE_MAX(-5.1*(Cunt - ZBW[1]),3*20,20*0.3);
				 PWM_ENABLE();
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
