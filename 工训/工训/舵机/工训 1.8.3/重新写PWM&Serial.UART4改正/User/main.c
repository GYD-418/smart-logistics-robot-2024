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
#include "ShiQv.h"
extern int aa,bb,kk;
int aakk;
extern float yaw;
uint8_t RxData;
int s;
int PWM_ENA;
int8_t Speed;
int ARR, ARRL ,ARRR, PSC , CCR;  //左L 右R
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
int cot,cnt,crt,AFT;
int RDX;
int KeyNum;
int GG = 44;
int x1,x2,x3;
int w1,w2,w3;
int y,cvc,mvc;
int next = 0;
int MUB; //此参数为转向时候的标志位 出发时0 转90度加1
int Pick_Up= 0;
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
void sad_kk(void)
{
	PWM_Initkk(ARR,PSC,ARR/2);
	PWMkk_ENABLE();
	aakk++;
	if(aakk%2==0)
	{
		Motor_SetSpeedkk(1);
	}else{
		Motor_SetSpeedkk(2);
	}
}
int main(void)
{
	 Key_Init();
	 OLED_Init();
	 ARR = 100;
   PSC = 150;
	 CCR = 50;
	 Timer_Init(220,720);
//	 Timer_Init_4(240,720);
	 Serial_Init3();
//	 Serial_Init22();	
	 Serial4_Init();
	 Motor_InitC();
	 Servo1_Init();
	 Servo2_Init();
	 Servo3_Init();
	 Motor_Init();
	 Motor_InitD();
//	 Servo1_SetAngle(10);   //10-35
//	 Servo2_SetAngle(45);		//159-45
//	 Servo3_SetAngle(87);		//10,87,170
	 PWM_Initkk(150,PSC,150/2);
//	 PWM_Init3(ARR,PSC,ARR/2);  //右
//	 PWM_Init2(ARR,PSC,ARR/2);  //右
//	 PWM_Init1(ARR,PSC,ARR/2);   //左
//	 PWM_Init(ARR,PSC,ARR/2);    //左   //ARR  PSC  CCR   arr过高会堵转  f越高脉冲越多
	 bb=1;
	while (1)
	{			 

		 x1 = Serial_RxPacket_2[19+next]-1;     //        串口三扫二维码 串口二发送x y color        cv xwc     mv c y 123321
		 x2 = Serial_RxPacket_2[19+next]+2;     //				 2cv x w  3 c y 123321
		 x3 = Serial_RxPacket_2[19+next]+5;     //     		 0x13 19 0x14 20 0x15 21
		 w1 = Serial_RxPacket_2[19+next]+8;     //
		 w2 = Serial_RxPacket_2[19+next]+12;    //
		 w3 = Serial_RxPacket_2[19+next]+15;    //
		 y  = Serial_RxPacket_2[1];
		 cvc= Serial_RxPacket_2[18];
		 if(cnt >= 200)
		 {
				Serial_hinght_xy_last[0] = Serial_hinght_xy[0];
				Serial_hinght_xy_last[1] = Serial_hinght_xy[1];
				Serial_hinght_xy_last[2] = Serial_hinght_xy[2];
				Serial_hinght_xy_last[3] = Serial_hinght_xy[3];
				Serial_hinght_xy_last[4] = Serial_hinght_xy[4];
				Serial_hinght_xy_last[5] = Serial_hinght_xy[5];
			 
				Serial_hinght_xy[0] = Serial_RxPacket_2[x1];  //百
				Serial_hinght_xy[1] = Serial_RxPacket_2[x2];  //十
				Serial_hinght_xy[2] = Serial_RxPacket_2[x3];  //个
				Serial_hinght_xy[3] = Serial_RxPacket_2[w1];  //百
				Serial_hinght_xy[4] = Serial_RxPacket_2[w2];  //十
				Serial_hinght_xy[5] = Serial_RxPacket_2[w3];  //个					
        cnt = 0;			 
		 }	
	 	 OLED_ShowSignedNum(1,1,ARR,8);
		 OLED_ShowSignedNum(2,1,yaw,3);
		 OLED_ShowSignedNum(3,1,ARRL,3);
		 OLED_ShowSignedNum(4,1,ARRR,3);
 
		 yaw=(float)stcAngle.Angle[2]/32768*180;
		 // 实际减目标是负数需要左转 右快左慢
		 // 实际减目标是正数需要右转 右慢左快
     if((Pick_Up == 1)&&(GG == 50))
		 {
				 ShiQv();				
				 GG = 44;	 
		 }
     if((Pick_Up == 2)&&(GG == 50))
		 {
				 ShiQv2();				
				 GG = 44;	 
		 }
		 
//		 else
//		 {
//			 PWM_DISABLE();
//		 }

	}
}
void TIM6_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM6, TIM_IT_Update) == SET)  //获取中断标志位
	{
		 		if(bb==1)
				{
					ShiQv2();
				}
//				else if(bb==2)
//				{
//					ShiQv();
//				}
//		else{
//				sad_kk();
//		}
		      aa++;
		      cot++;
		      cnt++;
		if(GG == 44)
		{
				if((cot >= 0) && (cot <= 600))
				{
					if(yaw <= -2.5)  //左转
					{		
							Motor_SetSpeed(1);   //右上       左
							Motor_SetSpeed1(1);  //左上   
							Motor_SetSpeed2(1);  //右下		
							Motor_SetSpeed3(1);  //左下			
							PWM_ENABLE();								

					}			
					if(yaw >= 2.5)  //右转
					{
							Motor_SetSpeed(2);   //右上       左
							Motor_SetSpeed1(2);  //左上   
							Motor_SetSpeed2(2);  //右下		
							Motor_SetSpeed3(2);  //左下			
							PWM_ENABLE();								
					}
					if((yaw > -2.5)&&(yaw < 2.5))
					{				
							Motor_SetSpeed(2);   //右上       前
							Motor_SetSpeed1(2);  //左上   
							Motor_SetSpeed2(1);  //右下		
							Motor_SetSpeed3(1);  //左下			
							PWM_ENABLE();
					}

				}
				if((cot >= 600) && (cot <= 3000))
				{
					if(yaw <= -2.5)  //左转
					{		
							Motor_SetSpeed(1);   //右上       左
							Motor_SetSpeed1(1);  //左上   
							Motor_SetSpeed2(1);  //右下		
							Motor_SetSpeed3(1);  //左下			
							PWM_ENABLE();								

					}			
					if(yaw >= 2.5)  //右转
					{
							Motor_SetSpeed(2);   //右上       左
							Motor_SetSpeed1(2);  //左上   
							Motor_SetSpeed2(2);  //右下		
							Motor_SetSpeed3(2);  //左下			
							PWM_ENABLE();								
					}
					if((yaw > -2.5)&&(yaw < 2.5))
					{				
							Motor_SetSpeed(2);   //右上       前
							Motor_SetSpeed1(1);  //左上   
							Motor_SetSpeed2(1);  //右下		
							Motor_SetSpeed3(2);  //左下			
							PWM_ENABLE();
					}
				}		
				if((cot >= 3000) && (cot <= 2300+10000))     //第一次转盘抓取物块
				{
							Motor_SetSpeed(3);   //右上       前
							Motor_SetSpeed1(3);  //左上   
							Motor_SetSpeed2(3);  //右下		
							Motor_SetSpeed3(3);  //左下			
							PWM_DISABLE();
					    Pick_Up = 1;
//					    GG = 71;
				}		
				if((cot >= 2300+10000) && (cot <= 3800+10000))
				{
					if(yaw <= -2.8)  //左转
					{		
							Motor_SetSpeed(1);   //右上       左
							Motor_SetSpeed1(1);  //左上   
							Motor_SetSpeed2(1);  //右下		
							Motor_SetSpeed3(1);  //左下			
							PWM_ENABLE();								

					}			
					if(yaw >= 2.8)  //右转
					{
							Motor_SetSpeed(2);   //右上       左
							Motor_SetSpeed1(2);  //左上   
							Motor_SetSpeed2(2);  //右下		
							Motor_SetSpeed3(2);  //左下			
							PWM_ENABLE();								
					}
					if((yaw > -2.8)&&(yaw < 2.8))
					{				
							Motor_SetSpeed(2);   //右上       前
							Motor_SetSpeed1(1);  //左上   
							Motor_SetSpeed2(1);  //右下		
							Motor_SetSpeed3(2);  //左下			
							PWM_ENABLE();
					}
				}		
///////////////////////////////////////////////////////////////////////////////				
				if((cot>13400)&&(cot<=13900+1500))
				{
					if(yaw <= 87.5)  //左转
					{		
							Motor_SetSpeed(1);   //右上       左
							Motor_SetSpeed1(1);  //左上   
							Motor_SetSpeed2(1);  //右下		
							Motor_SetSpeed3(1);  //左下			
							PWM_ENABLE();								

					}			
					if(yaw >= 92.5)  //右转
					{
							Motor_SetSpeed(2);   //右上       左
							Motor_SetSpeed1(2);  //左上   
							Motor_SetSpeed2(2);  //右下		
							Motor_SetSpeed3(2);  //左下			
							PWM_ENABLE();								
					}
					if((yaw > 87.5)&&(yaw < 92.5))
					{				
							Motor_SetSpeed(2);   //右上       前
							Motor_SetSpeed1(1);  //左上   
							Motor_SetSpeed2(1);  //右下		
							Motor_SetSpeed3(2);  //左下			
							PWM_ENABLE();
					}
				}
				if((cot>13900+1500)&&(cot<=13800+7700))    //第一次放置物块 
				{
							if((crt <= (200*(Serial_RxPacket_3[2+next] - Serial_RxPacket_3[1+next]))) && (AFT == 33))  //移动时间  按顺序抓取
							{
								 crt++;
								 Motor_go(Serial_RxPacket_3[2+next]);
								 crt = 0;
								 AFT = 44;
							}

							Motor_SetSpeed(3);   //右上       前
							Motor_SetSpeed1(3);  //左上   
							Motor_SetSpeed2(3);  //右下		
							Motor_SetSpeed3(3);  //左下			
							PWM_DISABLE();
							Pick_Up = 2;
//					    GG = 71;
				}
				if((cot>13800+7700)&&(cot<=14000+9000))
				{
					if(yaw <= 87.5)  //左转
					{		
							Motor_SetSpeed(1);   //右上       左
							Motor_SetSpeed1(1);  //左上   
							Motor_SetSpeed2(1);  //右下		
							Motor_SetSpeed3(1);  //左下			
							PWM_ENABLE();								

					}			
					if(yaw >= 92.5)  //右转
					{
							Motor_SetSpeed(2);   //右上       左
							Motor_SetSpeed1(2);  //左上   
							Motor_SetSpeed2(2);  //右下		
							Motor_SetSpeed3(2);  //左下			
							PWM_ENABLE();								
					}
					if((yaw > 87.5)&&(yaw < 92.5))
					{				
							Motor_SetSpeed(2);   //右上       前
							Motor_SetSpeed1(1);  //左上   
							Motor_SetSpeed2(1);  //右下		
							Motor_SetSpeed3(2);  //左下			
							PWM_ENABLE();
					}
				}
//////////////////////////////////////////////////////////////////////////////
				if((cot>23000)&&(cot<=13000+11800))
				{
					if((yaw <= 178.5) && (yaw >= 0))  //左转
					{		
							Motor_SetSpeed(1);   //右上       左
							Motor_SetSpeed1(1);  //左上   
							Motor_SetSpeed2(1);  //右下		
							Motor_SetSpeed3(1);  //左下			
							PWM_ENABLE();								

					}			
					if((yaw >= -176.6) && (yaw < 0))  //右转
					{
							Motor_SetSpeed(2);   //右上       左
							Motor_SetSpeed1(2);  //左上   
							Motor_SetSpeed2(2);  //右下		
							Motor_SetSpeed3(2);  //左下			
							PWM_ENABLE();								
					}
					if(((yaw > 178.5)&&(yaw < 180))||((yaw <= -176.6) && (yaw >= -180)))
					{				
							Motor_SetSpeed(2);   //右上       前
							Motor_SetSpeed1(1);  //左上   
							Motor_SetSpeed2(1);  //右下		
							Motor_SetSpeed3(2);  //左下			
							PWM_ENABLE();
					}
				}
				if((cot>24800)&&(cot<=15100+16000))  //第二次放置物料
				{
							if((crt <= (200*(Serial_RxPacket_3[2+next] - Serial_RxPacket_3[1+next]))) && (AFT == 33))  //移动时间  按顺序抓取
							{
								 crt++;
								 Motor_go(Serial_RxPacket_3[2+next]);
								 crt = 0;
								 AFT = 44;
							}

							Motor_SetSpeed(3);   //右上       前
							Motor_SetSpeed1(3);  //左上   
							Motor_SetSpeed2(3);  //右下		
							Motor_SetSpeed3(3);  //左下			
							PWM_DISABLE();
							Pick_Up = 1;
//					    GG = 71;
			 }
				if((cot>31100)&&(cot<=21100+2400))
				{
					if(yaw <= -92.1)  //左转
					{		
							Motor_SetSpeed(1);   //右上       左
							Motor_SetSpeed1(1);  //左上   
							Motor_SetSpeed2(1);  //右下		
							Motor_SetSpeed3(1);  //左下			
							PWM_ENABLE();								

					}			
					if(yaw >= -87.1) //右转
					{
							Motor_SetSpeed(2);   //右上       左
							Motor_SetSpeed1(2);  //左上   
							Motor_SetSpeed2(2);  //右下		
							Motor_SetSpeed3(2);  //左下			
							PWM_ENABLE();								
					}
					if((yaw > -92.1)&&(yaw < -87.1))
					{				
							Motor_SetSpeed(2);   //右上       前
							Motor_SetSpeed1(1);  //左上   
							Motor_SetSpeed2(1);  //右下		
							Motor_SetSpeed3(2);  //左下			
							PWM_ENABLE();
					}
				}
			}
//////////////////////////////////////////////////////第二圈////////////////////////////////////////////////////////

//				if((cot>21100+2400)&&(cot<=24500))
//				{
//					if((crt <= (200*(2-Serial_RxPacket_3[2+next]))) && (AFT == 33))  //移动时间  按顺序抓取
//					{
//						 crt++;
//						 Motor_go(Serial_RxPacket_3[2+next]);
//						 crt = 0;
//						 AFT = 44;
//					}
//							Motor_SetSpeed(3);   //右上       前
//							Motor_SetSpeed1(3);  //左上   
//							Motor_SetSpeed2(3);  //右下		
//							Motor_SetSpeed3(3);  //左下			
//							PWM_DISABLE();
//							Pick_Up = 1;
//				}
//				if((cot>24500)&&(cot<=25300))
//				{
//					if(yaw <= -2.8)  //左转
//					{		
//							Motor_SetSpeed(1);   //右上       左
//							Motor_SetSpeed1(1);  //左上   
//							Motor_SetSpeed2(1);  //右下		
//							Motor_SetSpeed3(1);  //左下			
//							PWM_ENABLE();								

//					}			
//					if(yaw >= 2.8)  //右转
//					{
//							Motor_SetSpeed(2);   //右上       左
//							Motor_SetSpeed1(2);  //左上   
//							Motor_SetSpeed2(2);  //右下		
//							Motor_SetSpeed3(2);  //左下			
//							PWM_ENABLE();								
//					}
//					if((yaw > -2.8)&&(yaw < 2.8))
//					{				
//							Motor_SetSpeed(2);   //右上       前
//							Motor_SetSpeed1(1);  //左上   
//							Motor_SetSpeed2(1);  //右下		
//							Motor_SetSpeed3(2);  //左下			
//							PWM_ENABLE();
//					}
//			 }
//////////////////////////////////////////////////////////////////////////////////////////////回物料区
//			 	if((cot>32400+3400)&&(cot<=35800))
//				{
//					if(yaw <= -2.8)  //左转
//					{		
//							Motor_SetSpeed(1);   //右上       左
//							Motor_SetSpeed1(1);  //左上   
//							Motor_SetSpeed2(1);  //右下		
//							Motor_SetSpeed3(1);  //左下			
//							PWM_ENABLE();								

//					}			
//					if(yaw >= 2.8)  //右转
//					{
//							Motor_SetSpeed(2);   //右上       左
//							Motor_SetSpeed1(2);  //左上   
//							Motor_SetSpeed2(2);  //右下		
//							Motor_SetSpeed3(2);  //左下			
//							PWM_ENABLE();								
//					}
//					if((yaw > -2.8)&&(yaw < 2.8))
//					{				
//							Motor_SetSpeed(2);   //右上       前
//							Motor_SetSpeed1(1);  //左上   
//							Motor_SetSpeed2(1);  //右下		
//							Motor_SetSpeed3(2);  //左下			
//							PWM_ENABLE();
//					}
//			 }
///////////////////////////////////////////////////////////////////////////////////////////////////抓取第二次
/////////////////////////////////////////////////////////////////////////////////				
//				if((cot>35800)&&(cot<=35800+2000))
//				{
//					if(yaw <= 87.5)  //左转
//					{		
//							Motor_SetSpeed(1);   //右上       左
//							Motor_SetSpeed1(1);  //左上   
//							Motor_SetSpeed2(1);  //右下		
//							Motor_SetSpeed3(1);  //左下			
//							PWM_ENABLE();								

//					}			
//					if(yaw >= 92.5)  //右转
//					{
//							Motor_SetSpeed(2);   //右上       左
//							Motor_SetSpeed1(2);  //左上   
//							Motor_SetSpeed2(2);  //右下		
//							Motor_SetSpeed3(2);  //左下			
//							PWM_ENABLE();								
//					}
//					if((yaw > 87.5)&&(yaw < 92.5))
//					{				
//							Motor_SetSpeed(2);   //右上       前
//							Motor_SetSpeed1(1);  //左上   
//							Motor_SetSpeed2(1);  //右下		
//							Motor_SetSpeed3(2);  //左下			
//							PWM_ENABLE();
//					}
//				}
//				if((cot>37800)&&(cot<=40000))    //第一次放置物块 
//				{
//							if((crt <= (200*(Serial_RxPacket_3[2+next] - Serial_RxPacket_3[1+next]))) && (AFT == 33))  //移动时间  按顺序抓取
//							{
//								 crt++;
//								 Motor_go(Serial_RxPacket_3[2+next]);
//								 crt = 0;
//								 AFT = 44;
//							}

//							Motor_SetSpeed(3);   //右上       前
//							Motor_SetSpeed1(3);  //左上   
//							Motor_SetSpeed2(3);  //右下		
//							Motor_SetSpeed3(3);  //左下			
//							PWM_DISABLE();
//							Pick_Up = 2;
////					    GG = 71;
//				}
//				if((cot>40000)&&(cot<=41100))
//				{
//					if(yaw <= 87.5)  //左转
//					{		
//							Motor_SetSpeed(1);   //右上       左
//							Motor_SetSpeed1(1);  //左上   
//							Motor_SetSpeed2(1);  //右下		
//							Motor_SetSpeed3(1);  //左下			
//							PWM_ENABLE();								

//					}			
//					if(yaw >= 92.5)  //右转
//					{
//							Motor_SetSpeed(2);   //右上       左
//							Motor_SetSpeed1(2);  //左上   
//							Motor_SetSpeed2(2);  //右下		
//							Motor_SetSpeed3(2);  //左下			
//							PWM_ENABLE();								
//					}
//					if((yaw > 87.5)&&(yaw < 92.5))
//					{				
//							Motor_SetSpeed(2);   //右上       前
//							Motor_SetSpeed1(1);  //左上   
//							Motor_SetSpeed2(1);  //右下		
//							Motor_SetSpeed3(2);  //左下			
//							PWM_ENABLE();
//					}
//				}
////////////////////////////////////////////////////////////////////////////////
//				if((cot>41100)&&(cot<=41100+1800))
//				{
//					if((yaw <= 178.5) && (yaw >= 0))  //左转
//					{		
//							Motor_SetSpeed(1);   //右上       左
//							Motor_SetSpeed1(1);  //左上   
//							Motor_SetSpeed2(1);  //右下		
//							Motor_SetSpeed3(1);  //左下			
//							PWM_ENABLE();								

//					}			
//					if((yaw >= -176.6) && (yaw < 0))  //右转
//					{
//							Motor_SetSpeed(2);   //右上       左
//							Motor_SetSpeed1(2);  //左上   
//							Motor_SetSpeed2(2);  //右下		
//							Motor_SetSpeed3(2);  //左下			
//							PWM_ENABLE();								
//					}
//					if(((yaw > 178.5)&&(yaw < 180))||((yaw <= -176.6) && (yaw >= -180)))
//					{				
//							Motor_SetSpeed(2);   //右上       前
//							Motor_SetSpeed1(1);  //左上   
//							Motor_SetSpeed2(1);  //右下		
//							Motor_SetSpeed3(2);  //左下			
//							PWM_ENABLE();
//					}
//				}
//				if((cot>42900)&&(cot<=44900))  //第二次放置物料
//				{
//							if((crt <= (200*(Serial_RxPacket_3[2+next] - Serial_RxPacket_3[1+next]))) && (AFT == 33))  //移动时间  按顺序抓取
//							{
//								 crt++;
//								 Motor_go(Serial_RxPacket_3[2+next]);
//								 crt = 0;
//								 AFT = 44;
//							}

//							Motor_SetSpeed(3);   //右上       前
//							Motor_SetSpeed1(3);  //左上   
//							Motor_SetSpeed2(3);  //右下		
//							Motor_SetSpeed3(3);  //左下			
//							PWM_DISABLE();
//							Pick_Up = 1;
////					    GG = 71;
//			 }
//				if((cot>44900)&&(cot<=44900+2400))
//				{
//					if((yaw <= 178.5) && (yaw >= 0))  //左转
//					{		
//							Motor_SetSpeed(1);   //右上       左
//							Motor_SetSpeed1(1);  //左上   
//							Motor_SetSpeed2(1);  //右下		
//							Motor_SetSpeed3(1);  //左下			
//							PWM_ENABLE();								

//					}			
//					if((yaw >= -176.6) && (yaw < 0))  //右转
//					{
//							Motor_SetSpeed(2);   //右上       左
//							Motor_SetSpeed1(2);  //左上   
//							Motor_SetSpeed2(2);  //右下		
//							Motor_SetSpeed3(2);  //左下			
//							PWM_ENABLE();								
//					}
//					if(((yaw > 178.5)&&(yaw < 180))||((yaw <= -176.6) && (yaw >= -180)))
//					{				
//							Motor_SetSpeed(2);   //右上       前
//							Motor_SetSpeed1(1);  //左上   
//							Motor_SetSpeed2(1);  //右下		
//							Motor_SetSpeed3(2);  //左下			
//							PWM_ENABLE();
//					}
//				}

//			}
////到达物料区停止，并掷出一个标志位sepan 对比像素点变化与定时器计时时间差
////cvc = Serial_RxPacket_3[2+next]时开始矫正	      
//					if(next >= 3)  //进行第二次抓取，标志位归一
//					{
//						GG = 56;						
//						next = 0;
//					}
//					if(GG==71)   //GG = 71是启动标志Serial_hinght_xy
//					{
//						if(((((hexToDec(Serial_hinght_xy[0])*100)+(hexToDec(Serial_hinght_xy[1])*10)+(hexToDec(Serial_hinght_xy[2])))-((hexToDec(Serial_hinght_xy_last[0])*100)+(hexToDec(Serial_hinght_xy_last[1])*10)+(hexToDec(Serial_hinght_xy_last[2])))) <= 200) && ((((hexToDec(Serial_hinght_xy[3])*100)+(hexToDec(Serial_hinght_xy[4])*10)+(hexToDec(Serial_hinght_xy[5]))) - ((hexToDec(Serial_hinght_xy_last[3])*100)+(hexToDec(Serial_hinght_xy_last[4])*10)+(hexToDec(Serial_hinght_xy_last[5])))) <= 200))
//						{
//								GG = 55;
//						}
//						
//						else
//						{
//									PWM_DISABLE();
//						}
//					}
//					
///**********************************************第一次矫正 opencv x矫正 *****************************************************************/	
//					l = hexToDec(Serial_RxPacket_2[18]);  //				 0x13 19 0x14 20 0x15 21
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
//							/***************************第四次判断 x矫正***********************************************************************************/
//								if(GG == 46)
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
//											if(cvc == Serial_RxPacket_3[2+next])
//											{
//												next++;//前为强转类型color 0x13 0x14 0x15  19 20 21 后为123+321
//												AFT = 33;
//											  GG = 50;
//											}
//								}
//						}

/*****************************************************************************************************************************************************************************************/
		TIM_ClearITPendingBit(TIM6, TIM_IT_Update);  //清除中断标志位
	}
}

