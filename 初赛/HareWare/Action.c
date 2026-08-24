#include "main.h"

uint16_t xuan_xiao =685;
uint16_t xuan_da =2025;
#define He_Zhua 1420				//740
#define Kai_zhua 750
uint8_t Flag_JiaQu = 0;
uint8_t Flag_JiaQu_1 = 1;
uint8_t Flag_JiaQu_2 = 0;
uint8_t Flag_JiaQu_3 = 0;


/**				
  * 函    数：用于夹取物料,,,,,,载物盘上的
  * 参    数：无					Receive_OpenMV[6]
  * 返 回 值：无					Wu_Liao_Color
  */
void Jia_QU_Pan(void)
{
	Mortor_5(0,2150);								//爪子下降
	delay(1000);
	TIM_SetCompare3(TIM4,He_Zhua);					//关爪660
	delay(8000);
	Mortor_5(1,2150);								//上升
	zhuan_da();										//2030				//旋转
	delay(1000);
	Mortor_5(0,2100);								//爪子下降
	TIM_SetCompare3(TIM4,Kai_zhua);					//开爪
	delay(5000);
									
	if(Flag_JiaQu_3 == 1)
	{
		Mortor_5(1,1250+2100);
	}
	else 
	{
		Mortor_5(1,2100);zhuan_xiao();
	}
}

/**				
  * 函    数：用于放物料
  * 参    数：无					Receive_OpenMV[6]
  * 返 回 值：无					Wu_Liao_Color
  */
void Jia_QU(void)
{
	Mortor_5(1,7650);								//爪子上升
	zhuan_da();	
	Mortor_5(0,3050);								//下降
	TIM_SetCompare3(TIM4,He_Zhua);						//关爪
	delay(8000);
	Mortor_5(1,3450);								//上升
	zhuan_xiao();						//旋转
	Mortor_5(0,8050);								//下降
	
	delay(4000);
	TIM_SetCompare3(TIM4,Kai_zhua);						//开爪	
	delay(1000);
}


/**				
  * 函    数：用于收回物料
  * 参    数：无					Receive_OpenMV[6]
  * 返 回 值：无					Wu_Liao_Color
  */
void Jia_QU_shou(void)
{
	TIM_SetCompare3(TIM4,He_Zhua);					//关爪
	delay(9000);	
	Mortor_5(1,6850);								//爪子上升
	zhuan_da();										//旋转	
	Mortor_5(0,1960);								//下降
	delay(2000);
	TIM_SetCompare3(TIM4,Kai_zhua);					//开爪
	delay(5000);
	Mortor_5(1,1960);								//上升
	zhuan_xiao();									//旋转
	Mortor_5(0,6850);
}


/**				
  * 函    数：用于罗物块
  * 参    数：无					
  * 返 回 值：无					
  */
void Jia_QU_fang(void)
{
	Mortor_5(1,3450);								//爪子上升
	zhuan_da();										//旋转
	Mortor_5(0,3020);								//下降
	TIM_SetCompare3(TIM4,He_Zhua);					//关爪
	delay(8000);
	Mortor_5(1,3020);								//上升
	zhuan_xiao();									//旋转
	Mortor_5(0,3700);								//下降
	delay(2000);
	TIM_SetCompare3(TIM4,Kai_zhua);						//开爪	
	delay(6000);
	Mortor_5(1,250);
}

/**				
  * 函    数：用于校正X,Y坐标
  * 参    数：无					
  * 返 回 值：无					
  */
void Jiao_Zheng(void)
{

			if(color==2||color==3)delay(5000);
			else delay(2000);
			if(X_Value_goal - X<-5.1||X_Value_goal - X>5.1)			
			{
				TIM_Cmd(TIM2,ENABLE);				//先校正X
				while(1)
				{
					if(Temp[8] == 1)	
					{
						TIM_Cmd(TIM2,DISABLE);
						Mortor_Stop();
						Temp[8] = 0;			//清零，方便下次校正用						
						break;
					}
				}
			}
			if(Y_Value_goal - Y<-5.1||Y_Value_goal - Y>5.1)
			{
				TIM_Cmd(TIM3,ENABLE);
				while(1)
				{
					if(Temp[10] == 1)
					{
						TIM_Cmd(TIM3,DISABLE);
						Mortor_Stop();
						Temp[10] = 0;						
						break;
					}
				}
			}
			
			delay(3000);
			if(X_Value_goal - X<-0.1||X_Value_goal - X>0.1)
			{
				int16_t Mai_chong_x;
				Mai_chong_x = (X_Value_goal - X)*4.3478;		//4.3478
				Mortor_Straight(Mai_chong_x);
			}
			delay(3000);
			if(Y_Value_goal - Y<-0.1||Y_Value_goal - Y>0.1)
			{
				int16_t Mai_chong_y;
				Mai_chong_y = -(Y_Value_goal - Y)*4.5454;		//4.5454
				Mortor_Heng_Yi(Mai_chong_y);
			}

}


