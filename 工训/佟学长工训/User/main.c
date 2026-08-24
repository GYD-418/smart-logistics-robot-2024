#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "usart.h"
#include "Motor.h"  
#include "Motor_Init.h"  
#include "HWT101.h"
#include "tim.h"
#include "pid.h"
#include "control2.h"
#include "control.h"
#include "key.h"
//extern	uint8_t	motor_sta		;	
/*************************************************************************

*@note:	
//					----直走     从右往左看分别填进4，3，2，1
//					++++后退
//					--++左转
//					++--右转
//					-++-右移
//					+--+左移
            左前2，3正
						右前1，4正
						左后1，4反
						右后2，3反	
左前电机 +为正转   —为反转			
*@note舵机——转盘 		TIM_SetCompare1(TIM4,242); //第三个盘子   放蓝色
											TIM_SetCompare1(TIM4,151);	//第二个盘子  放绿色
											TIM_SetCompare1(TIM4,67);  //第一个盘子   放红色
*@note舵机——肘子			TIM_SetCompare2(TIM4,145);  //抓物块儿的位置	
											TIM_SetCompare2(TIM4,245);  //放物块儿的位置
*@note舵机——爪子     TIM_SetCompare3(TIM4,58);	  //夹紧物块儿的位置
											TIM_SetCompare3(TIM4,85);	  //松物块儿的位置
											
*@note步进电机         MOTOR_Move_5(-9150,150,150,150)  //现在：负往下正往上, 给360大约是上升1mm，结构改了因为。



		MOTOR_Move_5(-3200,150,150,150);		//抓物块儿时下降高度
		TIM_SetCompare4(TIM4,60);               //抓物块儿时合抓大小
		MOTOR_Move_5(3900,150,150,150);	//	3900  放物块儿时下降高度
		TIM_SetCompare4(TIM4,140);          //松抓大小

*@note 四个电机
			 MOTOR_Move_1(6050,150,150,150);       //需要时调用     电机四，正为反
			 MOTOR_Move_2(6050,150,150,150);       //需要时调用        电机三， 正为反
			 MOTOR_Move_3(6050,150,150,150);       //需要时调用        电机二，正为反
			 MOTOR_Move_4(6005,150,150,150);       //需要时调用        电机一，正为反
			 
			 
			 x280,y210

**************************************************************************/

int main(void)
{
 	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	KEY_Init();
  MOTOR5_GPIO_Init();
	MOTOR_GPIO_Init();
	MOTOR_TIM_Init();
	MOTOR5_TIM2_Init();
	Basic_TIM_Init(1999,719);
	MYUSART1_Init(9600);
	PID_Init_Angle();
  USART3_Init(115200);
	MYUSART2_Init(115200);
  TIM5_Init(999,71);
	MYUART4_Init(9600);
//	 MOTOR_Move_1(-200,100,150,150);       //需要时调用    800   240
//	 MOTOR_Move_2(-200,100,150,150);       //需要时调用
//	 MOTOR_Move_3(200,100,150,150);       //需要时调用
//	 MOTOR_Move_4(200,100,150,150);       //需要时调用			2000  9000
//Set_ang();
//Speed_Test_State.T_state = Acting;
//		MOTOR_Move_5(30000,150,150,150);		
//	Delay_ms(10000);
//	
//	TIM_SetCompare2(TIM4,145);  //抓物块儿的位置
//	Delay_ms(2000);+
//	TIM_SetCompare3(TIM4,150);  //松物块儿的位置
//	Delay_ms(2000);	
//			TIM_SetCompare2(TIM4,245);
//			Delay_ms(500);
//      TIM_SetCompare1(TIM4,242);
//		TIM_SetCompare4(TIM4,140);
//			Delay_ms(5000);
//			MOTOR_Move_5(-4000,150,150,150);    //4000
//			Delay_ms(5000);
	PID_CREATE(&K210y_pid, 0.2, 0.0,0.0, 0, 7, 7, 15); 
	PID_CREATE(&K210x_pid, 0.2,0.00001,0.2, 0, 7, 7, 15); 
TIM_SetCompare3(TIM4,98);	  //松物块儿的位置
//TIM_SetCompare2(TIM4,145); 
//				Choose = 1; 
//				Send_To_K210();		
////			MOTOR_Move_5(53632,150,150,250);            //53632           //11920   
//			MOTOR_Move_5(19420,150,150,250);            //53632           //11920      25977
//int a = 0;

  while (1)
	{	
//		MOTOR_Move_5(30000,150,150,105);		
//		Delay_ms(8000);
//		MOTOR_Move_5(-30000,150,150,300);		
//		Delay_ms(8000);
		
//		CJG_ZhuaWuKuaiEr_TongYong();
//			GreenWuLiao();
//		Key_Scan();
//		if(KEY1 == 0&&a == 0)
//		{
//			a++;
////			 MOTOR_Move_1(-50,100,150,150);       //需要时调用    800   240
////			 MOTOR_Move_2(-50,100,150,150);       //需要时调用
////			 MOTOR_Move_3(-50,100,150,150);       //需要时调用
////			 MOTOR_Move_4(-50,100,150,150);       //需要时调用			2000  9000 //52532           //11920      25977
////		MOTOR_Move_5(-52532,650,650,650);         //下降
//			MOTOR_Move_5(-500,250,250,250);            //上升8960*2 500*3 19420
//		}
		Main_Mission();
//			CJG_FangWuKuaiEr_TongYong();
	}
}
















