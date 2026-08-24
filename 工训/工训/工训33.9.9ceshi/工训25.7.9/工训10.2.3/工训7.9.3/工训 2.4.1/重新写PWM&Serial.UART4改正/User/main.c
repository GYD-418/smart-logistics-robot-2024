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
#include "Serial5.h"
#include "WZJZ.h"
#include "ShiQv.h"
#include "LED.h"
#include "math.h"
#include "stdlib.h"
#include "T_BuJin_Sp.h"
//389 191
extern int aa,bb,kk;
extern int F_a,F_b,F_c,F_d,F_Yaw,F_Ybw,F_e;
int aakk;
extern float yaw;
uint8_t RxData;
int s,hh;
int PWM_ENA;
int8_t Speed;
int ARR, ARRL ,ARRR, PSC , CCR;  //×óL ÓÒR
int QWZ,MC;
float SHJh,SHJz;
float  RXH , RXZ;
int yzw = 11;
float  RHD,RZD;   //´®¿Ú½ÓÊÕºá×Ý×ø±êÖµ
int YVY = 36;  //¶æ»ú¶¯×÷×é×¥È¡±êÖ¾Î»
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
int LUJING = 0;
extern int next;
int MUB; //´Ë²ÎÊýÎª×ªÏòÊ±ºòµÄ±êÖ¾Î» ³ö·¢Ê±0 ×ª90¶È¼Ó1
extern int Pick_Up;
extern uint32_t set_speed;         // ×î´óËÙ¶ÈÉèÖÃ µ¥Î»Îª0.1rad/sec
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
	 uartInit(9600);
	 PWM_Initkk(50,PSC,50/2);
	 Timer_Init(120,720);
	 Timer_Init_seven(120,720);
	 Serial_Init3();
	 Serial_Init22();
	 Serial4_Init();
	 Serial_Init5();
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
	 Servo4_SetAngle(170);		//65£¬155
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
	 ActionGroup0();
	 LED1_ON();
	 GG = 21;
