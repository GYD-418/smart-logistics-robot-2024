#include "control.h"	
#include "pid.h"
#include "HWT101.h"
#include "Motor_Init.h"
#include "niming.h"
#include "Motor.h"
#include "control2.h"
#include "delay.h"
#include "usart.h"
#include "tim.h"
#include "usart.h"
#include "key.h"
#include "stdlib.h"
uint32_t set_speed_1  = 150;         // 最大速度设置 单位为0.1rad/sec
uint32_t step_accel_1 =100;         // 加速度 单位为0.1rad/sec^2
uint32_t step_decel_1 = 100;         // 减速度 单位为0.1rad/sec^2

uint32_t set_speed_2  = 150;         // 最大速度设置 单位为0.1rad/se
uint32_t step_accel_2 =100;         // 加速度 单位为0.1rad/sec^2
uint32_t step_decel_2 = 100;         // 减速度 单位为0.1rad/sec^2

uint32_t set_speed_3  = 150;         // 最大速度设置 单位为0.1rad/sec     
uint32_t step_accel_3 =100;         // 加速度 单位为0.1rad/sec^2
uint32_t step_decel_3 = 100;         // 减速度 单位为0.1rad/sec^2

uint32_t set_speed_4  = 150;         // 最大速度设置 单位为0.1rad/sec
uint32_t step_accel_4 =100;         // 加速度 单位为0.1rad/sec^2
uint32_t step_decel_4 = 100;         // 减速度 单位为0.1rad/sec^2

int ZhuanQuShuLiang = 0;
int WuKuaiShuLiang = 0;
int x = 0;
int JiaoDu = 0;
int YouXianJi = 0;   //用于第一次抓取在转盘区的物料
int YouXianJi_2 = 0;   //用于第一次将物料放在圆环处的 优先级标志位
int YouXianJi_3 = 0;   //用于第一次将放下的物料抓起。
int Choose = 0; //用于选择发送什么字符，以选择k210识别什么东西
int Cishu_F1 = 0;  //用于限制发送次数
int Cishu_F2 = 0;  //用于限制发送次数
int Cishu_F3 = 0;  //用于限制发送次数
int Cishu_F4 = 0;  //用于限制发送次数
int Cishu_F5 = 0;  //用于限制发送次数
int Main_Mission_State = 0;
int Second = 0;  //用于选择第二次抓取的顺序
 void Main_Mission(void)
{
	Key_Scan();
	yaw=(float)stcAngle.Angle[2]/32768*180;
		switch(test)
		{
			case 1://直走任务
				if(Speed_Test_State.T_state == Acting)
				{
					test_1();			
				}
				else if(Speed_Test_State.T_state == End)
				{
					Speed_Test_State.T_state = Acting;
					test = 2;
				}
				break;
			case 2://直走任务
				if(Speed_Test_State.T_state == Acting)
				{
					test_2();			
				}
				else if(Speed_Test_State.T_state == End)
				{
					Speed_Test_State.T_state = Acting;
					test = 3;
				}
				break;
			case 3://识别二维码
				if(USART2_RX_Flag == 1)//等待扫码成功，获取搬取信息					
				{
					ChuanKouPing();  
					Speed_Test_State.T_state = Acting;
				  USART_Cmd(USART2, DISABLE);                    //使能串口2			
					test = 4;
				}
				break; 	
			case 4://前往转盘区
				if(Speed_Test_State.T_state == Acting)
				{
					test_3();			
				}
				else if(Speed_Test_State.T_state == End)
				{
					Speed_Test_State.T_state = Acting;
					test = 5;
					Delay_ms(500);		
				}
				break;
			case 5://靠近转盘区
				if(Speed_Test_State.T_state == Acting)
				{
					test_4();			
				}
				else if(Speed_Test_State.T_state == End)
				{
					Speed_Test_State.T_state = Acting;
					TIM_SetCompare3(TIM4,80);   //开爪子准备识别
					test = 6;			
				}
				break;
							
			case 6://在转盘区抓取物块儿
				if( WuKuaiShuLiang <4)  //抓第一个物块儿           //(USART2_RX_BUF_2[1] == USART_RX_ZQ_BUF[1]) &&
				{
					//往下放爪子（步进电机）                           	WuKuaiShuLiang++;
					//闭合爪子（舵机）
					//往上抬爪子（步进电机）,旋转之后方，同时托盘旋转至对应位置（舵机*2）
					//往下放爪子（步进电机）
					//松爪子（舵机）
					//往上抬爪子（步进电机）,旋转爪子志前方，同时托盘复位（舵机*2） (重复三次)
					New_PanDuan();
				}
				else if(WuKuaiShuLiang>=4)
				{
					Speed_Test_State.T_state = ZhuaQuJieShu;
					Speed_Test_State.T_state = Acting;
					WuKuaiShuLiang = 0;  //清零物块儿数，方便下次使用
					test = 7;
				}
				break;
				
			case 7://等待抓取结束后，前往转角
					if(Speed_Test_State.T_state == Acting)
					{
						test_5();			
					}
					else if(Speed_Test_State.T_state == End)
					{
						Speed_Test_State.T_state = Acting;
						test = 8;
						test_count = 0;
					}	
				break;
				
			case 8:   //转90度任务
				if(Speed_Test_State.T_state == Acting)
				{					
					if(test_count<2) 
					{
						PID_Init_Angle();
						Set_ang();
						test_count++;
					}
					ZhuanJiaoDu();			
				}
				else if(Speed_Test_State.T_state == End)
				{
					Speed_Test_State.T_state = Acting;
					test = 9;
					test_count = 0;
					Reset_ang();
				}
				break;
			case 9:
				if(WuKuaiShuLiang <4)
				{
					PanDuan_2(speedx,speedy);       //抓取物块儿的任务
				}
				else 
				{
					test = 10;
					WuKuaiShuLiang = 0;  //清零以方便下次使用
					Speed_Test_State.T_state = Acting;   //可能没啥用，暂时放这儿
					Choose = 5; 
					Send_To_K210();	
					Mission_State = 0;
				}
				break;
			case 10:
				if(Speed_Test_State.T_state == Acting)
				{					
					if(test_count<1) 
					{
						PID_Init_Angle();
						Set_ang(); 
						test_count++; 
					}
					Angle_PID.Setpoint = 180;
					test_7();			
				}
				else if(Speed_Test_State.T_state == End)
				{
					Speed_Test_State.T_state = Acting;
					test = 11;
					test_count = 0;
					Mission_State = 0;
					Reset_ang();
				}
				break;
			case 11:
				if(WuKuaiShuLiang <4)
				{
					PanDuan_3(speedx,speedy);
				}
				else 
				{
					test = 12;
					WuKuaiShuLiang = 0;  //清零以方便下次使用
					Speed_Test_State.T_state = Acting;   //可能没啥用，暂时放这儿
					Choose = 5;
					Send_To_K210();						
				}
				break;
			case 12:  //返回转盘区时第一个转角
				if(Speed_Test_State.T_state == Acting)
				{					
					if(test_count<1) 
					{
						PID_Init_Angle();
						Set_ang();
						test_count++;
					}
					Angle_PID.Setpoint = 90;					
					test_8();			
				}
				else if(Speed_Test_State.T_state == End)
				{
					Speed_Test_State.T_state = Acting;
					test = 13;
					test_count = 0;
					Mission_State = 0;
					Reset_ang();
				}
				break;
			case 13: //直走
//				PanDuan_4(speedx,speedy);
				if(Speed_Test_State.T_state == Acting)
				{
					test_15();			
				}
				else if(Speed_Test_State.T_state == End)
				{
					Speed_Test_State.T_state = Acting;
					test = 14;
				}				
				break;
			case 14:
				if(Speed_Test_State.T_state == Acting)
				{					
					if(test_count<2) 
					{
						Set_ang();
						test_count++;
					}
					test_13();			
				}
				else if(Speed_Test_State.T_state == End)
				{
					Speed_Test_State.T_state = Acting;
					test = 15;
					test_count = 0;
					Reset_ang();
				}
				break;
			case 15://后退至转盘区
				if(Speed_Test_State.T_state == Acting)
				{
					test_11();			
				}
				else if(Speed_Test_State.T_state == End)
				{
					Speed_Test_State.T_state = Acting;
					test = 16;
					Second = 1; //抓第二批物块儿。
				}	
				break;
/************************************** ***************************************************************************/
			case 16://在转盘区抓取物块儿
				if( WuKuaiShuLiang <4)  //抓第一个物块儿           //(USART2_RX_BUF_2[1] == USART_RX_ZQ_BUF[1]) &&
				{
					//往下放爪子（步进电机）                           	WuKuaiShuLiang++;
					//闭合爪子（舵机）
					//往上抬爪子（步进电机）,旋转之后方，同时托盘旋转至对应位置（舵机*2）
					//往下放爪子（步进电机）
					//松爪子（舵机）
					//往上抬爪子（步进电机）,旋转爪子志前方，同时托盘复位（舵机*2） (重复三次)
					New_PanDuan();
				}
				else if(WuKuaiShuLiang>=4)
				{
					Speed_Test_State.T_state = ZhuaQuJieShu;
					Speed_Test_State.T_state = Acting;
					WuKuaiShuLiang = 0;  //清零物块儿数，方便下次使用
					test = 17;
				}
				break;
				
			case 17://等待抓取结束后，前往转角
					if(Speed_Test_State.T_state == Acting)
					{
						test_5();			
					}
					else if(Speed_Test_State.T_state == End)
					{
						Speed_Test_State.T_state = Acting;
						test = 18;
						test_count = 0;
					}	
				break;
				
			case 18:   //转90度任务
				if(Speed_Test_State.T_state == Acting)
				{					
					if(test_count<2) 
					{
						PID_Init_Angle();
						Set_ang();
						test_count++;
					}
					ZhuanJiaoDu();			
				}
				else if(Speed_Test_State.T_state == End)
				{
					Speed_Test_State.T_state = Acting;
					test_count = 0;
					Reset_ang();
					WuKuaiShuLiang = 0;
					Mission_State = 0;
					CJG_state = 0;
					test = 19;
				}
				break;
			case 19:
				if(WuKuaiShuLiang <4)
				{
					PanDuan_2(speedx,speedy);       //抓取物块儿的任务
				}
				else 
				{
					test = 20;
					WuKuaiShuLiang = 0;  //清零以方便下次使用
					Speed_Test_State.T_state = Acting;   //可能没啥用，暂时放这儿
					Choose = 5; 
					Send_To_K210();	
					Mission_State = 0;
				}
				break;
			case 20:
				if(Speed_Test_State.T_state == Acting)
				{					
					if(test_count<1) 
					{
						PID_Init_Angle();
						Set_ang(); 
						test_count++; 
					}
					Angle_PID.Setpoint = 180;
					test_7();			
				}
				else if(Speed_Test_State.T_state == End)
				{
					Speed_Test_State.T_state = Acting;
					test = 21;
					test_count = 0;
					Mission_State = 0;
					Reset_ang();
				}
				break;
			case 21:
				if(WuKuaiShuLiang <4)
				{
					PanDuan_3(speedx,speedy);
				}
				else 
				{
					test = 22;
					WuKuaiShuLiang = 0;  //清零以方便下次使用
					Speed_Test_State.T_state = Acting;   //可能没啥用，暂时放这儿
					Choose = 5;
					Send_To_K210();	
				}
				break;				
			case 22: //转角度
				if(Speed_Test_State.T_state == Acting)
				{					
					if(test_count<1) 
					{
						PID_Init_Angle();
						Set_ang(); 
						test_count++; 
					}
					Angle_PID.Setpoint = 270;
					test_7();			
				}
				else if(Speed_Test_State.T_state == End)
				{
					Speed_Test_State.T_state = Acting;
					test = 23;
					test_count = 0;
					Mission_State = 0;
					Reset_ang();
				}
				break;
			case 23:
					if(Speed_Test_State.T_state == Acting)
					{
						test_23();			
					}
					else if(Speed_Test_State.T_state == End)
					{
						Speed_Test_State.T_state = Acting;
						test = 24;
						test_count = 0;
					}	
				break;



			case 28:   //测试矫正
				if(Speed_Test_State.T_state == Acting)
				{
					
					if(test_count<1) 
					{
						Set_ang();
						test_count++;
					}
					if(Cishu_F3<1)
					{
						Cishu_F3 ++;
						Choose = 2;
						Send_To_K210();			
					}
					TEST_JiaoZheng(speedx,speedy);			
				}
				else if(Speed_Test_State.T_state == End)
				{
					Speed_Test_State.T_state = Acting;
					test = 100;
					test_count = 0;
					Choose = 5;
					Send_To_K210();			

					Reset_ang();
				}
				break;
				
			default:
				break;
		
		}

}



	int a = 0;
