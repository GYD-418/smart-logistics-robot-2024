#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "chuankou.h"
#include "OLED.h"
#include "Motor.h"
#include "PWM.h"
#include "Delay.h"
#include "Timer.h"
#include "WZJZ.h"
#include "Serial.h"
#include "Serial3.h"
#include "Servo.h"

//串口一蓝牙机械臂  串口二摄像头 串口三语音
uint8_t RxData;
int s;
int PWM_ENA;
int8_t Speed;
int ARR, PSC , CCR;
int MC,QWZ;
int x;
float SHJh,SHJz;
float  RXH , RXZ;
float  RHD,RZD;   //串口接收横纵坐标值
int YVY;
int YUY =12;
//int GG;
int CFG;
int chang;
int kuan;
int ECR = 16;
int ECG = 16;
int JIXIEBI = 11;
int cot;
int RDX;
int TZCS;  //无故停止 ， 测试标志位
int FC;   // 第三次判断没有跳出 解决方法
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
	 OLED_Init();
	 ARR = 200;
   PSC = 720;
	 CCR = 50;
	 QWZ = 100000;
	 Motor_Init();
	 Motor_InitD();
	 WYF = 16;
	 IG = 19;
	 PWM_Init(ARR,PSC,CCR);    //ARR  PSC  CCR   arr过高会堵转  f越高脉冲越多
	 PWM_Init1(ARR,PSC,CCR);
	 PWM_Init2(ARR,PSC,CCR);
	 PWM_Init3(ARR,PSC,CCR);
	 Timer_Init(150,720);
