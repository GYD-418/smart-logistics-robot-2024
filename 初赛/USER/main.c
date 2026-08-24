#include "main.h"

extern struct SAcc 		stcAcc;
extern struct SGyro 	stcGyro;
extern struct SAngle 	stcAngle;

unsigned char data[]={'1','3'};
float yaw;
float yaw_1;
uint16_t ch[6]={0XFF,0XAA,0X76,0X00,0X00};//角度归零数组

uint16_t X_Value_goal=197;
uint16_t Y_Value_goal=185;


uint16_t Temp[16];						//用作临时变量,已用0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15
uint8_t test = 0;
int main(void)
{
	Deng_Dai_Init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	//设置中断优先级分组为组2
	USART3_Init(115200);								//HWT101
	usart_init();										//电机USART1初始化
	TIM_PID_XUAN();										//失能状态TIM1
	TIM2_PID_X();										//X,Y轴校正
	TIM3_PID_Y();
	USART2_Init();										//二维码
	Servo_Init();										//舵机初始化
	OpenMV_Init();										//用于识别物料与色环
	PWM_Init();
	PWM_Init_2();
	
	PID_X_Para();										//X设置参数
	PID_Y_Para();										//Y设置参数
	PID_ZI_Para();										//角度环PID参数初始化
/*****************************************************************************************
以下为车子出发前到夹取物块的操作，放在while前面，只执行一次
****************************************************************************************/	
//	Mortor_5(0,1680);				
//	Mortor_Heng_Yi(2600);
	TIM_SetCompare3(TIM4,1420);		//1420关		750开
	TIM_SetCompare4(TIM4,670);		//			670,1336,1986
	TIM_SetCompare2(TIM4,685);	
////	zhuan_xiao();					//向外转
////	zhuan_da();						//向内转
//	USART2_send();
//	Mortor_Straight(6550);
//	
//	while(Flag_OpenMV == 0);
//	Receive_OpenMV[0]=Recevie[0];Receive_OpenMV[1]=Recevie[1];Receive_OpenMV[2]=Recevie[2];Receive_OpenMV[3]=Recevie[4];Receive_OpenMV[4]=Recevie[5];Receive_OpenMV[5]=Recevie[6];
//	USART_Cmd(USART2,DISABLE);
//	USART_Cmd(UART4,ENABLE);
//	open_mv_send();
//	
//	Mortor_Straight(12300);								//前往二维码区
//	Mortor_Heng_Yi(-1250);
//	Serial_SendByte_UART4(0XFA);Serial_SendByte_UART4(1);Serial_SendByte_UART4(0XFB);
//	uint8_t lin=0;
//	while(lin==0){lin=Wu_Liao_Color;};
//	while(lin==Wu_Liao_Color);
//	Serial_SendByte_UART4(0XFA);Serial_SendByte_UART4(7);Serial_SendByte_UART4(0XFB);
//	X_Value_goal=197;Y_Value_goal=185;delay(2000);			Jiao_Zheng();
//	Serial_SendByte_UART4(0XFA);Serial_SendByte_UART4(1);Serial_SendByte_UART4(0XFB);
///*****************************************************************************************
//****************************************************************************************/
//	test = 1;
	while(1)
	{

		Main_Action();
		
	}
}