void test_1(void)   //出发时横着向左走
{
	static int count = 0;
	count ++;
	if(count <2)
	{
		MOTOR_Move_1(2000,210,210,150);       //需要时调用
		MOTOR_Move_2(-2000,210,210,150);       //需要时调用
		MOTOR_Move_3(-2000,210,210,150);       //需要时调用
		MOTOR_Move_4(2000,210,210,150);       //需要时调用	
	}
	//之走完后更新状态
	Delay_ms(2);
	if(motor_sta_1 == STOP)
	{
		Speed_Test_State.T_state = End;
		Delay_ms(100);
		TIM_Cmd(TIM3,DISABLE);
		TIM_SetCounter(TIM3,0);		
	}

}
void test_2(void)   //向前走前往二维码区
{
	static int count = 0;

	if(count <1)
	{
		count ++;		
		MOTOR_Move_1(-10190,200,200,150);       //需要时调用
		MOTOR_Move_2(-10190,200,200,150);       //需要时调用
		MOTOR_Move_3(-10190,200,200,150);       //需要时调用
		MOTOR_Move_4(-10190,200,200,150);       //需要时调用
		Delay_ms(200);
	}
		//之走完后更新状态
	if(motor_sta_4 == STOP)
	{
		Speed_Test_State.T_state = End;
		TIM_SetCompare2(TIM4,145);  //抓物块儿的位置
		TIM_SetCompare3(TIM4,95);
		Delay_ms(100);	
		TIM_Cmd(TIM3,DISABLE);
		TIM_SetCounter(TIM3,0);
	}

}

void test_3(void)   //向前走前往转盘区
{
	static int count = 0;
	if(count < 1)
	{
		count ++;	
		MOTOR_Move_1(-8800,210,210,150);       //需要时调用         14850
		MOTOR_Move_2(-8800,210,210,150);       //需要时调用
		MOTOR_Move_3(-8800,210,210,150);       //需要时调用
		MOTOR_Move_4(-8800,210,210,150);       //需要时调用
	}	
		//之走完后更新状态
	if(motor_sta_4 == STOP)
	{
		Delay_ms(200);
		Speed_Test_State.T_state = End;
		TIM_Cmd(TIM3,DISABLE);
		TIM_SetCounter(TIM3,0);
	}

}
void test_4(void)   //靠近转盘区
{
	static int count = 0;
	if(count < 1)
	{
		count ++;
		MOTOR_Move_1(-600,200,200,150);       //需要时调用    
		MOTOR_Move_2(600,200,200,150);       //需要时调用
		MOTOR_Move_3(600,200,200,150);       //需要时调用
		MOTOR_Move_4(-600,200,200,150);       //需要时调用
		Delay_ms(200);
	}
		//之走完后更新状态
//		Delay_ms(2);
	if(motor_sta_4 == STOP)
	{
		Speed_Test_State.T_state = End;
		Delay_ms(1000);		
		TIM_Cmd(TIM3,DISABLE);
		TIM_SetCounter(TIM3,0);
	}

}

void test_5(void)   //前往转角
{
	static int count = 0;
	if(count < 1)
	{	
		count ++;
		MOTOR_Move_1(-5750,200,200,150);       //需要时调用
		MOTOR_Move_2(-5750,200,200,150);       //需要时调用
		MOTOR_Move_3(-5750,200,200,150);       //需要时调用
		MOTOR_Move_4(-5750,200,200,150);       //需要时调用
	}
		Delay_ms(500);
		//之走完后更新状态
//		Delay_ms(2);
	if(motor_sta_4 == STOP)
	{
		count = 0;
		Speed_Test_State.T_state = End;
		Delay_ms(100);		
		TIM_Cmd(TIM3,DISABLE);
		TIM_SetCounter(TIM3,0);
	}
 
}
int z = 0;
void test_7(void)   //转90度
{
	//左转90度
	
	 Ang_cal_1();//循环调用该函数即可闭环 到达固定角度 建议周期15ms以内（陀螺仪串口通信速度也要快） 如果反应不够快速 可增加frame_period
	 if(Angle_PID.error>-0.2&&Angle_PID.error<0.2)
	 {
		 z++;
		 if(z>100)
		 {
				Speed_Test_State.T_state = End;		 			 
		 }
	 }
}

void ZhuanJiaoDu(void)   //转90度
{
	//左转90度
	 Angle_PID.Setpoint = 90;
	 Ang_cal();//循环调用该函数即可闭环 到达固定角度 建议周期15ms以内（陀螺仪串口通信速度也要快） 如果反应不够快速 可增加frame_period
	 if(Angle_PID.error>-0.2&&Angle_PID.error<0.2)
	 {
		 z++;
		 if(z>100)
		 {
				Speed_Test_State.T_state = End;		 			 
		 }
	 }

}