/**				
  * 函    数：用于校正X,Y坐标
  * 参    数：无					
  * 返 回 值：无					
  */
void Jiao_Zheng2(void)
{
			
			delay(5000);
			if(X_Value_goal - X<-8.1||X_Value_goal - X>8.1)			
			{
				TIM_Cmd(TIM2,ENABLE);				//先校正X
				while(1)
				{
					if(Temp[8] == 1)	
					{
						TIM_Cmd(TIM2,DISABLE);
						Mortor_Stop();
						Temp[8] = 0;			//清零，方便下次校正用
			//			Temp[9] = 0;
						
						break;
					}
				}
			}
			if(Y_Value_goal - Y<-8.1||Y_Value_goal - Y>8.1)
			{
				TIM_Cmd(TIM3,ENABLE);
				while(1)
				{
					if(Temp[10] == 1)
					{
						TIM_Cmd(TIM3,DISABLE);
						Mortor_Stop();
			//			Temp[11] = 0;			//清零，方便下次校正用
						Temp[10] = 0;
						
						break;
					}
				}
			}
			USART_Cmd(UART4,ENABLE);
			delay(4000);
			if(X_Value_goal - X<-0.1||X_Value_goal - X>0.1)
			{
				int16_t Mai_chong_x;
				Mai_chong_x = (X_Value_goal - X)*4.3478;
				Mortor_Straight(Mai_chong_x);
			}
			delay(2000);
			if(Y_Value_goal - Y<-0.1||Y_Value_goal - Y>0.1)
			{
				int16_t Mai_chong_y;
				Mai_chong_y = -(Y_Value_goal - Y)*4.5454;
				Mortor_Heng_Yi(Mai_chong_y);
			}

//			USART_Cmd(UART4,DISABLE);

}

uint8_t Shun_xu;
uint8_t  Wu_liao_se()
{
	uint8_t i=0;
	for(;i<3;i++)
	{
		if(Receive_OpenMV[i]==Wu_Liao_Color)
		{
			return i+1;
		}
	}
}


uint8_t Colar_last;

uint8_t Blue = 0;
/**				
  * 函    数：用于识别并取物料
  * 参    数：无					Receive_OpenMV[6]
  * 返 回 值：无					Wu_Liao_Color
  */

void Define_Wu_Liao(void)
{
		if(Flag_JiaQu_1 == 1&&Wu_Liao_Color!=0)				
		{

			delay(1000);
			Colar_last = Wu_Liao_Color;
			USART_Cmd(UART4,DISABLE);
			switch (Wu_liao_se())
			{
				case 1:TIM_SetCompare4(TIM4,670);break;
				case 2:TIM_SetCompare4(TIM4,1336);break;
				case 3:TIM_SetCompare4(TIM4,1986);break;
			}
			Jia_QU_Pan();
			Flag_JiaQu=0;
			Flag_JiaQu_1 = 0;
			Flag_JiaQu_2 = 1;
			USART_Cmd(UART4,ENABLE);
			while(Wu_Liao_Color == Colar_last);
			delay(1000);									//给摄像头一点时间进行识别
		}
		else if(Colar_last != Wu_Liao_Color && Flag_JiaQu_2 == 1 && Wu_Liao_Color!=10)
		{
			delay(1000);
			Colar_last = Wu_Liao_Color;
			USART_Cmd(UART4,DISABLE);
			switch (Wu_liao_se())
			{
				case 1:TIM_SetCompare4(TIM4,670);break;
				case 2:TIM_SetCompare4(TIM4,1336);break;
				case 3:TIM_SetCompare4(TIM4,1986);break;
			}
			delay(15000);
			Jia_QU_Pan();
			Flag_JiaQu_2 = 0;
			Flag_JiaQu_3 = 1;
			delay(1000);
			USART_Cmd(UART4,ENABLE);
			while(Wu_Liao_Color == Colar_last);
			delay(1000);									//给摄像头一点时间进行识别
		}
		else if(Colar_last != Wu_Liao_Color && Flag_JiaQu_3 == 1 && Wu_Liao_Color!=10)
		{
			delay(1000);
//			Serial_SendByte_UART4(0);						//让摄像头停止工作
			USART_Cmd(UART4,DISABLE);
			switch (Wu_liao_se())
			{
				case 1:TIM_SetCompare4(TIM4,670);break;
				case 2:TIM_SetCompare4(TIM4,1336);break;
				case 3:TIM_SetCompare4(TIM4,1986);break;
			}
			delay(15000);
			Jia_QU_Pan();
			Flag_JiaQu_3 = 0;
			Flag_JiaQu_1 = 1;
			test = 2;
			GPIO_SetBits(GPIOA,GPIO_Pin_5);
		}	
}
	


