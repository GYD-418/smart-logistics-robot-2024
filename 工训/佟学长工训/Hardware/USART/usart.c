#include "usart.h"
//#include "sys.h"
#include "stdio.h"
#include "delay.h"
#include "stdlib.h"
#include "control.h"
//////////////////////////////////////////////////////////////////
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while((USART2->SR&0X40)==0);//循环发送,直到发送完毕   
    USART2->DR = (u8) ch;      
	return ch;
}
#endif 



/*****************************发送一个字节的数据**************************************/
void Serial_SendByte(USART_TypeDef* USARTx,int8_t Byte)
{
	USART_SendData(USARTx, Byte);
	while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
}
/*****************************发送一个数组的数据**************************************/
void Serial_SendArray(USART_TypeDef* USARTx,uint8_t *Array, uint16_t Length)
{
	uint16_t i;
	for (i = 0; i < Length; i ++)
	{
		Serial_SendByte(USARTx,Array[i]);
	}
}
/*****************************发送一个字符串的数据**************************************/
void Usart_SendStr(USART_TypeDef* USARTx,char *str)
{
	char i=0;
	do
	{
		Serial_SendByte(USARTx,*(str+i));
		i++;
	
	}while(*(str+i) != '\0'); //也可以直接写0，'\0 '是0的转义字符
}


void MYUSART1_Init(int bound)                
{
  //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;         
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟
  
	//USART1_TX   GPIOA.9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.9
   
  //USART1_RX	  GPIOA.10初始化
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.10  

  //Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
   //USART 初始化设置

	USART_InitStructure.USART_BaudRate = bound;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

  USART_Init(USART1, &USART_InitStructure); //初始化串口1
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启串口接受中断
  USART_Cmd(USART1, ENABLE);                    //使能串口1 

}


//u16 USART_RX_STA=0;       //接收状态标记	  
char USART1_RX_ZQ_BUF[13];     //接收缓冲,最大USART_REC_LEN个字节.
u8 RX_Flag = 0;
float speedx = 0;//k210传来的数据
float speedy = 0;
int test_count1112 = 0; //测试一下一共进入了多少次中断

void ChuLi(int H_x, int L_x, int H_y, int L_y)
{
	int h_x = 0;
	int l_x = 0;
	int h_y = 0;
	int l_y = 0;
	
	h_x = H_x; // 高8位数据 
	l_x = L_x; // 低8位数据 
	speedx = ((uint16_t)h_x << 8) | l_x;

	h_y = h_y; // 高8位数据 
	l_y = l_y; // 低8位数据 
	speedy = ((uint16_t)h_y << 8) | l_y;	
			
}
char arr1[4] = {0};
char arr2[4] = {0};	
char arr3[4] = {0};
char arr4[4] = {0};	
void ChuLi_String(void)
{
	int i,j;
		
		for(i = 1;i<4;i++)
		{
			arr3[i-1] = arr1[i];	
		}
		for(j = 1;j<4;j++)
		{
			arr4[j-1] = arr2[j];	
		}
		for(i = 0;i<4;i++)
		{
			arr1[i] = 0;
			arr2[i] = 0;			
		}		
		speedx = atoi(arr3);
		speedy = atoi(arr4);		
}

void USART1_IRQHandler(void)                	//串口1中断服务程序
{
	static int RXState = 0;  //状态机标志位，用来确定数据的接收情况。
	static int RXState_x = 0;  //状态机标志位，用来确定数据的接收情况。
	static int RXState_y = 0;  //状态机标志位，用来确定数据的接收情况。
	static int P_RXState = 0;  //用于确定接收到哪个数据了
	static int Arr1_State = 0;  //用于确定接收到哪个数据了
	static int Arr2_State = 0;  //用于确定接收到哪个数据了
	
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
		{
			USART_ClearFlag(USART1,USART_FLAG_RXNE);	 
			unsigned int RxDate = USART_ReceiveData(USART1);
			test_count1112++;
			if(test_count1112 > 1000) test_count1112 = 0;
			if(RXState == 0)
			{
				if(RxDate == 'a')
				{
					USART1_RX_ZQ_BUF[P_RXState] = RxDate;
					P_RXState++;
					RXState = 1;					
				}
				
			}
			else if(RXState == 1)
			{
				if(RxDate != 'b')
				{
					if(RxDate == 'X')
					{
						RXState_x = 1;
					}
					else if(RxDate == 'Y')
					{
						RXState_y =1;
					}
					if(RXState_x == 1 && RXState_y != 1)
					{
						arr1[Arr1_State++] = RxDate;
					}
					else if(RXState_y == 1)
					{
						arr2[Arr2_State++] = RxDate;
					}
					USART1_RX_ZQ_BUF[P_RXState] = RxDate;
					P_RXState++;				
				}
				else if(RxDate == 'b')
				{
					USART1_RX_ZQ_BUF[P_RXState] = RxDate;
//					ChuLi(USART1_RX_ZQ_BUF[2],USART1_RX_ZQ_BUF[3],USART1_RX_ZQ_BUF[4],USART1_RX_ZQ_BUF[5]);
				  ChuLi_String();
					RXState = 0;	
					P_RXState = 0;
					Arr1_State = 0;
					Arr2_State = 0;
					RXState_y = 0;
					RXState_x = 0;					
				}			
			}
//			USART1_RX_ZQ_BUF[1] = RxDate;
		}
} 