void ZhuaQuZhuanPanWL(void)   //抓取物块儿的任务
{
	static int count = 0;
	if(ZhuaQu_State.T_state == 0)
	{
		TIM_SetCompare3(TIM4,95);
		Delay_ms(50);
		if(count < 1)
		{
			count ++;   //此时count = 1	
			MOTOR_Move_5(-19420,600,600,600);		//下降高度
		}
		if(motor_sta_5 ==STOP)   //motor_sta_1 == STOP&&
		{
			ZhuaQu_State.T_state = 1;//表示执行完第一步了	
			TIM_Cmd(TIM2,DISABLE);
			TIM_Cmd(TIM3,DISABLE);			
			TIM_SetCounter(TIM2,0);
			TIM_SetCounter(TIM3,0);
			
		}
	}
	else if(ZhuaQu_State.T_state == 1)    //合爪子
	{
		TIM_SetCompare3(TIM4,57);
		ZhuaQu_State.T_state = 2;
		Delay_ms(100);
	}
	else if(ZhuaQu_State.T_state == 2)  //上升高度，转爪子到放物块儿区域,选择转盘
	{
		if(count < 2)
		{
			count ++;   //此时count = 2
			MOTOR_Move_5(19420,300,300,300);		//上升高度	
			switch(WuKuaiShuLiang)    //判断是放到哪个盘子上
			{
				case 0:    //第一个物块儿
					switch(USART2_RX_BUF_2[1])
					{
						case 1:
							TIM_SetCompare1(TIM4,67);//转到第一个盘子
							break;
						case 2:
							TIM_SetCompare1(TIM4,151);//转到第二个盘子
							break;
						case 3:
							TIM_SetCompare1(TIM4,242);//转到第三个盘子
							break;
						default:
							break;		
					}
					break;
				case 1:     //第二个物块儿
					switch(USART2_RX_BUF_2[2])
					{
						case 1:
							TIM_SetCompare1(TIM4,67);//转到第一个盘子
							break;
						case 2:
							TIM_SetCompare1(TIM4,151);//转到第二个盘子
							break;
						case 3:
							TIM_SetCompare1(TIM4,242);//转到第三个盘子
							break;
						default:
							break;		
					}				
					break;
				case 2:    //第三个物块儿
					switch(USART2_RX_BUF_2[3])
					{
						case 1:
							TIM_SetCompare1(TIM4,67);//转到第一个盘子
							break;
						case 2:
							TIM_SetCompare1(TIM4,151);//转到第二个盘子
							break;
						case 3:
							TIM_SetCompare1(TIM4,242);//转到第三个盘子
							break;
						default:
							break;		
					}								
					break;
				default:
					break;
			}
			
		}
		if(motor_sta_5 == STOP)
		{
			ZhuaQu_State.T_state = 3;//	
			TIM_SetCompare2(TIM4,245);        //爪子转到放物块儿的位置
			Delay_ms(200);		
	
			TIM_Cmd(TIM2,DISABLE);
			TIM_SetCounter(TIM2,0);
		}	
		else return;
		
		
	}
	else if(ZhuaQu_State.T_state == 3)
	{
		if(count < 3)
		{
			count ++;   //此时count = 3
			MOTOR_Move_5(-11920,300,300,300);		//下降高度
		}
		if(motor_sta_5 == STOP)
		{
			ZhuaQu_State.T_state = 4;//
	
			TIM_Cmd(TIM2,DISABLE);
			TIM_SetCounter(TIM2,0);
		}
		else return;		
	}
	else if(ZhuaQu_State.T_state == 4)   //松爪子
	{
		TIM_SetCompare3(TIM4,95);

		ZhuaQu_State.T_state = 5;		
	}
	else if(ZhuaQu_State.T_state == 5)  //上升高度，转爪子
	{
		if(count < 4)
		{
			count ++;   //此时count 4
			MOTOR_Move_5(11920,300,300,300);					
		}
		if(motor_sta_5 == STOP)         //motor_sta_1 == STOP &&
		{
			WuKuaiShuLiang++;
			if(WuKuaiShuLiang == 3)                          //可能是要改成4，暂时还不清楚，外面的判断那一块儿要改！！！三改成四
			{
				ZhuaQu_State.T_state = 6;  //不需要再次执行动作组
				TIM_SetCompare2(TIM4,145);   //转到抓物块儿的地方
				Delay_ms(300);				
//				TIM_SetCompare3(TIM4,58);//合爪
//				Delay_ms(300);		
				Choose = 5; 
				Send_To_K210();				
				TIM_Cmd(TIM2,DISABLE);
				TIM_SetCounter(TIM2,0);
				TIM_Cmd(TIM3,DISABLE);
				TIM_SetCounter(TIM3,0);						
			}
			else if(WuKuaiShuLiang != 3)
			{
				ZhuaQu_State.T_state = 0;   //表示执行完所有步骤了，但还需要要执行上述步骤
				count = 0;
				Choose = 5; 
				Send_To_K210();		
				TIM_SetCompare2(TIM4,145);   //转到抓物块儿的地方
				Cishu_F1 = 0;
				Cishu_F5 = 0;
				TIM_Cmd(TIM2,DISABLE);
				TIM_SetCounter(TIM2,0);
				TIM_Cmd(TIM3,DISABLE);
				TIM_SetCounter(TIM3,0);	
				Delay_ms(500);
				YouXianJi = 0;						
			}
		}		
		else return;
		
	}
	else if(ZhuaQu_State.T_state == 6)
	{
		if(count < 5)
		{
			count ++;   //此时count 5
			MOTOR_Move_1(800,200,200,150);       //需要时调用    
			MOTOR_Move_2(-800,200,200,150);       //需要时调用
			MOTOR_Move_3(-800,200,200,150);       //需要时调用
			MOTOR_Move_4(800,200,200,150);       //需要时调用
		}
		if(motor_sta_4 == STOP)
		{
				ZhuaQu_State.T_state = 0;   //表示执行完所有步骤了，但还需要要执行上述步骤
				count = 0;
			  WuKuaiShuLiang ++;
				Choose = 5; 
				Send_To_K210();		
				Cishu_F1 = 0;
				Cishu_F5 = 0;
				TIM_Cmd(TIM2,DISABLE);
				TIM_SetCounter(TIM2,0);
				TIM_Cmd(TIM3,DISABLE);
				TIM_SetCounter(TIM3,0);		
				YouXianJi = 0;									
		}
		

	}
	else
		return;
}
int count_DengYuCiShu = 0;
int count_BuDengCiShu = 0;
int Color = 0;
int last_Color = 0;
int analyse_count = 0;
int LuanMaShu = 0;
int Wait = 0;
int XianZhengQue = 0;
int XianCuoWu = 0;
void PanDuan(void)
{
	if(YouXianJi == 0)
	{
			
		if(Cishu_F1 < 1)
		{
			Choose = 1;
			Send_To_K210();			
			Cishu_F1 ++;
		}
		char arr_buff [1] = {0};
		if(WuKuaiShuLiang < 2)
		{
			arr_buff[0] = USART1_RX_ZQ_BUF[1];
			Color = atoi(arr_buff) + 1;
			if(Color != last_Color)
			{
				analyse_count ++;     
				switch(WuKuaiShuLiang)
				{
					case 0:
						if(Color == USART2_RX_BUF_2[1])
						{
							if(XianCuoWu == 0) XianZhengQue = 1;
							count_DengYuCiShu ++;
							if(count_DengYuCiShu > 2) 
							{
								count_DengYuCiShu = 0;
								XianCuoWu = 0;
								XianZhengQue = 0;
							}
							if(count_BuDengCiShu == 2 && XianZhengQue == 1&& count_DengYuCiShu ==2)
							{
								YouXianJi ++;
								count_DengYuCiShu = 0;
								count_BuDengCiShu = 0;
								XianCuoWu = 0;
								XianZhengQue = 0;																
							}
							else if(count_DengYuCiShu == 1 && count_BuDengCiShu ==1 &&XianCuoWu == 1)
							{
								YouXianJi ++;	
								count_DengYuCiShu = 0;
							  count_BuDengCiShu = 0;
								XianZhengQue = 0;
								XianCuoWu = 0;
							}
							else if(count_DengYuCiShu == 1 && count_BuDengCiShu == 2 && XianCuoWu == 1)
							{
								YouXianJi ++;	
								count_DengYuCiShu = 0;
								count_BuDengCiShu = 0;
								XianCuoWu = 0;
								XianZhengQue = 0;								
							}
						}
						else if(Color != USART2_RX_BUF_2[1])
						{
							if(XianZhengQue == 0)   XianCuoWu = 1;
							count_BuDengCiShu++;
							if(count_BuDengCiShu >2) 
							{
								count_BuDengCiShu = 0;							
								XianZhengQue = 0;
								XianCuoWu = 0;
							}
						}
						break;
					case 1:
						if(Color == USART2_RX_BUF_2[2])
						{
							if(XianCuoWu == 0) XianZhengQue = 1;
							count_DengYuCiShu ++;
							if(count_DengYuCiShu > 2) 
							{
								count_DengYuCiShu = 0;
								XianCuoWu = 0;
								XianZhengQue = 0;
							}
							if(count_DengYuCiShu == 1 && XianZhengQue == 1)
							{
								YouXianJi ++;
								count_DengYuCiShu = 0;
								count_BuDengCiShu = 0;
								XianCuoWu = 0;
								XianZhengQue = 0;																
							}
							else if(count_DengYuCiShu == 1 && count_BuDengCiShu ==1 &&XianCuoWu == 1)
							{
								YouXianJi ++;	
								count_DengYuCiShu = 0;
							  count_BuDengCiShu = 0;
								XianZhengQue = 0;
								XianCuoWu = 0;
							}
						}
						else if(Color != USART2_RX_BUF_2[2])
						{
							if(XianZhengQue == 0)   XianCuoWu = 1;
							count_BuDengCiShu++;
							if(count_BuDengCiShu >1) 
							{
								count_BuDengCiShu = 0;							
								XianZhengQue = 0;
								XianCuoWu = 0;
							}
						}
						break;
					default:
						break;
				}			
			}
		last_Color = Color;   //更新色块儿		
		}
		else if(WuKuaiShuLiang>=2)                   //剩最后一个物块儿
		{
			static int a = 0;
			if (a < 1)
			{
				a++;
				Color = 0;
				int j = 0;
				for(j = 0; j < 13; j++)
				{
					USART1_RX_ZQ_BUF[j] = 0;				
				}
			}
			arr_buff[0] = USART1_RX_ZQ_BUF[1];
			Color = atoi(arr_buff) + 1;
			
			if(Color == USART2_RX_BUF_2[3])
			{
				count_DengYuCiShu ++;
				if(count_DengYuCiShu == 2)
				{
					YouXianJi ++;							
					count_BuDengCiShu = 0;
					count_DengYuCiShu = 0;
					LuanMaShu = 0;
				}
				else if(count_DengYuCiShu>2)
				{
					count_DengYuCiShu = 0;
				}
			}
		 }
			
	}
	else if(YouXianJi != 0)
	{
		ZhuaQuZhuanPanWL(); 
		if(Cishu_F5 == 0)
		{
			Cishu_F5 ++;
			Choose = 5; 
			Send_To_K210();
		}
	}

}


/**************************************************粗加工区任务逻辑******************************************************/

int Mission_State = 0;         //为0表示前往对应区域，为1表示矫正姿态，为2表示放物块儿

