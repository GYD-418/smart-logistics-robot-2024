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
extern int Rct;
int Color_BaiFangShunXv = 0;
int Xjs_Cnt_1 = 0;
int Xjs_Cnt_2 = 0;
int SCENT_ZN = 0;
extern float F_a,F_b,F_c,F_d,F_Yaw,F_Ybw,F_e;
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
int rmt;
int Xjs_1_1;
int Xjs_2_2;
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
int MUB; //此参数为转向时候的标志位 出发时0 转90度加1
extern int Pick_Up;
int Xjs_1;
int Xjs_2;

int ShiJun_X[1000];
int SHiJUN_W[1000];
int JiaoZheng_X[1000];
int JiaoZheng_W[1000];

int SCENT_UN;
extern uint32_t set_speed;         // 最大速度设置 单位为0.1rad/sec
int ZBZ[2];
int WBW[2];
extern int ZBX[2];
extern int ZBW[2];
int XJC;
int Three_CnEn_WL;
int POCT_OUT_1;
int POCT_OUT_2;

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
	 Timer_Init_seven(100,700);
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
//	 ActionGroup0();
	 LED1_ON();
	 GG = 61;
	 Xjs_1 = 1;
	 Xjs_2 = 1;

   Pick_Up = 777;
   PWM_ENABLE();
	while (1)
	{	  
		 x11 =  hexToDec(Serial_RxPacket_3[0]);    //        串口三扫二维码 串口二发送x y color        cv xwc     mv c y 123321
		 x22 =  hexToDec(Serial_RxPacket_3[1]);    //				 2cv x w  3 c y 123321
		 x33 =  hexToDec(Serial_RxPacket_3[2]);    //     		 0x13 19 0x14  20 0x15 21
		 w11 =  hexToDec(Serial_RxPacket_3[3]);   //696
		 w22 =  hexToDec(Serial_RxPacket_3[4]);   //
		 w33 =  hexToDec(Serial_RxPacket_3[5]);   //
		
//	 	 OLED_ShowSignedNum(1,1,yaw,3);  //212
		 OLED_ShowSignedNum(1,5,((hexToDec(Serial_RxPacket_2[1])*100)+(hexToDec(Serial_RxPacket_2[4])*10)+(hexToDec(Serial_RxPacket_2[7]))),3);  //337     
		 OLED_ShowSignedNum(1,9,((hexToDec(Serial_RxPacket_2[10])*100)+(hexToDec(Serial_RxPacket_2[13])*10)+(hexToDec(Serial_RxPacket_2[16]))),3);  //326     
		 OLED_ShowSignedNum(2,10,yaw,5);  //
		 OLED_ShowSignedNum(4,1,JIXIEBI,3);  //118
		 OLED_ShowSignedNum(3,10,SPEET_BUSHU_STOP,5);  //13009
		 OLED_ShowSignedNum(1,1,XJC,3);  //0
		
//		 OLED_ShowSignedNum(3,5,Serial_hinght_erweima[10],3);		
		     OLED_ShowSignedNum(2,1,GG,3);   //150
	   		 OLED_ShowSignedNum(4,7,ShiJun_X[Xjs_1],4);   //1
	   		 OLED_ShowSignedNum(4,12,SCENT_UN,4);   //
//	   		 OLED_ShowSignedNum(3,7,GG,3);   //1
				 OLED_ShowSignedNum(3,1,Pick_Up,4);   //169

				 

				 if((Pick_Up == 1)&&(GG == 69) && (Color_BaiFangShunXv == 1))   //抓转盘物料 抓第一个，其他两个写死
				 {
					 
						 if(next <= 3)
						 {
								if(hexToDec(Serial_hinght_erweima[9]) == 1)   //红色在下
								{
										 PWM_DISABLE();
									   LED1_OFF();
										 if(next == 1)
										 {
													 if((Serial_hinght_erweima[next] == 1)&&(GG == 69)&&(next == 1))//圆盘转动前，红色在下摆放抓俩
													 {
															ActionGroup5();  //抓下 //1600
															ActionGroup31();  //1200
													 }
													 if((Serial_hinght_erweima[next] == 2)&&(GG == 69)&&(next == 1))//圆盘转动前，红色在下摆放抓俩
													 {
															ActionGroup6();  //抓左  2300
															ActionGroup32(); //1200
													 }
													 if((Serial_hinght_erweima[next] == 3)&&(GG == 69)&&(next == 1))//圆盘转动后，第三个抓的位置变成之前动作组位置加一
													 {
															ActionGroup7();  //抓右  //2800
															ActionGroup33(); //1500
													 }			
										 }	
										 if(next == 2)
										 {
													 if((Serial_hinght_erweima[next] == 1)&&(GG == 69)&&(next == 2))//圆盘转动前，红色在下摆放抓俩
													 {
															ActionGroup5();  //抓下 //1600
															ActionGroup31();  //1200
													 }
													 if((Serial_hinght_erweima[next] == 2)&&(GG == 69)&&(next == 2))//圆盘转动前，红色在下摆放抓俩
													 {
															ActionGroup6();  //抓左  2300
															ActionGroup32(); //1200
													 }
													 if((Serial_hinght_erweima[next] == 3)&&(GG == 69)&&(next == 2))//圆盘转动后，第三个抓的位置变成之前动作组位置加一
													 {
															ActionGroup7();  //抓右  //2800
															ActionGroup33(); //1500
													 }			
														  			
										 }	
										 if(next == 3)
										 {
											     Delay_ms(1300);
													 if((Serial_hinght_erweima[next] == 2)&&(GG == 69)&&(next == 3))//圆盘转动前，红色在下摆放抓俩
													 {
//														  Delay_ms(1300);
														 	ActionGroup7();  //2800
															ActionGroup32();  //1200
													 }
													 if((Serial_hinght_erweima[next] == 3)&&(GG == 69)&&(next == 3))//圆盘转动前，红色在下摆放抓俩
													 {
//														  Delay_ms(1300);
														  ActionGroup5(); //1600
															ActionGroup33();  //1200
													 }
													 if((Serial_hinght_erweima[next] == 1)&&(GG == 69)&&(next == 3))//圆盘转动后，第三个抓的位置变成之前动作组位置加一
													 {
//														 Delay_ms(1300);
														 ActionGroup6();  //2300
														 ActionGroup31();  //1500								
													 }			
										 }							 				 
								}
								if(hexToDec(Serial_hinght_erweima[9]) == 2)   //绿色在下
								{
										 PWM_DISABLE();
									   LED1_OFF();
										 if(next == 1)
										 {
													 if((Serial_hinght_erweima[next] == 2)&&(GG == 69)&&(next == 1))//圆盘转动前，红色在下摆放抓俩
													 {
															ActionGroup5();  //1600  
															ActionGroup32();  //1200
													 }
													 if((Serial_hinght_erweima[next] == 3)&&(GG == 69)&&(next == 1))//圆盘转动前，红色在下摆放抓俩
													 {
															ActionGroup6();  //2300
															ActionGroup33();  //1500
													 }
													 if((Serial_hinght_erweima[next] == 1)&&(GG == 69)&&(next == 1))//圆盘转动后，第三个抓的位置变成之前动作组位置加一
													 {
															ActionGroup7();  //2800
															ActionGroup31();   //1200
													 }			
										 }	
										 if(next == 2)
										 {
													 if((Serial_hinght_erweima[next] == 2)&&(GG == 69)&&(next == 2))//圆盘转动前，红色在下摆放抓俩
													 {
																ActionGroup5();  //1600  
																ActionGroup32();  //1200
														}
													 if((Serial_hinght_erweima[next] == 3)&&(GG == 69)&&(next == 2))//圆盘转动前，红色在下摆放抓俩
													 {
															  ActionGroup6();  //2300
																ActionGroup33();  //1500
														}
													 if((Serial_hinght_erweima[next] == 1)&&(GG == 69)&&(next == 2))//圆盘转动后，第三个抓的位置变成之前动作组位置加一
													 {
																ActionGroup7();  //2800
																ActionGroup31();   //1200
													 }																 			
										 }				 
										 if(next == 3)
										 {
											     Delay_ms(1300);
													 if((Serial_hinght_erweima[next] == 1)&&(GG == 69)&&(next == 3))//圆盘转动前，红色在下摆放抓俩
													 {
//														  Delay_ms(1300);
															ActionGroup5();  //抓下 //1600
															ActionGroup31();  //1200
													 }
													 if((Serial_hinght_erweima[next] == 2)&&(GG == 69)&&(next == 3))//圆盘转动前，红色在下摆放抓俩
													 {
//														  Delay_ms(1300);
															ActionGroup6();  //抓左  2300
															ActionGroup32(); //1200
													 }
													 if((Serial_hinght_erweima[next] == 3)&&(GG == 69)&&(next == 3))//圆盘转动后，第三个抓的位置变成之前动作组位置加一
													 {
//														  Delay_ms(1300);
															ActionGroup7();  //抓右  //2800
															ActionGroup33(); //1500
													 }			
										 }							 				 
								}
								if(hexToDec(Serial_hinght_erweima[9]) == 3)   //蓝色在下
								{
										 PWM_DISABLE();
									   LED1_OFF();
										 if(next == 1)
										 {
													 if((Serial_hinght_erweima[next] == 3)&&(GG == 69)&&(next == 1))//圆盘转动前，红色在下摆放抓俩
													 {
															ActionGroup5();  //1600
															ActionGroup33(); //1500
													 }
													 if((Serial_hinght_erweima[next] == 1)&&(GG == 69)&&(next == 1))//圆盘转动前，红色在下摆放抓俩
													 {
															ActionGroup6(); //2300
															ActionGroup31();  //1200
													 }
													 if((Serial_hinght_erweima[next] == 2)&&(GG == 69)&&(next == 1))//圆盘转动后，第三个抓的位置变成之前动作组位置加一
													 {
															ActionGroup7();  //2800
															ActionGroup32();  //1200
													 }			
										 }	
										 if(next == 2)
										 {
													 if((Serial_hinght_erweima[next] == 3)&&(GG == 69)&&(next == 2))//圆盘转动前，红色在下摆放抓俩
													 {
															ActionGroup5();  //1600
															ActionGroup33(); //1500
													 }
													 if((Serial_hinght_erweima[next] == 1)&&(GG == 69)&&(next == 2))//圆盘转动前，红色在下摆放抓俩
													 {
															ActionGroup6(); //2300
															ActionGroup31();  //1200
													 }
													 if((Serial_hinght_erweima[next] == 2)&&(GG == 69)&&(next == 2))//圆盘转动后，第三个抓的位置变成之前动作组位置加一
													 {
															ActionGroup7();  //2800
															ActionGroup32();  //1200
													 }			
										 }							 
										 if(next == 3)
										 {
											     Delay_ms(1300);
													 if((Serial_hinght_erweima[next] == 2)&&(GG == 69)&&(next == 3))//圆盘转动前，红色在下摆放抓俩
													 {
//														  Delay_ms(1300);
															ActionGroup5();  //1600  
															ActionGroup32();  //1200
													 }
													 if((Serial_hinght_erweima[next] == 3)&&(GG == 69)&&(next == 3))//圆盘转动前，红色在下摆放抓俩
													 {
//														  Delay_ms(1300);
															ActionGroup6();  //2300
															ActionGroup33();  //1500
													 }
													 if((Serial_hinght_erweima[next] == 1)&&(GG == 69)&&(next == 3))//圆盘转动后，第三个抓的位置变成之前动作组位置加一
													 {
//														  Delay_ms(1300);
															ActionGroup7();  //2800
															ActionGroup31();   //1200
													 }			
										 }
								 }										 
				     }
			   }
				 if((Pick_Up == 1)&&(GG == 69) && (Color_BaiFangShunXv == 2))   //抓转盘物料 抓第一个，其他两个写死
				 {
						 if(next <= 3)
						 {
								if(hexToDec(Serial_hinght_erweima[9]) == 1)   //红色在下
								{
										 PWM_DISABLE();
									   LED1_OFF();
										 if(next == 1)
										 {
													 if((Serial_hinght_erweima[next] == 1)&&(GG == 69)&&(next == 1))//圆盘转动前，红色在下摆放抓俩
													 {
															ActionGroup5();  //抓下 //1600
															ActionGroup31();  //1200
													 }
													 if((Serial_hinght_erweima[next] == 3)&&(GG == 69)&&(next == 1))//圆盘转动前，红色在下摆放抓俩
													 {
															ActionGroup6();  //抓左  2300
															ActionGroup33(); //1200
													 }
													 if((Serial_hinght_erweima[next] == 2)&&(GG == 69)&&(next == 1))//圆盘转动后，第三个抓的位置变成之前动作组位置加一
													 {
															ActionGroup7();  //抓右  //2800
															ActionGroup32(); //1500
													 }			
										 }	
										 if(next == 2)
										 {
													 if((Serial_hinght_erweima[next] == 1)&&(GG == 69)&&(next == 2))//圆盘转动前，红色在下摆放抓俩
													 {
															ActionGroup5();  //抓下 //1600
															ActionGroup31();  //1200
													 }
													 if((Serial_hinght_erweima[next] == 3)&&(GG == 69)&&(next == 2))//圆盘转动前，红色在下摆放抓俩
													 {
															ActionGroup6();  //抓左  2300
															ActionGroup33(); //1200
													 }
													 if((Serial_hinght_erweima[next] == 2)&&(GG == 69)&&(next == 2))//圆盘转动后，第三个抓的位置变成之前动作组位置加一
													 {
															ActionGroup7();  //抓右  //2800
															ActionGroup32(); //1500
													 }			
										 }	
										 if(next == 3)
										 {
											     Delay_ms(1300);
													 if((Serial_hinght_erweima[next] == 3)&&(GG == 69)&&(next == 3))//圆盘转动前，红色在下摆放抓俩
													 {
//														  Delay_ms(1300);
															ActionGroup7();  //2800
															ActionGroup33();  //1200													
													 }
													 if((Serial_hinght_erweima[next] == 2)&&(GG == 69)&&(next == 3))//圆盘转动前，红色在下摆放抓俩
													 {
//														  Delay_ms(1300);
														  ActionGroup5(); //1600
															ActionGroup32();  //1200
													 }
													 if((Serial_hinght_erweima[next] == 1)&&(GG == 69)&&(next == 3))//圆盘转动后，第三个抓的位置变成之前动作组位置加一
													 {
//														  Delay_ms(1300);
															ActionGroup6();  //2300
															ActionGroup31();  //1500
													 }			
										 }							 				 
								}
								if(hexToDec(Serial_hinght_erweima[9]) == 2)   //绿色在下
								{
										 PWM_DISABLE();
									   LED1_OFF();
										 if(next == 1)
										 {
													 if((Serial_hinght_erweima[next] == 3)&&(GG == 69)&&(next == 1))//圆盘转动前，红色在下摆放抓俩
													 {
															ActionGroup7();  //2800
															ActionGroup33();  //1200													
													 }
													 if((Serial_hinght_erweima[next] == 2)&&(GG == 69)&&(next == 1))//圆盘转动前，红色在下摆放抓俩
													 {
														  ActionGroup5(); //1600
															ActionGroup32();  //1200
													 }
													 if((Serial_hinght_erweima[next] == 1)&&(GG == 69)&&(next == 1))//圆盘转动后，第三个抓的位置变成之前动作组位置加一
													 {
															ActionGroup6();  //2300
															ActionGroup31();  //1500
													 }			
										 }	
										 if(next == 2)
										 {
													 if((Serial_hinght_erweima[next] == 3)&&(GG == 69)&&(next == 2))//圆盘转动前，红色在下摆放抓俩
													 {
															ActionGroup7();  //2800
															ActionGroup33();  //1200	
                             														 
													 }
													 if((Serial_hinght_erweima[next] == 2)&&(GG == 69)&&(next == 2))//圆盘转动前，红色在下摆放抓俩
													 {
														  ActionGroup5(); //1600
															ActionGroup32();  //1200
													   
													 }
													 if((Serial_hinght_erweima[next] == 1)&&(GG == 69)&&(next == 2))//圆盘转动后，第三个抓的位置变成之前动作组位置加一
													 {
															ActionGroup6();  //2300
															ActionGroup31();  //1500
													   
													 }						
										 }				 
										 if(next == 3)
										 {
											     Delay_ms(1300);
													 if((Serial_hinght_erweima[next] == 3)&&(GG == 69)&&(next == 3))//圆盘转动前，红色在下摆放抓俩
													 {
//														  Delay_ms(1300);
															ActionGroup5();  //1600
															ActionGroup33(); //1500
													 }
													 if((Serial_hinght_erweima[next] == 2)&&(GG == 69)&&(next == 3))//圆盘转动前，红色在下摆放抓俩
													 {
//														  Delay_ms(1300);
															ActionGroup6(); //2300
															ActionGroup32();  //1200
													 }
													 if((Serial_hinght_erweima[next] == 1)&&(GG == 69)&&(next == 3))//圆盘转动后，第三个抓的位置变成之前动作组位置加一
													 {
//														  Delay_ms(1300);
															ActionGroup7();  //2800
															ActionGroup31();  //1200
													 }			
										 }							 				 
								}
								if(hexToDec(Serial_hinght_erweima[9]) == 3)   //蓝色在下
								{
										 PWM_DISABLE();
									   LED1_OFF();
										 if(next == 1)
										 {
													 if((Serial_hinght_erweima[next] == 3)&&(GG == 69)&&(next == 1))//圆盘转动前，红色在下摆放抓俩
													 {
															ActionGroup5();  //1600
															ActionGroup33(); //1500
													 }
													 if((Serial_hinght_erweima[next] == 2)&&(GG == 69)&&(next == 1))//圆盘转动前，红色在下摆放抓俩
													 {
															ActionGroup6(); //2300
															ActionGroup32();  //1200
													 }
													 if((Serial_hinght_erweima[next] == 1)&&(GG == 69)&&(next == 1))//圆盘转动后，第三个抓的位置变成之前动作组位置加一
													 {
															ActionGroup7();  //2800
															ActionGroup31();  //1200
													 }			
										 }	
										 if(next == 2)
										 {
													 if((Serial_hinght_erweima[next] == 3)&&(GG == 69)&&(next == 2))//圆盘转动前，红色在下摆放抓俩
													 {
															ActionGroup5();  //1600  
															ActionGroup33();  //1200
													 }
													 if((Serial_hinght_erweima[next] == 2)&&(GG == 69)&&(next == 2))//圆盘转动前，红色在下摆放抓俩
													 {
															ActionGroup6();  //2300
															ActionGroup32();  //1500
													 }
													 if((Serial_hinght_erweima[next] == 1)&&(GG == 69)&&(next == 2))//圆盘转动后，第三个抓的位置变成之前动作组位置加一
													 {
															ActionGroup7();  //2800
															ActionGroup31();   //1200
													 }																 
										 }							 
										 if(next == 3)
										 {
											     Delay_ms(1300);
										       if((Serial_hinght_erweima[next] == 1)&&(GG == 69)&&(next == 3))//圆盘转动前，红色在下摆放抓俩
													 {
//														  Delay_ms(1300);
															ActionGroup5();  //抓下 //1600
															ActionGroup31();  //1200
													 }
													 if((Serial_hinght_erweima[next] == 3)&&(GG == 69)&&(next == 3))//圆盘转动前，红色在下摆放抓俩
													 {
//														  Delay_ms(1300);
															ActionGroup6();  //抓左  2300
															ActionGroup33(); //1200
													 }
													 if((Serial_hinght_erweima[next] == 2)&&(GG == 69)&&(next == 3))//圆盘转动后，第三个抓的位置变成之前动作组位置加一
													 {
//														  Delay_ms(1300);
															ActionGroup7();  //抓右  //2800
															ActionGroup32(); //1500
													 }	
										 }
								 }										 
				     }
			   }
				 
				 if((Pick_Up == 121)&&(GG == 69)&&(Color_BaiFangShunXv == 1))   //抓转盘物料 抓第一个，其他两个写死
				 {
						 if((next <= 6) && (next >= 4))
						 {
								if(hexToDec(Serial_hinght_erweima[10]) == 1)   //红色在下
								{
										 if(next == 4)
										 {
													 if((Serial_hinght_erweima[next] == 1)&&(GG == 69)&&(next == 4))//圆盘转动前，红色在下摆放抓俩
													 {
															ActionGroup5();  //抓下 //1600
															ActionGroup31();  //1200
													 }
													 if((Serial_hinght_erweima[next] == 2)&&(GG == 69)&&(next == 4))//圆盘转动前，红色在下摆放抓俩
													 {
															ActionGroup6();  //抓左  2300
															ActionGroup32(); //1200
													 }
													 if((Serial_hinght_erweima[next] == 3)&&(GG == 69)&&(next == 4))//圆盘转动后，第三个抓的位置变成之前动作组位置加一
													 {
															ActionGroup7();  //抓右  //2800
															ActionGroup33(); //1500
													 }			
										 }	
										 if(next == 5)
										 {
													 if((Serial_hinght_erweima[next] == 1)&&(GG == 69)&&(next == 5))//圆盘转动前，红色在下摆放抓俩
													 {
															ActionGroup5();  //抓下 //1600
															ActionGroup31();  //1200
													 }
													 if((Serial_hinght_erweima[next] == 2)&&(GG == 69)&&(next == 5))//圆盘转动前，红色在下摆放抓俩
													 {
															ActionGroup6();  //抓左  2300
															ActionGroup32(); //1200
													 }
													 if((Serial_hinght_erweima[next] == 3)&&(GG == 69)&&(next == 5))//圆盘转动后，第三个抓的位置变成之前动作组位置加一
													 {
															ActionGroup7();  //抓右  //2800
															ActionGroup33(); //1500
													 }						  			
										 }	
										 if(next == 6)
										 {
											     Delay_ms(1300);
													 if((Serial_hinght_erweima[next] == 2)&&(GG == 69)&&(next == 6))//圆盘转动前，红色在下摆放抓俩
													 {
//														  Delay_ms(1300);
														 	ActionGroup7();  //2800
															ActionGroup32();  //1200
													 }
													 if((Serial_hinght_erweima[next] == 3)&&(GG == 69)&&(next == 6))//圆盘转动前，红色在下摆放抓俩
													 {
//														  Delay_ms(1300);
														  ActionGroup5(); //1600
															ActionGroup33();  //1200
													 }
													 if((Serial_hinght_erweima[next] == 1)&&(GG == 69)&&(next == 6))//圆盘转动后，第三个抓的位置变成之前动作组位置加一
													 {
//														 Delay_ms(1300);
														 ActionGroup6();  //2300
														 ActionGroup31();  //1500								
													 }			
										 }							 				 
								}
								if(hexToDec(Serial_hinght_erweima[10]) == 2)   //绿色在下
								{
										 PWM_DISABLE();
									   LED1_OFF();
										 if(next == 4)
										 {
													 if((Serial_hinght_erweima[next] == 2)&&(GG == 69)&&(next == 4))//圆盘转动前，红色在下摆放抓俩
													 {
															ActionGroup5();  //1600  
															ActionGroup32();  //1200
													 }
													 if((Serial_hinght_erweima[next] == 3)&&(GG == 69)&&(next == 4))//圆盘转动前，红色在下摆放抓俩
													 {
															ActionGroup6();  //2300
															ActionGroup33();  //1500
													 }
													 if((Serial_hinght_erweima[next] == 1)&&(GG == 69)&&(next == 4))//圆盘转动后，第三个抓的位置变成之前动作组位置加一
													 {
															ActionGroup7();  //2800
															ActionGroup31();   //1200
													 }			
										 }	
										 if(next == 5)
										 {
													 if((Serial_hinght_erweima[next] == 2)&&(GG == 69)&&(next == 5))//圆盘转动前，红色在下摆放抓俩
													 {
																ActionGroup5();  //1600  
																ActionGroup32();  //1200
														}
													 if((Serial_hinght_erweima[next] == 3)&&(GG == 69)&&(next == 5))//圆盘转动前，红色在下摆放抓俩
													 {
															  ActionGroup6();  //2300
																ActionGroup33();  //1500
														}
													 if((Serial_hinght_erweima[next] == 1)&&(GG == 69)&&(next == 5))//圆盘转动后，第三个抓的位置变成之前动作组位置加一
													 {
																ActionGroup7();  //2800
																ActionGroup31();   //1200
													 }																 			
										 }				 
										 if(next == 6)
										 {
											     Delay_ms(1300);
													 if((Serial_hinght_erweima[next] == 1)&&(GG == 69)&&(next == 6))//圆盘转动前，红色在下摆放抓俩
													 {
//														  Delay_ms(1300);
															ActionGroup5();  //抓下 //1600
															ActionGroup31();  //1200
													 }
													 if((Serial_hinght_erweima[next] == 2)&&(GG == 69)&&(next == 6))//圆盘转动前，红色在下摆放抓俩
													 {
//														  Delay_ms(1300);
															ActionGroup6();  //抓左  2300
															ActionGroup32(); //1200
													 }
													 if((Serial_hinght_erweima[next] == 3)&&(GG == 69)&&(next == 6))//圆盘转动后，第三个抓的位置变成之前动作组位置加一
													 {
//														  Delay_ms(1300);
															ActionGroup7();  //抓右  //2800
															ActionGroup33(); //1500
													 }			
										 }							 				 
								}
								if(hexToDec(Serial_hinght_erweima[10]) == 3)   //蓝色在下
								{
										 PWM_DISABLE();
									   LED1_OFF();
										 if(next == 4)
										 {
													 if((Serial_hinght_erweima[next] == 3)&&(GG == 69)&&(next == 4))//圆盘转动前，红色在下摆放抓俩
													 {
															ActionGroup5();  //1600
															ActionGroup33(); //1500
													 }
													 if((Serial_hinght_erweima[next] == 1)&&(GG == 69)&&(next == 4))//圆盘转动前，红色在下摆放抓俩
													 {
															ActionGroup6(); //2300
															ActionGroup31();  //1200
													 }
													 if((Serial_hinght_erweima[next] == 2)&&(GG == 69)&&(next == 4))//圆盘转动后，第三个抓的位置变成之前动作组位置加一
													 {
															ActionGroup7();  //2800
															ActionGroup32();  //1200
													 }			
										 }	
										 if(next == 5)
										 {
													 if((Serial_hinght_erweima[next] == 3)&&(GG == 69)&&(next == 5))//圆盘转动前，红色在下摆放抓俩
													 {
															ActionGroup5();  //1600
															ActionGroup33(); //1500
													 }
													 if((Serial_hinght_erweima[next] == 1)&&(GG == 69)&&(next == 5))//圆盘转动前，红色在下摆放抓俩
													 {
															ActionGroup6(); //2300
															ActionGroup31();  //1200
													 }
													 if((Serial_hinght_erweima[next] == 2)&&(GG == 69)&&(next == 5))//圆盘转动后，第三个抓的位置变成之前动作组位置加一
													 {
															ActionGroup7();  //2800
															ActionGroup32();  //1200
													 }			
										 }							 
										 if(next == 6)
										 {
											     Delay_ms(1300);
													 if((Serial_hinght_erweima[next] == 2)&&(GG == 69)&&(next == 6))//圆盘转动前，红色在下摆放抓俩
													 {
//														  Delay_ms(1300);
															ActionGroup5();  //1600  
															ActionGroup32();  //1200
													 }
													 if((Serial_hinght_erweima[next] == 3)&&(GG == 69)&&(next == 6))//圆盘转动前，红色在下摆放抓俩
													 {
//														  Delay_ms(1300);
															ActionGroup6();  //2300
															ActionGroup33();  //1500
													 }
													 if((Serial_hinght_erweima[next] == 1)&&(GG == 69)&&(next == 6))//圆盘转动后，第三个抓的位置变成之前动作组位置加一
													 {
//														  Delay_ms(1300);
															ActionGroup7();  //2800
															ActionGroup31();   //1200
													 }			
										 }
								 }										 
				     }
			   }
				 if((Pick_Up == 121)&&(GG == 69)&&(Color_BaiFangShunXv == 2))   //抓转盘物料 抓第一个，其他两个写死
				 {
						 if((next <= 6) && (next >= 4))
						 {
								if(hexToDec(Serial_hinght_erweima[10]) == 1)   //红色在下
								{
										 PWM_DISABLE();
									   LED1_OFF();
										 if(next == 4)
										 {
													 if((Serial_hinght_erweima[next] == 1)&&(GG == 69)&&(next == 4))//圆盘转动前，红色在下摆放抓俩
													 {
															ActionGroup5();  //抓下 //1600
															ActionGroup31();  //1200
													 }
													 if((Serial_hinght_erweima[next] == 3)&&(GG == 69)&&(next == 4))//圆盘转动前，红色在下摆放抓俩
													 {
															ActionGroup6();  //抓左  2300
															ActionGroup33(); //1200
													 }
													 if((Serial_hinght_erweima[next] == 2)&&(GG == 69)&&(next == 4))//圆盘转动后，第三个抓的位置变成之前动作组位置加一
													 {
															ActionGroup7();  //抓右  //2800
															ActionGroup32(); //1500
													 }			
										 }	
										 if(next == 5)
										 {
													 if((Serial_hinght_erweima[next] == 1)&&(GG == 69)&&(next == 5))//圆盘转动前，红色在下摆放抓俩
													 {
															ActionGroup5();  //抓下 //1600
															ActionGroup31();  //1200
													 }
													 if((Serial_hinght_erweima[next] == 3)&&(GG == 69)&&(next == 5))//圆盘转动前，红色在下摆放抓俩
													 {
															ActionGroup6();  //抓左  2300
															ActionGroup33(); //1200
													 }
													 if((Serial_hinght_erweima[next] == 2)&&(GG == 69)&&(next == 5))//圆盘转动后，第三个抓的位置变成之前动作组位置加一
													 {
															ActionGroup7();  //抓右  //2800
															ActionGroup32(); //1500
													 }			
										 }	
										 if(next == 6)
										 {
											     Delay_ms(1300);
													 if((Serial_hinght_erweima[next] == 3)&&(GG == 69)&&(next == 6))//圆盘转动前，红色在下摆放抓俩
													 {
//														  Delay_ms(1300);
															ActionGroup7();  //2800
															ActionGroup33();  //1200													
													 }
													 if((Serial_hinght_erweima[next] == 2)&&(GG == 69)&&(next == 6))//圆盘转动前，红色在下摆放抓俩
													 {
//														  Delay_ms(1300);
														  ActionGroup5(); //1600
															ActionGroup32();  //1200
													 }
													 if((Serial_hinght_erweima[next] == 1)&&(GG == 69)&&(next == 6))//圆盘转动后，第三个抓的位置变成之前动作组位置加一
													 {
//														  Delay_ms(1300);
															ActionGroup6();  //2300
															ActionGroup31();  //1500
													 }			
										 }							 				 
								}
								if(hexToDec(Serial_hinght_erweima[10]) == 2)   //绿色在下
								{
										 PWM_DISABLE();
									   LED1_OFF();
										 if(next == 4)
										 {
													 if((Serial_hinght_erweima[next] == 3)&&(GG == 69)&&(next == 4))//圆盘转动前，红色在下摆放抓俩
													 {
															ActionGroup7();  //2800
															ActionGroup33();  //1200													
													 }
													 if((Serial_hinght_erweima[next] == 2)&&(GG == 69)&&(next == 4))//圆盘转动前，红色在下摆放抓俩
													 {
														  ActionGroup5(); //1600
															ActionGroup32();  //1200
													 }
													 if((Serial_hinght_erweima[next] == 1)&&(GG == 69)&&(next == 4))//圆盘转动后，第三个抓的位置变成之前动作组位置加一
													 {
															ActionGroup6();  //2300
															ActionGroup31();  //1500
													 }			
										 }	
										 if(next == 5)
										 {
													 if((Serial_hinght_erweima[next] == 3)&&(GG == 69)&&(next == 5))//圆盘转动前，红色在下摆放抓俩
													 {
															ActionGroup7();  //2800
															ActionGroup33();  //1200	
                             														 
													 }
													 if((Serial_hinght_erweima[next] == 2)&&(GG == 69)&&(next == 5))//圆盘转动前，红色在下摆放抓俩
													 {
														  ActionGroup5(); //1600
															ActionGroup32();  //1200
													   
													 }
													 if((Serial_hinght_erweima[next] == 1)&&(GG == 69)&&(next == 5))//圆盘转动后，第三个抓的位置变成之前动作组位置加一
													 {
															ActionGroup6();  //2300
															ActionGroup31();  //1500
													   
													 }						
										 }				 
										 if(next == 6)
										 {
											     Delay_ms(1300);
													 if((Serial_hinght_erweima[next] == 3)&&(GG == 69)&&(next == 6))//圆盘转动前，红色在下摆放抓俩
													 {
//														  Delay_ms(1300);
															ActionGroup5();  //1600
															ActionGroup33(); //1500
													 }
													 if((Serial_hinght_erweima[next] == 2)&&(GG == 69)&&(next == 6))//圆盘转动前，红色在下摆放抓俩
													 {
//														  Delay_ms(1300);
															ActionGroup6(); //2300
															ActionGroup32();  //1200
													 }
													 if((Serial_hinght_erweima[next] == 1)&&(GG == 69)&&(next == 6))//圆盘转动后，第三个抓的位置变成之前动作组位置加一
													 {
//														  Delay_ms(1300);
															ActionGroup7();  //2800
															ActionGroup31();  //1200
													 }			
										 }							 				 
								}
								if(hexToDec(Serial_hinght_erweima[10]) == 3)   //蓝色在下
								{
										 PWM_DISABLE();
									   LED1_OFF();
										 if(next == 4)
										 {
													 if((Serial_hinght_erweima[next] == 3)&&(GG == 69)&&(next == 4))//圆盘转动前，红色在下摆放抓俩
													 {
															ActionGroup5();  //1600
															ActionGroup33(); //1500
													 }
													 if((Serial_hinght_erweima[next] == 2)&&(GG == 69)&&(next == 4))//圆盘转动前，红色在下摆放抓俩
													 {
															ActionGroup6(); //2300
															ActionGroup32();  //1200
													 }
													 if((Serial_hinght_erweima[next] == 1)&&(GG == 69)&&(next == 4))//圆盘转动后，第三个抓的位置变成之前动作组位置加一
													 {
															ActionGroup7();  //2800
															ActionGroup31();  //1200
													 }			
										 }	
										 if(next == 5)
										 {
													 if((Serial_hinght_erweima[next] == 3)&&(GG == 69)&&(next == 5))//圆盘转动前，红色在下摆放抓俩
													 {
															ActionGroup5();  //1600  
															ActionGroup33();  //1200
													 }
													 if((Serial_hinght_erweima[next] == 2)&&(GG == 69)&&(next == 5))//圆盘转动前，红色在下摆放抓俩
													 {
															ActionGroup6();  //2300
															ActionGroup32();  //1500
													 }
													 if((Serial_hinght_erweima[next] == 1)&&(GG == 69)&&(next == 5))//圆盘转动后，第三个抓的位置变成之前动作组位置加一
													 {
															ActionGroup7();  //2800
															ActionGroup31();   //1200
													 }																 
										 }							 
										 if(next == 6)
										 {
											     Delay_ms(1300);
										       if((Serial_hinght_erweima[next] == 1)&&(GG == 69)&&(next == 6))//圆盘转动前，红色在下摆放抓俩
													 {
//														  Delay_ms(1300);
															ActionGroup5();  //抓下 //1600
															ActionGroup31();  //1200
													 }
													 if((Serial_hinght_erweima[next] == 3)&&(GG == 69)&&(next == 6))//圆盘转动前，红色在下摆放抓俩
													 {
//														  Delay_ms(1300);
															ActionGroup6();  //抓左  2300
															ActionGroup33(); //1200
													 }
													 if((Serial_hinght_erweima[next] == 2)&&(GG == 69)&&(next == 6))//圆盘转动后，第三个抓的位置变成之前动作组位置加一
													 {
//														  Delay_ms(1300);
															ActionGroup7();  //抓右  //2800
															ActionGroup32(); //1500
													 }	
										 }
								 }										 
				     }
			   }

		 if(Pick_Up == 996)
		 {
		 	 ActionGroup1();//上电初始化状态		   
		 }
		 if(Pick_Up == 777)
		 {
		 	 ActionGroup199();//上电初始化状态		   
		 }
		 if((Pick_Up == 998) && (GG == 150))   //收回
		 {
			 ActionGroup199();
		 }
		 if((Pick_Up == 161) && (GG == 150))  //到加工区准备矫正姿势 
		 {
			 ActionGroup393();
		 }
		 if((Pick_Up == 169) && (GG == 150))  //到加工区准备矫正姿势 
		 {
			 ActionGroup219();//码垛矫正
			 GG = 61;
		 }
		 if(Pick_Up == 121 && GG == 150)  //回城  
		 {
			 ActionGroup199();
		 }	 		 		 
     if((Pick_Up == 2)&&(GG == 50)&&(next <= 3))   //放第一个位置
		 {
			   F_e = 66;
				 F1(Serial_hinght_erweima[next]);
			 	 PWM_DISABLE();		 
		 }
		 if((Pick_Up == 3)&&(GG == 50) && (next <= 3))   //抓取
		 {
			   F_e = 66;
				 F2(Serial_hinght_erweima[next]);
			 	 PWM_DISABLE();		 
		 }
     if((Pick_Up == 21)&&(GG == 50)&&(next >= 4))   //放第一个位置
		 {
				 F1(Serial_hinght_erweima[next]);
			 	 PWM_DISABLE();		 
		 }
		 if((Pick_Up == 31)&&(GG == 50) && (next >= 4) && (next <= 6))   //抓取
		 {
				 F2(Serial_hinght_erweima[next]);
			 	 PWM_DISABLE();		 
		 }
		 if((Pick_Up == 61)&&(GG == 50)&&(next >= 4) && (next <= 6))   //码垛
		 {
				 F3(Serial_hinght_erweima[next]);
			 	 PWM_DISABLE();
		 }
	}
}
void TIM6_IRQHandler(void)         //定时器6，处理标志位响应逻辑
{
	 	LED1_ON();

	if (TIM_GetITStatus(TIM6, TIM_IT_Update) == SET)  //获取中断标志位
	{ 
		 SCENT_UN++;
		 cnt++;
		 aa++;		
		 cot++;
     cxt++;
	 	 crt++;
		/****************************************************************/
		 //停止小车对五十次数据进行滤波后取均值，随后只进行一次矫正
		/****************************************************************/
		 if((hexToDec(Serial_RxPacket_2[18]) !=0) && (GG == 55)) //检测到颜色
		 {
				 Serial_hinght_erweima[8] = Serial_hinght_erweima[7];  //先7后8
				 Serial_hinght_erweima[7] = hexToDec(Serial_RxPacket_2[18]);
				 if((Serial_hinght_erweima[7] == 1 && Serial_hinght_erweima[8] == 3)
						 || (Serial_hinght_erweima[7] == 3 && Serial_hinght_erweima[8] == 2) 
							 || (Serial_hinght_erweima[7] == 2 && Serial_hinght_erweima[8] == 1))
				 {					  
						 Color_BaiFangShunXv = 2;
					   if((Pick_Up == 1) && (GG == 55))
						 {
								 Serial_hinght_erweima[9] = hexToDec(Serial_RxPacket_2[18]);
							   GG = 69;
						 }
						 if((Pick_Up == 121) && (GG == 55))
						 {
								 Serial_hinght_erweima[10] = hexToDec(Serial_RxPacket_2[18]);
								 GG = 69;							 
						 }
				 }
				 //摆放方式只有两种，摆放方式2情况
				 if((Serial_hinght_erweima[7] == 1 && Serial_hinght_erweima[8] == 2)
						 || (Serial_hinght_erweima[7] == 3 && Serial_hinght_erweima[8] == 1) 
							 || (Serial_hinght_erweima[7] == 2 && Serial_hinght_erweima[8] == 3))
				 {					  
						 Color_BaiFangShunXv = 1;
					   if((Pick_Up == 1) && (GG == 55))
						 {
								 Serial_hinght_erweima[9] = hexToDec(Serial_RxPacket_2[18]);
								 GG = 69;
						 }
						 if((Pick_Up == 121) && (GG == 55))
						 {
								 Serial_hinght_erweima[10] = hexToDec(Serial_RxPacket_2[18]);
								 GG = 69;							 
						 }
				 }
		 }
		 xiansgu1();    //分段矫正  242 342
		 xiansgu2();
		 xiansgu3();
		 yaw=(float)stcAngle.Angle[2]/32768*180;
     T00 =  Serial_hinght_erweima[next];
					x1 = T00-1;
					x2 = T00+2;
					x3 = T00+5;
					w1 = T00+8;
					w2 = T00+11;
					w3 = T00+14;       
					if(((x11+x22+x33+w11+w22+w33) == 12) && (YUY == 76))
					{
						Serial_hinght_erweima[1] = hexToDec(Serial_RxPacket_3[0]);  //1
						Serial_hinght_erweima[2] = hexToDec(Serial_RxPacket_3[1]);  //2
					  Serial_hinght_erweima[3] = hexToDec(Serial_RxPacket_3[2]);  //3
						Serial_hinght_erweima[4] = hexToDec(Serial_RxPacket_3[3]);  //3
						Serial_hinght_erweima[5] = hexToDec(Serial_RxPacket_3[4]);  //2
						Serial_hinght_erweima[6] = hexToDec(Serial_RxPacket_3[5]);  //1
						GYD = 19;						
						Pick_Up = 123;
						Serial_Printf5("page0.t1.txt=\"%d\"\xff\xff\xff",x11);
						Serial_Printf5("page0.t2.txt=\"%d\"\xff\xff\xff",x22);
						Serial_Printf5("page0.t3.txt=\"%d\"\xff\xff\xff",x33);
						Serial_Printf5("page0.t4.txt=\"%d\"\xff\xff\xff",w11);
						Serial_Printf5("page0.t5.txt=\"%d\"\xff\xff\xff",w22);
						Serial_Printf5("page0.t6.txt=\"%d\"\xff\xff\xff",w33);
						USART_Cmd(UART5, DISABLE);
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

					if(next > 3 && JIXIEBI == 14)   //第一次放置
					{
						SPEET_BUSHU_STOP = 2000;
						GG = 61;
						next = 1;
					}
					if(next > 6 && JIXIEBI == 114)   //第二圈一次放置
					{
						SPEET_BUSHU_STOP = 12000;
						GG = 61;
						next = 4;
					}
					if(next > 3 && JIXIEBI == 15)   //第一次放置在抓起
					{
						SPEET_BUSHU_STOP = 3000;
						GG = 61;
						next = 1;
					}
					if(next > 6 && JIXIEBI == 115)   //第一次放置在抓起
					{
						SPEET_BUSHU_STOP = 13000;
						GG = 61;
						next = 4;
					}
					if(next > 3 && JIXIEBI == 19)   //第一次整体放置完不抓起准备找转盘
					{
						SPEET_BUSHU_STOP = 3999;
					  GG = 61;
						JIXIEBI = 1152;
					}
					if((next > 6) && (JIXIEBI == 119) && (next != 777))   //第一次放置在抓起
					{
						SPEET_BUSHU_STOP = 14000;
					  GG = 61;
						next = 777;
					}
          if(next > 3 && JIXIEBI == 12)   //第一段标志位 抓取
					{						
						SPEET_BUSHU_STOP = 1000;
			      GG = 61;
						next = 1;
					}
          if(next > 6 && JIXIEBI == 112)   //第一段标志位 抓取
					{						
						SPEET_BUSHU_STOP = 11000;
			      GG = 61;
						next = 4;
					}
					if((JIXIEBI == 11) && (Pick_Up == 123))
					{
						SPEET_BUSHU_STOP = 9000;
						GG = 61;
						Pick_Up = 997;
					}
					if(GG == 78)
					{
							if(next == 1 && JIXIEBI == 12)   //第一次抓转盘物料需要矫正
							{
									GG = 71;
							}							
							if(next == 4 && JIXIEBI == 112)   //第一次抓转盘物料需要矫正
							{
									GG = 71;
							}
			  	}
					if(SCENT_UN >= 1000)    //第一次抓取之前停滞时间
					{
						if((XJC <= 999) && ((hexToDec(Serial_RxPacket_2[1])*100+hexToDec(Serial_RxPacket_2[4])*10+hexToDec(Serial_RxPacket_2[7])) != 0) && ((hexToDec(Serial_RxPacket_2[10])*100+hexToDec(Serial_RxPacket_2[13])*10+hexToDec(Serial_RxPacket_2[16])) != 0))  //两次误差过大不参与运算
						{
										JiaoZheng_X[XJC] = ((hexToDec(Serial_RxPacket_2[1])*100)+(hexToDec(Serial_RxPacket_2[4]))*10+hexToDec(Serial_RxPacket_2[7]));  //210
										JiaoZheng_W[XJC] = ((hexToDec(Serial_RxPacket_2[10])*100)+(hexToDec(Serial_RxPacket_2[13])*10)+hexToDec(Serial_RxPacket_2[16]));  //29
										XJC++;
										if(XJC == 999)
										{
											Rct = 1001;
										}
						}
					}
						if(GG == 21)   //矫正色环
						{
							Rct = 0;
							ZBX[1] = 0;
							ZBW[1] = 0;
							ZBZ[1] = 0;
							WBW[1] = 0;
							Xjs_2 = 1;
							Xjs_1 = 1;
							Xjs_2_2 = 0;
							Xjs_1_1 = 0;
              XJC = 0;
							SCENT_UN = 0;
							Xjs_Cnt_1 = 0;
							Xjs_Cnt_2 = 0;
							GG = 696;
						}
						if(GG == 696 && Rct == 1001)
						{
								if(Xjs_Cnt_1 >= 100 && Xjs_Cnt_2 >= 100) 
								{
									  ZBX[1] = ZBZ[1]/100;
									  ZBW[1] = WBW[1]/100;
									  Xjs_Cnt_1 = 0;
									  Xjs_Cnt_2 = 0;
										GG = 210;  //开始矫正
								}
								if(((abs(JiaoZheng_X[Xjs_1] - JiaoZheng_X[Xjs_1-1]) <= 100) && (JiaoZheng_X[Xjs_1-1] != 0)) && (Xjs_Cnt_1 <= 100) && (JiaoZheng_X[Xjs_1] != 0))  //两次误差过大不参与运算
								{
									ZBZ[1] = ZBZ[1] + JiaoZheng_X[Xjs_1];   //ZBZ[1]/50为接近位置坐标点
									Xjs_1++;
									Xjs_Cnt_1 ++;
									Xjs_1_1++;
								}
								if(((abs(JiaoZheng_W[Xjs_2] - JiaoZheng_W[Xjs_2-1]) <= 100) && (JiaoZheng_W[Xjs_2-1] != 0)) && (Xjs_Cnt_2 <= 100) && (JiaoZheng_W[Xjs_2] != 0))  //两次误差过大不参与运算
								{
									WBW[1] = WBW[1] + JiaoZheng_W[Xjs_2];
									Xjs_2++;
									Xjs_Cnt_2 ++;
									Xjs_2_2++;
								}
								if((abs(JiaoZheng_X[Xjs_1] - JiaoZheng_X[Xjs_1-1]) > 100) && (Xjs_Cnt_1 <= 100))
								{
									Xjs_1++;
									POCT_OUT_1++;
									Xjs_1_1++;
								}
								if((abs(JiaoZheng_W[Xjs_2] - JiaoZheng_W[Xjs_2-1]) > 100) && (Xjs_Cnt_2 <= 100))
								{
									Xjs_2++;
									POCT_OUT_2++;
									Xjs_2_2++;
								}
								if(POCT_OUT_1>=7 && Xjs_1_1 <= 21)   //每五个中有两个不一样的情况，直接重新记录
								{
									Xjs_Cnt_1 = 0;
									Xjs_1_1 = 0;
									POCT_OUT_1 = 0;
								}
								if(POCT_OUT_2>=7 && Xjs_2_2 <= 21)   //每五个中有两个不一样的情况，直接重新记录
								{
									Xjs_Cnt_2 = 0;
									Xjs_2_2 = 0;
									POCT_OUT_2 = 0;
								}
						}
		TIM_ClearITPendingBit(TIM6, TIM_IT_Update);  //清除中断标志位
	}
}

void TIM7_IRQHandler(void)         //定時时器7，处理基本方向逻辑
{
	if (TIM_GetITStatus(TIM7, TIM_IT_Update) == SET)  //获取中断标志位
	{
		Motor_conter(yaw,F_Yaw,F_Ybw,F_a,F_b,F_c,F_d,F_e);
//			speed_decision();
		if(GG==61)
		{		
				if((SPEET_BUSHU_STOP == 0)&&(GG==61))   //出发侧着走
				{
					ard = 10;
					F_Yaw = 1;
					F_Ybw = -1;
					F_a = 1;   
					F_b = 1;
					F_c = 2;
					F_d = 2;
					Pick_Up = 998;
					GG = 150;
					PWM_ENABLE();
					MOROR_MOVE_MAX(-1*5,100,100);
					aakk=717;
				}
				if((SPEET_BUSHU_STOP == 1)&&(GG==61))   //出发侧着走
				{
					F_Yaw = 1;
					F_Ybw = -1;
					F_a = 1;   
					F_b = 1;
					F_c = 2;
					F_d = 2;
					PWM_ENABLE();
					MOROR_MOVE_MAX(-620*6,100,100);
					aakk=717;
				}
				if((SPEET_BUSHU_STOP == 2)&&(GG==61))   //朝前走
				{
					F_Yaw = 1;
					F_Ybw = -1;
					F_a = 2;
					F_b = 1;
					F_c = 1;
					F_d = 2;
					PWM_ENABLE();
					MOROR_MOVE_MAX(-2290*5,400,400);		
					aakk=717;
				}
				if((SPEET_BUSHU_STOP == 3)&&(GG==61))  //朝前走
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
					aakk=717;
				}
				
				if((SPEET_BUSHU_STOP == 9000)&&(GG==61))   //朝前走
				{
					F_Yaw = 1;
					F_Ybw = -1;
					F_a = 2;
					F_b = 1;
					F_c = 1;
					F_d = 2;
					PWM_ENABLE();
					MOROR_MOVE_MAX(-1480*10,400,400);
					aakk=717;
				}
				if((SPEET_BUSHU_STOP == 9001)&&(GG==61))   //朝nei白?
				{
					F_Yaw = 1;
					F_Ybw = -1;
					F_a = 2;
					F_b = 2;
					F_c = 1;
					F_d = 1;
					PWM_ENABLE();
					if(Pick_Up == 997)
					{
						Pick_Up = 996;
					}
					MOROR_MOVE_MAX(-400*1,100,100);		
					aakk=717;
				}

				if((SPEET_BUSHU_STOP == 9002)&&(GG==61))   //抓
				{
					ard = 5;
					if(JIXIEBI == 11)
					{
						  PWM_DISABLE();
						  Pick_Up = 1;
							JIXIEBI = 12;
					}         
					F_Yaw = 200;
					F_Ybw = -200;					
					GG = 78;
					aakk=717;
				}
				if((SPEET_BUSHU_STOP == 1000)&&(GG==61))  //后退
				{ 	
          ard = 10;					
					F_Yaw = 1;
					F_Ybw = -1;
					F_a = 1;
					F_b = 1;
					F_c = 2;
					F_d = 2;	
          MOROR_MOVE_MAX(-1*1,200,200);
          PWM_ENABLE();	
					aakk=717;
				}
				if((SPEET_BUSHU_STOP == 1001)&&(GG==61))  //后撤步
				{ 					
					F_Yaw = 1;
					F_Ybw = -1;
					F_a = 1;
					F_b = 1;
					F_c = 2;
					F_d = 2;	
					if(Pick_Up == 1)
					{
						Pick_Up = 998;
					}
					GG = 150;
          MOROR_MOVE_MAX(-200*5,400,200);
          PWM_ENABLE();	
					aakk=717;
				}
				if((SPEET_BUSHU_STOP == 1002)&&(GG==61))  //后撤步
				{		
					F_Yaw = 1;
					F_Ybw = -1;
					F_a = 1;
					F_b = 2;
					F_c = 2;
					F_d = 1;	
          MOROR_MOVE_MAX(-2180*4,400,400);	
          PWM_ENABLE();			
				  aakk=717;
				}
				if((SPEET_BUSHU_STOP == 1003)&&(GG==61))  //转向
				{
					F_Yaw = 200;
					F_Ybw = -200;
					F_a = 1;
					F_b = 1;
					F_c = 1;
					F_d = 1;
          MOROR_MOVE_MAX(-1450*4,200,200);
					aakk=717;
				}
				if((SPEET_BUSHU_STOP == 1004)&&(GG==61))  //到加工区
				{
					F_Yaw = 88.3;
					F_Ybw = 87.7;
					F_a = 2;
					F_b = 1;
					F_c = 1;
					F_d = 2;
					PWM_ENABLE();
          MOROR_MOVE_MAX(-1*1,10,10);
					aakk=717;
				}

				if((SPEET_BUSHU_STOP == 1005)&&(GG==61))  //到加工区
				{
					F_Yaw = 88.3;
					F_Ybw = 87.7;
					F_a = 2;
					F_b = 1;
					F_c = 1;
					F_d = 2;
          MOROR_MOVE_MAX(-1620*18,400,400);
					PWM_ENABLE();
					aakk=717;
				}
				if((SPEET_BUSHU_STOP == 1006)&&(GG==61))  //旋转准备加工
				{
					LED1_ON();
					PWM_DISABLE();
					F_Yaw = 200;
					F_Ybw = -200;
					F_a = 1;
					F_b = 1;
					F_c = 1;
					F_d = 1;
          MOROR_MOVE_MAX(-1510*4,200,200);   //旋转
					if(JIXIEBI == 12)
					{
						  JIXIEBI = 13;
					}       
					PWM_ENABLE();
					aakk=717;
				}
				if((SPEET_BUSHU_STOP == 1007) && (GG == 61))  //放置
				{
					F_e = 55;   
					MOROR_MOVE_MAX(-1*4,70,70);					
					ard = 10;
					if(GG == 61)
          {  						
						  Pick_Up = 161;
          	  GG = 150;
          }					
					//????????????????????????????????????????????????????????????????????????????????????????????
					
					PWM_ENABLE();
					aakk=717;
				}
				if((SPEET_BUSHU_STOP == 1008) && (GG == 61))  //放置
				{
					ard = 5;
					F_e = 55;
					if(JIXIEBI == 13)
					{          	 
						  Pick_Up = 2;
          	  GG = 21;
							JIXIEBI = 14;                                            
			 		}
					aakk=717;
				}
				if((SPEET_BUSHU_STOP == 2000)&&(GG==61))  //抓取
				{
					ard = 5;
					if(JIXIEBI == 14)
					{
							JIXIEBI = 15;
					}       
             Pick_Up = 3;
          	 GG = 50;
					aakk=717;
				}
				if((SPEET_BUSHU_STOP == 3000) && (GG == 61))  //抓取
				{
					F_e = 55;
					F_a = 1;    //四个轮子方向
					F_b = 1;
					F_c = 2;
					F_d = 2;
					MOROR_MOVE_MAX(-1*4,70,70);
					PWM_ENABLE();
					aakk=717;
				}
				if((SPEET_BUSHU_STOP == 3001)&&(GG==61))  //走到拐角
				{		
          ard = 10;										
					F_e = 55;
					F_a = 1;    //四个轮子方向
					F_b = 1;
					F_c = 2;
					F_d = 2;
          if(Pick_Up == 3)
          {					
             Pick_Up = 998;
          }
					if(GG == 61)
					{
						GG = 150;					
					}          
				  MOROR_MOVE_MAX(-1*1,10,10);   //1600一圈  ， 十圈  ，300 速度 ，350加速度  ，全改成200
					PWM_ENABLE();
					aakk=717;
				}
				if((SPEET_BUSHU_STOP == 3002)&&(GG==61))  //走到拐角
				{		
          ard = 10;					
					F_e = 55;
					F_a = 1;    //四个轮子方向
					F_b = 2;
					F_c = 2;
					F_d = 1;
				  MOROR_MOVE_MAX(-3*1,3,3);   //1600一圈  ， 十圈  ，300 速度 ，350加速度  ，全改成200
					PWM_ENABLE();	
					aakk=717;
				}
				if((SPEET_BUSHU_STOP == 3003)&&(GG==61))  //走到拐角
				{		
          ard = 10;					
					F_e = 55;
					F_a = 1;    //四个轮子方向
					F_b = 2;
					F_c = 2;
					F_d = 1;
				  MOROR_MOVE_MAX(-1500*10,400,400);   //1600一圈  ， 十圈  ，300 速度 ，350加速度  ，全改成200
					PWM_ENABLE();	
					aakk=717;
				}
				if((SPEET_BUSHU_STOP == 3004)&&(GG==61))  //旋转准备加工
				{
					F_e = 00;
					F_Yaw = 200;
					F_Ybw = -200;
					F_a = 2;
					F_b = 2;
					F_c = 2;
					F_d = 2;
          MOROR_MOVE_MAX(-1500*4,200,200);
					if(JIXIEBI == 15)
					{
							JIXIEBI = 17;
					}        
					PWM_ENABLE();	
					aakk=717;
				}
				if((SPEET_BUSHU_STOP == 3005)&&(GG==61))  //缓冲
				{
					F_Yaw = 91;
					F_Ybw = 89;
					F_a = 1;
					F_b = 2;
					F_c = 2;
					F_d = 1;
					PWM_ENABLE();
          MOROR_MOVE_MAX(-1*1,70,70);
					aakk=717;
				}
				if((SPEET_BUSHU_STOP == 3006)&&(GG==61))  //到堆料区
				{
					LED1_ON();
					F_Yaw = 91;
					F_Ybw = 89;
					F_a = 1;
					F_b = 2;
					F_c = 2;
					F_d = 1;
					PWM_ENABLE();
          MOROR_MOVE_MAX(-1100*11,400,400);
					aakk=717;
				}
				if((SPEET_BUSHU_STOP == 3007)&&(GG==61))  //到堆料区
				{
					LED1_ON();
					F_Yaw = 91;
					F_Ybw = 89;
					F_a = 1;
					F_b = 2;
					F_c = 2;
					F_d = 1;
					PWM_ENABLE();
          MOROR_MOVE_MAX(-1*1,400,400);
					if(GG == 61)
					{
						  Pick_Up = 161;
					    GG = 150;				
					}
					if(JIXIEBI == 17)
					{
						  JIXIEBI = 18;
					}       
					aakk=717;
				}
				if((SPEET_BUSHU_STOP == 3008)&&(GG==61))  //放置
				{
					ard = 5;
					F_Yaw = 91;
					F_Ybw = 89;
					if(JIXIEBI == 18)
					{
						 JIXIEBI = 19;
			 		}
											  
					Pick_Up = 2;
          GG = 21;
					aakk=717;
				}
				if((SPEET_BUSHU_STOP == 3999)&&(GG==61))  //后退
				{
				  ard = 10;
					F_e = 0;
					F_Yaw = 91;
					F_Ybw = 89;
					F_a = 1;
					F_b = 1;
					F_c = 2;
					F_d = 2;
          MOROR_MOVE_MAX(-1*1,100,100);
					PWM_ENABLE();
					aakk=717;
				}
				if((SPEET_BUSHU_STOP == 4000)&&(GG==61))  //后退
				{
					F_e = 0;
					F_Yaw = 91;
					F_Ybw = 89;
					F_a = 1;
					F_b = 1;
					F_c = 2;
					F_d = 2;
          MOROR_MOVE_MAX(-500*1,70,70);
					PWM_ENABLE();
					aakk=717;
				}
				if((SPEET_BUSHU_STOP == 4001)&&(GG==61))  //准备去下一个拐角找转盘
				{
					F_Yaw = 91;
					F_Ybw = 89;
					F_a = 1;
					F_b = 2;
					F_c = 2;
					F_d = 1;
          MOROR_MOVE_MAX(-1820*9,400,400);
					if(JIXIEBI == 1152)
					{
						  
							JIXIEBI = 21;
					}    
          Pick_Up = 998;					
          GG = 150;	
					aakk=717;
				}
				if((SPEET_BUSHU_STOP == 4002)&&(GG==61))  //旋转准备去转盘
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
//					    GG = 150;
							JIXIEBI = 22;
					}     
					aakk=717;
				}
				if((SPEET_BUSHU_STOP == 4003)&&(GG==61))   //缓冲
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
					aakk=717;
				}
				if((SPEET_BUSHU_STOP == 4004)&&(GG==61))   //找转盘
				{
					F_Yaw = 3;
					F_Ybw = 2;
					F_a = 1;
					F_b = 2;
					F_c = 2;
					F_d = 1;			
          if(Pick_Up == 998)
					{
              Pick_Up = 996;
					}						
					MOROR_MOVE_MAX(-1200*4,400,400);
					aakk=717;
				}			
				if((SPEET_BUSHU_STOP == 4005)&&(GG==61))   //抓
				{
					ard = 5;
					if(JIXIEBI == 111)
					{
						  PWM_DISABLE();
						  Pick_Up = 121;
							JIXIEBI = 112;
					}         
					F_Yaw = 200;
					F_Ybw = -200;					
					GG = 78;
					aakk=717;
				}
				if((SPEET_BUSHU_STOP == 11000)&&(GG==61))  //后撤步
				{ 	
          ard = 10;					
					F_Yaw = 1;
					F_Ybw = -1;
					F_a = 1;
					F_b = 1;
					F_c = 2;
					F_d = 2;	
					if(Pick_Up == 996)
					{
              Pick_Up = 998;
					}					
          GG = 150;					
          MOROR_MOVE_MAX(-1*4,400,200);
          PWM_ENABLE();	
					aakk=717;
				}
				if((SPEET_BUSHU_STOP == 11001)&&(GG==61))  //后撤步
				{ 					
					F_Yaw = 1;
					F_Ybw = -1;
					F_a = 1;
					F_b = 1;
					F_c = 2;
					F_d = 2;	
          MOROR_MOVE_MAX(-300*1,200,200);
          PWM_ENABLE();		
					aakk=717;
				}

				if((SPEET_BUSHU_STOP == 11002)&&(GG==61))  //后撤步
				{		
					F_Yaw = 1;
					F_Ybw = -1;
					F_a = 1;
					F_b = 2;
					F_c = 2;
					F_d = 1;	
          MOROR_MOVE_MAX(-2200*4,400,400);	
          PWM_ENABLE();
					aakk=717;
				}
				if((SPEET_BUSHU_STOP == 11003)&&(GG==61))  //转向
				{
					F_Yaw = 200;
					F_Ybw = -200;
					F_a = 1;
					F_b = 1;
					F_c = 1;
					F_d = 1;
          MOROR_MOVE_MAX(-1500*4,200,200);
					aakk=717;
				}
				if((SPEET_BUSHU_STOP == 11004)&&(GG==61))  //到加工区
				{
					F_Yaw = 89;
					F_Ybw = 88;
					F_a = 2;
					F_b = 1;
					F_c = 1;
					F_d = 2;
					PWM_ENABLE();
          MOROR_MOVE_MAX(-1*4,10,10);
					aakk=717;
				}
				if((SPEET_BUSHU_STOP == 11005)&&(GG==61))  //到加工区
				{
					F_Yaw = 89;
					F_Ybw = 88;
					F_a = 2;
					F_b = 1;
					F_c = 1;
					F_d = 2;
          MOROR_MOVE_MAX(-1570*19,400,400);
					PWM_ENABLE();
					aakk=717;
				}
				if((SPEET_BUSHU_STOP == 11006)&&(GG==61))  //旋转准备加工
				{
					LED1_ON();
					F_Yaw = 200;
					F_Ybw = -200;
					F_a = 1;
					F_b = 1;
					F_c = 1;
					F_d = 1;
					PWM_ENABLE();
          MOROR_MOVE_MAX(-1450*4,200,200);   //旋转
					aakk=717;
				}
				if((SPEET_BUSHU_STOP == 11007)&&(GG==61))  //放置
				{
					F_e = 55;
					if(JIXIEBI == 112)
					{
						  JIXIEBI = 113;
					}						 
					Pick_Up = 161;
					GG = 150;  
					PWM_ENABLE();
          MOROR_MOVE_MAX(-1*4,10,10);
					aakk=717;
				}
				if((SPEET_BUSHU_STOP == 11008)&&(GG==61))  //放置
				{
					ard = 5;
					F_e = 55;
					if(JIXIEBI == 113)
					{          	 
							JIXIEBI = 114;                                            
			 		}						  
					Pick_Up = 21;
          GG = 21;
					aakk=717;
				}
				if((SPEET_BUSHU_STOP == 12000)&&(GG==61))  //抓取
				{
					ard = 5;
					if(JIXIEBI == 114)
					{             
						  JIXIEBI = 115;
					}						  
					Pick_Up = 31;
          GG = 50;
					aakk=717;
				}
				if((SPEET_BUSHU_STOP == 13000)&&(GG==61))  //抓取
				{
					ard = 10;
					if(Pick_Up == 31)
					{             
						  Pick_Up = 998;
					}       
					
					GG = 150;
          MOROR_MOVE_MAX(-1*1,70,70);
					PWM_ENABLE();
					aakk=717;
				}
				if((SPEET_BUSHU_STOP == 13001)&&(GG==61))  //走到拐角
				{		         					
					F_e = 55;
					F_a = 1;    //四个轮子方向
					F_b = 1;
					F_c = 2;
					F_d = 2;
				  MOROR_MOVE_MAX(-1*1,20,20);   //1600一圈  ， 十圈  ，300 速度 ，350加速度  ，全改成200
					PWM_ENABLE();
					aakk=717;
				}
				if((SPEET_BUSHU_STOP == 13002)&&(GG==61))  //houche
				{			
					F_e = 55;
					F_a = 1;    //四个轮子方向
					F_b = 1;
					F_c = 2;
					F_d = 2;
				  MOROR_MOVE_MAX(-200*1,200,200);   //1600一圈  ， 十圈  ，300 速度 ，350加速度  ，全改成200
					PWM_ENABLE();
					aakk=717;
				}
				if((SPEET_BUSHU_STOP == 13003)&&(GG==61))  //走到拐角
				{					
					F_e = 55;
					F_a = 1;    //四个轮子方向
					F_b = 2;
					F_c = 2;
					F_d = 1;
					MOROR_MOVE_MAX(-1500*10,400,400);
					if(JIXIEBI == 115)
					{
							JIXIEBI = 116;
					}       
					PWM_ENABLE();
					aakk=717;
				}
				if((SPEET_BUSHU_STOP == 13004)&&(GG==61))  //旋转准备加工
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
					aakk=717;
				}
				if((SPEET_BUSHU_STOP == 13005)&&(GG==61))  //缓冲
				{
					F_Yaw = 91;
					F_Ybw = 89;
					F_a = 1;
					F_b = 2;
					F_c = 2;
					F_d = 1;
          MOROR_MOVE_MAX(-1*1,70,70);
					PWM_ENABLE();
					aakk=717;
					}
				if((SPEET_BUSHU_STOP == 13006)&&(GG==61))  //到堆料区
				{
					LED1_ON();
					Cunt1 = 330;
					Cunt2 = 261;
					F_Yaw = 91;
					F_Ybw = 89;
					F_a = 1;
					F_b = 2;
					F_c = 2;
					F_d = 1;
					PWM_ENABLE();
          MOROR_MOVE_MAX(-1500*8,400,400);
					aakk=717;
				}
				if((SPEET_BUSHU_STOP == 13007)&&(GG==61))  //放置
				{
					F_Yaw = 91;
					F_Ybw = 89;
					if(JIXIEBI == 117)
					{
						  JIXIEBI = 118;
					} 						  
					Pick_Up = 169;
					GG = 150;
    
					PWM_ENABLE();
          MOROR_MOVE_MAX(-1*1,10,10);
					aakk=717;
				}
				if((SPEET_BUSHU_STOP == 13008)&&(GG==61))  //放置
				{
					ard = 5;
					F_Yaw = 91;
					F_Ybw = 89;
					if(JIXIEBI == 118)
					{
							JIXIEBI = 119;
			 		}
          	 Pick_Up = 61;
          	 GG = 21;
					aakk=717;
				}
				if((SPEET_BUSHU_STOP == 14000)&&(GG==61))  //后退
				{
					ard = 10;
					F_Yaw = 91;
					F_Ybw = 89;
					F_a = 1;
					F_b = 1;
					F_c = 2;
					F_d = 2;
					PWM_ENABLE();
          MOROR_MOVE_MAX(-1*1,70,70);
					aakk=717;
				}				
				if((SPEET_BUSHU_STOP == 14001)&&(GG==61))  //后退
				{
					ard = 10;
					F_Yaw = 91;
					F_Ybw = 89;
					F_a = 1;
					F_b = 1;
					F_c = 2;
					F_d = 2;
					PWM_ENABLE();
          MOROR_MOVE_MAX(-400*1,70,70);
					if(JIXIEBI == 119)
					{   
						  JIXIEBI = 120;
					}					
					aakk=717;
				}
				if((SPEET_BUSHU_STOP == 14002)&&(GG==61))  //准备回家
				{
					F_Yaw = 91;
					F_Ybw = 89;
					F_a = 1;
					F_b = 2;
					F_c = 2;
					F_d = 1;
          MOROR_MOVE_MAX(-1750*9,400,400);
					if(JIXIEBI == 120)
					{
							JIXIEBI = 121;
					}						  
					Pick_Up = 121;
					GG = 150;
         aakk=717;
				}
				if((SPEET_BUSHU_STOP == 14003)&&(GG==61))  //旋转回家
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
					aakk=717;
				}
				if((SPEET_BUSHU_STOP == 14004)&&(GG==61))   //缓冲
				{
					F_e = 00;
					F_Yaw = 3;
					F_Ybw = 2;
					F_a = 1;
					F_b = 2;
					F_c = 2;
					F_d = 1;						  
					MOROR_MOVE_MAX(-1*2,70,70);
					aakk=717;
				}
				if((SPEET_BUSHU_STOP == 14005)&&(GG==61))   //回家
				{
					F_Yaw = 3;
					F_Ybw = 2;
					F_a = 1;
					F_b = 2;
					F_c = 2;
					F_d = 1;				  
					MOROR_MOVE_MAX(-1830*17,400,400);	
					aakk=717;
				}
				
				if((SPEET_BUSHU_STOP == 14006)&&(GG==61))   //回家缓冲
				{
					F_Yaw = 1;
					F_Ybw = -1;
					F_a = 2;   
					F_b = 2;
					F_c = 1;
					F_d = 1;					
					MOROR_MOVE_MAX(-1*3,150,150);
					PWM_ENABLE();
					aakk=717;
				}
				if((SPEET_BUSHU_STOP == 14007)&&(GG==61))   //回家
				{
					F_Yaw = 1;
					F_Ybw = -1;
					F_a = 2;   
					F_b = 2;
					F_c = 1;
					F_d = 1;					
					MOROR_MOVE_MAX(-3000*1,150,150);
					PWM_ENABLE();
					aakk=717;
				}

			}
		TIM_ClearITPendingBit(TIM7, TIM_IT_Update);  //清除中断标志位
	}
}


