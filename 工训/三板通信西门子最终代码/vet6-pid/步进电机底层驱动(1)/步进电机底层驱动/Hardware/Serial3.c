#include "stm32f10x.h"                  // Device header


uint8_t  Serial_RxData111;
uint8_t Serial_RxPacket_3[3];
uint8_t Serial_RxFlag_3;

void Serial_Init3(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD|RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);     //注意串口3的位置，以免出错！！
	GPIO_PinRemapConfig(GPIO_FullRemap_USART3,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART3, &USART_InitStructure);
	
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
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
	if (Serial_RxFlag_3 == 1)
	{
		Serial_RxFlag_3 = 0;
		return 1;
	}
	return 0;
}


void USART3_IRQHandler(void)
{
	static uint8_t RxState = 0;
	static uint8_t pRxPacket = 0;
	if (USART_GetITStatus(USART3, USART_IT_RXNE) == SET)
	{
		uint8_t RxData = USART_ReceiveData(USART3);
		
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
				Serial_RxFlag_3 = 1;
			}
		}
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
	}
}