int LiChengXiShu = 0;   //用于判断是否需要乘上11550！！！
int JZ_CLim = 0;
void CJG_FangWuKuaiEr_TongYong(void)   //可以用。
{
	static int count = 0;
	if(ZhuaQu_State.T_state == 0)  //下降高度高度，转爪子到转盘区,选择转盘，开爪子到特定位置
	{
		TIM_SetCompare2(TIM4,245);        //爪子转到选择物块儿的位置
		Delay_ms(100);				
		if(count < 1)
		{
			count ++;   //此时count = 1
			TIM_SetCompare3(TIM4,95);
			Delay_ms(100);					
			MOTOR_Move_5(-11920,300,300,300);		//下降高度			
		}		
		if(Second == 0)
		{
			switch(WuKuaiShuLiang)    //判断是放到哪个盘子上
			{
				case 0:    //第一个物块儿
					switch(USART2_RX_BUF_2[1])
					{
						case 1:
							TIM_SetCompare1(TIM4,67);//转到第一个盘子
							break;
						case 2:
							TIM_SetCompare1(TIM4,151);//转到第二个盘子
							break;
						case 3:
							TIM_SetCompare1(TIM4,242);//转到第三个盘子
							break;
						default:
							break;		
					}
					break;
				case 1:     //第二个物块儿
					switch(USART2_RX_BUF_2[2])
					{
						case 1:
							TIM_SetCompare1(TIM4,67);//转到第一个盘子
							break;
						case 2:
							TIM_SetCompare1(TIM4,151);//转到第二个盘子
							break;
						case 3:
							TIM_SetCompare1(TIM4,242);//转到第三个盘子
							break;
						default:
							break;		
					}				
					break;
				case 2:    //第三个物块儿
					switch(USART2_RX_BUF_2[3])
					{
						case 1:
							TIM_SetCompare1(TIM4,67);//转到第一个盘子
							break;
						case 2:
							TIM_SetCompare1(TIM4,151);//转到第二个盘子
							break;
						case 3:
							TIM_SetCompare1(TIM4,242);//转到第三个盘子
							break;
						default:
							break;		
					}								
					break;
				default:
					break;
			}
					
		}	
		else if(Second == 1)
		{
				switch(WuKuaiShuLiang)    //判断是放到哪个盘子上
				{
					case 0:    //第一个物块儿
						switch(USART2_RX_BUF_2[4])
						{
							case 1:
								TIM_SetCompare1(TIM4,67);//转到第一个盘子
								break;
							case 2:
								TIM_SetCompare1(TIM4,151);//转到第二个盘子
								break;
							case 3:
								TIM_SetCompare1(TIM4,242);//转到第三个盘子
								break;
							default:
								break;		
						}
						break;
					case 1:     //第二个物块儿
						switch(USART2_RX_BUF_2[5])
						{
							case 1:
								TIM_SetCompare1(TIM4,67);//转到第一个盘子
								break;
							case 2:
								TIM_SetCompare1(TIM4,151);//转到第二个盘子
								break;
							case 3:
								TIM_SetCompare1(TIM4,242);//转到第三个盘子
								break;
							default:
								break;		
						}				
						break;
					case 2:    //第三个物块儿
						switch(USART2_RX_BUF_2[6])
						{
							case 1:
								TIM_SetCompare1(TIM4,67);//转到第一个盘子
								break;
							case 2:
								TIM_SetCompare1(TIM4,151);//转到第二个盘子
								break;
							case 3:
								TIM_SetCompare1(TIM4,242);//转到第三个盘子
								break;
							default:
								break;		
						}								
						break;
					default:
						break;
				}
						
			


		
		}
		if(motor_sta_5 == STOP)
		{
			ZhuaQu_State.T_state = 1;//  接下来要合爪子
			TIM_Cmd(TIM2,DISABLE);
			TIM_SetCounter(TIM2,0);
		}	
		else return;
		
		
	}
	else if(ZhuaQu_State.T_state == 1)   //合爪子
	{
		TIM_SetCompare3(TIM4,58);
		Delay_ms(400);
		ZhuaQu_State.T_state = 2;		
	}
	else if(ZhuaQu_State.T_state == 2)  //上升高度，转爪子
	{
		if(count < 2)
		{
			count ++;   //此时count 2
			
			MOTOR_Move_5(11920,250,250,250);		
				
		}
		if(motor_sta_5 == STOP)         //motor_sta_1 == STOP &&
		{
			TIM_SetCompare2(TIM4,145);//转到抓物块儿的地方
			Delay_ms(400);				
			TIM_Cmd(TIM2,DISABLE);
   		TIM_SetCounter(TIM2,0);
		  ZhuaQu_State.T_state = 3;			
		}	
		
	}
	else if(ZhuaQu_State.T_state == 3)//下降高度准备放物块儿
	{
		if(count < 3)
		{
			count ++;   //此时count 3
			
			MOTOR_Move_5(-52532,650,650,650);			
		}
		if(motor_sta_5 == STOP)         //motor_sta_1 == STOP &&
		{
			Delay_ms(400);				
			TIM_Cmd(TIM2,DISABLE);
   		TIM_SetCounter(TIM2,0);
			TIM_SetCompare3(TIM4,95);	//松爪		
		  ZhuaQu_State.T_state = 4;			
		}		
	}
	else if(ZhuaQu_State.T_state == 4)//上升高度准备返程
	{
		if(count < 4)
		{
			count ++;   //此时count 4		
			MOTOR_Move_5(52532,250,250,250);					
		}
		if(motor_sta_5 == STOP)         //motor_sta_1 == STOP &&
		{
			count = 0;
			WuKuaiShuLiang++;	
			if(WuKuaiShuLiang == 3)
			{
				Mission_State = 3;
				WuKuaiShuLiang = 0;  //清零用于接下来抓取物块儿判断。
				TIM_Cmd(TIM2,DISABLE);
				TIM_SetCounter(TIM2,0);
				ZhuaQu_State.T_state = 0;

			}
			else
			{
				count = 0;
				Mission_State = 0;
				TIM_Cmd(TIM2,DISABLE);
				TIM_SetCounter(TIM2,0);
				ZhuaQu_State.T_state = 0;//表示执行完所有步骤				
			}
		}	
	}
	
}	
void JiaoZheng(float x, float y)
{
	if(JZ_CLim<1)
	{
		TIM_Cmd(TIM5,ENABLE);	
		PID_Init_Angle_1();
		Set_ang();
		JZ_CLim++;
		switch(WuKuaiShuLiang)
		{
			case 0:
				if(Second == 0)			Choose = USART2_RX_BUF_2[1]+1;
				else if(Second ==1) Choose = USART2_RX_BUF_2[4]+1;
				Send_To_K210();			
				break;
			case 1:
				if(Second == 0)			Choose = USART2_RX_BUF_2[2]+1;
				else if(Second ==1) Choose = USART2_RX_BUF_2[5]+1;
				Send_To_K210();			
				break;
			case 2:
				if(Second == 0)			Choose = USART2_RX_BUF_2[3]+1;
				else if(Second ==1) Choose = USART2_RX_BUF_2[6]+1;
				Send_To_K210();
				break;
			default:
				break;
		}
	}
	K210y_pid.SetPoint= 218;
	K210x_pid.SetPoint= 327;
	Angle_PID.Setpoint = 90;
	yaw=(float)stcAngle.Angle[2]/32768*180;		//如果需要角度换 直接把这段取消注释
	xy_speed_cal(speedx,speedy);
	TIM_Cmd(TIM5,ENABLE);
	if((K210x_pid.SetPoint-speedx>=-4&&K210x_pid.SetPoint-speedx<=4) && (K210y_pid.SetPoint-speedy>=-4&&K210y_pid.SetPoint-speedy<=4)&&((Angle_PID.Setpoint-yaw)>=-0.95&&(Angle_PID.Setpoint-yaw)<=0.95))
	{
		Mission_State = 2;
		JZ_CLim = 0;
	  Reset_ang();		
	}		 
} 
int Step = 0;  //应该走的步数
int CJG_state = 0;

void ZhiZou_1(void)    //从第一个转角前往粗加工区
{ 
	static int count = 0;

	switch(WuKuaiShuLiang)
	{
		case 0://放第一个物块儿的时候
			if(Second == 0)			Color = USART2_RX_BUF_2[1];
		  else if(Second ==1) Color = USART2_RX_BUF_2[4];
			last_Color = 0;  //防止之前赋值操作的影响
			LiChengXiShu = 1;
			break;
		case 1://放第二个物块儿的时候
			if(Second == 0)			Color = USART2_RX_BUF_2[2];
		  else if(Second ==1) Color = USART2_RX_BUF_2[5];
			LiChengXiShu = 0;
			break;
		case 2://放最后一个物块儿的时候
			if(Second == 0)			Color = USART2_RX_BUF_2[3];
		  else if(Second ==1) Color = USART2_RX_BUF_2[6];
		  LiChengXiShu = 0;
			break;
		case 3:
			CJG_state = 1;
			break;
		default:
			break;
	}
	Step = LiChengXiShu * 7550 + (Color - last_Color) * 1800;
	last_Color = Color;
	if(ZhuaQu_State.T_state == 0 && CJG_state != 1)
	{
		if(count < 1)
		{
			count ++;   //此时count = 1
			MOTOR_Move_1(-Step,130,100,160);       //需要时调用
			MOTOR_Move_2(-Step,130,100,160);       //需要时调用
			MOTOR_Move_3(-Step,130,100,160);       //需要时调用
			MOTOR_Move_4(-Step,130,100,160);       //需要时调用	
		}
		if(motor_sta_4 == STOP)
		{
			if(LiChengXiShu == 1)    // 初次前往加工区
			{
				ZhuaQu_State.T_state = 1;  // 需要再进行一次横移任务  
				TIM_Cmd(TIM2,DISABLE);
				TIM_SetCounter(TIM2,0);			
			}
			else   //不再需要横移，直接开始矫正
			{
				count = 0;
				ZhuaQu_State.T_state = 0;//  
				TIM_Cmd(TIM2,DISABLE);
				TIM_SetCounter(TIM2,0);	
				Mission_State	= 1;
			}
		}			
	}
	else if(ZhuaQu_State.T_state == 1 && LiChengXiShu == 1) //横移到对应位置
	{
		if(count <2)
		{
			count ++;
			MOTOR_Move_1(-800,200,200,150);       //需要时调用    
			MOTOR_Move_2(800,200,200,150);       //需要时调用
			MOTOR_Move_3(800,200,200,150);       //需要时调用
			MOTOR_Move_4(-800,200,200,150);       //需要时调用		
		}
		if(motor_sta_4 == STOP)
		{
			count = 0; //
			ZhuaQu_State.T_state = 0;//   
			TIM_Cmd(TIM2,DISABLE);
			TIM_SetCounter(TIM2,0);		
			Mission_State	 = 1;  //开始矫正
		}			
	}

}
void ZhiZou_2(void)    //从第一个转角前往粗加工区
{ 
	static int count = 0;

	switch(WuKuaiShuLiang)
	{
		case 0://抓第一个物块儿的时候
			if(Second == 0)			Color = USART2_RX_BUF_2[1];
		  else if(Second ==1) Color = USART2_RX_BUF_2[4];
			break;
		case 1://抓第二个物块儿的时候
			if(Second == 0)			Color = USART2_RX_BUF_2[2];
		  else if(Second ==1) Color = USART2_RX_BUF_2[5];
			break;
		case 2://抓最后一个物块儿的时候
			if(Second == 0)			Color = USART2_RX_BUF_2[3];
		  else if(Second ==1) Color = USART2_RX_BUF_2[6];
			break;
		case 3:
			CJG_state = 1;
			break;
		default:
			break;
	}
	Step = (Color - last_Color) * 1800;
	last_Color = Color;
	if(ZhuaQu_State.T_state == 0 && CJG_state != 1)
	{
		if(count < 1)
		{
			count ++;   //此时count = 1
			MOTOR_Move_1(-Step,130,100,150);       //需要时调用
			MOTOR_Move_2(-Step,130,100,150);       //需要时调用
			MOTOR_Move_3(-Step,130,100,150);       //需要时调用
			MOTOR_Move_4(-Step,130,100,150);       //需要时调用	
		}
		if(motor_sta_4 == STOP)
		{
				count = 0;
				ZhuaQu_State.T_state = 0;//  
				TIM_Cmd(TIM2,DISABLE);
				TIM_SetCounter(TIM2,0);	
				Mission_State	= 5;  //走到对应的位置后直接抓取物块儿
		}			
	}
	else if(ZhuaQu_State.T_state == 0 && CJG_state == 1) //已全部抓取，直走之转角
	{
		if(count <1)
		{
			count ++;
			MOTOR_Move_1(800,200,200,150);       //需要时调用    
			MOTOR_Move_2(-800,200,200,150);       //需要时调用
			MOTOR_Move_3(-800,200,200,150);       //需要时调用
			MOTOR_Move_4(800,200,200,150);       //需要时调用		
		}
		if(motor_sta_4 == STOP)
		{
			ZhuaQu_State.T_state = 1;//   
			TIM_Cmd(TIM2,DISABLE);
			TIM_SetCounter(TIM2,0);		
		}			
	}
	else if(ZhuaQu_State.T_state == 1 && CJG_state == 1) //已全部抓取，直走之转角
	{
		Step = CJG_state * 7800 + (3 - Color) * 1650;
		if(count <2)
		{
			count ++;
			MOTOR_Move_1(-Step,130,130,160);       //需要时调用    
			MOTOR_Move_2(-Step,130,130,160);       //需要时调用
			MOTOR_Move_3(-Step,130,130,160);       //需要时调用
			MOTOR_Move_4(-Step,130,130,160);       //需要时调用		
		}
		if(motor_sta_4 == STOP)
		{
			count = 0; //
			CJG_state = 0;
			WuKuaiShuLiang++;  //此时应为4
			Mission_State = 19;  //防止出现问题
			ZhuaQu_State.T_state = 0;//   
			TIM_Cmd(TIM2,DISABLE);
			TIM_SetCounter(TIM2,0);	
			ZhuaQu_State.T_state = 0;
		}			
	}

}