/**				
  * 函    数：转完90°之后，走到对应的色环，然后进行校正放置。然后再走到对应色环
  * 参    数：无					
  * 返 回 值：无					
  */

uint8_t color = 0;
uint8_t Last_color = 2;			//因为是以绿色为中心

uint8_t Main_Mission;			//放物块时的任务顺序

void Zhi_Zou_1(void)
{
	
	int8_t Zhi_Zou_Step = 0;
	switch(Receive_OpenMV[Temp[6]])
	{
		case 1:color = 1;Temp[6]++;break;						//1:红,2:绿,3:蓝
		case 2:color = 2;Temp[6]++;break;
		case 3:color = 3;Temp[6]++;break;
				
		default :break;
	}
	
	Zhi_Zou_Step = color-Last_color;
	Last_color = color;
	if(Receive_OpenMV[0] == 2 && Temp[15] == 0)						//第一个颜色为绿色不走
	{
		Temp[15] = 1;
								//走到对应色环
	}
	else Mortor_Straight(Zhi_Zou_Step*1950);	
}

int8_t Zhi_Zou_2Step = 0;
/**				
  * 函    数：在加工区放完3个物块后，开始夹取物块
  * 参    数：无					
  * 返 回 值：无					
  */
void Zhi_Zou_2(void)
{
	
	switch(Receive_OpenMV[Temp[6]])
	{
		case 1:color = 1;Temp[6]++;break;						//1:红,2:绿,3:蓝
		case 2:color = 2;Temp[6]++;break;
		case 3:color = 3;Temp[6]++;break;
				
		default :break;
	}

	Zhi_Zou_2Step = color - Last_color;
	Last_color = color;
	Mortor_Straight(Zhi_Zou_2Step*1950);							//走到对应色环
		
}

/**				
  * 函    数：存储区用放物块用
  * 参    数：无					
  * 返 回 值：无					
  */
void Zhi_Zou_3(void)
{
	int8_t Zhi_Zou_3Step = 0;
	switch(Receive_OpenMV[Temp[6]])
	{
		case 1:color = 1;Temp[6]++;break;						//1:红,2:绿,3:蓝
		case 2:color = 2;Temp[6]++;break;
		case 3:color = 3;Temp[6]++;break;
				
		default :break;
	}

	Zhi_Zou_3Step = color - Last_color;
	Last_color = color;
	Mortor_Straight(Zhi_Zou_3Step*1950);					//走到对应色环
		
}

/**				
  * 函    数：用于第二批物料，将之前的标志位清零
  * 参    数：无					
  * 返 回 值：无					
  */

