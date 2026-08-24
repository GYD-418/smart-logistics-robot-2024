#include "stm32f10x.h"                  // Device header
//语音

uint8_t  Serial_RxData111;
uint8_t Serial_RxFlag111;

void Serial_Init3(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	/* 时钟设置 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	/* USART3 TX PB10 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	/* USART3 RX PB11 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
											
	/* USART 设置 */
	USART_DeInit(USART3);
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART3, &USART_InitStructure);
	
	/* 使能接收中断*/
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); 

	/* 使能USART3 */
	USART_Cmd(USART3, ENABLE);
	/* 清除中断标志位 */
	USART_ClearFlag(USART3, USART_FLAG_TC);

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);		   

	/* Enable the USART1 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;			 //′??ú?D??éè??
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}


void Serial_SendByte3(uint8_t Byte)
{
	USART_SendData(USART3, Byte);
	while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
}

uint8_t Serial_GetRxFlag3(void)
{
	if (Serial_RxFlag111 == 1)
	{
		Serial_RxFlag111 = 0;
		return 1;
	}
	return 0;
}

uint8_t Serial_GetRxData3(void)
{
	return Serial_RxData111;
}

void USART3_IRQHandler(void)
{
	if (USART_GetITStatus(USART3, USART_IT_RXNE) == SET)
	{
		Serial_RxData111 = USART_ReceiveData(USART3);
		Serial_RxFlag111 = 1;
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
	}
}