void CJG_ZhuaWuKuaiEr_TongYong(void)   //可以用。
{
	static int count = 0;
	if(ZhuaQu_State.T_state == 0)  //下降高度高度,合爪，抓取物块儿。
	{
		if(count < 1)
		{
			count ++;   //此时count = 1
			TIM_SetCompare3(TIM4,95);			
			MOTOR_Move_5(-52532,650,650,650);		//下降高度			
		}	
		if(motor_sta_5 == STOP)
		{
			TIM_SetCompare3(TIM4,58);  //合爪
			Delay_ms(200);					
			ZhuaQu_State.T_state = 1;//  
			TIM_Cmd(TIM2,DISABLE);
			TIM_SetCounter(TIM2,0);
		}	
	}
	else if(ZhuaQu_State.T_state == 1)   //上升高度，到顶部，转爪子，选择转盘
	{
		if(count < 2)
		{
			count ++;   //此时count = 2
						
			MOTOR_Move_5(52532,250,250,250);		//上升高度			
		}	
		
		if(Second == 0)
		{
			switch(WuKuaiShuLiang)    //判断是放到哪个盘子上
			{
				case 0:    //第一个物块儿
					switch(USART2_RX_BUF_2[1])
					{
						case 1:
							TIM_SetCompare1(TIM4,67);//转到第一个盘子
							break;
						case 2:
							TIM_SetCompare1(TIM4,151);//转到第二个盘子
							break;
						case 3:
							TIM_SetCompare1(TIM4,242);//转到第三个盘子
							break;
						default:
							break;		
					}
					break;
				case 1:     //第二个物块儿
					switch(USART2_RX_BUF_2[2])
					{
						case 1:
							TIM_SetCompare1(TIM4,67);//转到第一个盘子
							break;
						case 2:
							TIM_SetCompare1(TIM4,151);//转到第二个盘子
							break;
						case 3:
							TIM_SetCompare1(TIM4,242);//转到第三个盘子
							break;
						default:
							break;		
					}				
					break;
				case 2:    //第三个物块儿
					switch(USART2_RX_BUF_2[3])
					{
						case 1:
							TIM_SetCompare1(TIM4,67);//转到第一个盘子
							break;
						case 2:
							TIM_SetCompare1(TIM4,151);//转到第二个盘子
							break;
						case 3:
							TIM_SetCompare1(TIM4,242);//转到第三个盘子
							break;
						default:
							break;		
					}								
					break;
				default:
					break;
			}
					
		}

		else if(Second == 1)
		{
				switch(WuKuaiShuLiang)    //判断是放到哪个盘子上
				{
					case 0:    //第一个物块儿
						switch(USART2_RX_BUF_2[4])
						{
							case 1:
								TIM_SetCompare1(TIM4,67);//转到第一个盘子
								break;
							case 2:
								TIM_SetCompare1(TIM4,151);//转到第二个盘子
								break;
							case 3:
								TIM_SetCompare1(TIM4,242);//转到第三个盘子
								break;
							default:
								break;		
						}
						break;
					case 1:     //第二个物块儿
						switch(USART2_RX_BUF_2[5])
						{
							case 1:
								TIM_SetCompare1(TIM4,67);//转到第一个盘子
								break;
							case 2:
								TIM_SetCompare1(TIM4,151);//转到第二个盘子
								break;
							case 3:
								TIM_SetCompare1(TIM4,242);//转到第三个盘子
								break;
							default:
								break;		
						}				
						break;
					case 2:    //第三个物块儿
						switch(USART2_RX_BUF_2[6])
						{
							case 1:
								TIM_SetCompare1(TIM4,67);//转到第一个盘子
								break;
							case 2:
								TIM_SetCompare1(TIM4,151);//转到第二个盘子
								break;
							case 3:
								TIM_SetCompare1(TIM4,242);//转到第三个盘子
								break;
							default:
								break;		
						}								
						break;
					default:
						break;
				}
						
			
		}
		Delay_ms(300);
		
		if(motor_sta_5 == STOP)
		{
			TIM_SetCompare2(TIM4,245);  //放物块儿的位置
			Delay_ms(400);
			ZhuaQu_State.T_state = 2;		
		}	
	
	}
	else if(ZhuaQu_State.T_state == 2)  //下降高度，松爪子
	{
		if(count < 3)
		{
			count ++;   //此时count 3
			
			MOTOR_Move_5(-11920,650,650,650);		
				
		}
		if(motor_sta_5 == STOP)         //
		{
			TIM_SetCompare3(TIM4,95);//
			TIM_Cmd(TIM2,DISABLE);
   		TIM_SetCounter(TIM2,0);
		  ZhuaQu_State.T_state = 3;			
		}	
		
	}
	else if(ZhuaQu_State.T_state == 3)//上升高度，
	{
		if(count < 4)
		{
			count ++;   //此时count 3
			
			MOTOR_Move_5(11920,250,250,250);			
		}
		if(motor_sta_5 == STOP)         //motor_sta_1 == STOP &&
		{
			TIM_Cmd(TIM2,DISABLE);
   		TIM_SetCounter(TIM2,0);
		  ZhuaQu_State.T_state = 4;			
		}		
	}
	else if(ZhuaQu_State.T_state == 4)//转爪子
	{
		TIM_SetCompare2(TIM4,145);  //放物块儿的位置
		count = 0;
		WuKuaiShuLiang++;	//最后一次时为4
		if(WuKuaiShuLiang == 3)  //表明抓完了所有的物块儿
		{
			Mission_State = 3;
			ZhuaQu_State.T_state = 0;//表示执行完所有步骤				
			
		}
		else
		{
			count = 0;
			ZhuaQu_State.T_state = 0;//表示执行完所有步骤				
			Mission_State = 3;
		}
	}
	
}	

void PanDuan_2(int speedx, int speedy)              //前往粗加工区并放物料的判断
{
	switch(Mission_State)
	{
		case 0://前往放物块儿区域
			ZhiZou_1();
			break;
		case 1://矫正位置
			JiaoZheng(speedx,speedy);
			break;
		case 2://放物块儿
			c = 0;
			JZ_CLim = 0;
			CJG_FangWuKuaiEr_TongYong();
			break;
		case 3://直走，准备抓物块儿
			ZhiZou_2();
			break;
		case 4://校准？好像不需要。
			break;
		case 5://抓取物块儿
			CJG_ZhuaWuKuaiEr_TongYong();
			break;
		default:
			break;
	}
}




/**************************************************精加工区任务逻辑******************************************************/

void JJG_ZhiZou_1(void)
{
	static int count = 0;

	switch(WuKuaiShuLiang)
	{
		case 0://放第一个物块儿的时候
			if(Second == 0)			Color = USART2_RX_BUF_2[1];
		  else if(Second ==1) Color = USART2_RX_BUF_2[4];
			last_Color = 0;  //防止之前赋值操作的影响
			LiChengXiShu = 1;
			break;
		case 1://放第二个物块儿的时候
			if(Second == 0)			Color = USART2_RX_BUF_2[2];
		  else if(Second ==1) Color = USART2_RX_BUF_2[5];
			LiChengXiShu = 0;
			break;
		case 2://放最后一个物块儿的时候
			if(Second == 0)			Color = USART2_RX_BUF_2[3];
		  else if(Second ==1) Color = USART2_RX_BUF_2[6];
		  LiChengXiShu = 0;
			break;
		case 3:
			CJG_state = 1;
			break;
		default:
			break;
	}
	Step = LiChengXiShu * 7550 + (Color - last_Color) * 1800;
	last_Color = Color;
	if(ZhuaQu_State.T_state == 0 && CJG_state != 1)
	{
		if(count < 1)
		{
			count ++;   //此时count = 1
			MOTOR_Move_1(-Step,130,100,160);       //需要时调用
			MOTOR_Move_2(-Step,130,100,160);       //需要时调用
			MOTOR_Move_3(-Step,130,100,160);       //需要时调用
			MOTOR_Move_4(-Step,130,100,160);       //需要时调用	
		}
		if(motor_sta_4 == STOP)
		{
			if(LiChengXiShu == 1)    // 初次前往加工区
			{
				ZhuaQu_State.T_state = 1;  // 需要再进行一次横移任务  
				TIM_Cmd(TIM3,DISABLE);
				TIM_SetCounter(TIM3,0);			
			}
			else   //不再需要横移，直接开始矫正
			{
				count = 0;
				ZhuaQu_State.T_state = 0;//  
				TIM_Cmd(TIM2,DISABLE);
				TIM_SetCounter(TIM2,0);	
				Mission_State	= 1;
			}
		}			
	}
	else if(ZhuaQu_State.T_state == 1 && LiChengXiShu == 1) //横移到对应位置
	{
		if(count <2)
		{
			count ++;
			MOTOR_Move_1(-800,200,200,150);       //需要时调用    
			MOTOR_Move_2(800,200,200,150);       //需要时调用
			MOTOR_Move_3(800,200,200,150);       //需要时调用
			MOTOR_Move_4(-800,200,200,150);       //需要时调用		
			
		}
		if(motor_sta_4 == STOP)
		{
			count = 0; //
			ZhuaQu_State.T_state = 0;//   
			TIM_Cmd(TIM3,DISABLE);
			TIM_SetCounter(TIM3,0);		
			Mission_State	 = 1;  //开始矫正
		}			
	}
	else if(ZhuaQu_State.T_state == 0 && CJG_state == 1) //已全部抓取，直走之转角
	{
		if(count <1)
		{
			count ++;
			MOTOR_Move_1(1000,200,200,150);       //需要时调用    
			MOTOR_Move_2(-1000,200,200,150);       //需要时调用
			MOTOR_Move_3(-1000,200,200,150);       //需要时调用
			MOTOR_Move_4(1000,200,200,150);       //需要时调用		

		}
		if(motor_sta_4 == STOP)
		{
			ZhuaQu_State.T_state = 1;//   
			TIM_Cmd(TIM2,DISABLE);
			TIM_SetCounter(TIM2,0);		
		}			
	}
	else if(ZhuaQu_State.T_state == 1 && CJG_state == 1&&Second == 0) //已全部抓取，后退
	{
		if(Second ==0) Step = CJG_state * 8200 + (Color-1) * 1800;
		else if(Second == 1) Step = CJG_state * 8200 + (3 - Color) * 1800;
		if(count <2)
		{
			count ++;
			MOTOR_Move_1(Step,130,100,160);       //需要时调用    
			MOTOR_Move_2(Step,130,100,160);       //需要时调用
			MOTOR_Move_3(Step,130,100,160);       //需要时调用
			MOTOR_Move_4(Step,130,100,160);       //需要时调用		
			
		}
		if(motor_sta_4 == STOP)
		{
			count = 0; //
			WuKuaiShuLiang++;  //此时为4
			Choose  = 5; 
			Send_To_K210();						
			Mission_State = 19;  //防止出现问题
			ZhuaQu_State.T_state = 0;//   
			TIM_Cmd(TIM3,DISABLE);
			TIM_SetCounter(TIM3,0);	
			ZhuaQu_State.T_state = 0;
		}			
	}
	else if(ZhuaQu_State.T_state == 1 && CJG_state == 1&&Second == 1)
	{
	  Step = CJG_state * 9000 + (3 - Color) * 2350;
		if(count <2)
		{
			count ++;
			MOTOR_Move_1(-Step,130,100,160);       //需要时调用    
			MOTOR_Move_2(-Step,130,100,160);       //需要时调用
			MOTOR_Move_3(-Step,130,100,160);       //需要时调用
			MOTOR_Move_4(-Step,130,100,160);       //需要时调用		
			
		}
		if(motor_sta_4 == STOP)
		{
			count = 0; //
			WuKuaiShuLiang++;  //此时为4
			Mission_State = 19;  //防止出现问题
			ZhuaQu_State.T_state = 0;//   
			TIM_Cmd(TIM3,DISABLE);
			TIM_SetCounter(TIM3,0);	
			ZhuaQu_State.T_state = 0;
		}			
	}
		

	
	
}

