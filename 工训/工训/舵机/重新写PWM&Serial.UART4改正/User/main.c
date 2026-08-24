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
#include "WZJZ.h"
#include "Servo.h"

extern float yaw;
uint8_t RxData;
int aa,bb,kk,aakk;
int s;
int PWM_ENA;
int8_t Speed;
int ARR, PSC , CCR;
int MC,QWZ;
float SHJh,SHJz;
float  RXH , RXZ;
float  RHD,RZD;   //串口接收横纵坐标值
int YVY;
int YUY =12;
int CFG;
int chang;
int kuan;
int ECR = 16;
int ECG = 16;
int JIXIEBI = 11;
int cot;
int RDX;
int KeyNum;
int GG;
int x1,x2,x3;
int w1,w2,w3;
int y,cvc,mvc;
int next = 0;
uint8_t heng,zong;

uint8_t hexToDec(uint8_t hexValue) {
  uint8_t decValue = 0;
  uint8_t base = 1;
  while (hexValue != 0) {
    uint8_t digit = hexValue % 10;
    decValue += digit * base;
    base *= 16;
    hexValue /= 10;
  }
  return decValue;
}
uint16_t hexToDec_mv(uint16_t hexValue) {
  uint16_t decValue = 0;
  uint16_t base = 1;
  while (hexValue != 0) {
    uint16_t digit = hexValue % 10;
    decValue += digit * base;
    base *= 16;
    hexValue /= 10;
  }
  return decValue;
}

void kk_zhua(void)
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

