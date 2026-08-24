#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "Serial.h"
#include "LED.h"
#include "OLED.h"
#include "time.h"
#include "Serial2.h"
#include "Serial3.h"
#include "Serial4.h"

uint8_t RxData1;

uint16_t i=0;

int main(void)
{
	  LED_Init();
    OLED_Init();
    Serial_InitR();
	  Serial_InitYY();
	  Serial_Init();
	  timer_Init();
	  Serial_Init3();
 while (1)
	{
		if((uart_rxpa[3]<<8)|(uart_rxpa[4])<70) Serial_SendByte(0x09);  
		else Serial_SendByte(0x07);
		OLED_ShowNum(3,1,(uart_rxpa[3]<<8)|(uart_rxpa[4]),5);
		
		if((uart_rxpb[3]<<8)|(uart_rxpb[4])<70) Serial_SendByte(0x09);  
		else Serial_SendByte(0x08);
		OLED_ShowNum(3,1,(uart_rxpa[3]<<8)|(uart_rxpa[4]),5);
	}
}

void TIM2_IRQHandler(void)    
	{ 
	   if(TIM_GetFlagStatus(TIM2,TIM_IT_Update)==SET)
		 {
         if(Serial_GetRxFlag() == 1)
		 {
		    RxData1 =Serial_GetRxData();
			  if(RxData1==0x01) Serial_SendByte(RxData1);
		  }
		 } 
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
	}
