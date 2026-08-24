#include "stm32f10x.h"                  // Device header
#include "LED.h"
#include "Delay.h"

void KEY_Init(void)

{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	  GPIO_InitTypeDef GPIO_InitStruct;
	  GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPU;
	  GPIO_InitStruct.GPIO_Pin=GPIO_Pin_1|GPIO_Pin_11;
	  GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOB ,&GPIO_InitStruct);
	

}
   uint8_t  KEY_Getnum(void)
	 {
	    uint8_t KEYnum=0;
		 if(GPIO_ReadInputDataBit(GPIOB ,GPIO_Pin_1 )== 0)     //代表按键按下
		 {
		   Delay_ms(20);
			 while( GPIO_ReadInputDataBit(GPIOB ,GPIO_Pin_1)== 0);
			 Delay_ms(20);
			 KEYnum =1;         //定义按键1
		 }
		 	 if(GPIO_ReadInputDataBit(GPIOB ,GPIO_Pin_11 )==0)
		 {
		   Delay_ms(20);
			 while( GPIO_ReadInputDataBit(GPIOB ,GPIO_Pin_11)==0);
			 Delay_ms(20);
			 KEYnum =2;         //定义按键2
		 }
		 return  KEYnum;	 
	 }
	 