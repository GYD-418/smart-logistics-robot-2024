#include "stm32f10x.h"                  // Device header

uint8_t uart_rxpb[5];
uint8_t uart_txpb[3];

uint8_t  Serial_RxData222;
uint8_t  Serial_RxFlag222;

void Serial_Init3(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);     //注意串口3的位置，以免出错！！
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART3, &USART_InitStructure);
	
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_Cmd(USART3, ENABLE);
}


void Serial_SendByte3(uint8_t Byte)
{
	USART_SendData(USART3, Byte);
	while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
}

uint8_t Serial_GetRxFlag3(void)
{
	if (Serial_RxFlag222 == 1)
	{
		Serial_RxFlag222 = 0;
		return 1;
	}
	return 0;
}

uint8_t Serial_GetRxData3(void)
{
	
	return Serial_RxData222;
}

void USART3_IRQHandler(void)
{

	 static uint8_t RXnow=0;
	 static uint8_t RXnow_shu=0;
	
	if (USART_GetITStatus(USART3, USART_IT_RXNE) == SET)
	{
		Serial_RxData222=USART_ReceiveData(USART3);
    if(RXnow==0)        //状态机编写
		{
		  if(Serial_RxData222==0xA4)   //接收到帧头
			{
			   RXnow=1;               //转移到状态1
				 RXnow_shu=0;           //状态清零
			}
			
		}
		else if(RXnow==1)
		{
		  uart_rxpb[RXnow_shu]=Serial_RxData222;
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
				   Serial_RxFlag222=1;     //标志位置为1
//			}
		}
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
	}
}
