#include "delay.h"




//初始化延迟函数
//当使用OS的时候,此函数会初始化OS的时钟节拍
//配置等待时间单位为1ms
void delay_init(void)
{
	if(SysTick_Config(SystemCoreClock / 1000))//systick中断配置，1ms进入中断一次，stm32f10x_it.c 文件中的SysTick_Handler函数中调用计数
  { 	
		
  }
}								    


static __IO uint32_t TimingDelay;//时间计数值
//1ms延时
void delay_ms(__IO uint32_t  nTime)
{	 		  	  
  TimingDelay = nTime;

  while(TimingDelay != 0);
 	    
} 

//1ms被调用一次
void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  { 
    TimingDelay--;
  }
}

void delay(u16 num)
{
  u16 i,j;
  for(i=0;i<num;i++)
    for(j=0;j<0x800;j++);
}











