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
int CFG;
int chang;
int kuan;
int ECR = 16;
int ECG = 16;
int JIXIEBI = 11;
int cot;
int RDX;
int KeyNum;

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

int main(void)
{	   
	 OLED_Init();
	 ARR = 100;
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
	 Timer_Init(220,720);
//	 Timer_Init_4(240,720);
	 uartInit(9600);
	 Serial_Init3();
	 Serial_Init22();
	 ActionGroup0();   //抓取 100   初始状态103
	while (1)
	{					


	}
}

void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)  //获取中断标志位
	{

		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);  //清除中断标志位
	}
}
