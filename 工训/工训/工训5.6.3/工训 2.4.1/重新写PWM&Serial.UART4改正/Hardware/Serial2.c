#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <stdarg.h>
uint8_t Serial_hinght_erweima[7];
uint8_t Serial_RxPacket_3[8];
uint8_t Serial_hinght_xy[12];
uint8_t Serial_hinght_xy_last[12];
uint8_t Serial_RxFlag_2;
void Serial_Init22(void)
{
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);    //开启USART时钟
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);    //开启GPIO时钟
//	
//	GPIO_InitTypeDef GPIO_InitStructure;  //结构体
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出，一个IO口只能有一个输出，但是可以有多个输入  串口空闲状态时是高电平，不能使用下拉输入
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;         //配置通道映射引脚  TX
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);      //初始化 GPIOA 结构体

//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  //上拉输入，一个IO口只能有一个输出，但是可以有多个输入  串口空闲状态时是高电平，不能使用下拉输入
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;         //配置通道映射引脚   RX
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);      //初始化 GPIOA 结构体
//	
//	USART_InitTypeDef USART_InitStructure;     //结构体
//	USART_InitStructure.USART_BaudRate = 9600;    //波特率 9600 11520
//	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;   //硬件流控制（不使用控流）
//	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;  //TX 发送   Rx接收
//	USART_InitStructure.USART_Parity = USART_Parity_No;    //无校验   （无校验 奇校验 偶校验）
//	USART_InitStructure.USART_StopBits = USART_StopBits_1;    //停止位   一般使用1位校验位
//	USART_InitStructure.USART_WordLength = USART_WordLength_8b;   //字长 八位或者九位
//	USART_Init(USART2, &USART_InitStructure);       //初始化 USART 结构体
//	//如果使用（查询/中断）中断模式  开NVIC通道
//	//一旦置1   就向NVIC申请中断 在中断中接收数据
//	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);  //（USART_CR1寄存器中的RXNEIE为1，则产生中断。）开启RXNE标志位到NVIC的输出 
//	
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //中断分组

//	NVIC_InitTypeDef    NVIC_InitStructure;   //初始化NVIC的USART1通道
//	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;     //中断通道
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //优先级
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;      //优先级
//	
//	NVIC_Init(&NVIC_InitStructure);
//	USART_Cmd(USART2, ENABLE);
}

uint8_t Serial_GetRxFlag_2(void)
{
	if (Serial_RxFlag_2 == 1)
	{
		Serial_RxFlag_2 = 0;
		return 1;
	}
	return 0;
}
void USART2_IRQHandler(void)
{
	static uint8_t RxState = 0;
	static uint8_t pRxPacket = 0;
	if (USART_GetITStatus(USART2, USART_IT_RXNE) == SET)
	{
		uint8_t RxData = USART_ReceiveData(USART2);
		
		if (RxState == 0)     //等待包头
		{
			if (RxData == 0xFF)  //接受包头
			{
				RxState = 1;
				pRxPacket = 0;
			}
		}
		else if (RxState == 1)   //等待数据     多if判断时用else if 因为全局变量变化的时候 可能会造成两个if判断都成立的结果
		{
			Serial_RxPacket_3[pRxPacket] = RxData;    //缓存数据    每进入一次判断 缓存一次数组中的一位 位置每一次都加一
			pRxPacket ++;
			if (pRxPacket >= 8)
			{
				RxState = 2;
			}
		}
		else if (RxState == 2)   //等待包尾
		{
			if (RxData == 0xFE)     //接收包尾
			{
				RxState = 0;
				Serial_RxFlag_2 = 1;
			}
		}
		
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
	}
}
