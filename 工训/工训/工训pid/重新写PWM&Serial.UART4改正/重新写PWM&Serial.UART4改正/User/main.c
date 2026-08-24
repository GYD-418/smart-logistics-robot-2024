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
int cot;
int RDX;
int KeyNum;
int GG;
int x1,x2,x3,x1_last;
int w1,w2,w3;
int y,cvc,mvc;
int next = 0;
int sepan = 0x31;
int cot_last;
int MUB; //此参数为转向时候的标志位 出发时0 转90度加1
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
	 Key_Init();
	 OLED_Init();
	 ARR = 140;
   PSC = 150;
	 CCR = 50;
	 Timer_Init(220,720);
//	 Timer_Init_4(240,720);
	 Serial_Init3();
	 Serial_Init22();	
	 Serial4_Init();
	 Motor_Init();
	 Motor_InitD();

	while (1)
	{			 
		 yaw=(float)stcAngle.Angle[2]/32768*180;
		 PWM_Init(ARRR,PSC,CCR);    //ARR  PSC  CCR   arr过高会堵转  f越高脉冲越多
		 PWM_Init1(ARRL,PSC,CCR);
		 PWM_Init2(ARRR,PSC,CCR);
		 PWM_Init3(ARRL,PSC,CCR);
		 // 实际减目标是负数需要左转 右大左小
		 // 实际减目标是正数需要右转 右小左大
     ARRR =ARR - WeiZhi_Zong_PID(MUB*90 , yaw);    //目标值 实际值 右
		 ARRL =ARR + WeiZhi_Zong_PID(MUB*90 , yaw);    //目标值 实际值 左
     
 		 OLED_ShowSignedNum(1,1,ARR,8);
		 OLED_ShowSignedNum(2,1,yaw,3);
		 KeyNum = Key_GetNum();
	}
}

void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)  //获取中断标志位
	{
		cot++;
		if((cot >= 0) && (cot <= 3000))  //时间
		{
				MUB = 0;		
				Motor_SetSpeed(1);   //右下      左
				Motor_SetSpeed1(1);  //左下  
				Motor_SetSpeed2(1);  //右上	
				Motor_SetSpeed3(1);  //左上		
				PWM_ENABLE();							 
		}
		if((cot>3000)&&(cot<=6000))
		{
				MUB = 1;	
				Motor_SetSpeed(1);   //右下      左
				Motor_SetSpeed1(1);  //左下  
				Motor_SetSpeed2(1);  //右上	
				Motor_SetSpeed3(1);  //左上		
				PWM_ENABLE();								
		}
		if((cot>6000)&&(cot<=9000))
		{
				MUB = 2;		
				Motor_SetSpeed(1);   //右下      左
				Motor_SetSpeed1(1);  //左下  
				Motor_SetSpeed2(1);  //右上	
				Motor_SetSpeed3(1);  //左上		
				PWM_ENABLE();								
		}
		if((cot>9000)&&(cot<=12500))
		{
				MUB = -1;		
				Motor_SetSpeed(1);   //右下      左
				Motor_SetSpeed1(1);  //左下  
				Motor_SetSpeed2(1);  //右上	
				Motor_SetSpeed3(1);  //左上		
				PWM_ENABLE();								
		}
			if(cot>=12500)
			{
				Motor_SetSpeed(2);   //右下      左
				Motor_SetSpeed1(2);  //左下  
				Motor_SetSpeed2(2);  //右上	
				Motor_SetSpeed3(2);  //左上		
				PWM_ENABLE();								
			}
//到达物料区停止，并掷出一个标志位sepan 对比像素点变化与定时器计时时间差
//cvc = Serial_RxPacket_3[2+next]时开始矫正
			if(sepan == 0x31&&GG==55)   //
			{
				cot_last = cot;
				x1 = x1_last;
				if((cot - cot_last <= 200)&&(x1 - x1_last <= 10))   //转盘停止运作 置标志位开始矫正
				{
					sepan = 0x41;
				}
				else
				{
					PWM_DISABLE();
				}
			}
			if((sepan = 0x41) && (cvc = Serial_RxPacket_3[2+next]))
			{
				GG = 55;
			}
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
//												sepan = 0x31;
//								}
//						}
///*****************************************************************************************************************************************************************************************/

					

		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);  //清除中断标志位
	}
}

