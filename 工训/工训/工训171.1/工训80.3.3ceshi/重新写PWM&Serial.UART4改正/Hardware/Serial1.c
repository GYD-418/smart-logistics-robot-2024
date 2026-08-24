#include "stm32f10x.h"
#include "string.h"
#include "Serial1.h"
#include <stdarg.h>
#include <string.h>
#include "Delay.h"
extern int next;
extern int GG;
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
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);    //¿ªÆôUSARTÊ±ÖÓ
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);    //¿ªÆôGPIOÊ±ÖÓ
	
	GPIO_InitTypeDef GPIO_InitStructure;  //½á¹¹Ìå
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //¸´ÓÃÍÆÍìÊä³ö£¬Ò»¸öIO¿ÚÖ»ÄÜÓĞÒ»¸öÊä³ö£¬µ«ÊÇ¿ÉÒÔÓĞ¶à¸öÊäÈë  ´®¿Ú¿ÕÏĞ×´Ì¬Ê±ÊÇ¸ßµçÆ½£¬²»ÄÜÊ¹ÓÃÏÂÀ­ÊäÈë
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;         //ÅäÖÃÍ¨µÀÓ³ÉäÒı½Å  TX
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);      //³õÊ¼»¯ GPIOA ½á¹¹Ìå

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  //ÉÏÀ­ÊäÈë£¬Ò»¸öIO¿ÚÖ»ÄÜÓĞÒ»¸öÊä³ö£¬µ«ÊÇ¿ÉÒÔÓĞ¶à¸öÊäÈë  ´®¿Ú¿ÕÏĞ×´Ì¬Ê±ÊÇ¸ßµçÆ½£¬²»ÄÜÊ¹ÓÃÏÂÀ­ÊäÈë
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;         //ÅäÖÃÍ¨µÀÓ³ÉäÒı½Å   RX
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);      //³õÊ¼»¯ GPIOA ½á¹¹Ìå
	
	USART_InitTypeDef USART_InitStructure;     //½á¹¹Ìå
	USART_InitStructure.USART_BaudRate = bound;    //²¨ÌØÂÊ 9600 11520
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;   //Ó²¼şÁ÷¿ØÖÆ£¨²»Ê¹ÓÃ¿ØÁ÷£©
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;  //TX ·¢ËÍ   Rx½ÓÊÕ
	USART_InitStructure.USART_Parity = USART_Parity_No;    //ÎŞĞ£Ñé   £¨ÎŞĞ£Ñé ÆæĞ£Ñé Å¼Ğ£Ñé£©
	USART_InitStructure.USART_StopBits = USART_StopBits_1;    //Í£Ö¹Î»   Ò»°ãÊ¹ÓÃ1Î»Ğ£ÑéÎ»
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;   //×Ö³¤ °ËÎ»»òÕß¾ÅÎ»
	USART_Init(USART1, &USART_InitStructure);       //³õÊ¼»¯ USART ½á¹¹Ìå
	//Èç¹ûÊ¹ÓÃ£¨²éÑ¯/ÖĞ¶Ï£©ÖĞ¶ÏÄ£Ê½  ¿ªNVICÍ¨µÀ
	//Ò»µ©ÖÃ1   ¾ÍÏòNVICÉêÇëÖĞ¶Ï ÔÚÖĞ¶ÏÖĞ½ÓÊÕÊı¾İ
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);  //£¨USART_CR1¼Ä´æÆ÷ÖĞµÄRXNEIEÎª1£¬Ôò²úÉúÖĞ¶Ï¡££©¿ªÆôRXNE±êÖ¾Î»µ½NVICµÄÊä³ö 
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //ÖĞ¶Ï·Ö×é

	NVIC_InitTypeDef    NVIC_InitStructure;   //³õÊ¼»¯NVICµÄUSART1Í¨µÀ
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;     //ÖĞ¶ÏÍ¨µÀ
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //ÓÅÏÈ¼¶
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;      //ÓÅÏÈ¼¶
	
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
void Serial_SendByte1(uint8_t Byte)      //´ÓTXÒı½Å·¢ËÍÒ»¸ö×Ö½ÚÊı¾İ
{
	USART_SendData(USART1, Byte);     //°ÑĞÎ²Î´«Èë¸øDATE È»ºó¸ßÎ»ÇåÁã ÔÙ½«²ÎÊıĞ´ÈëDRÒ»Î»Ò»Î»ËÍµ½TXÒı½Å
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);   //µÈ´ı±êÖ¾Î»£¨·¢ËÍ¼Ä´æÆ÷¿Õ±êÖ¾Î»£© Êı¾İÒÑ¾­±»×ªÈëµ½ÒÆÎ»¼Ä´æÆ÷    TXEIE±»ÖÃ1Ê±²úÉúÖĞ¶ÏÖ´ĞĞĞ´²Ù×÷£¬¶Ô¸ÃÎ»ÖÃÁã ·ÀÖ¹Êı¾İ¸²¸Ç   ²»ĞèÊÖ¶¯ÇåÁã
}



