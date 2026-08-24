#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <stdarg.h>

uint8_t Serial_RxPacket_4[3];
uint8_t Serial_TxPacket_4[9];				//FF 01 02 03 04 FE

uint8_t Serial_RxFlag_4;
void Serial_Init4(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;     //TX
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(UART4, &USART_InitStructure);
	
	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_Cmd(UART4, ENABLE);
}
void Serial_SendByte4(uint8_t Byte)
{
	USART_SendData(UART4, Byte);
	while (USART_GetFlagStatus(UART4, USART_FLAG_TXE) == RESET);
}

void Serial_SendArray4(uint8_t *Array, uint16_t Length)  //串口发送数组  数组发送需要指针
{
	uint16_t i;
	for (i = 0; i < Length; i ++)
	{
		Serial_SendByte4(Array[i]);
	}
}

uint8_t Serial_GetRxFlag_4(void)
{
	if (Serial_RxFlag_4 == 1)
	{
		Serial_RxFlag_4 = 0;
		return 1;
	}
	return 0;
}
void Serial_SendPacket4(void)
{
	Serial_SendByte4(0xFF);       //发送包头
	Serial_SendArray4(Serial_TxPacket_4, 9);   //发送数组  长度为4
	Serial_SendByte4(0xFE);     //发送包尾
}

void USART4_IRQHandler(void)
{
	static uint8_t RxState = 0;
	static uint8_t pRxPacket = 0;
	if (USART_GetITStatus(UART4, USART_IT_RXNE) == SET)
	{
		uint8_t RxData = USART_ReceiveData(UART4);
		
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
			Serial_RxPacket_4[pRxPacket] = RxData;    //缓存数据    每进入一次判断 缓存一次数组中的一位 位置每一次都加一
			pRxPacket ++;
			if (pRxPacket >= 3)
			{
				RxState = 2;
			}
		}
		else if (RxState == 2)   //等待包尾
		{
			if (RxData == 0xFE)     //接收包尾
			{
				RxState = 0;
				Serial_RxFlag_4 = 1;
			}
		}
		
		USART_ClearITPendingBit(UART4, USART_IT_RXNE);
	}
}