void MYUSART2_Init(int bound)                
{
  //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;         
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟      RCC_APB2Periph_AFIO
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);	//使能USART1，GPIOA时钟      RCC_APB2Periph_AFIO
	
	//USART1_TX   GPIOA.9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PA.9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.2
   
  //USART1_RX	  GPIOA.10初始化
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.3 

  //Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
   //USART 初始化设置

	USART_InitStructure.USART_BaudRate = bound;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

  USART_Init(USART2, &USART_InitStructure); //初始化串口1
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启串口接受中断
  USART_Cmd(USART2, ENABLE);                    //使能串口1 

}


int8_t USART2_RX_BUF_2[8] = {0,1,3,2,3,2,1,0};     //接收缓冲,最大USART_REC_LEN个字节.  0,3,1,2,1,3,2,0
uint8_t USART2_RX_Flag = 0;
void USART2_IRQHandler(void)                	//串口1中断服务程序
		{
	static int RXState_2 = 0;  //状态机标志位，用来确定数据的接收情况。
	static int P_RXState_2 = 0;  //用于确定接收到哪个数据了
	
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
		{
			unsigned int RxDate_2 = USART_ReceiveData(USART2);
			
			if(RXState_2 == 0)
			{
				if(RxDate_2 == 0xff)
				{
					USART2_RX_BUF_2[P_RXState_2] = RxDate_2;
					P_RXState_2++;
					RXState_2 = 1;					
				}
			}
			else if(RXState_2 == 1)
			{
				if(RxDate_2 != 0xfe)
				{
					USART2_RX_BUF_2[P_RXState_2] = RxDate_2;
					P_RXState_2 ++;			
				}
				else if(RxDate_2 == 0xfe)
				{
					USART2_RX_BUF_2[P_RXState_2] = RxDate_2;
					P_RXState_2 = 0;			
					USART2_RX_Flag = 1;				
				}
			}
			USART_ClearFlag(USART2,USART_FLAG_RXNE);	
		}
}

void MYUSART3_Init(int bound)                
{
  //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;         
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//使能USART1，GPIOA时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
  
	//USART1_TX   GPIOA.9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
  GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIO
   
  //USART1_RX	  GPIOA.10初始化
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
  GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIO

  //Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
   //USART 初始化设置

	USART_InitStructure.USART_BaudRate = bound;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

  USART_Init(USART3, &USART_InitStructure); //初始化串口1
	
//  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启串口接受中断
  USART_Cmd(USART3, ENABLE);                    //使能串口1 

}

void MYUART4_Init(int bound)                
{
  //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;         
	USART_InitTypeDef USART_InitStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	//使能USART1，GPIOA时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);
  
	//USART1_TX   GPIOA.9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
  GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化GPIO
   
  //USART1_RX	  GPIOA.10初始化
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
  GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化GPIO

//  //Usart1 NVIC 配置
//  NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
//	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
//  
   //USART 初始化设置

	USART_InitStructure.USART_BaudRate = bound;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

  USART_Init(UART4, &USART_InitStructure); //初始化串口1
	
//  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启串口接受中断
  USART_Cmd(UART4, ENABLE);                    //使能串口1 

}

void Test_ShiBie_WuLiao(void)
{
	Serial_SendByte(USART1,0xf1);
	Serial_SendByte(USART1,0X01);	  //表示识别的是物料
	Serial_SendByte(USART1,0xe1);	
}