uint8_t Serial_GetRxFlag1(void)  //×Ô¶¯ÇåÁã×Ô¼ºµÄ±êÖ¾Î»
{
	if (Serial_RxFlag1 == 1)
	{
		Serial_RxFlag1 = 0;
		return 1;
	}
	return 0;
}

uint8_t Serial_GetRxData1(void) //·µ»ØUSARTxÍâÉè×î½üÊÕµ½µÄÊı¾İ
{
	return Serial_RxData1;       //·µ»ØUSARTxÍâÉè×î½üÊÕµ½µÄÊı¾İ
}

void USART1_IRQHandler(void)    //USART1_IRQHandlerÖĞ¶ÏÃû
{
	if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)    //ÅĞ¶Ï±êÖ¾Î»
	{
		Serial_RxData1 = USART_ReceiveData(USART1);    //·µ»ØUSARTxÍâÉè×î½üÊÕµ½µÄÊı¾İ
		Serial_RxFlag1 = 1;                  //ÖÃÒ»¸ö×Ô¼ºµÄ±êÖ¾Î»
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);    //¶ÁDR¼Ä´æÆ÷  Çå³ı±êÖ¾Î»
	}
}
/**************************************************************************************************/

/***************************************************/
void ActionGroup0(void)//»úĞµ±ÛÉÏµç³õÊ¼»¯
{	
	runActionGroup(118,1);//»úĞµ±ÛÉÏµç³õÊ¼»¯		
	Delay_ms(1100);	
	GG = 61;
	stopActionGroup();//Í£Ö¹¶¯×÷×éÔËĞĞ
}

void ActionGroup1(void)//»úĞµ±ÛÉ¨Íê¶şÎ¬Âë×¼±¸×¥×ªÅÌ
{	
	runActionGroup(118,1);//»úĞµ±ÛÉ¨Íê¶şÎ¬Âë×¼±¸×¥×ªÅÌ
	Delay_ms(1100);	
	stopActionGroup();//Í£Ö¹¶¯×÷×éÔËĞĞ		
}

void ActionGroup5(void)//×¥×ªÅÌÇ°
{	
	//·ÅµÚÒ»¸ö
	runActionGroup(119,1);//»úĞµ±ÛÉÏµç³õÊ¼»¯		
	Delay_ms(2100);		
	stopActionGroup();//Í£Ö¹¶¯×÷×éÔËĞĞ
}
void ActionGroup7(void)//×¥×ªÅÌÀ¶
{	
	//·ÅµÚ¶ş¸ö
	runActionGroup(122,1);//»úĞµ±ÛÉÏµç³õÊ¼»¯		
	Delay_ms(2100);	
	stopActionGroup();//Í£Ö¹¶¯×÷×éÔËĞĞ
}
void ActionGroup6(void)//×¥×ªÅÌÂÌ 
{	
	//·ÅµÚÈı¸ö
	runActionGroup(121,1);//»úĞµ±ÛÉÏµç³õÊ¼»¯		
	Delay_ms(2100);	
	stopActionGroup();//Í£Ö¹¶¯×÷×éÔËĞĞ
}
void ActionGroup91(void)//×¥µØºì
{	
	//·ÅµÚÒ»¸ö
	runActionGroup(131,1);//»úĞµ±ÛÉÏµç³õÊ¼»¯		
	Delay_ms(3600);
	stopActionGroup();//Í£Ö¹¶¯×÷×éÔËĞĞ	
	next++;
}
void ActionGroup92(void)//×¥µØÂÌ
{	
	//·ÅµÚ¶ş¸ö
	runActionGroup(130,1);//»úĞµ±ÛÉÏµç³õÊ¼»¯		
	Delay_ms(2700);
	stopActionGroup();//Í£Ö¹¶¯×÷×éÔËĞ	
	next++;
}
void ActionGroup93(void)//×¥µØÀ¶
{	
	//·ÅµÚÈı¸ö
	runActionGroup(129,1);//»úĞµ±ÛÉÏµç³õÊ¼»¯		
	Delay_ms(3200);
	stopActionGroup();//Í£Ö¹¶¯×÷×éÔËĞĞ
  next++;
}

