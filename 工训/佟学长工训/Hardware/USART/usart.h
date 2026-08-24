#ifndef _USART_H_
#define _USART_H_

#include "stm32f10x.h"


#define USART_REC_LEN  			200  	//定义最大接收字节数 200

extern float speedx;//k210传来的数据
extern float speedy;

extern u8 RX_Flag;
extern char USART1_RX_ZQ_BUF[13];     //接收缓冲,最大14个字节.
extern int8_t USART2_RX_BUF_2[8];   //接收缓冲,用于接收二维码的信息
extern uint8_t USART2_RX_Flag;      //用于判断接收是否成功
extern uint16_t USART_RX_BUF_3[20];  //接收缓冲,最大4个字节.     

extern u8 RX2_Flag;


void MYUSART1_Init(int bound);
void MYUSART2_Init(int bound);  
void MYUSART3_Init(int bound);
void MYUART4_Init(int bound);
void Serial_SendByte(USART_TypeDef* USARTx,int8_t Byte);
void Usart_SendStr(USART_TypeDef* USARTx,char *str);
void Serial_SendArray(USART_TypeDef* USARTx,uint8_t *Array, uint16_t Length);
void Deal_Date(uint8_t *Arr,int Length);

void HexToDecimal(USART_TypeDef* USARTx,uint16_t *Arr, char* decimalString, int Length) ;
int Deal_2(uint8_t *hex);

void Test_ShiBie_SeHuan(void);
void Test_ShiBie_WuLiao(void);

void ChuanKouPing(void);
void ChuLi_String(void);
void Send_To_K210(void);

#endif //_USART_H_

