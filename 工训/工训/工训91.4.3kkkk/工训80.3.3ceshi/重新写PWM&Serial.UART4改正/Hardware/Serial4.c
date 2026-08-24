#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include "Serial4.h"
#include "string.h"

float yaw;

struct SAcc 		stcAcc;
struct SGyro 		stcGyro;
struct SAngle 		stcAngle;

static unsigned char TxBuffer[256];
static unsigned char TxCounter=0;
static unsigned char count=0; 

extern void CopeSerial2Data(unsigned char ucData);
char angle_buf[30];

void Serial4_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
	USART_DeInit(UART4);
	
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
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(UART4, &USART_InitStructure);
	
	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_Cmd(UART4, ENABLE);
}
void UART4_IRQHandler(void)
{
  if(USART_GetITStatus(UART4, USART_IT_TXE) != RESET)
  {   
    USART_SendData(UART4, TxBuffer[TxCounter++]); 
    USART_ClearITPendingBit(UART4, USART_IT_TXE);
    if(TxCounter == count) USART_ITConfig(UART4, USART_IT_TXE, DISABLE);
  }
	else if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)
  {
		CopeSerial2Data((unsigned char)UART4->DR);//处理数据
		USART_ClearITPendingBit(UART4, USART_IT_RXNE);
  }
	
	USART_ClearITPendingBit(UART4,USART_IT_ORE);
}
void CopeSerial2Data(unsigned char ucData)
{
	static unsigned char ucRxBuffer[250];
	static unsigned char ucRxCnt = 0;	
	
//	LED_REVERSE();					//接收到数据，LED灯闪烁一下
	ucRxBuffer[ucRxCnt++]=ucData;	//将收到的数据存入缓冲区中
	if (ucRxBuffer[0]!=0x55) //数据头不对，则重新开始寻找0x55数据头
	{
		ucRxCnt=0;
		return;
	}
	if (ucRxCnt<11) {return;}//数据不满11个，则返回
	else
	{
		switch(ucRxBuffer[1])//判断数据是哪种数据，然后将其拷贝到对应的结构体中，有些数据包需要通过上位机打开对应的输出后，才能接收到这个数据包的数据
		{
			//memcpy为编译器自带的内存拷贝函数，需引用"string.h"，将接收缓冲区的字符拷贝到数据结构体里面，从而实现数据的解析。
//			case 0x51:	memcpy(&stcAcc,&ucRxBuffer[2],8);break;
			case 0x52:	memcpy(&stcGyro,&ucRxBuffer[2],8);break;
			case 0x53:	memcpy(&stcAngle,&ucRxBuffer[2],8);break;
		}
		ucRxCnt=0;//清空缓存区
	}
}

void Usart_SendHalfWord(USART_TypeDef * pUSARTx, uint16_t ch)
{
	uint8_t High;	
	High =ch;
	USART_SendData(pUSARTx,High);	
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
}

 void MyUSART_SendArr(uint16_t *str,uint8_t count)
{
	uint8_t i=0;
	for(i=0;i<count;i++)
	{
		Usart_SendHalfWord(USART3,*(str+i));
	}
	while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET);
}

