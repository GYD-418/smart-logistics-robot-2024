#include "niming.h"
#include "usart.h"

int8_t DATA_buffer[100];
int8_t DATA_buffer_2[100];


void ANODT_SendF1_1(int a)
{
  u8 sc=0;
	u8 ac=0;
	u8 _cnt=0;
	u8 i;
	DATA_buffer[_cnt++]=0xAA;
	DATA_buffer[_cnt++]=0xFF;
	DATA_buffer[_cnt++]=0xF1;
	DATA_buffer[_cnt++]=4;
	
	DATA_buffer[_cnt++]=BYTE0(a);
	DATA_buffer[_cnt++]=BYTE1(a);
	DATA_buffer[_cnt++]=BYTE2(a);
	DATA_buffer[_cnt++]=BYTE3(a);
	
	for(i=0;i<DATA_buffer[3]+4;i++)
	{
	sc+=DATA_buffer[i];
	ac+=sc;
	}
	DATA_buffer[_cnt++]=sc;
	DATA_buffer[_cnt++]=ac;
  
	Serial_SendArray(USART2,DATA_buffer,_cnt);
	
}

void ANODT_SendF1_2(int a)
{
  u8 sc=0;
	u8 ac=0;
	u8 _cnt=0;
	u8 i;
	DATA_buffer_2[_cnt++]=0xAA;
	DATA_buffer_2[_cnt++]=0xFF;
	DATA_buffer_2[_cnt++]=0xF2;
	DATA_buffer_2[_cnt++]=4;
	
	DATA_buffer_2[_cnt++]=BYTE0(a);
	DATA_buffer_2[_cnt++]=BYTE1(a);
	DATA_buffer_2[_cnt++]=BYTE2(a);
	DATA_buffer_2[_cnt++]=BYTE3(a);
	
	for(i=0;i<DATA_buffer_2[3]+4;i++)
	{
	sc+=DATA_buffer_2[i];
	ac+=sc;
	}
	DATA_buffer_2[_cnt++]=sc;
	DATA_buffer_2[_cnt++]=ac;
  
	Serial_SendArray(USART2,DATA_buffer_2,_cnt);
	
}
 void ANODT_SendF1_3(int a)
{
  u8 sc=0;
	u8 ac=0;
	u8 _cnt=0;
	u8 i;
	DATA_buffer_2[_cnt++]=0xAA;
	DATA_buffer_2[_cnt++]=0xFF;
	DATA_buffer_2[_cnt++]=0xF3;
	DATA_buffer_2[_cnt++]=4;
	
	DATA_buffer_2[_cnt++]=BYTE0(a);
	DATA_buffer_2[_cnt++]=BYTE1(a);
	DATA_buffer_2[_cnt++]=BYTE2(a);
	DATA_buffer_2[_cnt++]=BYTE3(a);
	
	for(i=0;i<DATA_buffer_2[3]+4;i++)
	{
	sc+=DATA_buffer_2[i];
	ac+=sc;
	}
	DATA_buffer_2[_cnt++]=sc;
	DATA_buffer_2[_cnt++]=ac;
  
	Serial_SendArray(USART2,DATA_buffer_2,_cnt);
	
}
void ANODT_SendF1_4(int a)
{
  u8 sc=0;
	u8 ac=0;
	u8 _cnt=0;
	u8 i;
	DATA_buffer_2[_cnt++]=0xAA;
	DATA_buffer_2[_cnt++]=0xFF;
	DATA_buffer_2[_cnt++]=0xF4;
	DATA_buffer_2[_cnt++]=4;
	
	DATA_buffer_2[_cnt++]=BYTE0(a);
	DATA_buffer_2[_cnt++]=BYTE1(a);
	DATA_buffer_2[_cnt++]=BYTE2(a);
	DATA_buffer_2[_cnt++]=BYTE3(a);
	
	for(i=0;i<DATA_buffer_2[3]+4;i++)
	{
	sc+=DATA_buffer_2[i];
	ac+=sc;
	}
	DATA_buffer_2[_cnt++]=sc;
	DATA_buffer_2[_cnt++]=ac;
  
	Serial_SendArray(USART2,DATA_buffer_2,_cnt);
	
}
void ANODT_SendF1_5(int a)
{
  u8 sc=0;
	u8 ac=0;
	u8 _cnt=0;
	u8 i;
	DATA_buffer_2[_cnt++]=0xAA;
	DATA_buffer_2[_cnt++]=0xFF;
	DATA_buffer_2[_cnt++]=0xF5;
	DATA_buffer_2[_cnt++]=4;
	
	DATA_buffer_2[_cnt++]=BYTE0(a);
	DATA_buffer_2[_cnt++]=BYTE1(a);
	DATA_buffer_2[_cnt++]=BYTE2(a);
	DATA_buffer_2[_cnt++]=BYTE3(a);
	
	for(i=0;i<DATA_buffer_2[3]+4;i++)
	{
	sc+=DATA_buffer_2[i];
	ac+=sc;
	}
	DATA_buffer_2[_cnt++]=sc;
	DATA_buffer_2[_cnt++]=ac;
  
	Serial_SendArray(USART2,DATA_buffer_2,_cnt);
	
}
void ANODT_SendF1_6(int a)
{
  u8 sc=0;
	u8 ac=0;
	u8 _cnt=0;
	u8 i;
	DATA_buffer_2[_cnt++]=0xAA;
	DATA_buffer_2[_cnt++]=0xFF;
	DATA_buffer_2[_cnt++]=0xF6;
	DATA_buffer_2[_cnt++]=4;
	
	DATA_buffer_2[_cnt++]=BYTE0(a);
	DATA_buffer_2[_cnt++]=BYTE1(a);
	DATA_buffer_2[_cnt++]=BYTE2(a);
	DATA_buffer_2[_cnt++]=BYTE3(a);
	
	for(i=0;i<DATA_buffer_2[3]+4;i++)
	{
	sc+=DATA_buffer_2[i];
	ac+=sc;
	}
	DATA_buffer_2[_cnt++]=sc;
	DATA_buffer_2[_cnt++]=ac;
  
	Serial_SendArray(USART2,DATA_buffer_2,_cnt);
	
}
void ANODT_SendF1_7(int a)
{
  u8 sc=0;
	u8 ac=0;
	u8 _cnt=0;
	u8 i;
	DATA_buffer_2[_cnt++]=0xAA;
	DATA_buffer_2[_cnt++]=0xFF;
	DATA_buffer_2[_cnt++]=0xF7;
	DATA_buffer_2[_cnt++]=4;
	
	DATA_buffer_2[_cnt++]=BYTE0(a);
	DATA_buffer_2[_cnt++]=BYTE1(a);
	DATA_buffer_2[_cnt++]=BYTE2(a);
	DATA_buffer_2[_cnt++]=BYTE3(a);
	
	for(i=0;i<DATA_buffer_2[3]+4;i++)
	{
	sc+=DATA_buffer_2[i];
	ac+=sc;
	}
	DATA_buffer_2[_cnt++]=sc;
	DATA_buffer_2[_cnt++]=ac;
  
	Serial_SendArray(USART2,DATA_buffer_2,_cnt);
	
}
void ANODT_SendF1_8(int a)
{
  u8 sc=0;
	u8 ac=0;
	u8 _cnt=0;
	u8 i;
	DATA_buffer_2[_cnt++]=0xAA;
	DATA_buffer_2[_cnt++]=0xFF;
	DATA_buffer_2[_cnt++]=0xF8;
	DATA_buffer_2[_cnt++]=4;
	
	DATA_buffer_2[_cnt++]=BYTE0(a);
	DATA_buffer_2[_cnt++]=BYTE1(a);
	DATA_buffer_2[_cnt++]=BYTE2(a);
	DATA_buffer_2[_cnt++]=BYTE3(a);
	
	for(i=0;i<DATA_buffer_2[3]+4;i++)
	{
	sc+=DATA_buffer_2[i];
	ac+=sc;
	}
	DATA_buffer_2[_cnt++]=sc;
	DATA_buffer_2[_cnt++]=ac;
  
	Serial_SendArray(USART2,DATA_buffer_2,_cnt);	
}
void ANODT_SendF1_9(float a)
{
  u8 sc=0;
	u8 ac=0;
	u8 _cnt=0;
	u8 i;
	DATA_buffer_2[_cnt++]=0xAA;
	DATA_buffer_2[_cnt++]=0xFF;
	DATA_buffer_2[_cnt++]=0xF9;
	DATA_buffer_2[_cnt++]=4;
	
	DATA_buffer_2[_cnt++]=BYTE0(a);
	DATA_buffer_2[_cnt++]=BYTE1(a);
	DATA_buffer_2[_cnt++]=BYTE2(a);
	DATA_buffer_2[_cnt++]=BYTE3(a);
	
	for(i=0;i<DATA_buffer_2[3]+4;i++)
	{
	sc+=DATA_buffer_2[i];
	ac+=sc;
	}
	DATA_buffer_2[_cnt++]=sc;
	DATA_buffer_2[_cnt++]=ac;
  
	Serial_SendArray(USART2,DATA_buffer_2,_cnt);
		
}
void ANODT_SendF1_10(float a)
{
  u8 sc=0;
	u8 ac=0;
	u8 _cnt=0;
	u8 i;
	DATA_buffer_2[_cnt++]=0xAA;
	DATA_buffer_2[_cnt++]=0xFF;
	DATA_buffer_2[_cnt++]=0xFA;
	DATA_buffer_2[_cnt++]=4;
	
	DATA_buffer_2[_cnt++]=BYTE0(a);
	DATA_buffer_2[_cnt++]=BYTE1(a);
	DATA_buffer_2[_cnt++]=BYTE2(a);
	DATA_buffer_2[_cnt++]=BYTE3(a);
	
	for(i=0;i<DATA_buffer_2[3]+4;i++)
	{
	sc+=DATA_buffer_2[i];
	ac+=sc;
	}
	DATA_buffer_2[_cnt++]=sc;
	DATA_buffer_2[_cnt++]=ac;
  
	Serial_SendArray(USART2,DATA_buffer_2,_cnt);
		
}