//	 Timer_Init_4(240,720);
	 uartInit(9600);
	 Serial_Init3();
	 Serial_Init22();
	 ActionGroup0();   //抓取 100   初始状态103
	while (1)
	{					
//				ActionGroup1();//矫正结束抓取

//						OLED_ShowNum(2, 8,((hexToDec(Serial_RxPacket_2[0])*100)+(hexToDec(Serial_RxPacket_2[7])*10)+(hexToDec(Serial_RxPacket_2[8]))), 3);
//						OLED_ShowNum(2, 8,((hexToDec(Serial_RxPacket_2[9])*100)+(hexToDec(Serial_RxPacket_2[10])*10)+hexToDec(Serial_RxPacket_2[11])), 3);
//						OLED_ShowNum(3, 8,((hexToDec(Serial_RxPacket_2[0])*100)+(hexToDec(Serial_RxPacket_2[1])*10)+(hexToDec(Serial_RxPacket_2[2]))), 3);
//						OLED_ShowNum(4, 8,((hexToDec(Serial_RxPacket_2[3])*100)+(hexToDec(Serial_RxPacket_2[4])*10)+(hexToDec(Serial_RxPacket_2[5]))),3);						
//		
				OLED_ShowNum(1, 1,TZCS,4);					//46

//				OLED_ShowNum(1, 1,hexToDec_mv(Serial_RxPacket_2[0]),4);					//46
				OLED_ShowHexNum(2, 1,((hexToDec(Serial_RxPacket_2[1])*100)+(hexToDec(Serial_RxPacket_2[2])*10)+hexToDec(Serial_RxPacket_2[3])),4);//175   155
				OLED_ShowNum(3,1,GG, 3);
				OLED_ShowNum(4, 1,hexToDec_mv(Serial_RxPacket_2[0]),4);					//264   133


//				RDX = Serial_RxPacket_2[12];
        
		
		    //串口一蓝牙机械臂 串口二摄像头 串口三语音
		
		    /*蓝牙控制   串口1*/
			
		 }
}
void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)  //获取中断标志位
	{
				/*****************************第一次判断，横走  opmvcv***********************************************/
		        if(GG == 99)
						{
							PWM_DISABLE();
						}

		      if((Serial_RxPacket_3[0]==0x13) && JIXIEBI == 11)   //蓝牙
					{
						Serial_SendByte3(0x06);
						GG = 55;          
						JIXIEBI = 13;								
					}
         	if((Serial_RxPacket_2[4]==0x01) && YUY == 12)  //语音识别启动
					{					
						GG = 55;          
						YUY = 13;		
					}  
					if(Serial_RxPacket_3[0]==0x18)   //stop
					{
						PWM_DISABLE();
						YUY = 12;
						GG = 0;
						IG = 0;
					}
					if(Serial_RxPacket_2[4]==0x02)   //stop
					{
						PWM_DISABLE();
						GG = 0;
						IG = 0;
					}

					if(Serial_RxPacket_3[0]==0x14)  //前   蓝牙
					{
						PWM_ENABLE();
						Motor_SetSpeed(1);   //右下      前
						Motor_SetSpeed1(0);  //左下
						Motor_SetSpeed2(1);  //右上
						Motor_SetSpeed3(0);  //左上					
					}
					if(Serial_RxPacket_3[0]==0x15)  //后
					{
						PWM_ENABLE();
						Motor_SetSpeed(0);   //右下      后
						Motor_SetSpeed1(1);  //左下
						Motor_SetSpeed2(0);  //右上     //0000000000000000
						Motor_SetSpeed3(1);  //左上					//000000000000000000000
					}
					if(Serial_RxPacket_3[0]==0x16)  //左
					{
						PWM_ENABLE();
						Motor_SetSpeed(1);   //右下
						Motor_SetSpeed1(1);  //左下
						Motor_SetSpeed2(0);  //右上
						Motor_SetSpeed3(0);  //左上					
					}
					if(Serial_RxPacket_3[0]==0x17)  //右
					{
						PWM_ENABLE();
						Motor_SetSpeed(0);   //右下
						Motor_SetSpeed1(0);  //左下
						Motor_SetSpeed2(1);  //右上
						Motor_SetSpeed3(1);  //左上				
					}
/*********避障*********************************************************/
					if(Serial_RxPacket_3[0] == 0x19)
					{
							if(Serial_RxPacket_3[1] == 0x09 && Serial_RxPacket_3[2] == 0x09)     //前避障距离大
							{
								PWM_DISABLE();
							}
							if(Serial_RxPacket_3[1] == 0x07 && Serial_RxPacket_3[2] == 0x09)     //前避障距离大
							{
								Serial_SendByte3(0x01);
								PWM_ENABLE();
								Motor_SetSpeed(0);   //右下      后
								Motor_SetSpeed1(1);  //左下
								Motor_SetSpeed2(0);  //右上
								Motor_SetSpeed3(1);  //左上					
							}
							if(Serial_RxPacket_3[2] == 0x08 && Serial_RxPacket_3[1] == 0x09)  //右侧避障距离小
							{
								Serial_SendByte3(0x01);
								PWM_ENABLE();
								Motor_SetSpeed(0);   //右下
								Motor_SetSpeed1(0);  //左下
								Motor_SetSpeed2(1);  //右上
								Motor_SetSpeed3(1);  //左上								
							}
//							if(Serial_RxPacket_3[2] == 0x08 && Serial_RxPacket_3[1] == 0x07)  //右侧避障距离小
//							{
//								Serial_SendByte3(0x01);
//								PWM_ENABLE();
//								Motor_SetSpeed(1);   //右下
//								Motor_SetSpeed1(1);  //左下
//								Motor_Enable_1();
//							}
							
					}
					if(GG == 55)
					{
						if((((hexToDec(Serial_RxPacket_2[1])*100)+(hexToDec(Serial_RxPacket_2[2])*10)+hexToDec(Serial_RxPacket_2[3])) >310) && (((hexToDec(Serial_RxPacket_2[1])*100)+(hexToDec(Serial_RxPacket_2[2])*10)+hexToDec(Serial_RxPacket_2[3])) < 490))//zuo
						{							
							Motor_SetSpeed(1);   //右下
							Motor_SetSpeed1(1);  //左下
							Motor_SetSpeed2(0);  //右上
							Motor_SetSpeed3(0);  //左上			
							PWM_ENABLE();							
						}
						if((((hexToDec(Serial_RxPacket_2[1])*100)+(hexToDec(Serial_RxPacket_2[2])*10)+hexToDec(Serial_RxPacket_2[3])) <290) && (((hexToDec(Serial_RxPacket_2[1])*100)+(hexToDec(Serial_RxPacket_2[2])*10)+hexToDec(Serial_RxPacket_2[3])) > 150))  //you
						{							
							Motor_SetSpeed(0);   //右下
							Motor_SetSpeed1(0);  //左下
							Motor_SetSpeed2(1);  //右上
							Motor_SetSpeed3(1);  //左上					
							PWM_ENABLE();
						}
						if((((hexToDec(Serial_RxPacket_2[1])*100)+(hexToDec(Serial_RxPacket_2[2])*10)+hexToDec(Serial_RxPacket_2[3])) >= 290) && (((hexToDec(Serial_RxPacket_2[1])*100)+(hexToDec(Serial_RxPacket_2[2])*10)+hexToDec(Serial_RxPacket_2[3])) <= 310))  //后
						{
							PWM_DISABLE();
							GG = 71;
						}	
						else 
						{							
							Motor_SetSpeed(0);   //右下
							Motor_SetSpeed1(0);  //左下
							Motor_SetSpeed2(0);  //右上
							Motor_SetSpeed3(0);  //左上		
							PWM_ENABLE();							
						} 
					}

/***************************************************************************************************************/				
				/*****************************第二次判断，前后走***********************************************/
					if(Serial_RxPacket_2[5] == 0x02)  //干垃圾
					{
								if(GG == 71)
								{
									if((((hexToDec(Serial_RxPacket_2[1])*100)+(hexToDec(Serial_RxPacket_2[2])*10)+hexToDec(Serial_RxPacket_2[3])) >269) && (((hexToDec(Serial_RxPacket_2[1])*100)+(hexToDec(Serial_RxPacket_2[2])*10)+hexToDec(Serial_RxPacket_2[3])) < 490))//zuo
									{							
										Motor_SetSpeed(1);   //右下
										Motor_SetSpeed1(1);  //左下
										Motor_SetSpeed2(0);  //右上
										Motor_SetSpeed3(0);  //左上			
										PWM_ENABLE();							
									}
									if((((hexToDec(Serial_RxPacket_2[1])*100)+(hexToDec(Serial_RxPacket_2[2])*10)+hexToDec(Serial_RxPacket_2[3])) <259) && (((hexToDec(Serial_RxPacket_2[1])*100)+(hexToDec(Serial_RxPacket_2[2])*10)+hexToDec(Serial_RxPacket_2[3])) > 150))  //you
									{							
										Motor_SetSpeed(0);   //右下
										Motor_SetSpeed1(0);  //左下
										Motor_SetSpeed2(1);  //右上
										Motor_SetSpeed3(1);  //左上					
										PWM_ENABLE();
									}
									if((((hexToDec(Serial_RxPacket_2[1])*100)+(hexToDec(Serial_RxPacket_2[2])*10)+hexToDec(Serial_RxPacket_2[3])) >= 259) && (((hexToDec(Serial_RxPacket_2[1])*100)+(hexToDec(Serial_RxPacket_2[2])*10)+hexToDec(Serial_RxPacket_2[3])) <= 269))  //后
									{
										PWM_DISABLE();
										GG = 38;
									}	
									else 
									{							
										Motor_SetSpeed(0);   //右下
										Motor_SetSpeed1(0);  //左下
										Motor_SetSpeed2(0);  //右上
										Motor_SetSpeed3(0);  //左上		
										PWM_ENABLE();							
									} 
								}
							/***************************************************************************************************/
							/**************************第三次判断************************************************************************/
								if(GG == 38)
								{
										if((hexToDec_mv(Serial_RxPacket_2[0])<128))  //后
										{									
											Motor_SetSpeed(1);   //右下      前
											Motor_SetSpeed1(0);  //左下
											Motor_SetSpeed2(1);  //右上
											Motor_SetSpeed3(0);  //左上					
											PWM_ENABLE();
										}
										if((hexToDec_mv(Serial_RxPacket_2[0])>138)&&(hexToDec_mv(Serial_RxPacket_2[0])<350))
										{								
											Motor_SetSpeed(0);   //右下      后
											Motor_SetSpeed1(1);  //左下
											Motor_SetSpeed2(0);  //右上
											Motor_SetSpeed3(1);  //左上					
											PWM_ENABLE();						
										}
										if((hexToDec_mv(Serial_RxPacket_2[0])>=128) && (hexToDec_mv(Serial_RxPacket_2[0])<=138))  //后
										{
											PWM_DISABLE();
											TZCS = 10;	
											GG = 56;
											TZCS = 11;
										}
								}
							/****************************************************************************************************************/
							/***************************第四次判断***********************************************************************************/
								if(GG == 56)
								{
									  TZCS = 13;
										if((((hexToDec(Serial_RxPacket_2[1])*100)+(hexToDec(Serial_RxPacket_2[2])*10)+hexToDec(Serial_RxPacket_2[3])) >=269))//zuo
										{		
                       TZCS = 1;											
											 Motor_SetSpeed(1);   //右下
											 Motor_SetSpeed1(1);  //左下
											 Motor_SetSpeed2(0);  //右上
											 Motor_SetSpeed3(0);  //左上	
											 PWM_ENABLE();								
										}
										if((((hexToDec(Serial_RxPacket_2[1])*100)+(hexToDec(Serial_RxPacket_2[2])*10)+hexToDec(Serial_RxPacket_2[3])) <=259))  //you
										{		
											TZCS = 2;
											Motor_SetSpeed(0);   //右下
											Motor_SetSpeed1(0);  //左下
											Motor_SetSpeed2(1);  //右上
											Motor_SetSpeed3(1);  //左上		
											PWM_ENABLE();								
										}
										if((((hexToDec(Serial_RxPacket_2[1])*100)+(hexToDec(Serial_RxPacket_2[2])*10)+hexToDec(Serial_RxPacket_2[3])) >259) && (((hexToDec(Serial_RxPacket_2[1])*100)+(hexToDec(Serial_RxPacket_2[2])*10)+hexToDec(Serial_RxPacket_2[3])) < 269))  //后
										{
											TZCS = 3;
											GG = 22;
										}	
								}
								if(GG == 22)
								{
									    PWM_DISABLE();
											Serial_SendByte3(0x04);    //语音 检测到干垃圾
											ActionGroup2();   //抓取 100
											GG = 55;
								}
						}
/*****************************************************************************************************************************************************************************************/

					if(Serial_RxPacket_2[5] == 0x03)   //电池
					{
//							if(GG == 38)
//							{
//								if((((hexToDec(Serial_RxPacket_2[6])*100)+(hexToDec(Serial_RxPacket_2[7])*10)+hexToDec(Serial_RxPacket_2[8])) >=260) && (((hexToDec(Serial_RxPacket_2[6])*100)+(hexToDec(Serial_RxPacket_2[7])*10)+hexToDec(Serial_RxPacket_2[8])) <=400))  //后
//								{							
//									Motor_SetSpeed(0);   //右下      前
//									Motor_SetSpeed1(1);  //左下
//									Motor_SetSpeed2(0);  //右上
//									Motor_SetSpeed3(1);  //左上			
//									PWM_ENABLE();							
//								}
//								if((((hexToDec(Serial_RxPacket_2[6])*100)+(hexToDec(Serial_RxPacket_2[7])*10)+hexToDec(Serial_RxPacket_2[8])) <=250) && (((hexToDec(Serial_RxPacket_2[6])*100)+(hexToDec(Serial_RxPacket_2[7])*10)+hexToDec(Serial_RxPacket_2[8])) >= 100))  //后
//								{							
//									Motor_SetSpeed(1);   //右下      前
//									Motor_SetSpeed1(0);  //左下
//									Motor_SetSpeed2(1);  //右上
//									Motor_SetSpeed3(0);  //左上	
//									PWM_ENABLE();							
//								}
//								if((((hexToDec(Serial_RxPacket_2[6])*100)+(hexToDec(Serial_RxPacket_2[7])*10)+hexToDec(Serial_RxPacket_2[8])) > 250) && (((hexToDec(Serial_RxPacket_2[6])*100)+(hexToDec(Serial_RxPacket_2[7])*10)+hexToDec(Serial_RxPacket_2[8])) < 260))  //后
//								{
//									PWM_DISABLE();
//									GG = 46;
//								}
//							}
							/***************************************************************************************************/
								if(GG == 71)
								{
									if((((hexToDec(Serial_RxPacket_2[1])*100)+(hexToDec(Serial_RxPacket_2[2])*10)+hexToDec(Serial_RxPacket_2[3])) >180) && (((hexToDec(Serial_RxPacket_2[1])*100)+(hexToDec(Serial_RxPacket_2[2])*10)+hexToDec(Serial_RxPacket_2[3])) < 490))//zuo
									{							
										Motor_SetSpeed(1);   //右下
										Motor_SetSpeed1(1);  //左下
										Motor_SetSpeed2(0);  //右上
										Motor_SetSpeed3(0);  //左上			
										PWM_ENABLE();							
									}
									if((((hexToDec(Serial_RxPacket_2[1])*100)+(hexToDec(Serial_RxPacket_2[2])*10)+hexToDec(Serial_RxPacket_2[3])) <170) && (((hexToDec(Serial_RxPacket_2[1])*100)+(hexToDec(Serial_RxPacket_2[2])*10)+hexToDec(Serial_RxPacket_2[3])) > 150))  //you
									{							
										Motor_SetSpeed(0);   //右下
										Motor_SetSpeed1(0);  //左下
										Motor_SetSpeed2(1);  //右上
										Motor_SetSpeed3(1);  //左上					
										PWM_ENABLE();
									}
									if((((hexToDec(Serial_RxPacket_2[1])*100)+(hexToDec(Serial_RxPacket_2[2])*10)+hexToDec(Serial_RxPacket_2[3])) >= 170) && (((hexToDec(Serial_RxPacket_2[1])*100)+(hexToDec(Serial_RxPacket_2[2])*10)+hexToDec(Serial_RxPacket_2[3])) <= 180))  //后
									{
										PWM_DISABLE();
										GG = 38;
									}	
									else 
									{							
										Motor_SetSpeed(0);   //右下
										Motor_SetSpeed1(0);  //左下
										Motor_SetSpeed2(0);  //右上
										Motor_SetSpeed3(0);  //左上		
										PWM_ENABLE();							
									} 
								}
							
							/**************************第三次判断************************************************************************/
							if(GG == 38)
							{
								if((hexToDec_mv(Serial_RxPacket_2[0])<=150))  //后
								{									
									Motor_SetSpeed(1);   //右下      前
									Motor_SetSpeed1(0);  //左下
									Motor_SetSpeed2(1);  //右上
									Motor_SetSpeed3(0);  //左上					
									PWM_ENABLE();
								}
								if((hexToDec_mv(Serial_RxPacket_2[0])>=160)&&(hexToDec_mv(Serial_RxPacket_2[0])<=350))  //后
								{								
									Motor_SetSpeed(0);   //右下      后
									Motor_SetSpeed1(1);  //左下
									Motor_SetSpeed2(0);  //右上
									Motor_SetSpeed3(1);  //左上					
									PWM_ENABLE();						
								}
								if((hexToDec_mv(Serial_RxPacket_2[0])>150) && (hexToDec_mv(Serial_RxPacket_2[0])<160))  //后
								{
									PWM_DISABLE();
									GG = 56;
								}
							}
							/****************************************************************************************************************/
							/***************************第四次判断***********************************************************************************/
							if(GG == 56)
							{
									if((((hexToDec(Serial_RxPacket_2[1])*100)+(hexToDec(Serial_RxPacket_2[2])*10)+hexToDec(Serial_RxPacket_2[3])) >=180))//zuo
									{							
										 Motor_SetSpeed(1);   //右下
										 Motor_SetSpeed1(1);  //左下
										 Motor_SetSpeed2(0);  //右上
										 Motor_SetSpeed3(0);  //左上	
										 PWM_ENABLE();								
									}
									if((((hexToDec(Serial_RxPacket_2[1])*100)+(hexToDec(Serial_RxPacket_2[2])*10)+hexToDec(Serial_RxPacket_2[3])) <=170))  //you
									{							
										Motor_SetSpeed(0);   //右下
										Motor_SetSpeed1(0);  //左下
										Motor_SetSpeed2(1);  //右上
										Motor_SetSpeed3(1);  //左上		
										PWM_ENABLE();								
									}
									if((((hexToDec(Serial_RxPacket_2[1])*100)+(hexToDec(Serial_RxPacket_2[2])*10)+hexToDec(Serial_RxPacket_2[3])) >170) && (((hexToDec(Serial_RxPacket_2[1])*100)+(hexToDec(Serial_RxPacket_2[2])*10)+hexToDec(Serial_RxPacket_2[3])) <180))  //后
									{
										GG = 22;
									}	
							}
							if(GG == 22)    //电池
							{
								PWM_DISABLE();
								Serial_SendByte3(0x05);    //语音 检测到有害垃圾
								ActionGroup1();   //抓取 100
								GG = 55;
							}
							
					}
					
				/***********************第三次判断，横着走****************************************/

		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);  //清除中断标志位
	}
}
