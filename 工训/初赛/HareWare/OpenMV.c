#include "main.h"





void OpenMV_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure; 
	
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟
	
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource0,GPIO_AF_UART4); //GPIOA11复用为USART3
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource1,GPIO_AF_UART4); //GPIOA10复用为USART3

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_0;
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
	USART_Init(UART4, &USART_InitStructure);
	
	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);
	
	USART_ClearFlag(UART4,USART_IT_RXNE);	
	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_Cmd(UART4, DISABLE);
}



/**
  * 函    数：串口发送一个字节
  * 参    数：Byte 要发送的一个字节
  * 返 回 值：无
  */
void Serial_SendByte_UART4(uint8_t Byte)
{
	USART_SendData(UART4, Byte);		//将字节数据写入数据寄存器，写入后USART自动生成时序波形
	while (USART_GetFlagStatus(UART4, USART_FLAG_TXE) == RESET);	//等待发送完成
	/*下次写入数据寄存器会自动清除发送完成标志位，故此循环后，无需清除标志位*/
}


uint8_t State_UART4 = 0;			//标志位
uint8_t Receive_SeHuan[4];
uint8_t Wu_Liao_Color = 0;			//接收OpenMV传回来的当前的颜色
uint16_t X = 199;					//实时的坐标值
uint16_t Y = 190;

/**
  * 函    数：与OpenMV通信
  * 参    数：
  * 返 回 值：
  * 	  注：加入滤波，5个数据，只要中间的一个数据
  */

void UART4_IRQHandler(void)
{
	if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)
	{   
		uint8_t RxData;
		RxData = USART_ReceiveData(UART4);
		if(RxData == 0XDD &&State_UART4 == 0)			//DD,DE识别物料的帧头和帧尾
		{
			State_UART4 = 1;
		}
		else if(RxData == 0XAA &&State_UART4 == 0)		//色环
		{
			State_UART4 = 2;
		}
		else if(State_UART4 == 2)						//X,Y坐标
		{
			if(RxData == 0XAB)
			{											//将数据存到数组中
				X = (Receive_SeHuan[1]<<8) + Receive_SeHuan[0];
				Y = (Receive_SeHuan[3]<<8) + Receive_SeHuan[2];		
				Temp[12] = 0;							//接收完4个字节
				State_UART4 = 0;
			}
			else 
			{
				Receive_SeHuan[Temp[12]] = RxData;
				Temp[12]++;
			}
		}

		else if(State_UART4 == 1)
		{
			if(RxData == 0XDE)
			{
				State_UART4 = 0;
			}
			else
			{
				Wu_Liao_Color = RxData + 1;
			}
	  }
	 									
	  USART_ClearITPendingBit(UART4, USART_IT_RXNE);
	}
	
}


