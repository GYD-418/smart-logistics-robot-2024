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
extern int F_a,F_b,F_c,F_d,F_Yaw,F_Ybw,F_e;
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
int LUJING = 0;
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
	 uartInit(9600);
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
//	 Motor_conter(yaw,2,-2,200,1,200,2,1,1,2);
//	 Motor_conter(yaw,2,-2,0,0,300,2,1,1,2);
//	 PWM_DISABLE();
//	 set_speed = 200;
//   MOROR_MOVE_MAX(-1600*100,set_speed);
//	 MOTOR_Move(-1600*100,set_speed/4,set_speed/4,set_speed);       //需要时调用
//		Motor_conter(yaw,2,-2,0,0,2,1,1,2);
	 ActionGroup0();//上电初始化状态		   

	while (1)
	{	
		 x11 = hexToDec(Serial_RxPacket_2[19]);     //        串口三扫二维码 串口二发送x y color        cv xwc     mv c y 123321
		 x22 = hexToDec(Serial_RxPacket_2[20]);     //				 2cv x w  3 c y 123321
		 x33 = hexToDec(Serial_RxPacket_2[21]);     //     		 0x13 19 0x14  20 0x15 21
		 w11 = hexToDec(Serial_RxPacket_2[22]);    //
		 w22 = hexToDec(Serial_RxPacket_2[23]);    //
		 w33 = hexToDec(Serial_RxPacket_2[24]);    //
		
	 	 OLED_ShowSignedNum(1,1,Pick_Up,3);  //212
		 OLED_ShowSignedNum(1,7,GYD,3);  //384
		 OLED_ShowSignedNum(2,1,GG,3);
		 OLED_ShowSignedNum(3,1,JIXIEBI,8);
		 OLED_ShowSignedNum(4,1,next,8);

 		 if((Pick_Up == 1)&&(GG == 50))   //抓转盘物料 抓第一个，其他两个写死
		 {
//			 if((abs(Serial_rbg_wulaio[1] - Serial_rbg_wulaio[0])>=1) || (abs(Serial_rbg_wulaio[1] - Serial_rbg_wulaio[0])<=2))
//			 {
				 GG = 69;
//			 }
		 }

//		 // 实际减目标是负数需   要左转 右快左慢
//		 // 实际减目标是正数需要右转 右慢左快+++++++++
		 if((Pick_Up == 1)&&(GG == 69))   //抓转盘物料 抓第一个，其他两个写死
		 {
			   next = 4;
				 if(next <= 3)
				 {
					 if(hexToDec(Serial_RxPacket_2[24]) == 1)   //红色在下
					 {
								 if(next <= 2)
								 {
											 if(Serial_hinght_erweima[next] == 1)//圆盘转动前，红色在下摆放抓俩
											 {
													ActionGroup5();
											 }
											 if(Serial_hinght_erweima[next] == 2)//圆盘转动前，红色在下摆放抓俩
											 {
													ActionGroup6();
											 }
											 if(Serial_hinght_erweima[next] == 3)//圆盘转动后，第三个抓的位置变成之前动作组位置加一
											 {
													ActionGroup7();
											 }			
								 }	
								 if(next == 3)
								 {
											 if(Serial_hinght_erweima[next] == 2)//圆盘转动前，红色在下摆放抓俩
											 {
													ActionGroup5();
											 }
											 if(Serial_hinght_erweima[next] == 3)//圆盘转动前，红色在下摆放抓俩
											 {
													ActionGroup6();
											 }
											 if(Serial_hinght_erweima[next] == 1)//圆盘转动后，第三个抓的位置变成之前动作组位置加一
											 {
													ActionGroup7();
											 }			
								 }							 				 
					 }
					 if(hexToDec(Serial_RxPacket_2[24]) == 2)   //红色在下
					 {
								 if(next <= 2)
								 {
											 if(Serial_hinght_erweima[next] == 2)//圆盘转动前，红色在下摆放抓俩
											 {
													ActionGroup5();
											 }
											 if(Serial_hinght_erweima[next] == 3)//圆盘转动前，红色在下摆放抓俩
											 {
													ActionGroup6();
											 }
											 if(Serial_hinght_erweima[next] == 1)//圆盘转动后，第三个抓的位置变成之前动作组位置加一
											 {
													ActionGroup7();
											 }			
								 }	
								 if(next == 3)
								 {
											 if(Serial_hinght_erweima[next] == 1)//圆盘转动前，红色在下摆放抓俩
											 {
													ActionGroup5();
											 }
											 if(Serial_hinght_erweima[next] == 2)//圆盘转动前，红色在下摆放抓俩
											 {
													ActionGroup6();
											 }
											 if(Serial_hinght_erweima[next] == 3)//圆盘转动后，第三个抓的位置变成之前动作组位置加一
											 {
													ActionGroup7();
											 }			
								 }							 				 
					 }
					 if(hexToDec(Serial_RxPacket_2[24]) == 3)   //红色在下
					 {
								 if(next <= 2)
								 {
											 if(Serial_hinght_erweima[next] == 3)//圆盘转动前，红色在下摆放抓俩
											 {
													ActionGroup5();
											 }
											 if(Serial_hinght_erweima[next] == 1)//圆盘转动前，红色在下摆放抓俩
											 {
													ActionGroup6();
											 }
											 if(Serial_hinght_erweima[next] == 2)//圆盘转动后，第三个抓的位置变成之前动作组位置加一
											 {
													ActionGroup7();
											 }			
								 }	
								 if(next == 3)
								 {
											 if(Serial_hinght_erweima[next] == 2)//圆盘转动前，红色在下摆放抓俩
											 {
													ActionGroup5();
											 }
											 if(Serial_hinght_erweima[next] == 3)//圆盘转动前，红色在下摆放抓俩
											 {
													ActionGroup6();
											 }
											 if(Serial_hinght_erweima[next] == 1)//圆盘转动后，第三个抓的位置变成之前动作组位置加一
											 {
													ActionGroup7();
											 }			
								 }									 
					 }
		 }
	 }
		 if(Pick_Up == 123)  //发初始状态，看二维码   
		 {
			 ActionGroup1();
			 Pick_Up = 1;
		 }
		 
     if((Pick_Up == 2)&&(GG == 50))   //放第一个位置
		 {
			 	 if(next <= 3)
				 {
					 F1(Serial_hinght_erweima[next]);
				 }
			 	 PWM_DISABLE();		 
		 }
		 if((Pick_Up == 3)&&(GG == 50))   //第一次抓取
		 {
			 	 if(next <= 3)
				 {
					 F2(Serial_hinght_erweima[next]);
				 }
			 	 PWM_DISABLE();		 
		 }
		 if((Pick_Up == 6)&&(GG == 50))   //第一次抓取
		 {
			 	 if(next <= 3)
				 {
					 F3(Serial_hinght_erweima[next]);
				 }
			 	 PWM_DISABLE();
		 }
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
		 
		 Serial_hinght_ab[0] = Serial_RxPacket_2[1]; //百  //绿色
		 Serial_hinght_ab[1] = Serial_RxPacket_2[4]; //十
		 Serial_hinght_ab[2] = Serial_RxPacket_2[7]; //个
		 Serial_hinght_ab[3] = Serial_RxPacket_2[10]; //百
		 Serial_hinght_ab[4] = Serial_RxPacket_2[13];  //十
		 Serial_hinght_ab[5] = Serial_RxPacket_2[16];  //个	
		 Serial_rbg_wulaio[1] = Serial_rbg_wulaio[0];
	   Serial_rbg_wulaio[0] = hexToDec(Serial_RxPacket_2[24]);	 
		 if(((((hexToDec(Serial_hinght_ab[0])*100)+(hexToDec(Serial_hinght_ab[1])*10)+(hexToDec(Serial_hinght_ab[2])))-((hexToDec(Serial_hinght_ab[0])*100)+(hexToDec(Serial_hinght_ab[1])*10)+(hexToDec(Serial_hinght_ab[2])))) != 0) 
			 || ((((hexToDec(Serial_hinght_ab[3])*100)+(hexToDec(Serial_hinght_ab[4])*10)+(hexToDec(Serial_hinght_ab[5])))-((hexToDec(Serial_hinght_ab[9])*100)+(hexToDec(Serial_hinght_ab[10])*10)+(hexToDec(Serial_hinght_ab[11])))) != 0))
		 {
			 	 oint = 0;
			   TXT_HC[0] = Serial_RxPacket_2[1];
			   TXT_HC[1] = Serial_RxPacket_2[4];
			   TXT_HC[2] = Serial_RxPacket_2[7];
			   TXT_HC[3] = Serial_RxPacket_2[10];
			   TXT_HC[4] = Serial_RxPacket_2[13];
			   TXT_HC[5] = Serial_RxPacket_2[16];
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
					  Serial_hinght_erweima[7] = hexToDec(Serial_RxPacket_2[24]);
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
               
					if(next > 6 && JIXIEBI == 24)  //进行第二次放置
					{		
            SPEET_BUSHU_STOP = 5000;
						GG = 61;						
						next = 4;
					}					
					if(next > 6 && JIXIEBI == 28)   //第二段转折
					{
            SPEET_BUSHU_STOP = 6000;
						GG = 61;						
						next = 4;
					}
					if(next > 6 && JIXIEBI == 29)   //第二段转折
					{
            SPEET_BUSHU_STOP = 7000;
						GG = 61;						
						next = 4;
					}
					if(next > 6 && JIXIEBI == 35)   //第二段转折
					{
            SPEET_BUSHU_STOP = 8000;
						GG = 61;						
						JIXIEBI = 36;
					}
					if(next > 3 && JIXIEBI == 14)   //第一次放置
					{
						SPEET_BUSHU_STOP = 2000;
						GG = 61;
						next = 1;
					}
					if(next > 3 && JIXIEBI == 15)   //第一次放置在抓起
					{
						SPEET_BUSHU_STOP = 3000;
						GG = 61;
						next = 1;
					}
					if(next > 3 && JIXIEBI == 19)   //第一次放置在抓起
					{
						SPEET_BUSHU_STOP = 4000;
						GG = 61;
						JIXIEBI = 20;
					}
          if(next > 3 && JIXIEBI == 12)   //第一段标志位 抓取
					{
						SPEET_BUSHU_STOP = 1000;
						GG = 61;
						next = 1;
					}
					if(GG == 78)
					{
							if(next == 1 && JIXIEBI == 12)   //第一次抓转盘物料需要矫正
							{
									GG = 71;
							}
			  	}
///**********************************************第一次矫正 opencv x矫正 *****************************************************************/	
//					l = hexToDec(Serial_RxPacket_2[18]);  //				 0x13 19 0x14 20 0x15 21        //243 60  243 60 245 63
					if(GG == 38)
					{
						if(((hexToDec(Serial_RxPacket_2[w1])*100)+(hexToDec(Serial_RxPacket_2[w2])*10)+(hexToDec(Serial_RxPacket_2[w3])))>=386 && ((hexToDec(Serial_RxPacket_2[w1])*100)+(hexToDec(Serial_RxPacket_2[w2])*10)+(hexToDec(Serial_RxPacket_2[w3])))<=600)  //后
						{							
							F_a = 1;
							F_b = 2;
							F_c = 2;
							F_d = 1;		
              MOROR_MOVE_MAX(-800*1,50,25);
						}
						if(((hexToDec(Serial_RxPacket_2[w1])*100)+(hexToDec(Serial_RxPacket_2[w2])*10)+(hexToDec(Serial_RxPacket_2[w3])))<=79 && ((hexToDec(Serial_RxPacket_2[w1])*100)+(hexToDec(Serial_RxPacket_2[w2])*10)+(hexToDec(Serial_RxPacket_2[w3])))>=10)  //后
						{							
              F_a = 2;
							F_b = 1;
							F_c = 1;
							F_d = 2;		
              MOROR_MOVE_MAX(-800*1,50,25);							
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
						if((((hexToDec(Serial_RxPacket_2[x1])*100)+(hexToDec(Serial_RxPacket_2[x2])*10)+(hexToDec(Serial_RxPacket_2[x3]))) >=355) && (((hexToDec(Serial_RxPacket_2[x1])*100)+(hexToDec(Serial_RxPacket_2[x2])*10)+(hexToDec(Serial_RxPacket_2[x3]))) <= 600))//zuo
						{				
							F_a = 1;
							F_b = 1;
							F_c = 2;
							F_d = 2;		
              MOROR_MOVE_MAX(-800*1,50,25);		
              PWM_ENABLE();							
						}
						if((((hexToDec(Serial_RxPacket_2[x1])*100)+(hexToDec(Serial_RxPacket_2[x2])*10)+(hexToDec(Serial_RxPacket_2[x3]))) <=314) && (((hexToDec(Serial_RxPacket_2[x1])*100)+(hexToDec(Serial_RxPacket_2[x2])*10)+(hexToDec(Serial_RxPacket_2[x3]))) >= 10))  //you
						{							
							F_a = 2;
							F_b = 2;
							F_c = 1;
							F_d = 1;	
              MOROR_MOVE_MAX(-800*1,50,25);             							
						  PWM_ENABLE();
						}
						if(((hexToDec(Serial_RxPacket_2[x1])*100)+(hexToDec(Serial_RxPacket_2[x2])*10)+(hexToDec(Serial_RxPacket_2[x3])))==0)  //后
						{
										PWM_DISABLE();
						}	
						if((((hexToDec(Serial_RxPacket_2[x1])*100)+(hexToDec(Serial_RxPacket_2[x2])*10)+(hexToDec(Serial_RxPacket_2[x3]))) >314) && ((hexToDec(Serial_RxPacket_2[x1])*100)+(hexToDec(Serial_RxPacket_2[x2])*10)+(hexToDec(Serial_RxPacket_2[x3]))) <355)  //后
						{
										PWM_DISABLE();
										GG = 38;
						}
					
				}
				

/*****************************************************************************************************************************************************************************************/
		TIM_ClearITPendingBit(TIM6, TIM_IT_Update);  //清除中断标志位
	}
}

void TIM7_IRQHandler(void)         //定時时器7，处理基本方向逻辑
{
	if (TIM_GetITStatus(TIM7, TIM_IT_Update) == SET)  //获取中断标志位
	{
		Motor_conter(yaw,F_Yaw,F_Ybw,F_a,F_b,F_c,F_d,F_e);
		if(GG==61)
		{		
				if(SPEET_BUSHU_STOP == 0)   //出发侧着走
				{
					F_Yaw = 2;
					F_Ybw = -2;
					F_a = 1;
					F_b = 1;
					F_c = 2;
					F_d = 2;
					MOROR_MOVE_MAX(-1600*8,100,100);
				}
				if(SPEET_BUSHU_STOP == 1)   //朝前走
				{
					F_Yaw = 2;
					F_Ybw = -2;
					F_a = 2;
					F_b = 1;
					F_c = 1;
					F_d = 2;
					if(JIXIEBI == 0)
					{
							JIXIEBI = 11;
					} 
					PWM_ENABLE();
					MOROR_MOVE_MAX(-1600*25,150,150);				
				}
				if(SPEET_BUSHU_STOP == 2)   //抓
				{
					F_Yaw = 200;
					F_Ybw = -200;
					if(JIXIEBI == 11)
					{
						  PWM_DISABLE();
							JIXIEBI = 12;
					}         
					if(GG == 61)
					{
						 Pick_Up = 1;
					   GG = 78;
					}
				}
				if(SPEET_BUSHU_STOP == 1000)  //后撤步
				{
					F_Yaw = 2;
					F_Ybw = -2;
					F_a = 1;
					F_b = 2;
					F_c = 2;
					F_d = 1;
          MOROR_MOVE_MAX(-1600*10,300,350);
					if(JIXIEBI == 12)
					{
							JIXIEBI = 13;
					}         
				}
				if(SPEET_BUSHU_STOP == 1001)  //转向
				{
					F_Yaw = 200;
					F_Ybw = -200;
					F_a = 1;
					F_b = 1;
					F_c = 2;
					F_d = 2;
          MOROR_MOVE_MAX(-1600*10,300,350);
					if(JIXIEBI == 13)
					{
							JIXIEBI = 14;
					}         
				}
				if(SPEET_BUSHU_STOP == 1002)  //到加工区
				{
					F_Yaw = 89;
					F_Ybw = 91;
					F_a = 2;
					F_b = 1;
					F_c = 1;
					F_d = 2;
          MOROR_MOVE_MAX(-1600*25,300,350);
					if(JIXIEBI == 12)
					{
							JIXIEBI = 13;
					}         
				}
				if(SPEET_BUSHU_STOP == 1003)  //旋转准备加工
				{
					F_Yaw = 200;
					F_Ybw = -200;
					F_a = 1;
					F_b = 1;
					F_c = 2;
					F_d = 2;
          MOROR_MOVE_MAX(-1600*10,300,350);
					if(JIXIEBI == 13)
					{
							JIXIEBI = 14;
					}         
				}
				if(SPEET_BUSHU_STOP == 1004)  //放置
				{
					F_Yaw = 200;
					F_Ybw = -200;
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
				if(SPEET_BUSHU_STOP == 2000)  //抓取
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
				if(SPEET_BUSHU_STOP == 3000)  //走到拐角
				{					
					F_e = 55;
					F_a = 1;
					F_b = 1;
					F_c = 2;
					F_d = 2;
				  MOROR_MOVE_MAX(-1600*10,300,350);
					if(JIXIEBI == 15)
					{
							JIXIEBI = 16;
					}       
				}
				if(SPEET_BUSHU_STOP == 3001)  //旋转准备加工
				{
					F_e = 00;
					F_Yaw = 200;
					F_Ybw = -200;
					F_a = 1;
					F_b = 1;
					F_c = 2;
					F_d = 2;
          MOROR_MOVE_MAX(-1600*10,300,350);
					if(JIXIEBI == 16)
					{
							JIXIEBI = 17;
					}         
				}
				if(SPEET_BUSHU_STOP == 3002)  //到堆料区
				{
					F_Yaw = 89;
					F_Ybw = 91;
					F_a = 2;
					F_b = 1;
					F_c = 1;
					F_d = 2;
          MOROR_MOVE_MAX(-1600*10,300,350);
					if(JIXIEBI == 17)
					{
							JIXIEBI = 18;
					}         
				}
				if(SPEET_BUSHU_STOP == 3003)  //放置
				{
					F_Yaw = 200;
					F_Ybw = -200;
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
				if(SPEET_BUSHU_STOP == 4000)  //准备去下一个拐角找转盘
				{
					F_Yaw = 89;
					F_Ybw = 91;
					F_a = 2;
					F_b = 1;
					F_c = 1;
					F_d = 2;
          MOROR_MOVE_MAX(-1600*10,300,350);
					if(JIXIEBI == 20)
					{
							JIXIEBI = 21;
					}         
				}
				if(SPEET_BUSHU_STOP == 4001)  //旋转准备去转盘
				{
					F_e = 00;
					F_Yaw = 200;
					F_Ybw = -200;
					F_a = 1;
					F_b = 1;
					F_c = 2;
					F_d = 2;
          MOROR_MOVE_MAX(-1600*10,300,350);
					if(JIXIEBI == 21)
					{
							JIXIEBI = 22;
					}         
				}
				if(SPEET_BUSHU_STOP == 4002)   //找转盘
				{
					F_Yaw = 2;
					F_Ybw = -2;
					F_a = 1;
					F_b = 2;
					F_c = 2;
					F_d = 1;
					if(JIXIEBI == 22)
					{
							JIXIEBI = 23;
					} 
						  MOROR_MOVE_MAX(-1600*1,50,25);				
				}
				if(SPEET_BUSHU_STOP == 4003)   //抓第二次
				{
					F_Yaw = 200;
					F_Ybw = -200;
					if(JIXIEBI == 23)
					{
						  PWM_DISABLE();
							JIXIEBI = 24;
					}         
					if(GG == 61)
					{
						 Pick_Up = 1;
					   GG = 78;
					}
				}
				if(SPEET_BUSHU_STOP == 5000)  //后撤步
				{
					F_Yaw = 2;
					F_Ybw = -2;
					F_a = 1;
					F_b = 2;
					F_c = 2;
					F_d = 1;
          MOROR_MOVE_MAX(-1600*10,300,350);
					if(JIXIEBI == 24)
					{
							JIXIEBI = 25;
					}         
				}
				if(SPEET_BUSHU_STOP == 5001)  //转向
				{
					F_Yaw = 200;
					F_Ybw = -200;
					F_a = 1;
					F_b = 1;
					F_c = 2;
					F_d = 2;
          MOROR_MOVE_MAX(-1600*10,300,350);
					if(JIXIEBI == 25)
					{
							JIXIEBI = 26;
					}         
				}
				if(SPEET_BUSHU_STOP == 5002)  //到加工区
				{
					F_Yaw = 89;
					F_Ybw = 91;
					F_a = 2;
					F_b = 1;
					F_c = 1;
					F_d = 2;
          MOROR_MOVE_MAX(-1600*10,300,350);
					if(JIXIEBI == 26)
					{
							JIXIEBI = 27;
					}         
				}
				if(SPEET_BUSHU_STOP == 5003)  //旋转准备加工
				{
					F_Yaw = 200;
					F_Ybw = -200;
					F_a = 1;
					F_b = 1;
					F_c = 2;
					F_d = 2;
          MOROR_MOVE_MAX(-1600*10,300,350);
					if(JIXIEBI == 27)
					{
							JIXIEBI = 28;
					}         
				}
				if(SPEET_BUSHU_STOP == 5004)  //放置
				{
					F_Yaw = 200;
					F_Ybw = -200;
					if(JIXIEBI == 28)
					{
							JIXIEBI = 29;
			 		}
          if(GG == 61)
          {
          	 Pick_Up = 2;
          	 GG = 21;
          }
				}
				if(SPEET_BUSHU_STOP == 6000)  //抓取
				{
					if(JIXIEBI == 29)
					{
							JIXIEBI = 30;
					}       
          if(GG == 61)
          {
          	 Pick_Up = 3;
          	 GG = 50;
          }					
				}
				if(SPEET_BUSHU_STOP == 7000)  //走到拐角
				{					
					F_e = 55;
					F_a = 1;
					F_b = 1;
					F_c = 2;
					F_d = 2;
				  MOROR_MOVE_MAX(-1600*10,300,350);
					if(JIXIEBI == 30)
					{
							JIXIEBI = 31;
					}       
				}
				if(SPEET_BUSHU_STOP == 7001)  //旋转准备加工
				{
					F_e = 00;
					F_Yaw = 200;
					F_Ybw = -200;
					F_a = 1;
					F_b = 1;
					F_c = 2;
					F_d = 2;
          MOROR_MOVE_MAX(-1600*10,300,350);
					if(JIXIEBI == 31)
					{
							JIXIEBI = 32;
					}         
				}
				if(SPEET_BUSHU_STOP == 7002)  //到堆料区
				{
					F_Yaw = 89;
					F_Ybw = 91;
					F_a = 2;
					F_b = 1;
					F_c = 1;
					F_d = 2;
          MOROR_MOVE_MAX(-1600*10,300,350);
					if(JIXIEBI == 32)
					{
							JIXIEBI = 33;
					}         
				}
				if(SPEET_BUSHU_STOP == 7003)  //放置
				{
					F_Yaw = 200;
					F_Ybw = -200;
					if(JIXIEBI == 34)
					{
							JIXIEBI = 35;
			 		}
          if(GG == 61)
          {
          	 Pick_Up = 6;
          	 GG = 21;
          }
				}
				if(SPEET_BUSHU_STOP == 8000)  //准备去下一个拐角找转盘
				{
					F_Yaw = 89;
					F_Ybw = 91;
					F_a = 2;
					F_b = 1;
					F_c = 1;
					F_d = 2;
          MOROR_MOVE_MAX(-1600*10,300,350);
					if(JIXIEBI == 36)
					{
							JIXIEBI = 37;
					}         
				}
				if(SPEET_BUSHU_STOP == 8001)  //旋转准备回家
				{
					F_e = 00;
					F_Yaw = 200;
					F_Ybw = -200;
					F_a = 1;
					F_b = 1;
					F_c = 2;
					F_d = 2;
          MOROR_MOVE_MAX(-1600*10,300,350);
					if(JIXIEBI == 37)
					{
							JIXIEBI = 38;
					}         
				}
				if(SPEET_BUSHU_STOP == 8002)   //回家
				{
					F_Yaw = 2;
					F_Ybw = -2;
					F_a = 1;
					F_b = 2;
					F_c = 2;
					F_d = 1;
					if(JIXIEBI == 38)
					{
							JIXIEBI = 39;
					} 
						  MOROR_MOVE_MAX(-1600*1,50,25);				
				}
				if(SPEET_BUSHU_STOP == 8003)   //最终侧向回家
				{
					F_Yaw = 2;
					F_Ybw = -2;
					F_a = 2;
					F_b = 2;
					F_c = 1;
					F_d = 1;
					MOROR_MOVE_MAX(-1600*10,300,350);
					if(JIXIEBI == 39)
					{
							JIXIEBI = 40;
					}  
          PWM_DISABLE();					
				}
		}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		
		TIM_ClearITPendingBit(TIM7, TIM_IT_Update);  //清除中断标志位
	}
}


