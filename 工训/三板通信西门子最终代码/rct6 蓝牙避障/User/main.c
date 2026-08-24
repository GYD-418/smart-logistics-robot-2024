#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Serial.h"
#include "Serial2.h"
#include "Serial3.h"
#include "Timer.h"
#include "Serial4.h"
#include "pwm.h"
#include "Key.h"
/* 32向电脑发送数据包    电脑向32发送数据包 */
int Num;
int Cnt;
int Blue_RxData;
int main(void)
{
	OLED_Init();
	Serial_Init();
  Serial_Init2();
	Serial_Init3();
	Serial_Init4();
	PWM_Init(100,720,50);  //PWM初始化
  Timer_Init(100,720);
	Motor_Init();



	while (1)
	{

		if((uart_rxpa2[3]<<8)|(uart_rxpa2[4])<70) Serial_TxPacket_4[1] = 0x09;     //前
		else Serial_TxPacket_4[1] = 0x07;
		
		if((uart_rxpb3[3]<<8)|(uart_rxpb3[4])<70) Serial_TxPacket_4[2] = 0x09;      //左
		else Serial_TxPacket_4[2] = 0x08;
		if (Serial_GetRxFlag() == 1)    //不断判断RXNE的标志位 如果置1了 说明收到数据  TXE:发送数据寄存器空
		{
				  Blue_RxData = Serial_GetRxData();     //返回USART1最新接收到的数据 赋值给RxData
//				  Serial_SendByte(0x13);
			    Serial_TxPacket_4[0] = Blue_RxData;
		}
		Serial_SendPacket4();
	}
}
void TIM4_IRQHandler(void)
{
	
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) == SET)  //获取中断标志位
	{	
    if(Blue_RxData == 0x66)   //上
		{
			Num++;
			Motor_SetSpeed(1);
			if(Num <= 20000)
			{
				PWM_ENABLE();
			}
			else
			{
				PWM_DISABLE();
			}
		}
		
		if(Blue_RxData == 0x99)   //下
		{
			Num++;
			Motor_SetSpeed(0);
			if(Num <= 20000)
			{
				PWM_ENABLE();
			}
			else
			{
				PWM_DISABLE();
			}
		}
     if(Blue_RxData == 0x06)
		 {			  
			 Num = 0;
				PWM_DISABLE(); 
		 }
//		
//    if(Blue_RxData == 0x15)   //开始启动 串口三收到0x13发送串口
//		{
//			Serial_TxPacket_4[0] = 0x15;   //后退
//		}
//		
//    if(Blue_RxData == 0x16)   //开始启动 串口三收到0x13发送串口
//		{
//			Serial_TxPacket_4[0] = 0x16;   //左移
//		}
//		
//    if(Blue_RxData == 0x17)   //开始启动 串口三收到0x13发送串口
//		{
//			Serial_TxPacket_4[0] = 0x17;   //右移
//		}
//		
//    if(Blue_RxData == 0x18)   //开始启动 串口三收到0x13发送串口
//		{
//			Serial_TxPacket_4[0] = 0x18;   //stop
//		}

		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);  //清除中断标志位
	}
}
