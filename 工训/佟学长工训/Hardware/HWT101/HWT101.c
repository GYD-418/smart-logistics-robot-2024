#include <string.h>
#include "HWT101.h"

float yaw;

struct SAcc 		stcAcc;
struct SGyro 		stcGyro;
struct SAngle 	stcAngle;

static unsigned char TxBuffer[256];
static unsigned char TxCounter=0;
static unsigned char count=0; 

extern void CopeSerial2Data(unsigned char ucData);
char angle_buf[30];


void USART3_Init(unsigned long baudrate)
{
 	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure; 
	
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);	//使能USART1，GPIOA时钟|RCC_APB2Periph_GPIOD
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//使能USART1，GPIOA时钟|
//	GPIO_PinRemapConfig(GPIO_FullRemap_USART3, ENABLE);
//	USART_DeInit(USART3);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure); 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	 
	USART_InitStructure.USART_BaudRate = baudrate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART3, &USART_InitStructure); 
//	USART_ITConfig(USART3, USART_IT_TXE, DISABLE);    
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	USART_ClearFlag(USART3,USART_FLAG_TC);	
	USART_Cmd(USART3, ENABLE);

	
	
}



void USART3_IRQHandler(void)
{
  if(USART_GetITStatus(USART3, USART_IT_TXE) != RESET)
  {   
    USART_SendData(USART3, TxBuffer[TxCounter++]); 
    USART_ClearITPendingBit(USART3, USART_IT_TXE);
    if(TxCounter == count) USART_ITConfig(USART3, USART_IT_TXE, DISABLE);
  }
	else if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
  {
		CopeSerial2Data((unsigned char)USART3->DR);//处理数据
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
  }
	
	USART_ClearITPendingBit(USART3,USART_IT_ORE);
}

/*******************************************

void UART3_Put_Char(unsigned char DataToSend)
{
	TxBuffer[count++] = DataToSend;  
  USART_ITConfig(USART3, USART_IT_TXE, ENABLE);  
}
//这一块儿，没有用到
void UART3_Put_String(unsigned char *Str)
{
	while(*Str)
	{
		if(*Str=='\r')UART3_Put_Char(0x0d);
			else if(*Str=='\n')UART3_Put_Char(0x0a);
				else UART3_Put_Char(*Str);
		Str++;
	}
}



//用串口3给JY模块发送指令
void sendcmd(char cmd[])
{
	char i;
	for(i=0;i<5;i++)
		UART3_Put_Char(cmd[i]);
}
*******************************************/


//CopeSerialData为串口3中断调用函数，串口每收到一个数据，调用一次这个函数。
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
			case 0x51:	memcpy(&stcAcc,&ucRxBuffer[2],8);break;
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
/*****************  发送16Bit数组数据**********************/
 void MyUSART_SendArr(uint16_t *str,uint8_t count)
{
	uint8_t i=0;
	for(i=0;i<count;i++)
	{
		Usart_SendHalfWord(USART3,*(str+i));
	}
	while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET);
}
