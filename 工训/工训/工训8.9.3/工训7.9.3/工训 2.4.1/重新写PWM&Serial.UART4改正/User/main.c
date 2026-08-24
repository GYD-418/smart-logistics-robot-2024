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
#include "LED.h"
#include "math.h"
#include "stdlib.h"
#include "T_BuJin_Sp.h"

extern int aa,bb,kk;
int aakk;
extern float yaw;
uint8_t RxData;
int s,hh;
int PWM_ENA;
int8_t Speed;
int ARR, ARRL ,ARRR, PSC , CCR;  //左L 右R
int QWZ,MC;
float SHJh,SHJz;
float  RXH , RXZ;
int yzw = 11;
float  RHD,RZD;   //串口接收横纵坐标值
int YVY = 36;  //舵机动作组抓取标志位
extern int YUY;
int CFG;
int chang;
int kuan;
int ECR = 16;
int ECG = 16;
int JIXIEBI = 0,JIXIEAI = 13,JINXIECI = 21;
long int cyt,czt,cot,cnt,AFT;
int RDX;
int GYD;
int jn;
int rmt;
extern int rnt;
long int cxt;
extern long int crt;
int LED_Time;
int KeyNum;
float yaw_e,Speed_yaw_e;
int Speed_e,ARRG;
extern int GG;
int x1,x2,x3,x11,x22,x33,T00;
int w1,w2,w3,w11,w22,w33;
int y,cvc,mvc;
extern int next;
int MUB; //此参数为转向时候的标志位 出发时0 转90度加1
extern int Pick_Up;
extern int oint;
extern uint32_t set_speed;         // 最大速度设置 单位为0.1rad/sec
uint8_t Serial_hinght_ab[12];
uint8_t TXT_HC[6];
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
	 next = 1;
	 GG = 61;
	 YUY = 76;
	 OLED_Init();
	 ARR = 100;
   PSC = 150;
	 CCR = 50;
	 PWM_Initkk(50,PSC,50/2);
	 Timer_Init(120,720);
	 Timer_Init_seven(120,720);
	 Serial_Init3();
	 Serial4_Init();
	 Motor_InitC();
	 Servo1_Init();
	 Servo2_Init();
	 Servo3_Init();
	 Servo4_Init();
	 MOTOR_GPIO_Init();
	 PWM_Init();
	 PWM_Init1();
	 PWM_Init2();
	 PWM_Init3();

   LED_Init();
	 Motor_Init();
	 Motor_InitD();
	 Servo1_SetAngle(20);   //20 -- 90 
	 Servo2_SetAngle(96);		//158-96-39
	 Servo3_SetAngle(164);		//3 81 164
	 Servo4_SetAngle(170);		//65，155
	 Serial_hinght_xy_last[0]  = 1;
	 Serial_hinght_xy_last[1]  = 1;
	 Serial_hinght_xy_last[2]  = 1;
	 Serial_hinght_xy_last[3]  = 1;
	 Serial_hinght_xy_last[4]  = 1;
	 Serial_hinght_xy_last[5]  = 1;
	 Serial_hinght_xy[0]  = 10;
	 Serial_hinght_xy[1]  = 10;
	 Serial_hinght_xy[2]  = 10;
	 Serial_hinght_xy[3]  = 10;
	 Serial_hinght_xy[4]  = 10;
	 Serial_hinght_xy[5]  = 10;
	 Serial_hinght_erweima[0] = 0;
   LED1_OFF();
//	 PWM_DISABLE();
	 set_speed = 500;
   MOROR_MOVE_MAX(-1600*100,set_speed);