void Qing_Zero(void)
{
	Temp[0] = 0;Temp[1] = 0;Temp[2] = 0;Temp[3] = 0;Temp[4] = 0;Temp[5] = 0;
	Temp[6] = 0;Temp[7] = 0;Temp[8] = 0;Temp[9] = 0;Temp[10] = 0;Temp[11] = 0;
	Temp[12] = 0;Temp[14] = 0;Temp[15] = 0;
	
	Flag_JiaQu = 0;Flag_JiaQu_1 = 0; Flag_JiaQu_2 = 0; Flag_JiaQu_3 = 0;
}   
   
   
void Send_usart3(void)
{
	USART_Cmd(USART3, ENABLE);
	Serial_SendByte_USART3(0XFF);Serial_SendByte_USART3(0XAA);Serial_SendByte_USART3(0X76);
	Serial_SendByte_USART3(0X00);Serial_SendByte_USART3(0X00);
	USART_Cmd(USART3,DISABLE);
}


/**				
  * 函    数：主要功能
  * 参    数：无					
  * 返 回 值：无					
  */

void Main_Action(void)
{
		switch (test)
		{
			case 1: Define_Wu_Liao();break;								//夹取物块
			
			case 2: Mortor_Heng_Yi(1100);
					Mortor_Straight(-5250);test=3;break;				//后退
			
			case 3: 													//转90度
					
					if(Temp[0] == 0)
					{
						Mortor_5_Stop();
						
						Mortor_Zi_Zhuan_dir(1);
						PID_all_par.Setpoint=90;
						USART_Cmd(USART3, ENABLE);	
						TIM_Cmd(TIM1,ENABLE);
						Temp[0] = 1;
					}
					
					yaw_1=(float)stcAngle.Angle[2]/32768*180;			//串口3与陀螺仪通讯
					if(yaw_1 < -90)
					{
						yaw = 360 + yaw_1;
					}
					else yaw = yaw_1;
				
					if(Temp[5] == 1)
					{
						USART_Cmd(USART3,DISABLE);
						TIM_Cmd(TIM1,DISABLE);
						delay(100);
						Mortor_Stop();
						PID_all_par.output=0;
						Temp[5] = 0;
						test = 4;
						Mortor_5_Start();
					}
					break;
			case 4:	Mortor_Straight(21780);test=5;break;			//直走
			case 5:
					
					if(Temp[2] == 0)
					{
						Mortor_5_Stop();
						Mortor_Zi_Zhuan_dir(1);
						PID_all_par.Setpoint=180;
						USART_Cmd(USART3, ENABLE);	
						TIM_Cmd(TIM1,ENABLE);
						Temp[2] = 1;
						Temp[1] = 0;					//次数清零，定时器100次的时候，失能用
					}
					yaw_1=(float)stcAngle.Angle[2]/32768*180;			//串口3与陀螺仪通讯
					if(yaw_1 < -90)
					{
						yaw = 360 + yaw_1;
					}
					else yaw = yaw_1;
					
					if(Temp[5] == 1)
					{
						
						Last_color=2;
						USART_Cmd(USART3,DISABLE);
						TIM_Cmd(TIM1,DISABLE);
						delay(100);
						Mortor_Stop();
						PID_all_par.output=0;
						Temp[5] = 0;
						test = 6;
						Temp[6] = 0;
						Main_Mission = 1;					//首先进行任务一
						Mortor_Heng_Yi(-1200);				//靠近色环
						Mortor_5_Start();
						zhuan_xiao();
						Mortor_5(0,6500+1150);					//下降，摄像头进行识别
						GPIO_SetBits(GPIOA,GPIO_Pin_5);		//开灯
					
						USART_Cmd(UART4,ENABLE);
						Serial_SendByte_UART4(0XFA);
						Serial_SendByte_UART4(4);			//让摄像头识别色环
						Serial_SendByte_UART4(0XFB);
					
							X = X_Value_goal=194;				//XY坐标第一区域
							Y = Y_Value_goal=191;
						Send_usart3();							//陀螺仪清零
					}
					break;
			case 6: 										//第一加工区
					switch (Main_Mission)
					{
						case 1:	
								PID_X_Para();PID_Y_Para();
								Zhi_Zou_1();Main_Mission = 2;	//走到对应色环
								break;
						case 2:	switch (Temp[6])
								{
									case 1:TIM_SetCompare4(TIM4,670);break;						
									case 2:TIM_SetCompare4(TIM4,1336);break;
									case 3:TIM_SetCompare4(TIM4,1986);break;
									default: break;
								}
								Jiao_Zheng();Main_Mission = 3;break;	//进行校正
						case 3:	Main_Mission = 1;					//放物块
								
								Jia_QU();
								if(Temp[6] == 3)				//放完了3个物块
								{	
									USART_Cmd(UART4,DISABLE);
									Temp[6] = 0;				//清零，方便夹取物块使用
									Main_Mission = 4;			//开始抓物块,走到对应距离
								}		break;	
						case 4: 	
								Main_Mission = 5;
								switch (Temp[6])
								{
									case 0:TIM_SetCompare4(TIM4,670);break;						
									case 1:TIM_SetCompare4(TIM4,1336);break;
									case 2:TIM_SetCompare4(TIM4,1986);break;
									default: break;
								}
								Zhi_Zou_2();
								break;
						case 5:	Main_Mission = 4;Jia_QU_shou();	//夹物块
							    if(Temp[6] == 3)				//夹了3个物块
								{	
									Last_color = 0;
									Temp[6] = 0;				//清零，方便存放区使用
									Main_Mission = 6;			//开始抓物块,走到对应距离
								}		break;	
						case 6:	Mortor_Heng_Yi(1300);Temp[15] = 0;						
								delay(200);
								
								Mortor_Straight(-8300-(color-1)*2000);test = 7;break;//前往转角							//走到转角
						default : break;		
					}break;
			case 7:	
					if(Temp[7] == 0)
					{
						Mortor_5_Stop();
						Mortor_Zi_Zhuan_dir(0);
						PID_all_par.Setpoint=-90;
						USART_Cmd(USART3, ENABLE);
						TIM_Cmd(TIM1,ENABLE);
						Temp[7] = 1;
					}
					
					yaw_1=(float)stcAngle.Angle[2]/32768*180;			//串口3与陀螺仪通讯
					if(yaw_1 < -100)
					{
						yaw = 360 + yaw_1;
					}
					else yaw = yaw_1;
				
					if(Temp[5] == 1)
					{
						
						USART_Cmd(USART3,DISABLE);
						TIM_Cmd(TIM1,DISABLE);
						delay(100);
						Mortor_Stop();
						PID_all_par.output=0;
						Temp[5] = 0;
						test = 8;
						Mortor_5_Start();
						Send_usart3();						//清零
					}
					break;
			case 8:	TIM_SetCompare4(TIM4,560);
					
					if(Temp[13] ==1)						
					{
						Mortor_5(1,2200);			//罗物块时，需先上升一定距离
						Mortor_Straight(-8210);
						Mortor_Heng_Yi(-1380);
						USART_Cmd(UART4,ENABLE);
						Serial_SendByte_UART4(0XFA);Serial_SendByte_UART4(4);Serial_SendByte_UART4(0XFB);
						X_Value_goal=197;
						Y_Value_goal=190;					//罗物块
						Jiao_Zheng2();
						Mortor_5(1,2000);
						USART_Cmd(UART4,DISABLE);
//						Mortor_Straight(-400);
						Mortor_Straight(- 2000*(3-Receive_OpenMV[0]));
					}
					else 
					{
						
						Mortor_Straight(-8500 - 2000*(3-Receive_OpenMV[0]));
						Mortor_Heng_Yi(-1350);
						USART_Cmd(UART4,ENABLE);
						Serial_SendByte_UART4(0XFA);
						Serial_SendByte_UART4(4);			//让摄像头识别色环
						Serial_SendByte_UART4(0XFB);
						X_Value_goal=196;							//第二区域
						Y_Value_goal=188;
						
					}
					
					
					Last_color = Receive_OpenMV[0];Temp[6] = 1;		//更新Last_color，为case9铺垫
					
					color = Receive_OpenMV[0];test = 9;Main_Mission = 2;
					break;		
			case 9: 
					if(Temp[13] ==0)
					{	
						switch (Main_Mission)
						{						//因为case8直接走到了第一个色环，所以移动2次即可
							case 1:	
									Zhi_Zou_3();
									PID_X_Para();PID_Y_Para();
										
									Main_Mission = 2;break;			//走到对应色环
							case 2:	switch (Temp[6])
									{
										case 1:TIM_SetCompare4(TIM4,670);break;						
										case 2:TIM_SetCompare4(TIM4,1336);break;
										case 3:TIM_SetCompare4(TIM4,1986);break;
										default: break;
									}X = X_Value_goal;Y = Y_Value_goal;
									
									Jiao_Zheng();Main_Mission = 3;Jia_QU();							   
									break;	//进行校正
							case 3:	Main_Mission = 1;							//放物块
									
									if(Temp[6] == 3)				//放完了3个物块
									{	
										USART_Cmd(UART4,DISABLE);
										Last_color = 0;
										Temp[6] = 0;				//清零，case4使用
										Main_Mission = 4;			
									}		break;	
							case 4:	Mortor_Heng_Yi(1500);delay(200);
									Mortor_Straight(-9500-(color-1)*2000);test = 10;break;//前往转角							//走到转角
							default : break;		
						}
					}
					else 
					{
						switch (Main_Mission)
						{						//因为case8直接走到了第一个色环，所以移动2次即可
							case 1:	Zhi_Zou_3();Main_Mission = 2;break;			//走到对应色环
							case 2:	Main_Mission = 1;							//放物块
									switch (Temp[6])
									{
										case 1:TIM_SetCompare4(TIM4,670);break;						
										case 2:TIM_SetCompare4(TIM4,1336);break;
										case 3:TIM_SetCompare4(TIM4,1986);break;
										default: break;
									}
									Jia_QU_fang();
									if(Temp[6] == 3)				//放完了3个物块
									{	
										Last_color = 0;
										Temp[6] = 0;				//清零，case4使用
										Main_Mission = 3;			
									}		break;	
							case 3:	Mortor_Heng_Yi(1300);
									delay(200);
									
									Mortor_Straight(-9600-(color-1)*2000);test = 10;break;//前往转角							//走到转角
							default : break;		
						}
					}break;
			case 10:									//转90°
					if(Temp[14] == 0)
					{
						
						Mortor_5_Stop();
						Mortor_Zi_Zhuan_dir(0);	
						PID_all_par.Setpoint=-90;
						USART_Cmd(USART3, ENABLE);
						TIM_Cmd(TIM1,ENABLE);
						Temp[14] = 1;
					}
					
					yaw_1=(float)stcAngle.Angle[2]/32768*180;			//串口3与陀螺仪通讯
					if(yaw_1 < -100)
					{
						yaw = 360 + yaw_1;
					}
					else yaw = yaw_1;
				
					if(Temp[5] == 1)
					{
						USART_Cmd(USART3,DISABLE);
						TIM_Cmd(TIM1,DISABLE);
						delay(100);
						Mortor_Stop();
						PID_all_par.output=0;
						Temp[5] = 0;
						test = 11;
						Mortor_5_Start();
						Send_usart3();							//角度清零
					}
					break;
			case 11:
					if(Temp[13] == 0)
					{
						GPIO_ResetBits(GPIOA,GPIO_Pin_5);
						Receive_OpenMV[0] = Receive_OpenMV[3];
						Receive_OpenMV[1] = Receive_OpenMV[4];
						Receive_OpenMV[2] = Receive_OpenMV[5];			//更新颜色
						Wu_Liao_Color = 0;
						Qing_Zero();
						Mortor_5(1,6400);
						Mortor_Straight(-4810);
						Mortor_Heng_Yi(-1000);			//靠近转盘
						USART_Cmd(UART4,ENABLE);
						
						Serial_SendByte_UART4(0XFA);Serial_SendByte_UART4(1);Serial_SendByte_UART4(0XFB);
						Temp[13] = 1;
						
						test = 1;
						Flag_JiaQu_1=1;
						Colar_last = 0;
						while(Colar_last == 0){Colar_last = Wu_Liao_Color;};
						
						while(Colar_last==Wu_Liao_Color);
						Serial_SendByte_UART4(0XFA);Serial_SendByte_UART4(7);Serial_SendByte_UART4(0XFB);
						X_Value_goal=197;
						Y_Value_goal=185;					//转盘识别
						Jiao_Zheng2();
						Serial_SendByte_UART4(0XFA);Serial_SendByte_UART4(1);Serial_SendByte_UART4(0XFB);
						Wu_Liao_Color = 0;
						
					}
					else
					{
						Mortor_5(1,3200);
						zhuan_da();
						Mortor_Straight(-23940);
						Mortor_Heng_Yi(-2600);
						test = 0;
					}break;
			default : break;
		}

}