//   Pick_Up = 123;
	while (1)
	{	  
		 GG=21;
		 x11 =  1;    //        ´®¿ÚÈýÉ¨¶þÎ¬Âë ´®¿Ú¶þ·¢ËÍx y color        cv xwc     mv c y 123321
		 x22 =  2;    //				 2cv x w  3 c y 123321
		 x33 =  3;    //     		 0x13 19 0x14  20 0x15 21
		 w11 =  3;   //
		 w22 =  2;   //
		 w33 =  1;   //
		
//	 	 OLED_ShowSignedNum(1,1,yaw,3);  //212
		 OLED_ShowSignedNum(1,5,((Serial_RxPacket_2[1]*100)+(Serial_RxPacket_2[4]*10)+(Serial_RxPacket_2[7])),3);  //294
		 OLED_ShowSignedNum(1,9,((Serial_RxPacket_2[10]*100)+(Serial_RxPacket_2[13]*10)+(Serial_RxPacket_2[16])),3);  //145
//		 OLED_ShowSignedNum(1,13,Serial_hinght_erweima[3],3);  //384
//		 OLED_ShowSignedNum(2,5,Serial_hinght_erweima[4],3);  //384
//		 OLED_ShowSignedNum(2,9,Serial_hinght_erweima[5],3);  //384
		 OLED_ShowSignedNum(2,13,hexToDec(Serial_RxPacket_2[18]),3);  //384
		 OLED_ShowSignedNum(2,1,GG,3);  //
		 OLED_ShowSignedNum(3,1,abs(Serial_hinght_erweima[8] - Serial_hinght_erweima[7]),3);  //112
		
		 OLED_ShowSignedNum(3,5,Serial_hinght_erweima[10],3);		
		 OLED_ShowSignedNum(4,1,next,8);   //1
//		     if((Pick_Up == 1)&&(GG == 50))
//				 {
//					 GG = 69;
//				 }
				 if((Pick_Up == 1)&&(GG == 69))   //×¥×ªÅÌÎïÁÏ ×¥µÚÒ»¸ö£¬ÆäËûÁ½¸öÐ´ËÀ
				 {
						 if(next <= 3)
						 {
								if(hexToDec(Serial_hinght_erweima[9]) == 1)   //ºìÉ«ÔÚÏÂ
								{
										 PWM_DISABLE();
									   LED1_OFF();
										 if(next == 1)
										 {
													 if((Serial_hinght_erweima[next] == 1)&&(GG == 69))//Ô²ÅÌ×ª¶¯Ç°£¬ºìÉ«ÔÚÏÂ°Ú·Å×¥Á©
													 {
															ActionGroup5();  //×¥ÏÂ //1600
															ActionGroup31();  //1200
													 }
													 if((Serial_hinght_erweima[next] == 2)&&(GG == 69))//Ô²ÅÌ×ª¶¯Ç°£¬ºìÉ«ÔÚÏÂ°Ú·Å×¥Á©
													 {
															ActionGroup6();  //×¥×ó  2300
															ActionGroup32(); //1200
													 }
													 if((Serial_hinght_erweima[next] == 3)&&(GG == 69))//Ô²ÅÌ×ª¶¯ºó£¬µÚÈý¸ö×¥µÄÎ»ÖÃ±ä³ÉÖ®Ç°¶¯×÷×éÎ»ÖÃ¼ÓÒ»
													 {
															ActionGroup7();  //×¥ÓÒ  //2800
															ActionGroup33(); //1500
													 }			
										 }	
										 if(next == 2)
										 {
													 if((Serial_hinght_erweima[next] == 3)&&(GG == 69))//Ô²ÅÌ×ª¶¯Ç°£¬ºìÉ«ÔÚÏÂ°Ú·Å×¥Á©
													 {
															ActionGroup5();  //1600
															ActionGroup33(); //1500
													 }
													 if((Serial_hinght_erweima[next] == 1)&&(GG == 69))//Ô²ÅÌ×ª¶¯Ç°£¬ºìÉ«ÔÚÏÂ°Ú·Å×¥Á©
													 {
															ActionGroup6(); //2300
															ActionGroup31();  //1200
													 }
													 if((Serial_hinght_erweima[next] == 2)&&(GG == 69))//Ô²ÅÌ×ª¶¯ºó£¬µÚÈý¸ö×¥µÄÎ»ÖÃ±ä³ÉÖ®Ç°¶¯×÷×éÎ»ÖÃ¼ÓÒ»
													 {
															ActionGroup7();  //2800
															ActionGroup32();  //1200
													 }			
										 }	
										 if(next == 3)
										 {
													 if((Serial_hinght_erweima[next] == 2)&&(GG == 69))//Ô²ÅÌ×ª¶¯Ç°£¬ºìÉ«ÔÚÏÂ°Ú·Å×¥Á©
													 {
															ActionGroup5(); //1600
															ActionGroup32();  //1200
													 }
													 if((Serial_hinght_erweima[next] == 3)&&(GG == 69))//Ô²ÅÌ×ª¶¯Ç°£¬ºìÉ«ÔÚÏÂ°Ú·Å×¥Á©
													 {
															ActionGroup6();  //2300
															ActionGroup33();  //1500
													 }
													 if((Serial_hinght_erweima[next] == 1)&&(GG == 69))//Ô²ÅÌ×ª¶¯ºó£¬µÚÈý¸ö×¥µÄÎ»ÖÃ±ä³ÉÖ®Ç°¶¯×÷×éÎ»ÖÃ¼ÓÒ»
													 {
															ActionGroup7();  //2800
															ActionGroup31();  //1200
													 }			
										 }							 				 
								}
								if(hexToDec(Serial_hinght_erweima[9]) == 2)   //ÂÌÉ«ÔÚÏÂ
								{
										 PWM_DISABLE();
									   LED1_OFF();
										 if(next == 1)
										 {
													 if((Serial_hinght_erweima[next] == 2)&&(GG == 69))//Ô²ÅÌ×ª¶¯Ç°£¬ºìÉ«ÔÚÏÂ°Ú·Å×¥Á©
													 {
															ActionGroup5();  //1600  
															ActionGroup32();  //1200
													 }
													 if((Serial_hinght_erweima[next] == 3)&&(GG == 69))//Ô²ÅÌ×ª¶¯Ç°£¬ºìÉ«ÔÚÏÂ°Ú·Å×¥Á©
													 {
															ActionGroup6();  //2300
															ActionGroup33();  //1500
													 }
													 if((Serial_hinght_erweima[next] == 1)&&(GG == 69))//Ô²ÅÌ×ª¶¯ºó£¬µÚÈý¸ö×¥µÄÎ»ÖÃ±ä³ÉÖ®Ç°¶¯×÷×éÎ»ÖÃ¼ÓÒ»
													 {
															ActionGroup7();  //2800
															ActionGroup31();   //1200
													 }			
										 }	
										 if(next == 2)
										 {
													 if((Serial_hinght_erweima[next] == 1)&&(GG == 69))//Ô²ÅÌ×ª¶¯Ç°£¬ºìÉ«ÔÚÏÂ°Ú·Å×¥Á©
													 {
															ActionGroup5();  //×¥ÏÂ //1600
															ActionGroup31();  //1200
													 }
													 if((Serial_hinght_erweima[next] == 2)&&(GG == 69))//Ô²ÅÌ×ª¶¯Ç°£¬ºìÉ«ÔÚÏÂ°Ú·Å×¥Á©
													 {
															ActionGroup6();  //×¥×ó  2300
															ActionGroup32(); //1200
													 }
													 if((Serial_hinght_erweima[next] == 3)&&(GG == 69))//Ô²ÅÌ×ª¶¯ºó£¬µÚÈý¸ö×¥µÄÎ»ÖÃ±ä³ÉÖ®Ç°¶¯×÷×éÎ»ÖÃ¼ÓÒ»
													 {
															ActionGroup7();  //×¥ÓÒ  //2800
															ActionGroup33(); //1500
													 }			
										 }				 
										 if(next == 3)
										 {
													 if((Serial_hinght_erweima[next] == 3)&&(GG == 69))//Ô²ÅÌ×ª¶¯Ç°£¬ºìÉ«ÔÚÏÂ°Ú·Å×¥Á©
													 {
															ActionGroup5();  //1600
															ActionGroup33(); //1500
													 }
													 if((Serial_hinght_erweima[next] == 1)&&(GG == 69))//Ô²ÅÌ×ª¶¯Ç°£¬ºìÉ«ÔÚÏÂ°Ú·Å×¥Á©
													 {
															ActionGroup6(); //2300
															ActionGroup31();  //1200
													 }
													 if((Serial_hinght_erweima[next] == 2)&&(GG == 69))//Ô²ÅÌ×ª¶¯ºó£¬µÚÈý¸ö×¥µÄÎ»ÖÃ±ä³ÉÖ®Ç°¶¯×÷×éÎ»ÖÃ¼ÓÒ»
													 {
															ActionGroup7();  //2800
															ActionGroup32();  //1200
													 }			
										 }							 				 
								}
								if(hexToDec(Serial_hinght_erweima[9]) == 3)   //À¶É«ÔÚÏÂ
								{
										 PWM_DISABLE();
									   LED1_OFF();
										 if(next == 1)
										 {
													 if((Serial_hinght_erweima[next] == 3)&&(GG == 69))//Ô²ÅÌ×ª¶¯Ç°£¬ºìÉ«ÔÚÏÂ°Ú·Å×¥Á©
													 {
															ActionGroup5();  //1600
															ActionGroup33(); //1500
													 }
													 if((Serial_hinght_erweima[next] == 1)&&(GG == 69))//Ô²ÅÌ×ª¶¯Ç°£¬ºìÉ«ÔÚÏÂ°Ú·Å×¥Á©
													 {
															ActionGroup6(); //2300
															ActionGroup31();  //1200
													 }
													 if((Serial_hinght_erweima[next] == 2)&&(GG == 69))//Ô²ÅÌ×ª¶¯ºó£¬µÚÈý¸ö×¥µÄÎ»ÖÃ±ä³ÉÖ®Ç°¶¯×÷×éÎ»ÖÃ¼ÓÒ»
													 {
															ActionGroup7();  //2800
															ActionGroup32();  //1200
													 }			
										 }	
										 if(next == 2)
										 {
													 if((Serial_hinght_erweima[next] == 2)&&(GG == 69))//Ô²ÅÌ×ª¶¯Ç°£¬ºìÉ«ÔÚÏÂ°Ú·Å×¥Á©
													 {
															ActionGroup5();  //1600  
															ActionGroup32();  //1200
													 }
													 if((Serial_hinght_erweima[next] == 3)&&(GG == 69))//Ô²ÅÌ×ª¶¯Ç°£¬ºìÉ«ÔÚÏÂ°Ú·Å×¥Á©
													 {
															ActionGroup6();  //2300
															ActionGroup33();  //1500
													 }
													 if((Serial_hinght_erweima[next] == 1)&&(GG == 69))//Ô²ÅÌ×ª¶¯ºó£¬µÚÈý¸ö×¥µÄÎ»ÖÃ±ä³ÉÖ®Ç°¶¯×÷×éÎ»ÖÃ¼ÓÒ»
													 {
															ActionGroup7();  //2800
															ActionGroup31();   //1200
													 }																 
										 }							 
										 if(next == 3)
										 {
										       if((Serial_hinght_erweima[next] == 1)&&(GG == 69))//Ô²ÅÌ×ª¶¯Ç°£¬ºìÉ«ÔÚÏÂ°Ú·Å×¥Á©
													 {
															ActionGroup5();  //×¥ÏÂ //1600
															ActionGroup31();  //1200
													 }
													 if((Serial_hinght_erweima[next] == 2)&&(GG == 69))//Ô²ÅÌ×ª¶¯Ç°£¬ºìÉ«ÔÚÏÂ°Ú·Å×¥Á©
													 {
															ActionGroup6();  //×¥×ó  2300
															ActionGroup32(); //1200
													 }
													 if((Serial_hinght_erweima[next] == 3)&&(GG == 69))//Ô²ÅÌ×ª¶¯ºó£¬µÚÈý¸ö×¥µÄÎ»ÖÃ±ä³ÉÖ®Ç°¶¯×÷×éÎ»ÖÃ¼ÓÒ»
													 {
															ActionGroup7();  //×¥ÓÒ  //2800
															ActionGroup33(); //1500
													 }	
										 }
								 }										 
				     }
			   }
				 if((Pick_Up == 121)&&(GG == 69))   //×¥×ªÅÌÎïÁÏ ×¥µÚÒ»¸ö£¬ÆäËûÁ½¸öÐ´ËÀ
				 {
						 if((next <= 6) && (next >= 4))
						 {
								if(hexToDec(Serial_hinght_erweima[10]) == 1)   //ºìÉ«ÔÚÏÂ
								{
										 PWM_DISABLE();
									   LED1_OFF();
										 if(next == 4)
										 {
													 if((Serial_hinght_erweima[next] == 1)&&(GG == 69))//Ô²ÅÌ×ª¶¯Ç°£¬ºìÉ«ÔÚÏÂ°Ú·Å×¥Á©
													 {
															ActionGroup5();  //×¥ÏÂ //1600
															ActionGroup31();  //1200
													 }
													 if((Serial_hinght_erweima[next] == 2)&&(GG == 69))//Ô²ÅÌ×ª¶¯Ç°£¬ºìÉ«ÔÚÏÂ°Ú·Å×¥Á©
													 {
															ActionGroup6();  //×¥×ó  2300
															ActionGroup32(); //1200
													 }
													 if((Serial_hinght_erweima[next] == 3)&&(GG == 69))//Ô²ÅÌ×ª¶¯ºó£¬µÚÈý¸ö×¥µÄÎ»ÖÃ±ä³ÉÖ®Ç°¶¯×÷×éÎ»ÖÃ¼ÓÒ»
													 {
															ActionGroup7();  //×¥ÓÒ  //2800
															ActionGroup33(); //1500
													 }			
										 }	
										 if(next == 5)
										 {
													 if((Serial_hinght_erweima[next] == 3)&&(GG == 69))//Ô²ÅÌ×ª¶¯Ç°£¬ºìÉ«ÔÚÏÂ°Ú·Å×¥Á©
													 {
															ActionGroup5();  //1600
															ActionGroup33(); //1500
													 }
													 if((Serial_hinght_erweima[next] == 1)&&(GG == 69))//Ô²ÅÌ×ª¶¯Ç°£¬ºìÉ«ÔÚÏÂ°Ú·Å×¥Á©
													 {
															ActionGroup6(); //2300
															ActionGroup31();  //1200
													 }
													 if((Serial_hinght_erweima[next] == 2)&&(GG == 69))//Ô²ÅÌ×ª¶¯ºó£¬µÚÈý¸ö×¥µÄÎ»ÖÃ±ä³ÉÖ®Ç°¶¯×÷×éÎ»ÖÃ¼ÓÒ»
													 {
															ActionGroup7();  //2800
															ActionGroup32();  //1200
													 }			
										 }	
										 if(next == 6)
										 {
													 if((Serial_hinght_erweima[next] == 2)&&(GG == 69))//Ô²ÅÌ×ª¶¯Ç°£¬ºìÉ«ÔÚÏÂ°Ú·Å×¥Á©
													 {
															ActionGroup5(); //1600
															ActionGroup32();  //1200
													 }
													 if((Serial_hinght_erweima[next] == 3)&&(GG == 69))//Ô²ÅÌ×ª¶¯Ç°£¬ºìÉ«ÔÚÏÂ°Ú·Å×¥Á©
													 {
															ActionGroup6();  //2300
															ActionGroup33();  //1500
													 }
													 if((Serial_hinght_erweima[next] == 1)&&(GG == 69))//Ô²ÅÌ×ª¶¯ºó£¬µÚÈý¸ö×¥µÄÎ»ÖÃ±ä³ÉÖ®Ç°¶¯×÷×éÎ»ÖÃ¼ÓÒ»
													 {
															ActionGroup7();  //2800
															ActionGroup31();  //1200
													 }			
										 }							 				 
								}
								if(hexToDec(Serial_hinght_erweima[10]) == 2)   //ÂÌÉ«ÔÚÏÂ
								{
										 PWM_DISABLE();
									   LED1_OFF();
										 if(next == 4)
										 {
													 if((Serial_hinght_erweima[next] == 2)&&(GG == 69))//Ô²ÅÌ×ª¶¯Ç°£¬ºìÉ«ÔÚÏÂ°Ú·Å×¥Á©
													 {
															ActionGroup5();  //1600  
															ActionGroup32();  //1200
													 }
													 if((Serial_hinght_erweima[next] == 3)&&(GG == 69))//Ô²ÅÌ×ª¶¯Ç°£¬ºìÉ«ÔÚÏÂ°Ú·Å×¥Á©
													 {
															ActionGroup6();  //2300
															ActionGroup33();  //1500
													 }
													 if((Serial_hinght_erweima[next] == 1)&&(GG == 69))//Ô²ÅÌ×ª¶¯ºó£¬µÚÈý¸ö×¥µÄÎ»ÖÃ±ä³ÉÖ®Ç°¶¯×÷×éÎ»ÖÃ¼ÓÒ»
													 {
															ActionGroup7();  //2800
															ActionGroup31();   //1200
													 }			
										 }	
										 if(next == 5)
										 {
													 if((Serial_hinght_erweima[next] == 1)&&(GG == 69))//Ô²ÅÌ×ª¶¯Ç°£¬ºìÉ«ÔÚÏÂ°Ú·Å×¥Á©
													 {
															ActionGroup5();  //×¥ÏÂ //1600
															ActionGroup31();  //1200
													 }
													 if((Serial_hinght_erweima[next] == 2)&&(GG == 69))//Ô²ÅÌ×ª¶¯Ç°£¬ºìÉ«ÔÚÏÂ°Ú·Å×¥Á©
													 {
															ActionGroup6();  //×¥×ó  2300
															ActionGroup32(); //1200
													 }
													 if((Serial_hinght_erweima[next] == 3)&&(GG == 69))//Ô²ÅÌ×ª¶¯ºó£¬µÚÈý¸ö×¥µÄÎ»ÖÃ±ä³ÉÖ®Ç°¶¯×÷×éÎ»ÖÃ¼ÓÒ»
													 {
															ActionGroup7();  //×¥ÓÒ  //2800
															ActionGroup33(); //1500
													 }			
										 }				 
										 if(next == 6)
										 {
													 if((Serial_hinght_erweima[next] == 3)&&(GG == 69))//Ô²ÅÌ×ª¶¯Ç°£¬ºìÉ«ÔÚÏÂ°Ú·Å×¥Á©
													 {
															ActionGroup5();  //1600
															ActionGroup33(); //1500
													 }
													 if((Serial_hinght_erweima[next] == 1)&&(GG == 69))//Ô²ÅÌ×ª¶¯Ç°£¬ºìÉ«ÔÚÏÂ°Ú·Å×¥Á©
													 {
															ActionGroup6(); //2300
															ActionGroup31();  //1200
													 }
													 if((Serial_hinght_erweima[next] == 2)&&(GG == 69))//Ô²ÅÌ×ª¶¯ºó£¬µÚÈý¸ö×¥µÄÎ»ÖÃ±ä³ÉÖ®Ç°¶¯×÷×éÎ»ÖÃ¼ÓÒ»
													 {
															ActionGroup7();  //2800
															ActionGroup32();  //1200
													 }			
										 }							 				 
								}
								if(hexToDec(Serial_hinght_erweima[10]) == 3)   //À¶É«ÔÚÏÂ
								{
										 PWM_DISABLE();
									   LED1_OFF();
										 if(next == 4)
										 {
													 if((Serial_hinght_erweima[next] == 3)&&(GG == 69))//Ô²ÅÌ×ª¶¯Ç°£¬ºìÉ«ÔÚÏÂ°Ú·Å×¥Á©
													 {
															ActionGroup5();  //1600
															ActionGroup33(); //1500
													 }
													 if((Serial_hinght_erweima[next] == 1)&&(GG == 69))//Ô²ÅÌ×ª¶¯Ç°£¬ºìÉ«ÔÚÏÂ°Ú·Å×¥Á©
													 {
															ActionGroup6(); //2300
															ActionGroup31();  //1200
													 }
													 if((Serial_hinght_erweima[next] == 2)&&(GG == 69))//Ô²ÅÌ×ª¶¯ºó£¬µÚÈý¸ö×¥µÄÎ»ÖÃ±ä³ÉÖ®Ç°¶¯×÷×éÎ»ÖÃ¼ÓÒ»
													 {
															ActionGroup7();  //2800
															ActionGroup32();  //1200
													 }			
										 }	
										 if(next == 5)
										 {
													 if((Serial_hinght_erweima[next] == 2)&&(GG == 69))//Ô²ÅÌ×ª¶¯Ç°£¬ºìÉ«ÔÚÏÂ°Ú·Å×¥Á©
													 {
															ActionGroup5();  //1600  
															ActionGroup32();  //1200
													 }
													 if((Serial_hinght_erweima[next] == 3)&&(GG == 69))//Ô²ÅÌ×ª¶¯Ç°£¬ºìÉ«ÔÚÏÂ°Ú·Å×¥Á©
													 {
															ActionGroup6();  //2300
															ActionGroup33();  //1500
													 }
													 if((Serial_hinght_erweima[next] == 1)&&(GG == 69))//Ô²ÅÌ×ª¶¯ºó£¬µÚÈý¸ö×¥µÄÎ»ÖÃ±ä³ÉÖ®Ç°¶¯×÷×éÎ»ÖÃ¼ÓÒ»
													 {
															ActionGroup7();  //2800
															ActionGroup31();   //1200
													 }																 
										 }							 
										 if(next == 6)
										 {
										       if((Serial_hinght_erweima[next] == 1)&&(GG == 69))//Ô²ÅÌ×ª¶¯Ç°£¬ºìÉ«ÔÚÏÂ°Ú·Å×¥Á©
													 {
															ActionGroup5();  //×¥ÏÂ //1600
															ActionGroup31();  //1200
													 }
													 if((Serial_hinght_erweima[next] == 2)&&(GG == 69))//Ô²ÅÌ×ª¶¯Ç°£¬ºìÉ«ÔÚÏÂ°Ú·Å×¥Á©
													 {
															ActionGroup6();  //×¥×ó  2300
															ActionGroup32(); //1200
													 }
													 if((Serial_hinght_erweima[next] == 3)&&(GG == 69))//Ô²ÅÌ×ª¶¯ºó£¬µÚÈý¸ö×¥µÄÎ»ÖÃ±ä³ÉÖ®Ç°¶¯×÷×éÎ»ÖÃ¼ÓÒ»
													 {
															ActionGroup7();  //×¥ÓÒ  //2800
															ActionGroup33(); //1500
													 }	
										 }
								 }										 
				     }
			   }

		 if(Pick_Up == 0)
		 {
		 	 ActionGroup34();//ÉÏµç³õÊ¼»¯×´Ì¬		   
			   Pick_Up = 21;
		 }
		 if(Pick_Up == 161 && GG == 150)  //µ½¼Ó¹¤Çø×¼±¸½ÃÕý×ËÊÆ 
		 {
			 ActionGroup34();
		 }
		 if(Pick_Up == 121 && GG == 150)  //»Ø³Ç  
		 {
			 ActionGroup199();
		 }	 		 		 
     if((Pick_Up == 2)&&(GG == 50)&&(next <= 3))   //·ÅµÚÒ»¸öÎ»ÖÃ
		 {
				 F1(Serial_hinght_erweima[next]);
			 	 PWM_DISABLE();		 
		 }
		 if((Pick_Up == 3)&&(GG == 50) && (next <= 3))   //×¥È¡
		 {
				 F2(Serial_hinght_erweima[next]);
			 	 PWM_DISABLE();		 
		 }
     if((Pick_Up == 21)&&(GG == 50)&&(next >= 4))   //·ÅµÚÒ»¸öÎ»ÖÃ
		 {
				 F1(Serial_hinght_erweima[next]);
			 	 PWM_DISABLE();		 
		 }
		 if((Pick_Up == 31)&&(GG == 50) && (next >= 4) && (next <= 6))   //×¥È¡
		 {
				 F2(Serial_hinght_erweima[next]);
			 	 PWM_DISABLE();		 
		 }
		 if((Pick_Up == 61)&&(GG == 50)&&(next >= 4) && (next <= 6))   //Âë¶â
		 {
				 F3(Serial_hinght_erweima[next]);
			 	 PWM_DISABLE();
		 }
	}
}
void TIM6_IRQHandler(void)         //¶¨Ê±Æ÷6£¬´¦Àí±êÖ¾Î»ÏìÓ¦Âß¼­
{
	if (TIM_GetITStatus(TIM6, TIM_IT_Update) == SET)  //»ñÈ¡ÖÐ¶Ï±êÖ¾Î»
	{ 
		 cnt++;
		 aa++;		
		 cot++;
     cxt++;
	 	 crt++;
		 if(hexToDec(Serial_RxPacket_2[18]) !=0)
		 {
		 	 Serial_hinght_erweima[8] = Serial_hinght_erweima[7];
		 	 Serial_hinght_erweima[7] = hexToDec(Serial_RxPacket_2[18]);
		 }
     if((Pick_Up == 1) && (GG == 50) && (Serial_hinght_erweima[8] != 0) && (Serial_hinght_erweima[8] != Serial_hinght_erweima[7]))
		 {
			 Serial_hinght_erweima[9] = hexToDec(Serial_RxPacket_2[18]);
			 GG = 69;
		 }
     if((Pick_Up == 121) && (GG == 50) && (Serial_hinght_erweima[8] != 0) && (Serial_hinght_erweima[8] != Serial_hinght_erweima[7]))
		 {
			 Serial_hinght_erweima[10] = hexToDec(Serial_RxPacket_2[18]);
			 GG = 69;
		 }

//		 xiansgu1();    //·Ö¶Î½ÃÕý  242 342
//		 xiansgu2();
//		 xiansgu3();
//		 kk_adjust();
		 sadsadsad_kk();
		 yaw=(float)stcAngle.Angle[2]/32768*180;
     T00 =  Serial_hinght_erweima[next];
					x1 = T00-1;
					x2 = T00+2;
					x3 = T00+5;
					w1 = T00+8;
					w2 = T00+11;
					w3 = T00+14;       
//					if(T00 == 1)
//					{
//						Servo3_SetAngle(3);   //123
//					}
//					if(T00 == 20)
//					{
//						Servo3_SetAngle(81);  //2
//					}
//					if(T00 == 3)
//					{
//						Servo3_SetAngle(164);   //3
//					}
					if(((x11+x22+x33+w11+w22+w33) == 12) && (YUY == 76))
					{
//						hh++;
//						if(hh<=1)
//						{
//							Pick_Up = 123;
//						}
						Serial_hinght_erweima[1] = hexToDec(Serial_RxPacket_3[0]);  //1
						Serial_hinght_erweima[2] = hexToDec(Serial_RxPacket_3[1]);  //2
					  Serial_hinght_erweima[3] = hexToDec(Serial_RxPacket_3[2]);  //3
						Serial_hinght_erweima[4] = hexToDec(Serial_RxPacket_3[3]);  //3
						Serial_hinght_erweima[5] = hexToDec(Serial_RxPacket_3[4]);  //2
						Serial_hinght_erweima[6] = hexToDec(Serial_RxPacket_3[5]);  //1
						GYD = 21;
						Serial_Printf5("page0.t1.txt=\"%d\"\xff\xff\xff",x11);
						Serial_Printf5("page0.t2.txt=\"%d\"\xff\xff\xff",x22);
						Serial_Printf5("page0.t3.txt=\"%d\"\xff\xff\xff",x33);
						Serial_Printf5("page0.t4.txt=\"%d\"\xff\xff\xff",w11);
						Serial_Printf5("page0.t5.txt=\"%d\"\xff\xff\xff",w22);
						Serial_Printf5("page0.t6.txt=\"%d\"\xff\xff\xff",w33);
						USART_Cmd(UART5, DISABLE);
						YUY = 13;
				  } 
					if(GG==71 && GYD == 19)   //GG = 71ÊÇÆô¶¯±êÖ¾Serial_hinght_xy
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
					 
						Serial_hinght_xy[0] = Serial_RxPacket_2[x1];  //°Ù
						Serial_hinght_xy[1] = Serial_RxPacket_2[x2];  //Ê®
						Serial_hinght_xy[2] = Serial_RxPacket_2[x3];  //¸ö
						Serial_hinght_xy[3] = Serial_RxPacket_2[w1];  //°Ù
						Serial_hinght_xy[4] = Serial_RxPacket_2[w2];  //Ê®
						Serial_hinght_xy[5] = Serial_RxPacket_2[w3];  //¸ö					
						cnt = 2500;			 
				  }	

					if(next > 3 && JIXIEBI == 14)   //µÚÒ»´Î·ÅÖÃ
					{
						SPEET_BUSHU_STOP = 2000;
						GG = 61;
						next = 1;
					}
					if(next > 6 && JIXIEBI == 114)   //µÚÒ»´Î·ÅÖÃ
					{
						SPEET_BUSHU_STOP = 12000;
						GG = 61;
						next = 4;
					}
					if(next > 3 && JIXIEBI == 15)   //µÚÒ»´Î·ÅÖÃÔÚ×¥Æð
					{
						SPEET_BUSHU_STOP = 3000;
						GG = 61;
						next = 1;
					}
					if(next > 6 && JIXIEBI == 115)   //µÚÒ»´Î·ÅÖÃÔÚ×¥Æð
					{
						SPEET_BUSHU_STOP = 13000;
						GG = 61;
						next = 4;
					}
					if(next > 3 && JIXIEBI == 19)   //µÚÒ»´Î·ÅÖÃÔÚ×¥Æð
					{
						SPEET_BUSHU_STOP = 4000;
					  GG = 61;
						next = 4;
					}
					if(next > 6 && JIXIEBI == 119)   //µÚÒ»´Î·ÅÖÃÔÚ×¥Æð
					{
						SPEET_BUSHU_STOP = 14000;
					  GG = 61;
						next = 7;
					}
          if(next > 3 && JIXIEBI == 12)   //µÚÒ»¶Î±êÖ¾Î» ×¥È¡
					{						
						SPEET_BUSHU_STOP = 1000;
			      GG = 61;
						next = 1;
					}
          if(next > 6 && JIXIEBI == 112)   //µÚÒ»¶Î±êÖ¾Î» ×¥È¡
					{						
						SPEET_BUSHU_STOP = 11000;
			      GG = 61;
						next = 4;
					}
					if((JIXIEBI == 11) && (Pick_Up == 123))
					{
						SPEET_BUSHU_STOP = 9000;
						GG = 61;
						Pick_Up = 996;
					}
					if(GG == 78)
					{
							if(next == 1 && JIXIEBI == 12)   //µÚÒ»´Î×¥×ªÅÌÎïÁÏÐèÒª½ÃÕý
							{
									GG = 71;
							}							
							if(next == 4 && JIXIEBI == 112)   //µÚÒ»´Î×¥×ªÅÌÎïÁÏÐèÒª½ÃÕý
							{
									GG = 71;
							}
			  	}
					if(GG == 38)  //Ç°Ð¡  200
					{				 
						if(((hexToDec(Serial_RxPacket_2[w1])*100)+(hexToDec(Serial_RxPacket_2[w2])*10)+(hexToDec(Serial_RxPacket_2[w3])))>=257 && ((hexToDec(Serial_RxPacket_2[w1])*100)+(hexToDec(Serial_RxPacket_2[w2])*10)+(hexToDec(Serial_RxPacket_2[w3])))<=600)  //ºó
						{		
							F_a = 2;
							F_b = 2;
							F_c = 1;
							F_d = 1;	
							Motor_conter(yaw,F_Yaw,F_Ybw,F_a,F_b,F_c,F_d,F_e);
							PWM_ENABLE();							
              MOROR_MOVE_MAX(-200*1,200,200);
						}
						if(((hexToDec(Serial_RxPacket_2[w1])*100)+(hexToDec(Serial_RxPacket_2[w2])*10)+(hexToDec(Serial_RxPacket_2[w3])))<=253 && ((hexToDec(Serial_RxPacket_2[w1])*100)+(hexToDec(Serial_RxPacket_2[w2])*10)+(hexToDec(Serial_RxPacket_2[w3])))>=10)  //ºó
						{
              F_a = 1;
							F_b = 1;  //×ó
							F_c = 2;
							F_d = 2;	
							Motor_conter(yaw,F_Yaw,F_Ybw,F_a,F_b,F_c,F_d,F_e);
              PWM_ENABLE();							
              MOROR_MOVE_MAX(-200*1,200,200);							
						}
						if(((hexToDec(Serial_RxPacket_2[w1])*100)+(hexToDec(Serial_RxPacket_2[w2])*10)+(hexToDec(Serial_RxPacket_2[w3])))==0)  //ºó
						{
										PWM_DISABLE();
						}	
						if(((hexToDec(Serial_RxPacket_2[w1])*100)+(hexToDec(Serial_RxPacket_2[w2])*10)+(hexToDec(Serial_RxPacket_2[w3])) > 253) && (GG == 38) && ((hexToDec(Serial_RxPacket_2[w1])*100)+(hexToDec(Serial_RxPacket_2[w2])*10)+(hexToDec(Serial_RxPacket_2[w3])) < 257))  //ºó
						{
										PWM_DISABLE();
							      Serial_hinght_erweima[8] = 0;
										Serial_hinght_erweima[7] = 0;
										GG = 50;
						}	
					}
					if(GG == 55)    //×óÐ¡ 368
					{
						if((((hexToDec(Serial_RxPacket_2[x1])*100)+(hexToDec(Serial_RxPacket_2[x2])*10)+(hexToDec(Serial_RxPacket_2[x3]))) >=392) && (((hexToDec(Serial_RxPacket_2[x1])*100)+(hexToDec(Serial_RxPacket_2[x2])*10)+(hexToDec(Serial_RxPacket_2[x3]))) <= 600))//zuo
						{		
							F_a = 2;   //ºó
							F_b = 1;
							F_c = 1;
							F_d = 2;		
							Motor_conter(yaw,F_Yaw,F_Ybw,F_a,F_b,F_c,F_d,F_e);
              MOROR_MOVE_MAX(-200*1,200,200);								
              PWM_ENABLE();							
						}
						if((((hexToDec(Serial_RxPacket_2[x1])*100)+(hexToDec(Serial_RxPacket_2[x2])*10)+(hexToDec(Serial_RxPacket_2[x3]))) <=388) && (((hexToDec(Serial_RxPacket_2[x1])*100)+(hexToDec(Serial_RxPacket_2[x2])*10)+(hexToDec(Serial_RxPacket_2[x3]))) >= 10))  //you
						{	
							F_a = 1;
							F_b = 2;
							F_c = 2;
							F_d = 1;	
							Motor_conter(yaw,F_Yaw,F_Ybw,F_a,F_b,F_c,F_d,F_e);
              MOROR_MOVE_MAX(-200*1,200,200);             							
						  PWM_ENABLE();
						}
						if(((hexToDec(Serial_RxPacket_2[x1])*100)+(hexToDec(Serial_RxPacket_2[x2])*10)+(hexToDec(Serial_RxPacket_2[x3])))==0)  //ºó
						{
										PWM_DISABLE();
						}	
						if((((hexToDec(Serial_RxPacket_2[x1])*100)+(hexToDec(Serial_RxPacket_2[x2])*10)+(hexToDec(Serial_RxPacket_2[x3]))) >388) && (GG == 55) && ((hexToDec(Serial_RxPacket_2[x1])*100)+(hexToDec(Serial_RxPacket_2[x2])*10)+(hexToDec(Serial_RxPacket_2[x3]))) <392)  //ºó
						{
										PWM_DISABLE();
										GG = 38;
						}				
				}
		TIM_ClearITPendingBit(TIM6, TIM_IT_Update);  //Çå³ýÖÐ¶Ï±êÖ¾Î»
	}
}