//	 MOTOR_Move(-1600*100,set_speed,set_speed,set_speed);       //需要时调用
   GG = 0;
	while (1)
	{	
		   Motor_SetSpeed(1);   //右下      前
			 Motor_SetSpeed1(2);  //左下
			 Motor_SetSpeed2(2);  //右上
			 Motor_SetSpeed3(1);
		   PWM_ENABLE();
		   
			 x11 = hexToDec(Serial_RxPacket_2[19]);     //        串口三扫二维码 串口二发送x y color        cv xwc     mv c y 123321
			 x22 = hexToDec(Serial_RxPacket_2[20]);     //				 2cv x w  3 c y 123321
			 x33 = hexToDec(Serial_RxPacket_2[21]);     //     		 0x13 19 0x14  20 0x15 21
			 w11 = hexToDec(Serial_RxPacket_2[22]);    //
			 w22 = hexToDec(Serial_RxPacket_2[23]);    //
			 w33 = hexToDec(Serial_RxPacket_2[24]);    //



		 y  = Serial_RxPacket_2[1];
		 cvc= hexToDec(Serial_RxPacket_2[18]);
	 	 OLED_ShowSignedNum(1,1,((hexToDec(TXT_HC[3])*100)+(hexToDec(TXT_HC[4])*10)+(hexToDec(TXT_HC[5]))),3);  //212
		 OLED_ShowSignedNum(1,7,((hexToDec(TXT_HC[0])*100)+(hexToDec(TXT_HC[1])*10)+(hexToDec(TXT_HC[2]))),3);  //384
		 OLED_ShowSignedNum(2,1,BCCTR,3);
		 OLED_ShowSignedNum(3,1,(HV_DE[3]*100+HV_DE[4]*10+HV_DE[5]-342),8);
		 OLED_ShowSignedNum(4,1,(oint-10000*(HV_DE[0]*100+HV_DE[1]*10+HV_DE[2]-342)),8);

 
		 // 实际减目标是负数需   要左转 右快左慢
		 // 实际减目标是正数需要右转 右慢左快+++++++++
     if((Pick_Up == 1)&&(GG == 50))
		 {
			 	 PWM_DISABLE();
//				 ShiQv4();		
//         Delay_ms(10000);
         
//			   cot += 100000;   //抓完就走，只需要设置if(大于这个数 && 小于这个数+=这个数)就可以执行下一步，因为这个数够大，完全执行一圈两圈也不会超过+100000到+200000的区间
//		 		 GG = 61;			 
		}
		 if(Pick_Up == 369)
		 {
//			   	ShiQv5();			 
		 }
     if((Pick_Up == 2)&&(GG == 50))
		 {
//			   srggb1 = 0;
			 	 PWM_DISABLE();
//				 ShiQv2();			 
//			   Delay_ms(10000);
			   
			 //			 	 cot += 100000;   //抓完就走，只需要设置大于这个数就可以执行下一步，因为这个数够大，完全执行一圈两圈也不会超过+100000到+200000的区间
//				 GG = 61;	 
		 }
		 if(Pick_Up == 123)
		 {
//				 ShiQv3();			 
//			   Delay_ms(10000);
			   
			 //			 	 cot += 100000;   //抓完就走，只需要设置大于这个数就可以执行下一步，因为这个数够大，完全执行一圈两圈也不会超过+100000到+200000的区间
//				 GG = 61;	 
		 }

//		 else
//		 {
//			 PWM_DISABLE();
//		 }

	}
}
void TIM6_IRQHandler(void)         //定时器6，处理标志位响应逻辑
{
	if (TIM_GetITStatus(TIM6, TIM_IT_Update) == SET)  //获取中断标志位
	{
		                                                        //中断执行函数

		 cnt++;
		 aa++;		
		 cot++;
     cxt++;
	 	 crt++;
		 oint++;
		HV_DE[3] = hexToDec(TXT_HC[3]); //x1 - w3  242
		HV_DE[4] = hexToDec(TXT_HC[4]);
		HV_DE[5] = hexToDec(TXT_HC[5]);
		HV_DE[0] = hexToDec(TXT_HC[0]);   //352
		HV_DE[1] = hexToDec(TXT_HC[1]);
		HV_DE[2] = hexToDec(TXT_HC[2]);

		 xiansgu1();    //分段矫正  242 342
		 xiansgu2();
     		
     Serial_hinght_ab[6] = Serial_hinght_ab[0];
		 Serial_hinght_ab[7] = Serial_hinght_ab[1];
		 Serial_hinght_ab[8] = Serial_hinght_ab[2];
		 Serial_hinght_ab[9] = Serial_hinght_ab[3];
		 Serial_hinght_ab[10] = Serial_hinght_ab[4];
		 Serial_hinght_ab[11] = Serial_hinght_ab[5];
		 
		 Serial_hinght_ab[0] = Serial_RxPacket_2[0];  //百
		 Serial_hinght_ab[1] = Serial_RxPacket_2[3];  //十
		 Serial_hinght_ab[2] = Serial_RxPacket_2[6];  //个
		 Serial_hinght_ab[3] = Serial_RxPacket_2[9];  //百
		 Serial_hinght_ab[4] = Serial_RxPacket_2[12];  //十
		 Serial_hinght_ab[5] = Serial_RxPacket_2[15];  //个					
		 if(((((hexToDec(Serial_hinght_ab[0])*100)+(hexToDec(Serial_hinght_ab[1])*10)+(hexToDec(Serial_hinght_ab[2])))-((hexToDec(Serial_hinght_ab[0])*100)+(hexToDec(Serial_hinght_ab[1])*10)+(hexToDec(Serial_hinght_ab[2])))) != 0) 
			 || ((((hexToDec(Serial_hinght_ab[3])*100)+(hexToDec(Serial_hinght_ab[4])*10)+(hexToDec(Serial_hinght_ab[5])))-((hexToDec(Serial_hinght_ab[9])*100)+(hexToDec(Serial_hinght_ab[10])*10)+(hexToDec(Serial_hinght_ab[11])))) != 0))
		 {
			 	 oint = 0;
			   TXT_HC[0] = Serial_RxPacket_2[0];
			   TXT_HC[1] = Serial_RxPacket_2[3];
			   TXT_HC[2] = Serial_RxPacket_2[6];
			   TXT_HC[3] = Serial_RxPacket_2[9];
			   TXT_HC[4] = Serial_RxPacket_2[12];
			   TXT_HC[5] = Serial_RxPacket_2[15];
			   jn = 556;
		 }
		 yaw=(float)stcAngle.Angle[2]/32768*180;
     T00 =  Serial_hinght_erweima[next];
						x1 = T00-1;
						x2 = T00+2;
						x3 = T00+5;
						w1 = T00+8;
						w2 = T00+11;
						w3 = T00+14;       
				if(T00 == 1)
				{
					Servo3_SetAngle(3);   //123
				}
				if(T00 == 2)
				{
					Servo3_SetAngle(81);  //2
				}
				if(T00 == 3)
				{
					Servo3_SetAngle(164);   //3
				}

				if(((x11+x22+x33+w11+w22+w33) == 12) && (YUY == 76))
				{
						hh++;
						if(hh<=1)
						{
							Servo2_SetAngle(40);
							Servo4_SetAngle(170);		//75，170
							Pick_Up = 123;
						}
						Serial_hinght_erweima[1] = hexToDec(Serial_RxPacket_2[19]);  //1
						Serial_hinght_erweima[2] = hexToDec(Serial_RxPacket_2[20]);  //2
					  Serial_hinght_erweima[3] = hexToDec(Serial_RxPacket_2[21]);  //3
						Serial_hinght_erweima[4] = hexToDec(Serial_RxPacket_2[22]);  //3
						Serial_hinght_erweima[5] = hexToDec(Serial_RxPacket_2[23]);  //2
						Serial_hinght_erweima[6] = hexToDec(Serial_RxPacket_2[24]);  //1
						GYD = 19;
						YUY = 13;
				}
		 
		 if(GG==71 && GYD == 19)   //GG = 71是启动标志Serial_hinght_xy
		 {
				if((((((hexToDec(Serial_hinght_xy[0])*100)+(hexToDec(Serial_hinght_xy[1])*10)+(hexToDec(Serial_hinght_xy[2])))-((hexToDec(Serial_hinght_xy_last[0])*100)+(hexToDec(Serial_hinght_xy_last[1])*10)+(hexToDec(Serial_hinght_xy_last[2])))) >= -15) 
					&& (((hexToDec(Serial_hinght_xy[0])*100)+(hexToDec(Serial_hinght_xy[1])*10)+(hexToDec(Serial_hinght_xy[2])))-((hexToDec(Serial_hinght_xy_last[0])*100)+(hexToDec(Serial_hinght_xy_last[1])*10)+(hexToDec(Serial_hinght_xy_last[2])))) <= 15) 
				&& ((((hexToDec(Serial_hinght_xy[3])*100)+(hexToDec(Serial_hinght_xy[4])*10)+(hexToDec(Serial_hinght_xy[5]))) - ((hexToDec(Serial_hinght_xy_last[3])*100)+(hexToDec(Serial_hinght_xy_last[4])*10)+(hexToDec(Serial_hinght_xy_last[5])))) <= 15) 
				&& (((hexToDec(Serial_hinght_xy[3])*100)+(hexToDec(Serial_hinght_xy[4])*10)+(hexToDec(Serial_hinght_xy[5]))) - ((hexToDec(Serial_hinght_xy_last[3])*100)+(hexToDec(Serial_hinght_xy_last[4])*10)+(hexToDec(Serial_hinght_xy_last[5]))) >= -15)
				&& ((((hexToDec(Serial_hinght_xy[3])*100)+(hexToDec(Serial_hinght_xy[4])*10)+(hexToDec(Serial_hinght_xy[5]))) != 0)&& (((hexToDec(Serial_hinght_xy_last[3])*100)+(hexToDec(Serial_hinght_xy_last[4])*10)+(hexToDec(Serial_hinght_xy_last[5])))) != 0))
				{
						GG = 55;
				}
	//		 	
				else
				{
							PWM_DISABLE();
				}
		 }

		 if(cnt >= 2600)
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
        cnt = 2500;			 
		 }	

//到达物料区停止，并掷出一个标志位sepan 对比像素点变化与定时器计时时间差
//cvc = Serial_RxPacket_3[2+next]时开始矫正	 
               
					if(next > 6 && JIXIEBI == 15)  //进行第二次放置
					{						
						cyt = 99924300+9900000;
						czt = 9992300;
						next = 4;
					}					
					if(next > 3 && JIXIEBI == 14)   //第二段转折
					{
						rmt = 1;
					}
					if(rmt == 1)   //第二次抓取
					{
						LED1_ON();
						crt = (1200*abs(Serial_hinght_erweima[next] - Serial_hinght_erweima[next-1]))+99999999;
						cxt = 9994300+9900000;
						cyt = 9992300;    //时间    
						rmt = 2;
					}

////////////////////////////////////////////////////////////////////////////////
          if(next > 3 && JIXIEBI == 13)   //第一段标志位 抓取
					{
						rnt = 1;
						next = 1;
					}
					if(GG == 78)
					{
							if((next == 2 || next == 3) && JIXIEBI == 13)   //第一段标志位 抓取
							{
									if((((((hexToDec(Serial_hinght_xy[0])*100)+(hexToDec(Serial_hinght_xy[1])*10)+(hexToDec(Serial_hinght_xy[2])))-((hexToDec(Serial_hinght_xy_last[0])*100)+(hexToDec(Serial_hinght_xy_last[1])*10)+(hexToDec(Serial_hinght_xy_last[2])))) >= -15) 
										&& (((hexToDec(Serial_hinght_xy[0])*100)+(hexToDec(Serial_hinght_xy[1])*10)+(hexToDec(Serial_hinght_xy[2])))-((hexToDec(Serial_hinght_xy_last[0])*100)+(hexToDec(Serial_hinght_xy_last[1])*10)+(hexToDec(Serial_hinght_xy_last[2])))) <= 15) 
									&& ((((hexToDec(Serial_hinght_xy[3])*100)+(hexToDec(Serial_hinght_xy[4])*10)+(hexToDec(Serial_hinght_xy[5]))) - ((hexToDec(Serial_hinght_xy_last[3])*100)+(hexToDec(Serial_hinght_xy_last[4])*10)+(hexToDec(Serial_hinght_xy_last[5])))) <= 15) 
									&& (((hexToDec(Serial_hinght_xy[3])*100)+(hexToDec(Serial_hinght_xy[4])*10)+(hexToDec(Serial_hinght_xy[5]))) - ((hexToDec(Serial_hinght_xy_last[3])*100)+(hexToDec(Serial_hinght_xy_last[4])*10)+(hexToDec(Serial_hinght_xy_last[5]))) >= -15)
									&& ((((hexToDec(Serial_hinght_xy[3])*100)+(hexToDec(Serial_hinght_xy[4])*10)+(hexToDec(Serial_hinght_xy[5]))) != 0)&& (((hexToDec(Serial_hinght_xy_last[3])*100)+(hexToDec(Serial_hinght_xy_last[4])*10)+(hexToDec(Serial_hinght_xy_last[5])))) != 0))
									{
											GG = 50;
									}
									else
									{
												PWM_DISABLE();
									}
							}
							if(next == 1 && JIXIEBI == 13)   //第一段标志位 抓取
							{
									GG = 71;
							}
			  	}
					if(rnt == 1)   //第一次放置
					{
						cot = 9994300+9900000;
						cxt = 9992300;    //执行 走路+走路 -> 抓取     
						rnt = 2;
					}
					if((rnt == 2) && (GG == 155) && (next <= 3))
					{
					    crt = 9999999;  //移动时间  按顺序抓取
						  JIXIEAI = 13;
              rnt = 3;						
					}
///**********************************************第一次矫正 opencv x矫正 *****************************************************************/	
//					l = hexToDec(Serial_RxPacket_2[18]);  //				 0x13 19 0x14 20 0x15 21        //243 60  243 60 245 63
					if(GG == 38)
					{
						if(((hexToDec(Serial_RxPacket_2[w1])*100)+(hexToDec(Serial_RxPacket_2[w2])*10)+(hexToDec(Serial_RxPacket_2[w3])))>=386 && ((hexToDec(Serial_RxPacket_2[w1])*100)+(hexToDec(Serial_RxPacket_2[w2])*10)+(hexToDec(Serial_RxPacket_2[w3])))<=600)  //后
						{							
							Motor_SetSpeed(1);   //右下      前
							Motor_SetSpeed1(2);  //左下
							Motor_SetSpeed2(2);  //右上
							Motor_SetSpeed3(1);  //左上			
							PWM_ENABLE();							
						}
						if(((hexToDec(Serial_RxPacket_2[w1])*100)+(hexToDec(Serial_RxPacket_2[w2])*10)+(hexToDec(Serial_RxPacket_2[w3])))<=79 && ((hexToDec(Serial_RxPacket_2[w1])*100)+(hexToDec(Serial_RxPacket_2[w2])*10)+(hexToDec(Serial_RxPacket_2[w3])))>=10)  //后
						{							
							Motor_SetSpeed(2);   //右下      前
							Motor_SetSpeed1(1);  //左下
							Motor_SetSpeed2(1);  //右上
							Motor_SetSpeed3(2);  //左上	
							PWM_ENABLE();							
						}
						if(((hexToDec(Serial_RxPacket_2[w1])*100)+(hexToDec(Serial_RxPacket_2[w2])*10)+(hexToDec(Serial_RxPacket_2[w3])))==0)  //后
						{
										PWM_DISABLE();
						}	
						if(((hexToDec(Serial_RxPacket_2[w1])*100)+(hexToDec(Serial_RxPacket_2[w2])*10)+(hexToDec(Serial_RxPacket_2[w3])) > 79) && ((hexToDec(Serial_RxPacket_2[w1])*100)+(hexToDec(Serial_RxPacket_2[w2])*10)+(hexToDec(Serial_RxPacket_2[w3])) < 366))  //后
						{
							      LED1_OFF();
										PWM_DISABLE();
										GG = 50;
						}	
					}
				/*****************************第二次矫正，w前后走***********************************************/
				if(GG == 55)
				{
					  
						if(((hexToDec(Serial_RxPacket_2[x1])*100)+(hexToDec(Serial_RxPacket_2[x2])*10)+(hexToDec(Serial_RxPacket_2[x3])) >=355) && ((hexToDec(Serial_RxPacket_2[x1])*100)+(hexToDec(Serial_RxPacket_2[x2])*10)+(hexToDec(Serial_RxPacket_2[x3])) <= 600))//zuo
						{							
							Motor_SetSpeed(1);   //右下  //hou bbb
							Motor_SetSpeed1(1);  //左下
							Motor_SetSpeed2(2);  //右上
							Motor_SetSpeed3(2);  //左上			
              PWM_ENABLE();							
						}
						if(((hexToDec(Serial_RxPacket_2[x1])*100)+(hexToDec(Serial_RxPacket_2[x2])*10)+(hexToDec(Serial_RxPacket_2[x3])) <=314) && ((hexToDec(Serial_RxPacket_2[x1])*100)+(hexToDec(Serial_RxPacket_2[x2])*10)+(hexToDec(Serial_RxPacket_2[x3])) >= 10))  //you
						{							
							Motor_SetSpeed(2);   //右下
							Motor_SetSpeed1(2);  //左下
							Motor_SetSpeed2(1);  //右上
							Motor_SetSpeed3(1);  //左上					
						  PWM_ENABLE();
						}
						if(((hexToDec(Serial_RxPacket_2[x1])*100)+(hexToDec(Serial_RxPacket_2[x2])*10)+(hexToDec(Serial_RxPacket_2[x3])))==0)  //后
						{
										PWM_DISABLE();
						}	
						if(((hexToDec(Serial_RxPacket_2[x1])*100)+(hexToDec(Serial_RxPacket_2[x2])*10)+(hexToDec(Serial_RxPacket_2[x3])) >314) && ((hexToDec(Serial_RxPacket_2[x1])*100)+(hexToDec(Serial_RxPacket_2[x2])*10)+(hexToDec(Serial_RxPacket_2[x3])) <355))  //后
						{
					
											PWM_DISABLE();
											GG = 38;
						}
					}
				

/*****************************************************************************************************************************************************************************************/
		TIM_ClearITPendingBit(TIM6, TIM_IT_Update);  //清除中断标志位
	}
}

