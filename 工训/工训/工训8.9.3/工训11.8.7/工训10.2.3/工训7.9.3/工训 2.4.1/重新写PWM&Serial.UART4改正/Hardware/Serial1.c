#include "stm32f10x.h"
#include "string.h"
#include "Serial1.h"
#include <stdarg.h>
#include <string.h>
#include "Delay.h"
extern int GG;
extern int next;
u8 UART_RX_BUF[16];
bool isUartRxCompleted = false;
uint8_t Serial_RxData1;
uint8_t Serial_RxFlag1;
#define GET_LOW_BYTE(A) ((uint8_t)(A))
#define GET_HIGH_BYTE(A) ((uint8_t)((A) >> 8))

extern bool isUartRxCompleted;
extern uint8_t LobotRxBuf[16];

uint8_t LobotTxBuf[128];  //????
uint8_t LobotRxBuf[16];
uint16_t batteryVolt;
void moveServo(uint8_t servoID, uint16_t Position, uint16_t Time)
 {
	if (servoID > 31 || !(Time > 0)) {  //??ID????31,??????????
		return;
	}
	LobotTxBuf[0] = LobotTxBuf[1] = FRAME_HEADER;    //????
	LobotTxBuf[2] = 8;
	LobotTxBuf[3] = CMD_SERVO_MOVE;           //????=??????*3+5,??=1*3+5//????????
	LobotTxBuf[4] = 1;                        //????????
	LobotTxBuf[5] = GET_LOW_BYTE(Time);       //????????
	LobotTxBuf[6] = GET_HIGH_BYTE(Time);      //????????
	LobotTxBuf[7] = servoID;                  //??ID
	LobotTxBuf[8] = GET_LOW_BYTE(Position);   //??????????
	LobotTxBuf[9] = GET_HIGH_BYTE(Position);  //??????????

	uartWriteBuf(LobotTxBuf, 10);
}
void moveServosByArray(LobotServo servos[], uint8_t Num, uint16_t Time)
{
	uint8_t index = 7;
	uint8_t i = 0;

	if (Num < 1 || Num > 32 || !(Time > 0)) {
		return;                                          //??????????32,??????
	}
	LobotTxBuf[0] = LobotTxBuf[1] = FRAME_HEADER;      //????
	LobotTxBuf[2] = Num * 3 + 5;                       //???? = ??????*3+5
	LobotTxBuf[3] = CMD_SERVO_MOVE;                    //????????
	LobotTxBuf[4] = Num;                               //????????
	LobotTxBuf[5] = GET_LOW_BYTE(Time);                //????????
	LobotTxBuf[6] = GET_HIGH_BYTE(Time);               //????????

	for (i = 0; i < Num; i++) {                        //??????ID???????
		LobotTxBuf[index++] = servos[i].ID;              //????ID
		LobotTxBuf[index++] = GET_LOW_BYTE(servos[i].Position); //?????????
		LobotTxBuf[index++] = GET_HIGH_BYTE(servos[i].Position);//?????????
	}

	uartWriteBuf(LobotTxBuf, LobotTxBuf[2] + 2);             //??
}
void moveServos(uint8_t Num, uint16_t Time, ...)
{
	uint8_t index = 7;
	uint8_t i = 0;
	uint16_t temp;
	va_list arg_ptr;  //

	va_start(arg_ptr, Time); //?????????
	if (Num < 1 || Num > 32) {
		return;               //??????????32,??????0
	}
	LobotTxBuf[0] = LobotTxBuf[1] = FRAME_HEADER;      //????
	LobotTxBuf[2] = Num * 3 + 5;                //???? = ?????? * 3 + 5
	LobotTxBuf[3] = CMD_SERVO_MOVE;             //??????
	LobotTxBuf[4] = Num;                        //??????
	LobotTxBuf[5] = GET_LOW_BYTE(Time);         //????????
	LobotTxBuf[6] = GET_HIGH_BYTE(Time);        //????????

	for (i = 0; i < Num; i++) {//???????????????ID???????
		temp = va_arg(arg_ptr, int);//????????ID
		LobotTxBuf[index++] = GET_LOW_BYTE(((uint16_t)temp));
		temp = va_arg(arg_ptr, int);  //?????????????
		LobotTxBuf[index++] = GET_LOW_BYTE(((uint16_t)temp)); //?????????
		LobotTxBuf[index++] = GET_HIGH_BYTE(temp);//?????????
	}

	va_end(arg_ptr);  //??arg_ptr

	uartWriteBuf(LobotTxBuf, LobotTxBuf[2] + 2);    //??
}
void runActionGroup(uint8_t numOfAction, uint16_t Times)
{
	LobotTxBuf[0] = LobotTxBuf[1] = FRAME_HEADER;  //????
	LobotTxBuf[2] = 5;                      //????,?????????????,??????5
	LobotTxBuf[3] = CMD_ACTION_GROUP_RUN;   //?????????
	LobotTxBuf[4] = numOfAction;            //??????????
	LobotTxBuf[5] = GET_LOW_BYTE(Times);    //???????????
	LobotTxBuf[6] = GET_HIGH_BYTE(Times);   //???????????

	uartWriteBuf(LobotTxBuf, 7);            //??
}
void stopActionGroup(void)
{
	LobotTxBuf[0] = FRAME_HEADER;     //????
	LobotTxBuf[1] = FRAME_HEADER;
	LobotTxBuf[2] = 2;                //????,?????????????,??????2
	LobotTxBuf[3] = CMD_ACTION_GROUP_STOP;   //???????????

	uartWriteBuf(LobotTxBuf, 4);      //??
}
void setActionGroupSpeed(uint8_t numOfAction, uint16_t Speed)
{
	LobotTxBuf[0] = LobotTxBuf[1] = FRAME_HEADER;   //????
	LobotTxBuf[2] = 5;                       //????,?????????????,??????5
	LobotTxBuf[3] = CMD_ACTION_GROUP_SPEED;  //???????????
	LobotTxBuf[4] = numOfAction;             //??????????
	LobotTxBuf[5] = GET_LOW_BYTE(Speed);     //??????????
	LobotTxBuf[6] = GET_HIGH_BYTE(Speed);    //??????????

	uartWriteBuf(LobotTxBuf, 7);             //??
}
void setAllActionGroupSpeed(uint16_t Speed)
{
	setActionGroupSpeed(0xFF, Speed);  //?????????,???0xFF?????????
}
void getBatteryVoltage(void)
{
	LobotTxBuf[0] = FRAME_HEADER;  //????
	LobotTxBuf[1] = FRAME_HEADER;
	LobotTxBuf[2] = 2;             //????,?????????????,??????2
	LobotTxBuf[3] = CMD_GET_BATTERY_VOLTAGE;  //??????????

	uartWriteBuf(LobotTxBuf, 4);   //??
}