void TIM7_IRQHandler(void)         //¶¨•rÊ±Æ÷7£¬´¦Àí»ù±¾·½ÏòÂß¼­
{
	
	if (TIM_GetITStatus(TIM7, TIM_IT_Update) == SET)  //»ñÈ¡ÖÐ¶Ï±êÖ¾Î»
	{
		Motor_conter(yaw,F_Yaw,F_Ybw,F_a,F_b,F_c,F_d,F_e);
		if(GG==61)
		{		
				if(SPEET_BUSHU_STOP == 0)   //³ö·¢²à×Å×ß
				{
					F_Yaw = 1;
					F_Ybw = -1;
					F_a = 1;   
					F_b = 1;
					F_c = 2;
					F_d = 2;
					Pick_Up = 0;
					PWM_ENABLE();
					MOROR_MOVE_MAX(-1*1,100,100);
				}
				if(SPEET_BUSHU_STOP == 1)   //³ö·¢²à×Å×ß
				{
					F_Yaw = 1;
					F_Ybw = -1;
					F_a = 1;   
					F_b = 1;
					F_c = 2;
					F_d = 2;
					Pick_Up = 0;
					PWM_ENABLE();
					MOROR_MOVE_MAX(-1*1,100,100);
				}
				if(SPEET_BUSHU_STOP == 2)   //³¯Ç°×ß
				{
					F_Yaw = 1;
					F_Ybw = -1;
					F_a = 2;
					F_b = 1;
					F_c = 1;
					F_d = 2;
					PWM_ENABLE();
					MOROR_MOVE_MAX(-1*1,400,400);				
				}
				if(SPEET_BUSHU_STOP == 3)  //³¯Ç°×ß
				{
					F_Yaw = 200;
					F_Ybw = -200;
					F_a = 2;
					F_b = 1;
					F_c = 1;
					F_d = 2;
					PWM_ENABLE();
					if(JIXIEBI == 0)
					{
							JIXIEBI = 11;
						  GG = 77;
					} 
					PWM_DISABLE();
				}
				
				if(SPEET_BUSHU_STOP == 9000)   //³¯Ç°×ß
				{
					F_Yaw = 1;
					F_Ybw = -1;
					F_a = 2;
					F_b = 1;
					F_c = 1;
					F_d = 2;
					PWM_ENABLE();
					MOROR_MOVE_MAX(-1*1,400,400);				
				}
				if(SPEET_BUSHU_STOP == 9001)   //³¯nei°×ß
				{
					F_Yaw = 1;
					F_Ybw = -1;
					F_a = 2;
					F_b = 2;
					F_c = 1;
					F_d = 1;
					PWM_ENABLE();
					MOROR_MOVE_MAX(-1*1,100,100);				
				}

				if(SPEET_BUSHU_STOP == 9002)   //×¥
				{
					if(JIXIEBI == 11)
					{
						  PWM_DISABLE();
						  Pick_Up = 1;
							JIXIEBI = 12;
					}         
					F_Yaw = 200;
					F_Ybw = -200;					
					GG = 78;
				}
				if(SPEET_BUSHU_STOP == 1000)  //ºóÍË
				{ 					
					F_Yaw = 1;
					F_Ybw = -1;
					F_a = 1;
					F_b = 1;
					F_c = 2;
					F_d = 2;	
          MOROR_MOVE_MAX(-1*1,200,200);
          PWM_ENABLE();					
				}
				if(SPEET_BUSHU_STOP == 1001)  //ºó³·²½
				{ 					
					F_Yaw = 1;
					F_Ybw = -1;
					F_a = 1;
					F_b = 1;
					F_c = 2;
					F_d = 2;	
          MOROR_MOVE_MAX(-100*5,400,200);
          PWM_ENABLE();					
				}
				if(SPEET_BUSHU_STOP == 1002)  //ºó³·²½
				{		
					F_Yaw = 1;
					F_Ybw = -1;
					F_a = 1;
					F_b = 2;
					F_c = 2;
					F_d = 1;	
          MOROR_MOVE_MAX(-1900*4,400,400);	
          PWM_ENABLE();					
				}
				if(SPEET_BUSHU_STOP == 1003)  //×ªÏò
				{
					F_Yaw = 200;
					F_Ybw = -200;
					F_a = 1;
					F_b = 1;
					F_c = 1;
					F_d = 1;
          MOROR_MOVE_MAX(-1500*4,200,200);
				}
				if(SPEET_BUSHU_STOP == 1004)  //µ½¼Ó¹¤Çø
				{
					F_Yaw = 89;
					F_Ybw = 88;
					F_a = 2;
					F_b = 1;
					F_c = 1;
					F_d = 2;
					PWM_ENABLE();
          MOROR_MOVE_MAX(-1*1,70,70);
				}
				if(SPEET_BUSHU_STOP == 1005)  //µ½¼Ó¹¤Çø
				{
					F_Yaw = 89;
					F_Ybw = 88;
					F_a = 2;
					F_b = 1;
					F_c = 1;
					F_d = 2;
          Motor_conter(yaw,F_Yaw,F_Ybw,F_a,F_b,F_c,F_d,F_e);
          MOROR_MOVE_MAX(-1550*19,400,400);
					PWM_ENABLE();
					if(JIXIEBI == 12)
					{
						  Pick_Up = 161;
					    GG = 150;
						  JIXIEBI = 13;
					}       
				}
				if(SPEET_BUSHU_STOP == 1006)  //Ðý×ª×¼±¸¼Ó¹¤
				{
					LED1_ON();
					F_Yaw = 200;
					F_Ybw = -200;
					F_a = 1;
					F_b = 1;
					F_c = 1;
					F_d = 1;
					PWM_ENABLE();
          MOROR_MOVE_MAX(-1350*4,200,200);   //Ðý×ª
				}
				if(SPEET_BUSHU_STOP == 1007)  //·ÅÖÃ
				{
					F_e = 55;
//					F_Yaw = 180;
//					F_Ybw = 178;
					if(JIXIEBI == 13)
					{
							JIXIEBI = 14;                                            
			 		}
          if(GG == 61)
          {
          	 Pick_Up = 2;
          	 GG = 21;
          }
				}
				if(SPEET_BUSHU_STOP == 2000)  //×¥È¡
				{
					if(JIXIEBI == 14)
					{
							JIXIEBI = 15;
					}       
          if(GG == 61)
          {
             Pick_Up = 3;
          	 GG = 50;
          }					
				}
				if(SPEET_BUSHU_STOP == 3000)  //×ßµ½¹Õ½Ç
				{					
					F_e = 55;
					F_a = 1;    //ËÄ¸öÂÖ×Ó·½Ïò
					F_b = 1;
					F_c = 2;
					F_d = 2;
				  MOROR_MOVE_MAX(-800*1,200,200);   //1600Ò»È¦  £¬ Ê®È¦  £¬300 ËÙ¶È £¬350¼ÓËÙ¶È  £¬È«¸Ä³É200
					Motor_conter(yaw,F_Yaw,F_Ybw,F_a,F_b,F_c,F_d,F_e);
					PWM_ENABLE();
				}
				if(SPEET_BUSHU_STOP == 3001)  //×ßµ½¹Õ½Ç
				{					
					F_e = 55;
					F_a = 1;    //ËÄ¸öÂÖ×Ó·½Ïò
					F_b = 2;
					F_c = 2;
					F_d = 1;
				  MOROR_MOVE_MAX(-1500*10,400,400);   //1600Ò»È¦  £¬ Ê®È¦  £¬300 ËÙ¶È £¬350¼ÓËÙ¶È  £¬È«¸Ä³É200
					Motor_conter(yaw,F_Yaw,F_Ybw,F_a,F_b,F_c,F_d,F_e);
					if(JIXIEBI == 15)
					{
							JIXIEBI = 16;
					}       
					PWM_ENABLE();
				}
				if(SPEET_BUSHU_STOP == 3002)  //Ðý×ª×¼±¸¼Ó¹¤
				{
					F_e = 00;
					F_Yaw = 200;
					F_Ybw = -200;
					F_a = 2;
					F_b = 2;
					F_c = 2;
					F_d = 2;
          MOROR_MOVE_MAX(-1500*4,200,200);
					if(JIXIEBI == 16)
					{
							JIXIEBI = 17;
					}        
					PWM_ENABLE();					
				}
				if(SPEET_BUSHU_STOP == 3003)  //»º³å
				{
					F_Yaw = 91;
					F_Ybw = 89;
					F_a = 1;
					F_b = 2;
					F_c = 2;
					F_d = 1;
					PWM_ENABLE();
          MOROR_MOVE_MAX(-1*1,70,70);
				}
				if(SPEET_BUSHU_STOP == 3004)  //µ½¶ÑÁÏÇø
				{
					LED1_ON();
					F_Yaw = 91;
					F_Ybw = 89;
					F_a = 1;
					F_b = 2;
					F_c = 2;
					F_d = 1;
					PWM_ENABLE();
          MOROR_MOVE_MAX(-1100*9,400,400);
					if(JIXIEBI == 17)
					{
						  Pick_Up = 161;
					    GG = 150;
						  JIXIEBI = 18;
					}       
				}
				if(SPEET_BUSHU_STOP == 3005)  //·ÅÖÃ
				{
					F_Yaw = 91;
					F_Ybw = 89;
					F_a = 1;
					F_b = 2;
					F_c = 2;
					F_d = 1;
					if(JIXIEBI == 18)
					{
							JIXIEBI = 19;
			 		}
          if(GG == 61)
          {
          	 Pick_Up = 2;
          	 GG = 21;
          }
				}
				if(SPEET_BUSHU_STOP == 4000)  //ºóÍË
				{
					F_Yaw = 91;
					F_Ybw = 89;
					F_a = 1;
					F_b = 1;
					F_c = 2;
					F_d = 2;
					PWM_ENABLE();
          MOROR_MOVE_MAX(-800*1,70,70);
					if(JIXIEBI == 19)
					{
						  JIXIEBI = 20;
					}									
				}
				if(SPEET_BUSHU_STOP == 4001)  //×¼±¸È¥ÏÂÒ»¸ö¹Õ½ÇÕÒ×ªÅÌ
				{
					F_Yaw = 91;
					F_Ybw = 89;
					F_a = 1;
					F_b = 2;
					F_c = 2;
					F_d = 1;
          MOROR_MOVE_MAX(-1800*9,400,400);
					Serial_hinght_erweima[8] = 0;
					Serial_hinght_erweima[7] = 0;
					if(JIXIEBI == 20)
					{
						  Pick_Up = 0;
					    GG = 150;
							JIXIEBI = 21;
					}         
				}
				if(SPEET_BUSHU_STOP == 4002)  //Ðý×ª×¼±¸È¥×ªÅÌ
				{
					F_Yaw = 200;
					F_Ybw = -200;
					F_a = 2;
					F_b = 2;
					F_c = 2;
					F_d = 2;
          MOROR_MOVE_MAX(-1500*4,200,200);
					if(JIXIEBI == 21)
					{
							JIXIEBI = 22;
					}         
				}
				if(SPEET_BUSHU_STOP == 4003)   //»º³å
				{
					if(JIXIEBI == 22)
					{
							JIXIEBI = 111;
					}         					
					F_e = 00;
					F_Yaw = 3;
					F_Ybw = 2;
					F_a = 1;
					F_b = 2;
					F_c = 2;
					F_d = 1;						  
					MOROR_MOVE_MAX(-1*2,70,70);
				}
				if(SPEET_BUSHU_STOP == 4004)   //ÕÒ×ªÅÌ
				{
					F_Yaw = 3;
					F_Ybw = 2;
					F_a = 1;
					F_b = 2;
					F_c = 2;
					F_d = 1;				  
					MOROR_MOVE_MAX(-1300*4,400,400);	
				}			
				if(SPEET_BUSHU_STOP == 4005)   //×¥
				{
					if(JIXIEBI == 111)
					{
						  PWM_DISABLE();
						  Pick_Up = 121;
							JIXIEBI = 112;
					}         
					F_Yaw = 200;
					F_Ybw = -200;					
					GG = 78;
				}
				if(SPEET_BUSHU_STOP == 11000)  //ºó³·²½
				{ 					
					F_Yaw = 1;
					F_Ybw = -1;
					F_a = 1;
					F_b = 2;
					F_c = 2;
					F_d = 1;	
          MOROR_MOVE_MAX(-1*4,400,200);
          PWM_ENABLE();					
				}
				if(SPEET_BUSHU_STOP == 11001)  //ºó³·²½
				{		
					F_Yaw = 1;
					F_Ybw = -1;
					F_a = 1;
					F_b = 2;
					F_c = 2;
					F_d = 1;	
          MOROR_MOVE_MAX(-2040*4,400,400);	
          PWM_ENABLE();					
				}
				if(SPEET_BUSHU_STOP == 11002)  //×ªÏò
				{
					F_Yaw = 200;
					F_Ybw = -200;
					F_a = 1;
					F_b = 1;
					F_c = 1;
					F_d = 1;
          MOROR_MOVE_MAX(-1500*4,200,200);
				}
				if(SPEET_BUSHU_STOP == 11003)  //µ½¼Ó¹¤Çø
				{
					F_Yaw = 89;
					F_Ybw = 88;
					F_a = 2;
					F_b = 1;
					F_c = 1;
					F_d = 2;
					PWM_ENABLE();
          MOROR_MOVE_MAX(-800*4,70,70);
				}
				if(SPEET_BUSHU_STOP == 11004)  //µ½¼Ó¹¤Çø
				{
					F_Yaw = 89;
					F_Ybw = 88;
					F_a = 2;
					F_b = 1;
					F_c = 1;
					F_d = 2;
          Motor_conter(yaw,F_Yaw,F_Ybw,F_a,F_b,F_c,F_d,F_e);
          MOROR_MOVE_MAX(-1570*19,400,400);
					PWM_ENABLE();
					if(JIXIEBI == 112)
					{
						  Pick_Up = 161;
					    GG = 150;
						  JIXIEBI = 113;
					}       
				}
				if(SPEET_BUSHU_STOP == 11005)  //Ðý×ª×¼±¸¼Ó¹¤
				{
					LED1_ON();
					F_Yaw = 200;
					F_Ybw = -200;
					F_a = 1;
					F_b = 1;
					F_c = 1;
					F_d = 1;
					PWM_ENABLE();
          MOROR_MOVE_MAX(-1350*4,200,200);   //Ðý×ª
				}
				if(SPEET_BUSHU_STOP == 11006)  //·ÅÖÃ
				{
					F_e = 55;
//					F_Yaw = 180;
//					F_Ybw = 178;
					if(JIXIEBI == 113)
					{
							JIXIEBI = 114;                                            
			 		}
          if(GG == 61)
          {
          	 Pick_Up = 21;
          	 GG = 21;
          }
				}
				if(SPEET_BUSHU_STOP == 12000)  //×¥È¡
				{
					if(JIXIEBI == 114)
					{
							JIXIEBI = 115;
					}       
          if(GG == 61)
          {
             Pick_Up = 31;
          	 GG = 50;
          }					
				}
				if(SPEET_BUSHU_STOP == 13000)  //×ßµ½¹Õ½Ç
				{					
					F_e = 55;
					F_a = 1;    //ËÄ¸öÂÖ×Ó·½Ïò
					F_b = 1;
					F_c = 2;
					F_d = 2;
				  MOROR_MOVE_MAX(-800*1,200,200);   //1600Ò»È¦  £¬ Ê®È¦  £¬300 ËÙ¶È £¬350¼ÓËÙ¶È  £¬È«¸Ä³É200
					Motor_conter(yaw,F_Yaw,F_Ybw,F_a,F_b,F_c,F_d,F_e);
					PWM_ENABLE();
				}
				if(SPEET_BUSHU_STOP == 13001)  //×ßµ½¹Õ½Ç
				{					
					F_e = 55;
					F_a = 1;    //ËÄ¸öÂÖ×Ó·½Ïò
					F_b = 2;
					F_c = 2;
					F_d = 1;
				  MOROR_MOVE_MAX(-1500*10,400,400);   //1600Ò»È¦  £¬ Ê®È¦  £¬300 ËÙ¶È £¬350¼ÓËÙ¶È  £¬È«¸Ä³É200
					Motor_conter(yaw,F_Yaw,F_Ybw,F_a,F_b,F_c,F_d,F_e);
					if(JIXIEBI == 115)
					{
							JIXIEBI = 116;
					}       
					PWM_ENABLE();
				}
				if(SPEET_BUSHU_STOP == 13002)  //Ðý×ª×¼±¸¼Ó¹¤
				{
					F_e = 00;
					F_Yaw = 200;
					F_Ybw = -200;
					F_a = 2;
					F_b = 2;
					F_c = 2;
					F_d = 2;
          MOROR_MOVE_MAX(-1500*4,200,200);
					if(JIXIEBI == 116)
					{
							JIXIEBI = 117;
					}
					PWM_ENABLE();					
				}
				if(SPEET_BUSHU_STOP == 13003)  //»º³å
				{
					F_Yaw = 91;
					F_Ybw = 89;
					F_a = 1;
					F_b = 2;
					F_c = 2;
					F_d = 1;
					PWM_ENABLE();
          MOROR_MOVE_MAX(-1*1,70,70);
				}
				if(SPEET_BUSHU_STOP == 13004)  //µ½¶ÑÁÏÇø
				{
					LED1_ON();
					F_Yaw = 91;
					F_Ybw = 89;
					F_a = 1;
					F_b = 2;
					F_c = 2;
					F_d = 1;
					PWM_ENABLE();
          MOROR_MOVE_MAX(-1500*8,400,400);
					if(JIXIEBI == 117)
					{
						  Pick_Up = 161;
					    GG = 150;
						  JIXIEBI = 118;
					}       
				}
				if(SPEET_BUSHU_STOP == 13005)  //·ÅÖÃ
				{
					F_Yaw = 91;
					F_Ybw = 89;
					F_a = 1;
					F_b = 2;
					F_c = 2;
					F_d = 1;
					if(JIXIEBI == 118)
					{
							JIXIEBI = 119;
			 		}
          if(GG == 61)
          {
          	 Pick_Up = 61;
          	 GG = 21;
          }
				}
				if(SPEET_BUSHU_STOP == 14000)  //ºóÍË
				{
					F_Yaw = 91;
					F_Ybw = 89;
					F_a = 1;
					F_b = 1;
					F_c = 2;
					F_d = 2;
					PWM_ENABLE();
          MOROR_MOVE_MAX(-800*1,70,70);
					if(JIXIEBI == 119)
					{
						  JIXIEBI = 120;
					}									
				}
				if(SPEET_BUSHU_STOP == 14001)  //×¼±¸È¥ÏÂÒ»¸ö¹Õ½ÇÕÒ×ªÅÌ
				{
					F_Yaw = 91;
					F_Ybw = 89;
					F_a = 1;
					F_b = 2;
					F_c = 2;
					F_d = 1;
          MOROR_MOVE_MAX(-1900*9,400,400);
					if(JIXIEBI == 120)
					{
						  Pick_Up = 121;
					    GG = 150;
							JIXIEBI = 121;
					}         
				}
				if(SPEET_BUSHU_STOP == 14002)  //Ðý×ª×¼±¸È¥×ªÅÌ
				{
					F_Yaw = 200;
					F_Ybw = -200;
					F_a = 2;
					F_b = 2;
					F_c = 2;
					F_d = 2;
          MOROR_MOVE_MAX(-1500*4,200,200);
					if(JIXIEBI == 121)
					{
							JIXIEBI = 122;
					}         
				}
				if(SPEET_BUSHU_STOP == 14003)   //»º³å
				{
					F_e = 00;
					F_Yaw = 3;
					F_Ybw = 2;
					F_a = 1;
					F_b = 2;
					F_c = 2;
					F_d = 1;						  
					MOROR_MOVE_MAX(-1*2,70,70);
				}
				if(SPEET_BUSHU_STOP == 14004)   //ÕÒ×ªÅÌ
				{
					F_Yaw = 3;
					F_Ybw = 2;
					F_a = 1;
					F_b = 2;
					F_c = 2;
					F_d = 1;				  
					MOROR_MOVE_MAX(-1760*17,400,400);	
				}
				
				if(SPEET_BUSHU_STOP == 14005)   //»Ø¼Ò»º³å
				{
					F_Yaw = 1;
					F_Ybw = -1;
					F_a = 2;   
					F_b = 2;
					F_c = 1;
					F_d = 1;					
					MOROR_MOVE_MAX(-1*3,150,150);
					PWM_ENABLE();
				}
				if(SPEET_BUSHU_STOP == 14006)   //»Ø¼Ò
				{
					F_Yaw = 1;
					F_Ybw = -1;
					F_a = 2;   
					F_b = 2;
					F_c = 1;
					F_d = 1;					
					MOROR_MOVE_MAX(-1800*2,150,150);
					PWM_ENABLE();
				}

			}
		TIM_ClearITPendingBit(TIM7, TIM_IT_Update);  //Çå³ýÖÐ¶Ï±êÖ¾Î»
	}
}