void TIM7_IRQHandler(void)         //定時器7，处理基本方向逻辑
{
	if (TIM_GetITStatus(TIM7, TIM_IT_Update) == SET)  //获取中断标志位
	{
//		if(yaw < 0)                //
//		{                          //
//			yaw += 360;              //
//		}                          //
/*
		 0前进抓
		 0后退
		90前进放
		 0前进
	 -90前进放
	 -90前进
	   0后退抓
		 0后退
			90前进放
			 0前进
		 -90前进放
		 -90前进
			 0后退抓
*/
		if(GG==61)
		{		
				if((cot > 10) && (cot <= 1900))
				{
					if(JIXIEBI == 0)
					{
						  PWM_DISABLE();
							JIXIEBI = 11;
					}
          Motor_conter(yaw,2,-2,200,2,2,1,1);
				}
				if((cot > 1900) && (cot <= 3500))
				{
						if(JIXIEBI == 11)
						{
								PCS(40);								
								JIXIEBI = 12;
						}
            Motor_conter(yaw,2,-2,40,2,1,1,2);
				}		
				if((cot >= 3500) && (cot <= 9992300+9900000))     //第一次转盘抓取物块
				{
							if(JIXIEBI == 12)
							{
//									PWM_Init3(3000,PSC,3000/2);  //右
//									PWM_Init2(3000,PSC,3000/2);  //右
//									PWM_Init1(3000,PSC,3000/2);   //左
//									PWM_Init(3000,PSC,3000/2);    //左   //ARR  PSC  CCR   arr过高会堵转  f越高脉冲越多
								JIXIEBI = 13;
							}
							Motor_SetSpeed(3);   //右上       前
							Motor_SetSpeed1(3);  //左上   
							Motor_SetSpeed2(3);  //右下		
							Motor_SetSpeed3(3);  //左下			
							PWM_DISABLE();
					    if(GG == 61)
							{
					       GG = 78;
							}
				}		
/////////////////////////////////////////////////////////////////倒退		
					if((cxt > 9992300) && (cxt <= 9992300+2700))
					{
						if(JINXIECI == 21)
						{
//								PWM_Init3(200,PSC,200/2);  //右
//								PWM_Init2(200,PSC,200/2);  //右
//								PWM_Init1(200,PSC,200/2);   //左
//								PWM_Init(200,PSC,200/2);    //左   //ARR  PSC  CCR   arr过高会堵转  f越高脉冲越多
							  Pick_Up = 369;
								JINXIECI = 22;
						}
						Motor_conter(yaw,1,-1,50,1,2,2,1);
						if((yaw > -1)&&(yaw < 1))
						{				
								Motor_SetSpeed(1);   //右上       前
								Motor_SetSpeed1(2);  //左上   
								Motor_SetSpeed2(2);  //右下		
								Motor_SetSpeed3(1);  //左下			
								PWM_ENABLE();
						}
					}
					if((cxt > 9992300+2700) && (cxt <= 9992300+8100+5400))
					{
							Motor_conter(yaw,91,89,50,2,1,1,2);
							if((yaw > 89)&&(yaw < 91))
							{				
									Motor_SetSpeed(2);   //右上       前
									Motor_SetSpeed1(1);  //左上   
									Motor_SetSpeed2(1);  //右下		
									Motor_SetSpeed3(2);  //左下			
									PWM_ENABLE();
							}
					}		
				if((cxt >= 9992300+8100+5400) && (cxt <= 9992300+8100+7400))     //放置
				{
						Motor_conter(yaw,181,179,50,2,1,1,2);
				}
				if((cxt >= 9992300+8100+7400) && (cxt <= 9992300+9900000))     //放置
				{
							if(JIXIEBI == 13)
							{
//									PWM_Init3(24000,PSC,24000/2);  //右
//									PWM_Init2(24000,PSC,24000/2);  //右
//									PWM_Init1(24000,PSC,24000/2);   //左
//									PWM_Init(24000,PSC,24000/2);    //左   //ARR  PSC  CCR   arr过高会堵转  f越高脉冲越多
								JIXIEBI = 14;
							}
					    if(GG == 61)
							{
								 Pick_Up = 2;
								 rnt = 2;
					       GG = 155;
							}
				}		
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		}
		if(GG == 155)
		{
			  if((crt > 9999999) && (crt <= (1200*abs(Serial_hinght_erweima[next] - Serial_hinght_erweima[next-1]))+9999999))
			  {
			  		if(JIXIEAI == 13)
			  		{
//			  				PWM_Init3(300,PSC,300/2);  //右
//			  				PWM_Init2(300,PSC,300/2);  //右
//			  				PWM_Init1(300,PSC,300/2);   //左
//			  				PWM_Init(300,PSC,300/2);    //左   //ARR  PSC  CCR   arr过高会堵转  f越高脉冲越多
			  			JIXIEAI = 14;
			  		}
			  	  Motor_go((Serial_hinght_erweima[next] - Serial_hinght_erweima[next-1]));
			  }
			  if((crt > (1200*abs(Serial_hinght_erweima[next] - Serial_hinght_erweima[next-1]))+9999999) && (crt <= (1200*abs(Serial_hinght_erweima[next] - Serial_hinght_erweima[next-1]))+99999988))
			  {
						if(JIXIEAI == 14)					  
						{
//								PWM_Init3(6000,PSC,6000/2);  //右
//								PWM_Init2(6000,PSC,6000/2);  //右
//								PWM_Init1(6000,PSC,6000/2);   //左
//								PWM_Init(6000,PSC,6000/2);    //左   //ARR  PSC  CCR   arr过高会堵转  f越高脉冲越多
							JIXIEAI = 15;
						}
						GG = 21;
			  }							
		}
		TIM_ClearITPendingBit(TIM7, TIM_IT_Update);  //清除中断标志位
	}
}


