#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <stdarg.h>
/*
		void USART_DeInit(USART_TypeDef* USARTx);         将USART寄存器重置为默认值
		void USART_Init(USART_TypeDef* USARTx, USART_InitTypeDef* USART_InitStruct);   根据结构体的参数配置来对USARTx外设进行初始化
		void USART_StructInit(USART_InitTypeDef* USART_InitStruct);     将USART_InitStructure结构体变量成员按默认值填充
		void USART_ClockInit(USART_TypeDef* USARTx, USART_ClockInitTypeDef* USART_ClockInitStruct);		根据USART_ClockInitStruct中的指定参数初始化USARTx外围时钟
		void USART_ClockStructInit(USART_ClockInitTypeDef* USART_ClockInitStruct);     （时钟输出）将USART_ClockInitStructure结构体变量成员按默认值填充  只是填充不写入寄存器
		void USART_Cmd(USART_TypeDef* USARTx, FunctionalState NewState);          使能或者失能USART外设
		void USART_ITConfig(USART_TypeDef* USARTx, uint16_t USART_IT, FunctionalState NewState);        配置指定的USART中断 使能中断
		void USART_DMACmd(USART_TypeDef* USARTx, uint16_t USART_DMAReq, FunctionalState NewState);      使能或者失能USART的DMA请求（开启USART到DMA的触发通道，如果数据帧过大可以考虑DMA）
		void USART_SendData(USART_TypeDef* USARTx, uint16_t Data);     写    通过USARTx外设传输单个字节数据
		uint16_t USART_ReceiveData(USART_TypeDef* USARTx);             读   返回由USARTx外设接收的最新数据
		FlagStatus USART_GetFlagStatus(USART_TypeDef* USARTx, uint16_t USART_FLAG);           检查指定的USART标志是否设置，获取当前的USARTx状态
		void USART_ClearFlag(USART_TypeDef* USARTx, uint16_t USART_FLAG);                清除USARTx的挂起标志
		ITStatus USART_GetITStatus(USART_TypeDef* USARTx, uint16_t USART_IT);            检查指定的USART中断是否已经发生
		void USART_ClearITPendingBit(USART_TypeDef* USARTx, uint16_t USART_IT);          清除USARTx的中断挂起位
*/

/*
   1.开启时钟，打开USART和GPIO时钟打开
	 2.初始化GPIO，把TX配置成复用输出，RX配置成输入
	 3.配置USART，直接使用一个结构体
	 4.配置接收功能，需要中断
*/
uint16_t Serial1_TxPacket[5];				//FF 01 02 03 04 FE
uint16_t Serial1_RxPacket[5];
uint8_t Serial1_RxFlag;
uint8_t Cx,Cy;

void Serial1_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);    //开启USART时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);    //开启GPIO时钟
	
	GPIO_InitTypeDef GPIO_InitStructure;  //结构体
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出，一个IO口只能有一个输出，但是可以有多个输入  串口空闲状态时是高电平，不能使用下拉输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;         //配置通道映射引脚  TX
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);      //初始化 GPIOA 结构体

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  //上拉输入，一个IO口只能有一个输出，但是可以有多个输入  串口空闲状态时是高电平，不能使用下拉输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;         //配置通道映射引脚   RX
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);      //初始化 GPIOA 结构体
	
	USART_InitTypeDef USART_InitStructure;     //结构体
	USART_InitStructure.USART_BaudRate = 9600;    //波特率 9600 11520
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;   //硬件流控制（不使用控流）
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;  //TX 发送   Rx接收
	USART_InitStructure.USART_Parity = USART_Parity_No;    //无校验   （无校验 奇校验 偶校验）
	USART_InitStructure.USART_StopBits = USART_StopBits_1;    //停止位   一般使用1位校验位
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;   //字长 八位或者九位
	USART_Init(USART1, &USART_InitStructure);       //初始化 USART 结构体
	//如果使用（查询/中断）中断模式  开NVIC通道
	//一旦置1   就向NVIC申请中断 在中断中接收数据
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);  //（USART_CR1寄存器中的RXNEIE为1，则产生中断。）开启RXNE标志位到NVIC的输出 
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //中断分组

	NVIC_InitTypeDef    NVIC_InitStructure;   //初始化NVIC的USART1通道
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;     //中断通道
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;      //优先级
	
	NVIC_Init(&NVIC_InitStructure);
	USART_Cmd(USART1, ENABLE);              //使能USART1时钟
}


void Serial1_SendByte(uint8_t Byte)      //从TX引脚发送一个字节数据
{
	USART_SendData(USART1, Byte);     //把形参传入给DATE 然后高位清零 再将参数写入DR一位一位送到TX引脚
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);   //等待标志位（发送寄存器空标志位） 数据已经被转入到移位寄存器    TXEIE被置1时产生中断执行写操作，对该位置零 防止数据覆盖   不需手动清零
}

void Serial1_SendArray(uint16_t *Array, uint16_t Length)  //串口发送数组  数组发送需要指针
{
	uint16_t i;
	for (i = 0; i < Length; i ++)
	{
		Serial1_SendByte(Array[i]);
	}
}

void Serial1_SendPacket(void)
{
	Serial1_SendByte(0xFF);       //发送包头
	Serial1_SendArray(Serial1_TxPacket, 5);   //发送数组  长度为4
	Serial1_SendByte(0xFE);     //发送包尾
}

uint8_t Serial1_GetRxFlag(void)
{
	if (Serial1_RxFlag == 1)
	{
		Serial1_RxFlag = 0;
		return 1;
	}
	return 0;
}

void USART1_IRQHandler(void)
{
	static uint8_t RxState = 0;
	static uint8_t pRxPacket = 0;
	if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
	{
		uint8_t RxData = USART_ReceiveData(USART1);
		
		if (RxState == 0)     //等待包头
		{
			if (RxData == 0x7F)  //接受包头
			{
				RxState = 1;
				pRxPacket = 0;
			}
		}
		else if (RxState == 1)     //等待包头
		{
			if (RxData == 0x7F)  //接受包头
			{
				RxState = 2;
				pRxPacket = 0;
			}
		}

		else if (RxState == 2)   //等待数据     多if判断时用else if 因为全局变量变化的时候 可能会造成两个if判断都成立的结果
		{
			Serial1_RxPacket[pRxPacket] = RxData;    //缓存数据    每进入一次判断 缓存一次数组中的一位 位置每一次都加一
			pRxPacket ++;
			if (pRxPacket >= 5)
			{
				RxState = 3;
			}
		}
		else if (RxState == 3)   //等待包尾
		{
			if (RxData == 0x5B)     //接收包尾
			{				
				RxState = 0;
				Serial1_RxFlag = 1;
			}
		}
		
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	}
}