void receiveHandle()
{
	if (isUartRxCompleted) {
		isUartRxCompleted = false;
		switch (LobotRxBuf[3]) {
		case CMD_GET_BATTERY_VOLTAGE: //????
			batteryVolt = (((uint16_t)(LobotRxBuf[5])) << 8) | (LobotRxBuf[4]);
			break;
		default:
			break;
		}
	}
}
int fputc(int ch, FILE *f)
{
        if(ch =='\n')  
        {  
            USART_SendData(USART1, 0x0D);  
            while (!(USART1->SR & USART_FLAG_TXE)); 
            USART_SendData(USART1, 0x0A);  
            while (!(USART1->SR & USART_FLAG_TXE));  
        }  
        else  
        {  
            USART_SendData(USART1, (unsigned char)ch);  
            while (!(USART1->SR & USART_FLAG_TXE));   
        }  
	return ch;
}

void uartNVICInit(void) 
{
}

void uartInit(u32 bound)
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
	USART_InitStructure.USART_BaudRate = bound;    //波特率 9600 11520
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
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;      //优先级
	
	NVIC_Init(&NVIC_InitStructure);
	USART_Cmd(USART1,ENABLE);
}
void uartWriteBuf(uint8_t *buf, uint8_t len)
{
	while (len--) {
		while ((USART1->SR & 0x40) == 0);
		USART_SendData(USART1,*buf++);
	}
}
//void USART1_IRQHandler(void)
//{
//	uint8_t Res;
//	static bool isGotFrameHeader = false;
//	static uint8_t frameHeaderCount = 0;
//	static uint8_t dataLength = 2;
//	static uint8_t dataCount = 0;
//	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) { //??????
//		Res = USART_ReceiveData(USART1);//(USART1->DR);	//?????????
//		if (!isGotFrameHeader) {  //????
//			if (Res == FRAME_HEADER) {
//				frameHeaderCount++;
//				if (frameHeaderCount == 2) {
//					frameHeaderCount = 0;
//					isGotFrameHeader = true;
//					dataCount = 1;
//				}
//			} else {
//				isGotFrameHeader = false;
//				dataCount = 0;
//				frameHeaderCount = 0;
//			}
//		}
//		if (isGotFrameHeader) { //????????
//			UART_RX_BUF[dataCount] = Res;
//			if (dataCount == 2) {
//				dataLength = UART_RX_BUF[dataCount];
//				if (dataLength < 2 || dataLength > 8) {
//					dataLength = 2;
//					isGotFrameHeader = false;
//				}
//			}
//			dataCount++;
//			if (dataCount == dataLength + 2) {
//				if (isUartRxCompleted == false) {
//					isUartRxCompleted = true;
//					memcpy(LobotRxBuf, UART_RX_BUF, dataCount);
//				}
//				isGotFrameHeader = false;
//			}
//		}
//	}
//}
/* **************************************************************************************************************/
void Serial_SendByte1(uint8_t Byte)      //从TX引脚发送一个字节数据
{
	USART_SendData(USART1, Byte);     //把形参传入给DATE 然后高位清零 再将参数写入DR一位一位送到TX引脚
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);   //等待标志位（发送寄存器空标志位） 数据已经被转入到移位寄存器    TXEIE被置1时产生中断执行写操作，对该位置零 防止数据覆盖   不需手动清零
}