void Test_ShiBie_SeHuan(void)
{
	Serial_SendByte(USART1,0xf2);
	Serial_SendByte(USART1,0X02);	  //表示识别的是色环
	Serial_SendByte(USART1,0xe2);	
}
void ChuanKouPing(void)
{
	
	Serial_SendByte(UART4,0X74);
	Serial_SendByte(UART4,0X30);
	Serial_SendByte(UART4,0X2E);
	Serial_SendByte(UART4,0X74);
	Serial_SendByte(UART4,0X78);
	Serial_SendByte(UART4,0X74);
	Serial_SendByte(UART4,0X3D);
	Serial_SendByte(UART4,0X22);	
	switch(USART2_RX_BUF_2[1])
	{
		case 1:
				Serial_SendByte(UART4,0X31);	
			break;
		case 2:
				Serial_SendByte(UART4,0X32);				
			break;
		case 3:
				Serial_SendByte(UART4,0X33);				
			break;
		default:
			break;
	}
	switch(USART2_RX_BUF_2[2])
	{
		case 1:
				Serial_SendByte(UART4,0X31);	
			break;
		case 2:
				Serial_SendByte(UART4,0X32);				
			break;
		case 3:
				Serial_SendByte(UART4,0X33);				
			break;
		default:
			break;
	}
	switch(USART2_RX_BUF_2[3])
	{
		case 1:
				Serial_SendByte(UART4,0X31);	
			break;
		case 2:
				Serial_SendByte(UART4,0X32);				
			break;
		case 3:
				Serial_SendByte(UART4,0X33);				
			break;
		default:
			break;
	}
	
//	Serial_SendByte(UART4,USART2_RX_BUF_2[1]);   //USART2_RX_BUF_2[1]
//	Serial_SendByte(UART4,USART2_RX_BUF_2[2]);       //USART2_RX_BUF_2[2]
//	Serial_SendByte(UART4,USART2_RX_BUF_2[3]);       // USART2_RX_BUF_2[3]
	Serial_SendByte(UART4,0X22);	
	Serial_SendByte(UART4,0XFF);	
	Serial_SendByte(UART4,0XFF);	
	Serial_SendByte(UART4,0XFF);	
/*************************************************************************************/	
	Serial_SendByte(UART4,0X74);
	Serial_SendByte(UART4,0X31);
	Serial_SendByte(UART4,0X2E);
	Serial_SendByte(UART4,0X74);
	Serial_SendByte(UART4,0X78);
	Serial_SendByte(UART4,0X74);
	Serial_SendByte(UART4,0X3D);
	Serial_SendByte(UART4,0X22);		
	switch(USART2_RX_BUF_2[4])
	{
		case 1:
				Serial_SendByte(UART4,0X31);	
			break;
		case 2:
				Serial_SendByte(UART4,0X32);				
			break;
		case 3:
				Serial_SendByte(UART4,0X33);				
			break;
		default:
			break;
	}
	switch(USART2_RX_BUF_2[5])
	{
		case 1:
				Serial_SendByte(UART4,0X31);	
			break;
		case 2:
				Serial_SendByte(UART4,0X32);				
			break;
		case 3:
				Serial_SendByte(UART4,0X33);				
			break;
		default:
			break;
	}
	switch(USART2_RX_BUF_2[6])
	{
		case 1:
				Serial_SendByte(UART4,0X31);	
			break;
		case 2:
				Serial_SendByte(UART4,0X32);				
			break;
		case 3:
				Serial_SendByte(UART4,0X33);				
			break;
		default:
			break;
	}
		
//	Serial_SendByte(UART4,USART2_RX_BUF_2[4]);          //USART2_RX_BUF_2[4]
//	Serial_SendByte(UART4,USART2_RX_BUF_2[5]);         //USART2_RX_BUF_2[5]
//	Serial_SendByte(UART4,USART2_RX_BUF_2[6]);              //USART2_RX_BUF_2[6]
	Serial_SendByte(UART4,0X22);	
	Serial_SendByte(UART4,0XFF);	
	Serial_SendByte(UART4,0XFF);	
	Serial_SendByte(UART4,0XFF);	
}

void Send_To_K210(void)
{
	switch(Choose)
	{
		case 1:    //色块儿
			USART_SendData(USART1,0x31);   //
			break;
		case 2:    //红色色环
			USART_SendData(USART1,0x32);   //
			break;
		case 3:    //绿色色环
			USART_SendData(USART1,0x33);   //
			break;
		case 4:    //蓝色色环
			USART_SendData(USART1,0x34);   //
			break;
		case 5:    //啥也不识别  
			USART_SendData(USART1,0x35);   //
			break;
		default:
		 break;
		
	}
}