void ActionGroup11(void)//·Åºì	
{	
	//·ÅµÚÒ»¸ö
	runActionGroup(126,1);//»úĞµ±ÛÉÏµç³õÊ¼»¯		
	Delay_ms(4200);		
	stopActionGroup();//Í£Ö¹¶¯×÷×éÔËĞĞ	
	next++;
}
void ActionGroup12(void)//·ÅÂÌ
{	
	//·ÅµÚ¶ş¸ö
	runActionGroup(127,1);//»úĞµ±ÛÉÏµç³õÊ¼»¯		
	Delay_ms(3500);
	stopActionGroup();//Í£Ö¹¶¯×÷×éÔËĞĞ
	next++;
}
void ActionGroup13(void)//·ÅÀ¶
{	
	//·ÅµÚÈı¸ö
	runActionGroup(128,1);//»úĞµ±ÛÉÏµç³õÊ¼»¯		
	Delay_ms(4800);
	stopActionGroup();//Í£Ö¹¶¯×÷×éÔËĞĞ
	next++;	
}
void ActionGroup219(void)//Âë¶â½ÃÕı
{	
	//·ÅµÚÒ»¸ö
	runActionGroup(136,1);//»úĞµ±ÛÉÏµç³õÊ¼»¯		
	Delay_ms(1100);
	stopActionGroup();//Í£Ö¹¶¯×÷×éÔËĞĞ
}

void ActionGroup21(void)//Âë¶âºì
{	
	//·ÅµÚÒ»¸ö
	runActionGroup(132,1);//»úĞµ±ÛÉÏµç³õÊ¼»¯		
	Delay_ms(3500);
	stopActionGroup();//Í£Ö¹¶¯×÷×éÔËĞĞ
	next++;
}
void ActionGroup22(void)//Âë¶âÂÌ
{	
	//·ÅµÚ¶ş¸ö
	runActionGroup(133,1);//»úĞµ±ÛÉÏµç³õÊ¼»¯		
	Delay_ms(3900);	
	stopActionGroup();//Í£Ö¹¶¯×÷×éÔËĞĞ
	next++;
}
void ActionGroup23(void)//Âë¶âÀ¶
{	
	//·ÅµÚÈı¸ö
	runActionGroup(134,1);//»úĞµ±ÛÉÏµç³õÊ¼»¯		
	Delay_ms(4000);
	stopActionGroup();//Í£Ö¹¶¯×÷×éÔËĞĞ
	next++;
}
void ActionGroup31(void)//·Å³µ ºì
{	
	//·ÅµÚÒ»¸ö
	runActionGroup(153,1);//»úĞµ±ÛÉÏµç³õÊ¼»¯		
	Delay_ms(1100);		
	stopActionGroup();//Í£Ö¹¶¯×÷×éÔËĞĞ
	next ++;
}
void ActionGroup732(void)//·Å³µ ÂÌ
{	
	//·ÅµÚ¶ş¸ö
	runActionGroup(150,1);//»úĞµ±ÛÉÏµç³õÊ¼»¯		
	Delay_ms(1100);
	stopActionGroup();//Í£Ö¹¶¯×÷×éÔËĞĞ
	next ++;
}
void ActionGroup33(void)//·Å³µ À¶
{	
	//·ÅµÚÈı¸ö
	runActionGroup(154,1);//»úĞµ±ÛÉÏµç³õÊ¼»¯		
	Delay_ms(1100);
	stopActionGroup();//Í£Ö¹¶¯×÷×éÔËĞĞ
	next ++;
}
void ActionGroup393(void)//½ĞÕĞÉú
{	
	//·ÅµÚÈı¸ö
	runActionGroup(125,1);//»úĞµ±ÛÉÏµç³õÊ¼»¯		
	Delay_ms(1100);
	stopActionGroup();//Í£Ö¹¶¯×÷×éÔËĞĞ
}
void ActionGroup199(void)//»ØÊÕ
{	
	//·ÅµÚÈı¸ö
	runActionGroup(135,1);//»úĞµ±ÛÉÏµç³õÊ¼»¯		
	Delay_ms(1100);	
	stopActionGroup();//Í£Ö¹¶¯×÷×éÔËĞĞ	
}