void JJG_JiaoZheng(int speedx,int speedy)
{
	if(JZ_CLim<1)
	{
		TIM_Cmd(TIM5,ENABLE);	
		PID_Init_Angle_1();
		Set_ang();
		JZ_CLim++;
		switch(WuKuaiShuLiang)
		{
			case 0:				
				if(Second == 0)			Choose = USART2_RX_BUF_2[1]+1;
				else if(Second ==1) Choose = USART2_RX_BUF_2[4]+1;
				Send_To_K210();			
				break;
			case 1:
				if(Second == 0)			Choose = USART2_RX_BUF_2[2]+1;
				else if(Second ==1) Choose = USART2_RX_BUF_2[5]+1;
				Send_To_K210();			
				break;
			case 2:
				if(Second == 0)			Choose = USART2_RX_BUF_2[3]+1;
				else if(Second ==1) Choose = USART2_RX_BUF_2[6]+1;
				Send_To_K210();
				break;
			default:
				break;
		}
	}
	K210y_pid.SetPoint= 218;
	K210x_pid.SetPoint= 327;
	Angle_PID.Setpoint = 180;
	xy_speed_cal_1(speedx,speedy);
	if((K210x_pid.SetPoint-speedx>=-4&&K210x_pid.SetPoint-speedx<=4) && (K210y_pid.SetPoint-speedy>=-4&&K210y_pid.SetPoint-speedy<=4)&&((Angle_PID.Setpoint-yaw)>=-0.95&&(Angle_PID.Setpoint-yaw)<=0.95))
	{
			Mission_State = 2;
			JZ_CLim = 0;
			Reset_ang();		
	}		 	
}           
	
void JJG_FangWuKuaiEr_TongYong(void)
{
	static int count = 0;
	if(ZhuaQu_State.T_state == 0)  //下降高度高度，转爪子到转盘区,选择转盘，开爪子到特定位置
	{
		if(count < 1)
		{
			count ++;   //此时count = 1
			
			TIM_SetCompare2(TIM4,245);        //爪子转到选择物块儿的位置
			Delay_ms(200);		
			TIM_SetCompare3(TIM4,95);
			Delay_ms(200);					
			MOTOR_Move_5(-11920,650,650,650);		//下降高度			
		}
		if(Second == 0)
		{
			switch(WuKuaiShuLiang)    //判断是放到哪个盘子上
			{
				case 0:    //第一个物块儿
					switch(USART2_RX_BUF_2[1])
					{
						case 1:
							TIM_SetCompare1(TIM4,67);//转到第一个盘子
							break;
						case 2:
							TIM_SetCompare1(TIM4,151);//转到第二个盘子
							break;
						case 3:
							TIM_SetCompare1(TIM4,242);//转到第三个盘子
							break;
						default:
							break;		
					}
					break;
				case 1:     //第二个物块儿
					switch(USART2_RX_BUF_2[2])
					{
						case 1:
							TIM_SetCompare1(TIM4,67);//转到第一个盘子
							break;
						case 2:
							TIM_SetCompare1(TIM4,151);//转到第二个盘子
							break;
						case 3:
							TIM_SetCompare1(TIM4,242);//转到第三个盘子
							break;
						default:
							break;		
					}				
					break;
				case 2:    //第三个物块儿
					switch(USART2_RX_BUF_2[3])
					{
						case 1:
							TIM_SetCompare1(TIM4,67);//转到第一个盘子
							break;
						case 2:
							TIM_SetCompare1(TIM4,151);//转到第二个盘子
							break;
						case 3:
							TIM_SetCompare1(TIM4,242);//转到第三个盘子
							break;
						default:
							break;		
					}								
					break;
				default:
					break;
			}
					
		}

		else if(Second == 1)
		{
				switch(WuKuaiShuLiang)    //判断是放到哪个盘子上
				{
					case 0:    //第一个物块儿
						switch(USART2_RX_BUF_2[4])
						{
							case 1:
								TIM_SetCompare1(TIM4,67);//转到第一个盘子
								break;
							case 2:
								TIM_SetCompare1(TIM4,151);//转到第二个盘子
								break;
							case 3:
								TIM_SetCompare1(TIM4,242);//转到第三个盘子
								break;
							default:
								break;		
						}
						break;
					case 1:     //第二个物块儿
						switch(USART2_RX_BUF_2[5])
						{
							case 1:
								TIM_SetCompare1(TIM4,67);//转到第一个盘子
								break;
							case 2:
								TIM_SetCompare1(TIM4,151);//转到第二个盘子
								break;
							case 3:
								TIM_SetCompare1(TIM4,242);//转到第三个盘子
								break;
							default:
								break;		
						}				
						break;
					case 2:    //第三个物块儿
						switch(USART2_RX_BUF_2[6])
						{
							case 1:
								TIM_SetCompare1(TIM4,67);//转到第一个盘子
								break;
							case 2:
								TIM_SetCompare1(TIM4,151);//转到第二个盘子
								break;
							case 3:
								TIM_SetCompare1(TIM4,242);//转到第三个盘子
								break;
							default:
								break;		
						}								
						break;
					default:
						break;
				}
						
			
		}
		if(motor_sta_5 == STOP)
		{
			ZhuaQu_State.T_state = 1;//  接下来要合爪子
			TIM_Cmd(TIM2,DISABLE);
			TIM_SetCounter(TIM2,0);
		}	
		else return;
		
		
	}
	else if(ZhuaQu_State.T_state == 1)   //合爪子
	{
		TIM_SetCompare3(TIM4,58);
		Delay_ms(400);
		ZhuaQu_State.T_state = 2;		
	}
	else if(ZhuaQu_State.T_state == 2)  //上升高度，转爪子
	{
		if(count < 2)
		{
			count ++;   //此时count 2
			
			MOTOR_Move_5(11920,250,250,250);		
				
		}
		if(motor_sta_5 == STOP)         //motor_sta_1 == STOP &&
		{
			TIM_SetCompare2(TIM4,145);//转到抓物块儿的地方
			Delay_ms(400);				
			TIM_Cmd(TIM2,DISABLE);
   		TIM_SetCounter(TIM2,0);
		  ZhuaQu_State.T_state = 3;			
		}	
		
	}
	else if(ZhuaQu_State.T_state == 3)//下降高度准备放物块儿
	{
		if(count < 3)
		{
			count ++;   //此时count 3
			if(Second == 0) 			MOTOR_Move_5(-52532,650,650,650);		//下降高度			
			else if(Second == 1)			MOTOR_Move_5(-25977,650,650,650);			
		}
		if(motor_sta_5 == STOP)         //motor_sta_1 == STOP &&
		{
			Delay_ms(400);				
			TIM_Cmd(TIM2,DISABLE);
   		TIM_SetCounter(TIM2,0);
			TIM_SetCompare3(TIM4,95);	//松爪		
		  ZhuaQu_State.T_state = 4;			
		}		
	}
	else if(ZhuaQu_State.T_state == 4)//上升高度准备返程
	{
		if(count < 4)
		{
			count ++;   //此时count 4		
			if(Second == 0) 			MOTOR_Move_5(52532,650,650,650);		//下降高度			
			else if(Second == 1)			MOTOR_Move_5(25977,650,650,650);			
		}
		if(motor_sta_5 == STOP)         //motor_sta_1 == STOP &&
		{
			count = 0;
			WuKuaiShuLiang++;	
			if(WuKuaiShuLiang == 3)
			{
				Mission_State = 0;
//				WuKuaiShuLiang ;  //清零用于接下来抓取物块儿判断。
				TIM_Cmd(TIM2,DISABLE);
				TIM_SetCounter(TIM2,0);
				ZhuaQu_State.T_state = 0;

			}
			else
			{
				count = 0;
				Mission_State = 0;
				TIM_Cmd(TIM2,DISABLE);
				TIM_SetCounter(TIM2,0);
				ZhuaQu_State.T_state = 0;//表示执行完所有步骤				
			}
		}	
	}
	
}
void PanDuan_3(int speedx, int speedy)              //前往粗加工区并放物料的判断
{
	switch(Mission_State)
	{
		case 0://前往放物块儿区域
			JJG_ZhiZou_1();
			break;
		case 1://矫正位置
			JJG_JiaoZheng(speedx,speedy);
			break;
		case 2://放物块儿
			c = 0;
			JZ_CLim = 0;		
			JJG_FangWuKuaiEr_TongYong();
			break;
		default:
			break;
	}
}

/***************************************************返回转盘区任务逻辑**************************************************************/
int ZhiZouCiShu = 0;   //用于判断走了几次。

void test_8(void)  //回转盘区时第一个转角
{
	 Ang_cal_1();//循环调用该函数即可闭环 到达固定角度 建议周期15ms以内（陀螺仪串口通信速度也要快） 如果反应不够快速 可增加frame_period
	 if(Angle_PID.error>-0.2&&Angle_PID.error<0.2)
	 {
		 z++;
		 if(z>100)
		 {
				Speed_Test_State.T_state = End;		 			 
		 }
	 }
	
}
void test_9(void)   //向前走去粗加工区绿色环进行矫正。
{
	static int count = 0;

	if(count <1)
	{
		count ++;		
		MOTOR_Move_1(10800,200,200,150);       //需要时调用
		MOTOR_Move_2(10800,200,200,150);       //需要时调用
		MOTOR_Move_3(10800,200,200,150);       //需要时调用
		MOTOR_Move_4(10800,200,200,150);       //需要时调用
	}
		//之走完后更新状态
	if(motor_sta_4 == STOP )
	{
		Speed_Test_State.T_state = End;
		if(ZhiZouCiShu == 0)
		{
			count = 0;
			ZhiZouCiShu = 1;
			Mission_State = 1;
			WuKuaiShuLiang = 7;   //防止发送别的颜色
			Choose = 3;
			Send_To_K210();	
		}
		else 
		{
			Mission_State = 3;  //让起不再执行直走
			Choose = 5;
			Send_To_K210();	
		}
		TIM_Cmd(TIM3,DISABLE);
		TIM_SetCounter(TIM3,0);
	}

}