uint8_t Serial_GetRxFlag1(void)  //自动清零自己的标志位
{
	if (Serial_RxFlag1 == 1)
	{
		Serial_RxFlag1 = 0;
		return 1;
	}
	return 0;
}

uint8_t Serial_GetRxData1(void) //返回USARTx外设最近收到的数据
{
	return Serial_RxData1;       //返回USARTx外设最近收到的数据
}

void USART1_IRQHandler(void)    //USART1_IRQHandler中断名
{
	if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)    //判断标志位
	{
		Serial_RxData1 = USART_ReceiveData(USART1);    //返回USARTx外设最近收到的数据
		Serial_RxFlag1 = 1;                  //置一个自己的标志位
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);    //读DR寄存器  清除标志位
	}
}
/**************************************************************************************************/

/***************************************************/
void ActionGroup0(void)//机械臂上电初始化
{	
	runActionGroup(101,1);//机械臂上电初始化		
	Delay_ms(0);	
	stopActionGroup();//停止动作组运行
  GG = 61;	
}

void ActionGroup1(void)//机械臂扫完二维码准备抓转盘
{	
	runActionGroup(101,1);//机械臂扫完二维码准备抓转盘
	Delay_ms(0);	
	stopActionGroup();//停止动作组运行		
	GG = 61;
}

void ActionGroup5(void)//抓下
{	
	//放第一个
	runActionGroup(103,1);//机械臂上电初始化		
	Delay_ms(0);	
	stopActionGroup();//停止动作组运行
	next ++;
}
void ActionGroup6(void)//抓左
{	
	//放第二个
	runActionGroup(103,1);//机械臂上电初始化		
	Delay_ms(0);	
	stopActionGroup();//停止动作组运行
	next ++;
}
void ActionGroup7(void)//抓右
{	
	//放第三个
	runActionGroup(103,1);//机械臂上电初始化		
	Delay_ms(0);	
	stopActionGroup();//停止动作组运行
	next ++;
}
void ActionGroup11(void)//抓地
	
{	
	//放第一个
	runActionGroup(103,1);//机械臂上电初始化		
	Delay_ms(0);	
	stopActionGroup();//停止动作组运行
	next ++;
}
void ActionGroup12(void)//抓地
{	
	//放第二个
	runActionGroup(103,1);//机械臂上电初始化		
	Delay_ms(0);	
	stopActionGroup();//停止动作组运行
	next ++;
}
void ActionGroup13(void)//抓地
{	
	//放第三个
	runActionGroup(103,1);//机械臂上电初始化		
	Delay_ms(0);	
	stopActionGroup();//停止动作组运行
	next ++;
}
void ActionGroup21(void)//抓地
{	
	//放第一个
	runActionGroup(103,1);//机械臂上电初始化		
	Delay_ms(0);	
	stopActionGroup();//停止动作组运行
	next ++;
}
void ActionGroup22(void)//抓地
{	
	//放第二个
	runActionGroup(103,1);//机械臂上电初始化		
	Delay_ms(0);	
	stopActionGroup();//停止动作组运行
	next ++;
}
void ActionGroup23(void)//抓地
{	
	//放第三个
	runActionGroup(103,1);//机械臂上电初始化		
	Delay_ms(0);	
	stopActionGroup();//停止动作组运行
	next ++;
}
