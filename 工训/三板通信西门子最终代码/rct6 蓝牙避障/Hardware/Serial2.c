#include "stm32f10x.h"                  // Device header
#include <stdio.h>

uint8_t Serial_RxData2;           //激光传感器模块
uint8_t Serial_RxFlag2;

uint8_t uart_rxpa2[5];
uint8_t uart_txpa2[3];

float  a[]={0xA4,0x03,0x08,0x05}; //帧头

void Serial_Init2(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART2, &USART_InitStructure);
	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_Cmd(USART2, ENABLE);
}


uint8_t Serial_GetRxFlag2(void)
{
	if (Serial_RxFlag2 == 1)
	{
		Serial_RxFlag2 = 0;
		return 1;
	}
	
	return 0;
}

void Serial_SendByte2(uint8_t Byte)
{
	USART_SendData(USART1, Byte);
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
}

void Serial_SendArray2(uint8_t *Array, uint16_t Length)
{
	uint16_t i;
	for (i = 0; i < Length; i ++)
	{
		Serial_SendByte2(Array[i]);
	}
}

void send_hex2(void)
{
    Serial_SendByte2(0xA4);
	  Serial_SendArray2(uart_txpa2,3);
	  Serial_SendByte2(0xB4);
}

void USART2_IRQHandler(void)
{
	 static uint8_t RXnow=0;
	 static uint8_t RXnow_shu=0;
	
	if (USART_GetITStatus(USART2, USART_IT_RXNE) == SET)
	{
		Serial_RxData2=USART_ReceiveData(USART2);
    if(RXnow==0)        //状态机编写
		{
		  if(Serial_RxData2==0xA4)   //接收到帧头
			{
			   RXnow=1;               //转移到状态1
				 RXnow_shu=0;           //状态清零
			}
			
		}
		else if(RXnow==1)
		{
		  uart_rxpa2[RXnow_shu]=Serial_RxData2;
			RXnow_shu++;
			if(RXnow_shu>=5)
			{
			  RXnow=2;
			}
		}
		else if(RXnow==2)
		{
//			if(Serial_RxData==0x5B)
//			{
			     RXnow=0;
				   Serial_RxFlag2=1;     //标志位置为1
//			}
		}
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
	}
}