void test_10(void)  //回转盘区时第二个转角
{
	 Angle_PID.Setpoint = 0;
	 Ang_cal();//循环调用该函数即可闭环 到达固定角度 建议周期15ms以内（陀螺仪串口通信速度也要快） 如果反应不够快速 可增加frame_period
	 if(Angle_PID.error>-0.2&&Angle_PID.error<0.2)
	 {
		 z++;
		 if(z>100)
		 {
				Speed_Test_State.T_state = End;		 	
				test = 15;			 
		 }
	 }
	
}
void test_11(void)  //后退到转盘  
{
	static int count = 0;
	if(count == 0)
	{	
		count ++;
		MOTOR_Move_1(5700,200,200,150);       //需要时调用
		MOTOR_Move_2(5700,200,200,150);       //需要时调用
		MOTOR_Move_3(5700,200,200,150);       //需要时调用
		MOTOR_Move_4(5700,200,200,150);       //需要时调用
	}
		//之走完后更新状态
//		Delay_ms(2);
	if(motor_sta_4 == STOP && count == 1)
	{
//		Speed_Test_State.T_state = End;
		count ++;
		Delay_ms(200);		
		TIM_Cmd(TIM3,DISABLE);
		TIM_SetCounter(TIM3,0);
	}
	else if(count == 2)
	{
		count ++; //此时为3
		MOTOR_Move_1(-1000,200,200,150);       //需要时调用    
		MOTOR_Move_2(1000,200,200,150);       //需要时调用
		MOTOR_Move_3(1000,200,200,150);       //需要时调用
		MOTOR_Move_4(-1000,200,200,150);       //需要时调用			
	}
	if(motor_sta_4 == STOP && count == 3)
	{
		Speed_Test_State.T_state = End;
		count ++;
		Delay_ms(200);		
		TIM_Cmd(TIM3,DISABLE);
		TIM_SetCounter(TIM3,0);
		
	}
	
}
void test_12(void)  //后退到转盘  
{
	static int count = 0;
	if(count < 1)
	{	
		count ++;
		MOTOR_Move_1(950,200,200,150);       //需要时调用
		MOTOR_Move_2(-950,200,200,150);       //需要时调用
		MOTOR_Move_3(-950,200,200,150);       //需要时调用
		MOTOR_Move_4(950,200,200,150);       //需要时调用
	}
		//之走完后更新状态
//		Delay_ms(2);
	if(motor_sta_4 == STOP)
	{
		Speed_Test_State.T_state = End;
		TIM_Cmd(TIM3,DISABLE);
		TIM_SetCounter(TIM3,0);
		Mission_State = 3;
	}
	
}
void test_13(void)   //转90度
{
	//左转90度
	 Angle_PID.Setpoint = 0;
	 Ang_cal();//循环调用该函数即可闭环 到达固定角度 建议周期15ms以内（陀螺仪串口通信速度也要快） 如果反应不够快速 可增加frame_period
	 if(Angle_PID.error>-0.2&&Angle_PID.error<0.2)
	 {
		 z++;
		 if(z>1000)
		 {
				Speed_Test_State.T_state = End;		 			 
		 }
	 }

}

void PanDuan_4(int speedx, int speedy)              //前往转盘区并抓第二轮物料的判断
{
	switch(Mission_State)
	{
		case 0://矫正位置
			test_9();			
			break;
		case 1://矫正位置
			JiaoZheng(speedx,speedy);
			break;
		case 2:
			//横移一点
			test_12();
			break;
		case 3://直走前往转角
			test_9();			
			break;	
		case 4:
			test = 14;
		  WuKuaiShuLiang = 0;
			Speed_Test_State.T_state = Acting;
			break;			
		default:
			break;
	}
}

void test_15(void)  //后退到出发区
{
	static int count = 0;
	if(count < 1)	
	{	
		count ++;
		MOTOR_Move_1(20500,100,100,300);       //需要时调用
		MOTOR_Move_2(20500,100,100,300);       //需要时调用
		MOTOR_Move_3(20500,100,100,300);       //需要时调用
		MOTOR_Move_4(20500,100,100,300);       //需要时调用
	}
		//之走完后更新状态
//		Delay_ms(2);
	if(motor_sta_4 == STOP)
	{
		Speed_Test_State.T_state = End;
		TIM_Cmd(TIM3,DISABLE);
		TIM_SetCounter(TIM3,0);
	}
	
}

void test_23(void)   //返回出发区
{
	static int count = 0;
	if(count < 1)
	{
		count ++;	
		MOTOR_Move_1(-21500,210,210,150);       //需要时调用         14850
		MOTOR_Move_2(-21500,210,210,150);       //需要时调用
		MOTOR_Move_3(-21500,210,210,150);       //需要时调用
		MOTOR_Move_4(-21500,210,210,150);       //需要时调用
	}	
		//之走完后更新状态
	if(motor_sta_4 == STOP)
	{
		Speed_Test_State.T_state = End;
		TIM_Cmd(TIM3,DISABLE);
		TIM_SetCounter(TIM3,0);
	}

}


/***************************************************返回出发区任务逻辑**************************************************************/
void Sec_PanDuan(void)
{
	if(YouXianJi == 0)
	{
			
		if(Cishu_F1 < 1)
		{
			Choose = 1;
			Send_To_K210();			
			Cishu_F1 ++;
		}
		char arr_buff [1] = {0};
		if(WuKuaiShuLiang < 2)
		{
			arr_buff[0] = USART1_RX_ZQ_BUF[1];
			Color = atoi(arr_buff) + 1;
			if(Color != last_Color)
			{
				analyse_count ++;     
				switch(WuKuaiShuLiang)
				{
					case 0:
						if(Color == USART2_RX_BUF_2[4])
						{
							if(XianCuoWu == 0) XianZhengQue = 1;
							count_DengYuCiShu ++;
							if(count_DengYuCiShu > 2) 
							{
								count_DengYuCiShu = 0;
								XianCuoWu = 0;
								XianZhengQue = 0;
							}
							if(count_BuDengCiShu == 2 && XianZhengQue == 1&& count_DengYuCiShu ==2)
							{
								YouXianJi ++;
								count_DengYuCiShu = 0;
								count_BuDengCiShu = 0;
								XianCuoWu = 0;
								XianZhengQue = 0;																
							}
							else if(count_DengYuCiShu == 1 && count_BuDengCiShu ==1 &&XianCuoWu == 1)
							{
								YouXianJi ++;	
								count_DengYuCiShu = 0;
							  count_BuDengCiShu = 0;
								XianZhengQue = 0;
								XianCuoWu = 0;
							}
							else if(count_DengYuCiShu == 1 && count_BuDengCiShu == 2 && XianCuoWu == 1)
							{
								YouXianJi ++;	
								count_DengYuCiShu = 0;
								count_BuDengCiShu = 0;
								XianCuoWu = 0;
								XianZhengQue = 0;								
							}
						}
						else if(Color != USART2_RX_BUF_2[4])
						{
							if(XianZhengQue == 0)   XianCuoWu = 1;
							count_BuDengCiShu++;
							if(count_BuDengCiShu >2) 
							{
								count_BuDengCiShu = 0;							
								XianZhengQue = 0;
								XianCuoWu = 0;
							}
						}
						break;
					case 1:
						if(Color == USART2_RX_BUF_2[5])
						{
							if(XianCuoWu == 0) XianZhengQue = 1;
							count_DengYuCiShu ++;
							if(count_DengYuCiShu > 2) 
							{
								count_DengYuCiShu = 0;
								XianCuoWu = 0;
								XianZhengQue = 0;
							}
							if(count_DengYuCiShu == 1 && XianZhengQue == 1)
							{
								YouXianJi ++;
								count_DengYuCiShu = 0;
								count_BuDengCiShu = 0;
								XianCuoWu = 0;
								XianZhengQue = 0;																
							}
							else if(count_DengYuCiShu == 1 && count_BuDengCiShu ==1 &&XianCuoWu == 1)
							{
								YouXianJi ++;	
								count_DengYuCiShu = 0;
							  count_BuDengCiShu = 0;
								XianZhengQue = 0;
								XianCuoWu = 0;
							}
						}
						else if(Color != USART2_RX_BUF_2[5])
						{
							if(XianZhengQue == 0)   XianCuoWu = 1;
							count_BuDengCiShu++;
							if(count_BuDengCiShu >1) 
							{
								count_BuDengCiShu = 0;							
								XianZhengQue = 0;
								XianCuoWu = 0;
							}
						}
						break;
					default:
						break;
				}			
			}
		last_Color = Color;   //更新色块儿		
		}
		else if(WuKuaiShuLiang>=2)                   //剩最后一个物块儿
		{
			static int a = 0;
			if (a < 1)
			{
				a++;
				Color = 0;
				int j = 0;
				for(j = 0; j < 13; j++)
				{
					USART1_RX_ZQ_BUF[j] = 0;				
				}
			}
			arr_buff[0] = USART1_RX_ZQ_BUF[1];
			Color = atoi(arr_buff) + 1;
			
			if(Color == USART2_RX_BUF_2[6])
			{
				count_DengYuCiShu ++;
				if(count_DengYuCiShu == 2)
				{
					YouXianJi ++;							
					count_BuDengCiShu = 0;
					count_DengYuCiShu = 0;
					LuanMaShu = 0;
				}
				else if(count_DengYuCiShu>2)
				{
					count_DengYuCiShu = 0;
				}
			}
		 }
			
	}
	else if(YouXianJi != 0)
	{
		Sec_ZhuaQuZhuanPanWL(); 
		if(Cishu_F5 == 0)
		{
			Cishu_F5 ++;
			Choose = 5; 
			Send_To_K210();
		}
	}

}




