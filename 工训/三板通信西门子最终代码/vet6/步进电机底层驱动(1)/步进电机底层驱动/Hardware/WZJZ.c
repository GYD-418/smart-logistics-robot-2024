#include "stm32f10x.h"                  // Device header
/* 上下误差使用机械臂舵机矫正 */
//float RxDatah;  //串口接受具体位置横坐标
//float Kp1=2,Ki1=2,Kd1=1;
//float WeiZhi_Heng_PID(float MUB1 ,float SHJ1)    //目标值是实际点与中心点误差为0
//{
////	SHJ1 = 320 - RxDatah;  //  偏左为正 ， 偏右为负
////	MUB1 = 0;
//	float PWM_Pid_Value;
//	float Error,Error_Last,Error_Last_Last;  //偏差  上一次偏差  上上次偏差	

//	Error= SHJ1 - MUB1;  //偏左为正 偏右为负  
//	
//  //比例  积分  微分
//	PWM_Pid_Value += Kp1*(Error-Error_Last)+Ki1*Error+Kd1*(Error-2*Error_Last+Error_Last_Last);

//	Error_Last_Last = Error_Last; 

//	Error_Last =Error;	

//  return PWM_Pid_Value;	
//}
//float RxDataxz;  //串口接受具体位置横坐标
//float Kp2=2,Ki2=2,Kd2=1;
//float WeiZhi_Zong_PID(float MUB2 ,float SHJ2)    //目标值是实际点与中心点误差为0
//{
////	SHJ2 = 320 - RxDataxz;  //  偏下为正 ， 偏上为负
////	MUB2 = 0;
//	float PWM_Pid_Value;
//	float Error,Error_Last,Error_Last_Last;  //偏差  上一次偏差  上上次偏差	

//	Error= SHJ2 - MUB2;  //偏下为正 偏上为负  
//	
//  //比例  积分  微分
//	PWM_Pid_Value += Kp2*(Error-Error_Last)+Ki2*Error+Kd2*(Error-2*Error_Last+Error_Last_Last);

//	Error_Last_Last = Error_Last; 

//	Error_Last =Error;	

//  return PWM_Pid_Value;	
//}
