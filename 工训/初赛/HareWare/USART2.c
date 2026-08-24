#include "main.h"
#include <stdio.h>
#include <stdarg.h>

/**
  * 函    数：接收二维码识别，发送信息到串口屏
  * 参    数：无
  * 返 回 值：无
  */

uint8_t Flag_OpenMV = 0;
uint8_t Recevie[10];
void USART2_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure; 
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟
	
	 
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2); //GPIOA11复用为USART3
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2); //GPIOA10复用为USART3

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);    
	 
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure);
	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	
	USART_ClearFlag(USART2,USART_IT_RXNE);	
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_Cmd(USART2, ENABLE);
}

/**
  * 函    数：串口发送一个字节
  * 参    数：Byte 要发送的一个字节
  * 返 回 值：无
  */
void Serial_SendByte_USART2(uint8_t Byte)
{
	USART_SendData(USART2, Byte);		//将字节数据写入数据寄存器，写入后USART自动生成时序波形
	while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);	//等待发送完成
	/*下次写入数据寄存器会自动清除发送完成标志位，故此循环后，无需清除标志位*/
}

/**
  * 函    数：串口发送一个字符串
  * 参    数：String 要发送字符串的首地址
  * 返 回 值：无
  */
void Serial_SendString_USART2(char *String)
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i ++)//遍历字符数组（字符串），遇到字符串结束标志位后停止
	{
		Serial_SendByte_USART2(String[i]);		//依次调用Serial_SendByte发送每个字节数据
	}
}


/**
  * 函    数：使用printf需要重定向的底层函数
  * 参    数：保持原始格式即可，无需变动
  * 返 回 值：保持原始格式即可，无需变动
  */
int fputc(int ch, FILE *f)
{
	Serial_SendByte_USART2(ch);			//将printf的底层重定向到自己的发送字节函数
	return ch;
}

/**
  * 函    数：自己封装的prinf函数
  * 参    数：format 格式化字符串
  * 参    数：... 可变的参数列表
  * 返 回 值：无
  */
void Serial_Printf(char *format, ...)
{
	char String[100];				//定义字符数组
	va_list arg;					//定义可变参数列表数据类型的变量arg
	va_start(arg, format);			//从format开始，接收参数列表到arg变量
	vsprintf(String, format, arg);	//使用vsprintf打印格式化字符串和参数列表到字符数组中
	va_end(arg);					//结束变量arg
	Serial_SendString_USART2(String);		//串口发送字符数组（字符串）
}


uint8_t Receive_OpenMV[6]={1,2,3,3,1,2};
uint8_t State_USART2 = 0;
uint8_t Temp_USART2 = 0;
/**
  * 函    数：与OpenMV通信
  * 参    数：
  * 返 回 值：
  */

void USART2_IRQHandler(void)
{
  if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
  {   
	  uint8_t RxData;
	  RxData = USART_ReceiveData(USART2);
	  Recevie[Temp_USART2] = RxData-48;
	  Temp_USART2++;
	  if(Temp_USART2>7)Flag_OpenMV = 1;
	  USART_ClearITPendingBit(USART2, USART_IT_RXNE);
  }
	
}

void USART2_send(void)
{
	Serial_SendByte_USART2(0X16);
	Serial_SendByte_USART2(0X54);
	Serial_SendByte_USART2(0X0D);
}

void open_mv_send(void)
{
	Serial_SendByte_UART4(0XFC);
	Serial_SendByte_UART4(Receive_OpenMV[0]);
	Serial_SendByte_UART4(Receive_OpenMV[1]);
	Serial_SendByte_UART4(Receive_OpenMV[2]);
	Serial_SendByte_UART4(Receive_OpenMV[3]);
	Serial_SendByte_UART4(Receive_OpenMV[4]);
	Serial_SendByte_UART4(Receive_OpenMV[5]);
	Serial_SendByte_UART4(0XFD);

}
	
	