void Sec_ZhuaQuZhuanPanWL(void)   //抓取物块儿的任务
{
	static int count = 0;
	if(ZhuaQu_State.T_state == 0)
	{
		TIM_SetCompare3(TIM4,95);
		Delay_ms(100);
		if(count < 1)
		{
			count ++;   //此时count = 1	
			MOTOR_Move_5(-19420,300,300,300);		//下降高度
		}
		if(motor_sta_5 ==STOP)   //motor_sta_1 == STOP&&
		{
			ZhuaQu_State.T_state = 1;//表示执行完第一步了
			Delay_ms(200);		
			TIM_Cmd(TIM2,DISABLE);
			TIM_Cmd(TIM3,DISABLE);			
			TIM_SetCounter(TIM2,0);
			TIM_SetCounter(TIM3,0);
			
		}
	}
	else if(ZhuaQu_State.T_state == 1)    //合爪子
	{
		TIM_SetCompare3(TIM4,57);
		ZhuaQu_State.T_state = 2;
		Delay_ms(100);
	}
	else if(ZhuaQu_State.T_state == 2)  //上升高度，转爪子到放物块儿区域,选择转盘
	{
		if(count < 2)
		{
			count ++;   //此时count = 2
			MOTOR_Move_5(19420,300,300,300);		//上升高度	
			switch(WuKuaiShuLiang)    //判断是放到哪个盘子上
			{
				case 0:    //第一个物块儿
					switch(USART2_RX_BUF_2[4])
					{
						case 1:
							TIM_SetCompare1(TIM4,67);//转到第一个盘子
							break;
						case 2:
							TIM_SetCompare1(TIM4,151);//转到第二个盘子
							break;
						case 3:
							TIM_SetCompare1(TIM4,242);//转到第三个盘子
							break;
						default:
							break;		
					}
					break;
				case 1:     //第二个物块儿
					switch(USART2_RX_BUF_2[5])
					{
						case 1:
							TIM_SetCompare1(TIM4,67);//转到第一个盘子
							break;
						case 2:
							TIM_SetCompare1(TIM4,151);//转到第二个盘子
							break;
						case 3:
							TIM_SetCompare1(TIM4,242);//转到第三个盘子
							break;
						default:
							break;		
					}				
					break;
				case 2:    //第三个物块儿
					switch(USART2_RX_BUF_2[6])
					{
						case 1:
							TIM_SetCompare1(TIM4,67);//转到第一个盘子
							break;
						case 2:
							TIM_SetCompare1(TIM4,151);//转到第二个盘子
							break;
						case 3:
							TIM_SetCompare1(TIM4,242);//转到第三个盘子
							break;
						default:
							break;		
					}								
					break;
				default:
					break;
			}
			
		}
		if(motor_sta_5 == STOP)
		{
			ZhuaQu_State.T_state = 3;//	
			TIM_SetCompare2(TIM4,245);        //爪子转到放物块儿的位置
			Delay_ms(200);		
	
			TIM_Cmd(TIM2,DISABLE);
			TIM_SetCounter(TIM2,0);
		}	
		else return;
		
		
	}
	else if(ZhuaQu_State.T_state == 3)
	{
		if(count < 3)
		{
			count ++;   //此时count = 3
			MOTOR_Move_5(-11920,300,300,300);		//下降高度
		}
		if(motor_sta_5 == STOP)
		{
			ZhuaQu_State.T_state = 4;//
	
			TIM_Cmd(TIM2,DISABLE);
			TIM_SetCounter(TIM2,0);
		}
		else return;		
	}
	else if(ZhuaQu_State.T_state == 4)   //松爪子
	{
		TIM_SetCompare3(TIM4,95);

		ZhuaQu_State.T_state = 5;		
	}
	else if(ZhuaQu_State.T_state == 5)  //上升高度，转爪子
	{
		if(count < 4)
		{
			count ++;   //此时count 4
			MOTOR_Move_5(11920,300,300,300);					
		}
		if(motor_sta_5 == STOP)         //motor_sta_1 == STOP &&
		{
			WuKuaiShuLiang++;
			if(WuKuaiShuLiang == 3)                          //可能是要改成4，暂时还不清楚，外面的判断那一块儿要改！！！三改成四
			{
				ZhuaQu_State.T_state = 6;  //不需要再次执行动作组
				TIM_SetCompare2(TIM4,145);   //转到抓物块儿的地方
				Delay_ms(300);				
//				TIM_SetCompare3(TIM4,58);//合爪
//				Delay_ms(300);		
				Choose = 5; 
				Send_To_K210();				
				TIM_Cmd(TIM2,DISABLE);
				TIM_SetCounter(TIM2,0);
				TIM_Cmd(TIM3,DISABLE);
				TIM_SetCounter(TIM3,0);						
			}
			else if(WuKuaiShuLiang != 3)
			{
				ZhuaQu_State.T_state = 0;   //表示执行完所有步骤了，但还需要要执行上述步骤
				count = 0;
				Choose = 5; 
				Send_To_K210();		
				TIM_SetCompare2(TIM4,145);   //转到抓物块儿的地方
				Delay_ms(300);								
				Cishu_F1 = 0;
				Cishu_F5 = 0;
				TIM_Cmd(TIM2,DISABLE);
				TIM_SetCounter(TIM2,0);
				TIM_Cmd(TIM3,DISABLE);
				TIM_SetCounter(TIM3,0);	
				Delay_ms(800);
				YouXianJi = 0;						
			}
		}		
		else return;
		
	}
	else if(ZhuaQu_State.T_state == 6)
	{
		if(count < 5)
		{
			count ++;   //此时count 5
			MOTOR_Move_1(800,200,200,150);       //需要时调用    
			MOTOR_Move_2(-800,200,200,150);       //需要时调用
			MOTOR_Move_3(-800,200,200,150);       //需要时调用
			MOTOR_Move_4(800,200,200,150);       //需要时调用
		}
		if(motor_sta_4 == STOP)
		{
				ZhuaQu_State.T_state = 0;   //表示执行完所有步骤了，但还需要要执行上述步骤
				count = 0;
			  WuKuaiShuLiang ++;
				Choose = 5; 
				Send_To_K210();		
				Cishu_F1 = 0;
				Cishu_F5 = 0;
				TIM_Cmd(TIM2,DISABLE);
				TIM_SetCounter(TIM2,0);
				TIM_Cmd(TIM3,DISABLE);
				TIM_SetCounter(TIM3,0);		
				YouXianJi = 0;									
		}
		

	}
	else
		return;
}




void New_PanDuan(void)
{
	if(YouXianJi == 0)  //进行判断是否需要抓取
	{
		if(Cishu_F1 < 1)
		{
			Choose = 1;
			Send_To_K210();			
			Cishu_F1 ++;
//			Color = 0;   //防止上一次的判断对其进行的影响
			Delay_ms(300);
		}
		char arr_buff [1] = {0};  //用于暂存数据的数组
		if(WuKuaiShuLiang == 0)  //第一次抓取物块儿的时候需要进行一些判断，随后的抓取只需要看到以后就抓就行
		{
			arr_buff[0] = USART1_RX_ZQ_BUF[1];
			Color = atoi(arr_buff) + 1;
			if(Color != last_Color )  //颜色发生跳变的时候才进行判断
			{
				switch(Second)
				{
					case 0:  //表明是第一次抓取物块儿
						if(Color == USART2_RX_BUF_2[1])
						{
							if(XianCuoWu == 0)		XianZhengQue = 1;
							count_DengYuCiShu ++; 
							if(XianCuoWu == 1 && count_DengYuCiShu == 1) //如果第一眼看到的是错误的，下次看到正确的时候，直接抓
							{
								YouXianJi ++;
							}
							else if(XianZhengQue == 1 && count_DengYuCiShu == 2) //先看到正确的，则看到第二次的时候抓
							{
								YouXianJi ++;
							}
						}
						else if(Color != USART2_RX_BUF_2[1])
						{
							if(XianZhengQue == 0)  XianCuoWu = 1;
						}
						break;
					case 1:  //标明是第二次抓取物块儿
						if(Color == USART2_RX_BUF_2[4])
						{
							if(XianCuoWu == 0)		XianZhengQue = 1;
							count_DengYuCiShu ++; 
							if(XianCuoWu == 1 && count_DengYuCiShu == 1) //如果第一眼看到的是错误的，下次看到正确的时候，直接抓
							{
								YouXianJi ++;
								count_DengYuCiShu = 0;
								XianCuoWu = 0;
								XianZhengQue = 0;																								
							}
							else if(XianZhengQue == 1 && count_DengYuCiShu == 2) //先看到正确的，则看到第二次的时候抓
							{
								YouXianJi ++;
								count_DengYuCiShu = 0;
								XianCuoWu = 0;
								XianZhengQue = 0;																																
							}
						}
						else if(Color != USART2_RX_BUF_2[4])
						{
							if(XianZhengQue == 0)  XianCuoWu = 1;
						}
						
						break;
					default:
						break;
				}
			}
			last_Color = Color;

		}
		else if(WuKuaiShuLiang <3&&WuKuaiShuLiang >0) //只要看到物块儿就抓
		{
			arr_buff[0] = USART1_RX_ZQ_BUF[1];
			Color = atoi(arr_buff) + 1;
			switch(WuKuaiShuLiang)
			{
				case 1: //抓第二个物块儿
					if(Color == USART2_RX_BUF_2[1] && Second == 0) Color = 0;
					else if(Color == USART2_RX_BUF_2[4] && Second == 1) Color = 0;
					if(Color == USART2_RX_BUF_2[2] && Second == 0)  YouXianJi++;
					else if(Color == USART2_RX_BUF_2[5] && Second == 1)  YouXianJi++;
					break;
				case 2:
					if(Color == USART2_RX_BUF_2[1] && Second == 0) Color = 0;
				  else if(Color == USART2_RX_BUF_2[2] && Second == 0) Color = 0;
					else if(Color == USART2_RX_BUF_2[4] && Second == 1) Color = 0;	
					else if(Color == USART2_RX_BUF_2[5] && Second == 1) Color = 0;		
					if(Color == USART2_RX_BUF_2[3] && Second == 0) YouXianJi++;		
					if(Color == USART2_RX_BUF_2[6] && Second == 1) YouXianJi++;		
				
					break;
				default:
					break;				
			}			
		}
		

	}
	else if(YouXianJi == 1)
	{
		if(Cishu_F5 == 0)
		{
			Cishu_F5 ++;
			Choose = 5; 
			Send_To_K210();
			count_DengYuCiShu = 0;
			XianCuoWu = 0;
			XianZhengQue = 0;																								
			
		}		
		if(Second == 0)	ZhuaQuZhuanPanWL();
		else if(Second == 1)	Sec_ZhuaQuZhuanPanWL();	
	}
}

void TEST_JiaoZheng(float x, float y)
{
	if(JZ_CLim<1)
	{
		PID_Init_Angle_1();
		Set_ang();
		JZ_CLim++;
		switch(WuKuaiShuLiang)
		{
			case 0:
				if(Second == 0)			Choose = USART2_RX_BUF_2[1]+1;
				else if(Second ==1) Choose = USART2_RX_BUF_2[4]+1;
				Send_To_K210();			
				break;
			case 1:
				if(Second == 0)			Choose = USART2_RX_BUF_2[2]+1;
				else if(Second ==1) Choose = USART2_RX_BUF_2[5]+1;
				Send_To_K210();			
				break;
			case 2:
				if(Second == 0)			Choose = USART2_RX_BUF_2[3]+1;
				else if(Second ==1) Choose = USART2_RX_BUF_2[6]+1;
				Send_To_K210();
				break;
			default:
				break;
		}
	}
	K210y_pid.SetPoint= 218;
	K210x_pid.SetPoint= 327;
	Angle_PID.Setpoint = 0;
	yaw=(float)stcAngle.Angle[2]/32768*180;		//如果需要角度换 直接把这段取消注释
	xy_speed_cal(speedx,speedy);
	if((K210x_pid.SetPoint-speedx>=-4&&K210x_pid.SetPoint-speedx<=4) && (K210y_pid.SetPoint-speedy>=-4&&K210y_pid.SetPoint-speedy<=4)&&((Angle_PID.Setpoint-yaw)>=-0.5&&(Angle_PID.Setpoint-yaw)<=0.5))
	{
			test = 46;
			JZ_CLim = 0;
			Reset_ang();		
	}		 	
	
} 
