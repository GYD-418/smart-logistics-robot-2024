#include "key.h"
#include "Motor_Init.h"  
/************************************************
*@note 按键函数本意是在发车和更新一下初始的状态；
			 重置状态；
			 停止车。
			 
			 PB13!
*************************************************/

void My_delay(unsigned int  m)		//延时
{
	unsigned int x,y;
	for(x=m;x>0;x--)
		for(y=110;y>0;y--);
}


void KEY_Init(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//使能PORTD时钟

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_13;//KEY1-KEY66     
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入（浮空输入）
 	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOD  9 10 11 12 13 14
	GPIO_SetBits(GPIOB,GPIO_Pin_13);      
	
}
void Key_Scan()
{
	if (KEY1 == 0)
	{
		My_delay(100);
		if (KEY1 == 0)
		{
			My_delay(100);
			test = 1;      
    	Speed_Test_State.T_state = Acting;
			
			while (KEY1 == 0);
		}
	}
	

}