void kk_target(void)
{
//	PWM_Initkk(72,100,50); 
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

void sad_kk(void)
{
	PWM_Initkk(72,20000,50);
	PWMkk_ENABLE();
	aakk++;
	if(aakk%2==0)
	{
		Motor_SetSpeedkk(1);	//向下
	}else{
		Motor_SetSpeedkk(2);	//向上
	}
}	

int main(void)
{
	 Key_Init();
	
	 OLED_Init();
	 ARR = 140;
   PSC = 150;
	 CCR = 50;
	 Timer_Init(220,720);
	Servo1_Init();
	Servo2_Init();
	Servo3_Init();
//	 Timer_Init_4(240,720);
	 Serial_Init3();
	 Serial_Init22();	
	 Serial4_Init();
	 Motor_Init();
	 Motor_InitD();
	Motor_InitC();
	Servo1_SetAngle(55);   //角度给30-55  //30是关，50是开
	Servo2_SetAngle(45);   //角度给159-45  //45是抓，159是放 
	Servo3_SetAngle(87);  //10,87,170

	bb=2;
	while (1)
	{			 
		 yaw=(float)stcAngle.Angle[2]/32768*180;
//		 TIM5 -> CCR2 = 2400;	 //2400	  1500		620	
//  	 if(ARR<=70)
//		 {
//			 ARR = 70;
//		 }
//		 if(ARR >= 100)
//		 {
//			 ARR = 100;
//		 }
		 PWM_Init(ARR,PSC,CCR);    //ARR  PSC  CCR   arr过高会堵转  f越高脉冲越多
		 PWM_Init1(ARR,PSC,CCR);
		 PWM_Init2(ARR,PSC,CCR);
		 PWM_Init3(ARR,PSC,CCR);
		 PWM_Init3(ARR,PSC,CCR);
//		 PWM_Initkk(70,100,50); 
//		 	    ARR = 100;
//					Motor_SetSpeed(2);   //右下      左
//					Motor_SetSpeed1(2);  //左下  
//					Motor_SetSpeed2(2);  //右上	
//					Motor_SetSpeed3(2);  //左上		
//					PWM_ENABLE();								

 		 OLED_ShowSignedNum(1,1,ARR,8);
		 OLED_ShowSignedNum(2,1,yaw,3);
		 KeyNum = Key_GetNum();
//		OLED_ShowNum(1, 8,((hexToDec(Serial_RxPacket_2[0])*100)+(hexToDec(Serial_RxPacket_2[1])*10)+(hexToDec(Serial_RxPacket_2[2]))), 3);  // cv x
//		OLED_ShowNum(2, 8,((hexToDec(Serial_RxPacket_2[3])*100)+(hexToDec(Serial_RxPacket_2[4])*10)+(hexToDec(Serial_RxPacket_2[5]))), 3);  // cv w
//		OLED_ShowNum(4, 1,(hexToDec(Serial_RxPacket_2[6])), 3);  // cv id  
//		OLED_ShowNum(4, 8,(hexToDec(Serial_RxPacket_2[0])), 3);	 // mv id																																																																// mv id 
//		OLED_ShowNum(4, 8,(hexToDec(Serial_RxPacket_2[1])), 3);	 // mv y				
/********************零度********************************/	
//			if((yaw <= -0.99) && (yaw >= -50))  //左转
//			{		
//					ARR += WeiZhi_Zong_PID(0 ,yaw);
//					Motor_SetSpeed(1);   //右下      左
//					Motor_SetSpeed1(1);  //左下  
//					Motor_SetSpeed2(1);  //右上	
//					Motor_SetSpeed3(1);  //左上		
//					PWM_ENABLE();								
//			}			
//			if((yaw >= 0.99) && (yaw <= 50))  //右转
//			{
//					ARR -= WeiZhi_Zong_PID(0 ,yaw);
//					Motor_SetSpeed(2);   //右下      左
//					Motor_SetSpeed1(2);  //左下  
//					Motor_SetSpeed2(2);  //右上	
//					Motor_SetSpeed3(2);  //左上		
//					PWM_ENABLE();								
//			}
//			if((yaw >= -0.99)&&(yaw <= 0.99))
//			{				
//					ARR = 100;
//					Motor_SetSpeed(2);   //右下      前
//					Motor_SetSpeed1(1);  //左下  
//					Motor_SetSpeed2(1);  //右上	
//					Motor_SetSpeed3(2);  //左上		
//					PWM_ENABLE();
//			}
///***************九十**********************************/
//			if((yaw <= 89.4) && (yaw >= 50))  //左转
//			{		
//					ARR += WeiZhi_Zong_PID(0 ,yaw);
//					Motor_SetSpeed(1);   //右下      左
//					Motor_SetSpeed1(1);  //左下  
//					Motor_SetSpeed2(1);  //右上	
//					Motor_SetSpeed3(1);  //左上		
//					PWM_ENABLE();								
//			}			
//			if((yaw >= 90.6) && (yaw <= 130))  //右转
//			{
//					ARR -= WeiZhi_Zong_PID(0 ,yaw);
//					Motor_SetSpeed(2);   //右下      左
//					Motor_SetSpeed1(2);  //左下  
//					Motor_SetSpeed2(2);  //右上	
//					Motor_SetSpeed3(2);  //左上		
//					PWM_ENABLE();								
//			}
//			if((yaw >= 89.4)&&(yaw <= 90.6))
//			{				
//					ARR = 100;
//					Motor_SetSpeed(2);   //右下      前
//					Motor_SetSpeed1(1);  //左下  
//					Motor_SetSpeed2(1);  //右上	
//					Motor_SetSpeed3(2);  //左上		
//					PWM_ENABLE();
//			}

/////***************一百八***********************************/
//			if((yaw <= 179.4) && (yaw >= 140))  //左转
//			{		
//					ARR += WeiZhi_Zong_PID(0 ,yaw);
//					Motor_SetSpeed(1);   //右下      左
//					Motor_SetSpeed1(1);  //左下  
//					Motor_SetSpeed2(1);  //右上	
//					Motor_SetSpeed3(1);  //左上		
//					PWM_ENABLE();								
//			}			
//			if((yaw >= 180.6) && (yaw <= 220))  //右转
//			{
//					ARR -= WeiZhi_Zong_PID(0 ,yaw);
//					Motor_SetSpeed(2);   //右下      左
//					Motor_SetSpeed1(2);  //左下  
//					Motor_SetSpeed2(2);  //右上	
//					Motor_SetSpeed3(2);  //左上		
//					PWM_ENABLE();								
//			}
//			if((yaw >= 179.4)&&(yaw <= 180.6))
//			{				
//					ARR = 100;
//					Motor_SetSpeed(2);   //右下      前
//					Motor_SetSpeed1(1);  //左下  
//					Motor_SetSpeed2(1);  //右上	
//					Motor_SetSpeed3(2);  //左上		
//					PWM_ENABLE();
//			}
/////***************二百七***********************************/
//			if((yaw <= 269.4) && (yaw >= 310))  //左转
//			{		
//					ARR += WeiZhi_Zong_PID(0 ,yaw);
//					Motor_SetSpeed(1);   //右下      左
//					Motor_SetSpeed1(1);  //左下  
//					Motor_SetSpeed2(1);  //右上	
//					Motor_SetSpeed3(1);  //左上		
//					PWM_ENABLE();								
//			}			
//			if((yaw >= 270.6) && (yaw <= 230))  //右转
//			{
//					ARR -= WeiZhi_Zong_PID(0 ,yaw);
//					Motor_SetSpeed(2);   //右下      左
//					Motor_SetSpeed1(2);  //左下  
//					Motor_SetSpeed2(2);  //右上	
//					Motor_SetSpeed3(2);  //左上		
//					PWM_ENABLE();								
//			}
//			if((yaw >= 269.4)&&(yaw <= 270.6))
//			{				
//					ARR = 100;
//					Motor_SetSpeed(2);   //右下      前
//					Motor_SetSpeed1(1);  //左下  
//					Motor_SetSpeed2(1);  //右上	
//					Motor_SetSpeed3(2);  //左上		
//					PWM_ENABLE();
//			}

/********************************************************
		if (KeyNum == 1)
		{
				GG = 55;          
				YUY = 13;					
		}
		
		if(GG == 99)
		{
				PWM_DISABLE();
				YUY = 12;
				GG = 0;
		}
********************************************************/						

	}
}
void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)  //获取中断标志位
	{
		if(bb==1)
		{
		  kk_zhua();
		}
		else if(bb==2)
		{
		  kk_target();
		}
		else{
			sad_kk();
		}
//		if(bb==2)
//		{
//		  kk_target();
//		}else{
//			sad_kk();
//		}
		
		cot++;
		if((cot >= 0) && (cot <= 3000))
		{
			if((yaw <= -2) && (yaw >= -50))  //左转
			{		
					ARR += WeiZhi_Zong_PID(0 ,yaw);
				  if(ARR<=70)
					 {
						 ARR = 140;
					 }

					Motor_SetSpeed(1);   //右下      左
					Motor_SetSpeed1(1);  //左下  
					Motor_SetSpeed2(1);  //右上	
					Motor_SetSpeed3(1);  //左上		
					PWM_ENABLE();								
			}			
			if((yaw >= 2) && (yaw <= 50))  //右转
			{
					ARR -= WeiZhi_Zong_PID(0 ,yaw);
				  	 if(ARR<=70)
						 {
							 ARR = 140;
						 }

					Motor_SetSpeed(2);   //右下      左
					Motor_SetSpeed1(2);  //左下  
					Motor_SetSpeed2(2);  //右上	
					Motor_SetSpeed3(2);  //左上		
					PWM_ENABLE();								
			}
			if((yaw > -2)&&(yaw < 2))
			{				
					ARR = 100;
					Motor_SetSpeed(2);   //右下      前
					Motor_SetSpeed1(1);  //左下  
					Motor_SetSpeed2(1);  //右上	
					Motor_SetSpeed3(2);  //左上		
					PWM_ENABLE();
			}
		}
		if((cot>3000)&&(cot<=6000))
		{
			if(yaw <= 88)  //左转
			{		
					ARR += WeiZhi_Zong_PID(0 ,yaw);
					if(ARR<=70)
					{
					 ARR = 140;
					}
					Motor_SetSpeed(1);   //右下      左
					Motor_SetSpeed1(1);  //左下  
					Motor_SetSpeed2(1);  //右上	
					Motor_SetSpeed3(1);  //左上		
					PWM_ENABLE();								
			}			
			if(yaw >= 92)  //右转
			{
					ARR -= WeiZhi_Zong_PID(0 ,yaw);
					if(ARR<=70)
					{
					 ARR = 140;
					}
					Motor_SetSpeed(2);   //右下      左
					Motor_SetSpeed1(2);  //左下  
					Motor_SetSpeed2(2);  //右上	
					Motor_SetSpeed3(2);  //左上		
					PWM_ENABLE();								
			}
			if((yaw > 88)&&(yaw <92))
			{				
					ARR = 100;
					Motor_SetSpeed(2);   //右下      前
					Motor_SetSpeed1(1);  //左下  
					Motor_SetSpeed2(1);  //右上	
					Motor_SetSpeed3(2);  //左上		
					PWM_ENABLE();
			}

		}
		if((cot>6000)&&(cot<=9000))
		{
			 if(yaw<0)
			 {
				 yaw+=360;
			 }

			if(yaw <= 178)  //左转
			{		
					ARR += WeiZhi_Zong_PID(0 ,yaw);
					if(ARR<=70)
					{
					 ARR = 140;
					}
					Motor_SetSpeed(1);   //右下      左
					Motor_SetSpeed1(1);  //左下  
					Motor_SetSpeed2(1);  //右上	
					Motor_SetSpeed3(1);  //左上		
					PWM_ENABLE();								
			}			
			if(yaw >= 182)  //右转
			{
					ARR -= WeiZhi_Zong_PID(0 ,yaw);
					if(ARR<=70)
					{
					 ARR = 140;
					}
					Motor_SetSpeed(2);   //右下      you
					Motor_SetSpeed1(2);  //左下  
					Motor_SetSpeed2(2);  //右上	
					Motor_SetSpeed3(2);  //左上		
					PWM_ENABLE();								
			}
			if((yaw > 178)&&(yaw <182))
			{				
					ARR = 100;
					Motor_SetSpeed(2);   //右下      前
					Motor_SetSpeed1(1);  //左下  
					Motor_SetSpeed2(1);  //右上	
					Motor_SetSpeed3(2);  //左上		
					PWM_ENABLE();
			}

		}
		if((cot>9000)&&(cot<=12500))
		{
			 if(yaw<0)
			 {
				 yaw+=360;
			 }

			if(yaw <= 268)  //左转
			{		
					ARR -= WeiZhi_Zong_PID(0 ,yaw);
					if(ARR<=70)
					{
							 ARR = 140;
					}
					Motor_SetSpeed(1);   //右下      左
					Motor_SetSpeed1(1);  //左下  
					Motor_SetSpeed2(1);  //右上	
					Motor_SetSpeed3(1);  //左上		
					PWM_ENABLE();								
			}			
			if(yaw >= 272)  //右转
			{
					ARR += WeiZhi_Zong_PID(0 ,yaw);
					if(ARR<=70)
					{
						ARR = 140;
					}
					Motor_SetSpeed(2);   //右下      左
					Motor_SetSpeed1(2);  //左下  
					Motor_SetSpeed2(2);  //右上	
					Motor_SetSpeed3(2);  //左上		
					PWM_ENABLE();								
			}
			if((yaw > 268)&&(yaw < 272))
			{				
					ARR = 100;
					Motor_SetSpeed(2);   //右下      前
					Motor_SetSpeed1(1);  //左下  
					Motor_SetSpeed2(1);  //右上	
					Motor_SetSpeed3(2);  //左上		
					PWM_ENABLE();
			}
		}
			if(cot>=12500 && cot<=16000)
			{
					Motor_SetSpeed(2);   //右下      左
					Motor_SetSpeed1(2);  //左下  
					Motor_SetSpeed2(2);  //右上	
					Motor_SetSpeed3(2);  //左上		
					PWM_ENABLE();		
					
			}
//			if(cot>16000 && cot<16200)
//			{
//				bb=2;
//			}
	

///**********************************************第一次矫正 opencv x矫正 *****************************************************************/	
//					x1 = Serial_RxPacket_3[2+next]-1;     //         cv xwc     mv c y 123321
//					x2 = Serial_RxPacket_3[2+next]+2;     //				 2cv x w  3 c y 123321
//					x3 = Serial_RxPacket_3[2+next]+5;     //     		 0x13 19 0x14 20 0x15 21
//					w1 = Serial_RxPacket_3[2+next]+8;     //
//					w2 = Serial_RxPacket_3[2+next]+12;    //
//					w3 = Serial_RxPacket_3[2+next]+15;    //
//					y  = Serial_RxPacket_3[1];
//					cvc= Serial_RxPacket_2[18];
//					mvc= Serial_RxPacket_3[0];
////					l = hexToDec(Serial_RxPacket_2[18]);  //				 0x13 19 0x14 20 0x15 21
//					if(next >= 3)
//					{
//						GG = 56;						
//						next = 0;
//					}
//					if(GG == 55)
//					{
//						if(((hexToDec(Serial_RxPacket_2[x1])*100)+(hexToDec(Serial_RxPacket_2[x2])*10)+(hexToDec(Serial_RxPacket_2[x3])) >=300) && ((hexToDec(Serial_RxPacket_2[x1])*100)+(hexToDec(Serial_RxPacket_2[x2])*10)+(hexToDec(Serial_RxPacket_2[x3])) <= 490))//zuo
//						{							
//							Motor_SetSpeed(1);   //右下
//							Motor_SetSpeed1(1);  //左下
//							Motor_SetSpeed2(0);  //右上
//							Motor_SetSpeed3(0);  //左上			
//              PWM_ENABLE();							
//						}
//						if(((hexToDec(Serial_RxPacket_2[x1])*100)+(hexToDec(Serial_RxPacket_2[x2])*10)+(hexToDec(Serial_RxPacket_2[x3])) <=290) && ((hexToDec(Serial_RxPacket_2[x1])*100)+(hexToDec(Serial_RxPacket_2[x2])*10)+(hexToDec(Serial_RxPacket_2[x3])) >= 150))  //you
//						{							
//							Motor_SetSpeed(0);   //右下
//							Motor_SetSpeed1(0);  //左下
//							Motor_SetSpeed2(1);  //右上
//							Motor_SetSpeed3(1);  //左上					
//						  PWM_ENABLE();
//						}
//						if(((hexToDec(Serial_RxPacket_2[x1])*100)+(hexToDec(Serial_RxPacket_2[x2])*10)+(hexToDec(Serial_RxPacket_2[x3])) >290) && ((hexToDec(Serial_RxPacket_2[x1])*100)+(hexToDec(Serial_RxPacket_2[x2])*10)+(hexToDec(Serial_RxPacket_2[x3])) <300))  //后
//						{
//							PWM_DISABLE();
//							GG = 38;
//						}	
//						else   //此处作为范围内左右移动寻找物块
//						{							
//							Motor_SetSpeed(0);   //右下
//							Motor_SetSpeed1(0);  //左下
//							Motor_SetSpeed2(0);  //右上
//							Motor_SetSpeed3(0);  //左上		
//							PWM_ENABLE();							
//						} 
//					}
//				/*****************************第二次矫正，w前后走***********************************************/
//					if(Serial_RxPacket_2[5] == 0x02)  //干垃圾
//					{
//								if(GG == 38)
//								{
//									if(((hexToDec(Serial_RxPacket_2[w1])*100)+(hexToDec(Serial_RxPacket_2[w2])*10)+(hexToDec(Serial_RxPacket_2[w3])))>=203)  //后
//									{							
//										Motor_SetSpeed(0);   //右下      前
//										Motor_SetSpeed1(1);  //左下
//										Motor_SetSpeed2(0);  //右上
//										Motor_SetSpeed3(1);  //左上			
//										PWM_ENABLE();							
//									}
//									if(((hexToDec(Serial_RxPacket_2[w1])*100)+(hexToDec(Serial_RxPacket_2[w2])*10)+(hexToDec(Serial_RxPacket_2[w3])))<=193)  //后
//									{							
//										Motor_SetSpeed(1);   //右下      前
//										Motor_SetSpeed1(0);  //左下
//										Motor_SetSpeed2(1);  //右上
//										Motor_SetSpeed3(0);  //左上	
//										PWM_ENABLE();							
//									}
//									if(((hexToDec(Serial_RxPacket_2[w1])*100)+(hexToDec(Serial_RxPacket_2[w2])*10)+(hexToDec(Serial_RxPacket_2[w3])) > 193) && ((hexToDec(Serial_RxPacket_2[w1])*100)+(hexToDec(Serial_RxPacket_2[w2])*10)+(hexToDec(Serial_RxPacket_2[w3])) < 203))  //后
//									{
//										PWM_DISABLE();
//										GG = 46;
//									}
//								}
//							/***************************************************************************************************/
//							/**************************第三次矫正 y矫正************************************************************************/
//								if(GG == 46)
//								{
//										if(hexToDec(Serial_RxPacket_2[y])<=127)  //后
//										{									
//											Motor_SetSpeed(1);   //右下      前
//											Motor_SetSpeed1(0);  //左下
//											Motor_SetSpeed2(1);  //右上
//											Motor_SetSpeed3(0);  //左上					
//											PWM_ENABLE();
//										}
//										if(hexToDec(Serial_RxPacket_2[y])>=137)
//										{								
//											Motor_SetSpeed(0);   //右下      后
//											Motor_SetSpeed1(1);  //左下
//											Motor_SetSpeed2(0);  //右上
//											Motor_SetSpeed3(1);  //左上					
//											PWM_ENABLE();						
//										}
//										if((hexToDec(Serial_RxPacket_2[y])>127) && (hexToDec(Serial_RxPacket_2[y])<137))  //后
//										{
//											PWM_DISABLE();
//											GG = 56;
//										}
//								}
//							/****************************************************************************************************************/
//							/***************************第四次判断 x矫正***********************************************************************************/
//								if(GG == 56)
//								{
//										if((((hexToDec(Serial_RxPacket_2[x1])*100)+(hexToDec(Serial_RxPacket_2[x2])*10)+(hexToDec(Serial_RxPacket_2[x3]))) >=310) && (((hexToDec(Serial_RxPacket_2[x1])*100)+(hexToDec(Serial_RxPacket_2[x2])*10)+(hexToDec(Serial_RxPacket_2[x3]))) <= 490))//zuo
//										{							
//											 Motor_SetSpeed(1);   //右下
//											 Motor_SetSpeed1(1);  //左下
//											 Motor_SetSpeed2(0);  //右上
//											 Motor_SetSpeed3(0);  //左上	
//											 PWM_ENABLE();								
//										}
//										if((((hexToDec(Serial_RxPacket_2[x1])*100)+(hexToDec(Serial_RxPacket_2[x2])*10)+(hexToDec(Serial_RxPacket_2[x3]))) <=300) && (((hexToDec(Serial_RxPacket_2[x1])*100)+(hexToDec(Serial_RxPacket_2[x2])*10)+(hexToDec(Serial_RxPacket_2[x3]))) >= 150))  //you
//										{							
//											Motor_SetSpeed(0);   //右下
//											Motor_SetSpeed1(0);  //左下
//											Motor_SetSpeed2(1);  //右上
//											Motor_SetSpeed3(1);  //左上		
//											PWM_ENABLE();								
//										}
//										if((((hexToDec(Serial_RxPacket_2[x1])*100)+(hexToDec(Serial_RxPacket_2[x2])*10)+(hexToDec(Serial_RxPacket_2[x3]))) >300) && (((hexToDec(Serial_RxPacket_2[x1])*100)+(hexToDec(Serial_RxPacket_2[x2])*10)+(hexToDec(Serial_RxPacket_2[x3]))) <310))  //后
//										{
//											PWM_DISABLE();
//											GG = 22;
//										}	
//								}
//								if(GG == 22)
//								{
//									    //夹取逻辑color与规则红绿蓝 123相同   
//									    if(((hexToDec(Serial_RxPacket_2[18])-Serial_RxPacket_3[2+next])!=18) || ((hexToDec(Serial_RxPacket_3[0]))-Serial_RxPacket_3[2+next])!=18)  
//											{
//												next++;//前为强转类型color 0x13 0x14 0x15  19 20 21 后为123+321
//											}
//											GG = 55;
//								}
//						}
///*****************************************************************************************************************************************************************************************/

					

		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);  //清除中断标志位
	}
}

