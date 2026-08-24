#include "main.h"

//uint8_t i=0;									//用在for循环
//uint8_t RX_Flag = 0;							//接收完一整个数据
//uint8_t Receive_USART1[20];						//存放接收的数据

/**
  * 函    数：控制灯带的引脚PA5
  * 参    数：Byte 要发送的一个字节
  * 返 回 值：无
  */

void Deng_Dai_Init(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
 	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);    

	GPIO_ResetBits(GPIOA,GPIO_Pin_5);
}




/**
  * 函    数：串口发送一个字节
  * 参    数：Byte 要发送的一个字节
  * 返 回 值：无
  */
void Serial_SendByte_USART1(uint8_t Byte)
{
	USART_SendData(USART1, Byte);		//将字节数据写入数据寄存器，写入后USART自动生成时序波形
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);	//等待发送完成
	/*下次写入数据寄存器会自动清除发送完成标志位，故此循环后，无需清除标志位*/
}


/**				
  * 函    数：启动函数,如前进、横移，需调用此函数
  * 参    数：无
  * 返 回 值：无
  * 串口返回：01,FF,02,6B
  */
void Mortor_Start(void)
{
	Serial_SendByte_USART1(0X00);
	Serial_SendByte_USART1(0XFF);
	Serial_SendByte_USART1(0X66);
	Serial_SendByte_USART1(0X6B);
	delay(1000);
}

void Mortor_Stop(void)
{
	
	Emm_V5_Stop_Now(0,0);
	delay(1000);
	
}
	
/**				
  * 函    数：横移
  * 参    数：
  * 返 回 值：无
  */

void Mortor_Heng_Yi(int16_t Mai_Chong)
{
	uint8_t a,b;
	if(Mai_Chong>0){a=0;b=1;}
	else{Mai_Chong = -Mai_Chong;a=1;b=0;}	
	
	Emm_V5_Pos_Control(1, b, 200, 120, Mai_Chong, 0, 0);delay(10);
	Emm_V5_Pos_Control(2, b, 200, 120, Mai_Chong, 0, 0);delay(10);
	Emm_V5_Pos_Control(3, a, 200, 120, Mai_Chong, 0, 0);delay(10);
	Emm_V5_Pos_Control(4, a, 200, 120, Mai_Chong, 0, 0);delay(10);
	while(rxFrameFlag == false); 
	rxFrameFlag = false;

}

/**				
  * 函    数：横移速度模式
  * 参    数：
  * 返 回 值：无
  */

void Mortor_Heng_Yi_Speed(int8_t speed)
{
	uint8_t a,b;
	if (speed > 0)
	{
		a = 0; b = 1;
	}
	else 
	{
		a = 1; b = 0;
		speed = -speed;
	}
	Serial_SendByte_USART1(0X01);
	Serial_SendByte_USART1(0XF6);
	Serial_SendByte_USART1(a);
	Serial_SendByte_USART1(0X00);
	Serial_SendByte_USART1(speed);		//速度
	Serial_SendByte_USART1(0X00);		//加速度
	Serial_SendByte_USART1(0X00);		
	Serial_SendByte_USART1(0X6B);

	delay(20);
	Serial_SendByte_USART1(0X02);
	Serial_SendByte_USART1(0XF6);
	Serial_SendByte_USART1(a);
	Serial_SendByte_USART1(0X00);
	Serial_SendByte_USART1(speed);		//速度
	Serial_SendByte_USART1(0X00);		//加速度
	Serial_SendByte_USART1(0X00);		
	Serial_SendByte_USART1(0X6B);

	delay(20);
	Serial_SendByte_USART1(0X03);
	Serial_SendByte_USART1(0XF6);
	Serial_SendByte_USART1(b);
	Serial_SendByte_USART1(0X00);
	Serial_SendByte_USART1(speed);		//速度
	Serial_SendByte_USART1(0X00);		//加速度
	Serial_SendByte_USART1(0X00);		
	Serial_SendByte_USART1(0X6B);

	delay(20);
	Serial_SendByte_USART1(0X04);
	Serial_SendByte_USART1(0XF6);
	Serial_SendByte_USART1(b);
	Serial_SendByte_USART1(0X00);
	Serial_SendByte_USART1(speed);		//速度
	Serial_SendByte_USART1(0X00);		//加速度
	Serial_SendByte_USART1(0X00);		
	Serial_SendByte_USART1(0X6B);
	delay(20);
	Mortor_Start();
	
}



/**				
  * 函    数：直走速度式
  * 参    数：
  * 返 回 值：无
  */

void Mortor_Zhi_Xing_Speed_1(int8_t speed)
{
	
	uint8_t a,b;
	if (speed > 0)
	{
		a = 0; b = 1;
	}
	else 
	{
		a = 1; b = 0;
		speed = -speed;
	}
	Serial_SendByte_USART1(0X01);
	Serial_SendByte_USART1(0XF6);
	Serial_SendByte_USART1(a);
	Serial_SendByte_USART1(0X00);
	Serial_SendByte_USART1(speed);		//速度
	Serial_SendByte_USART1(0X00);		//加速度
	Serial_SendByte_USART1(0X00);		
	Serial_SendByte_USART1(0X6B);

	delay(20);
	Serial_SendByte_USART1(0X02);
	Serial_SendByte_USART1(0XF6);
	Serial_SendByte_USART1(b);
	Serial_SendByte_USART1(0X00);
	Serial_SendByte_USART1(speed);		//速度
	Serial_SendByte_USART1(0X00);		//加速度
	Serial_SendByte_USART1(0X00);		
	Serial_SendByte_USART1(0X6B);

	delay(20);
	Serial_SendByte_USART1(0X03);
	Serial_SendByte_USART1(0XF6);
	Serial_SendByte_USART1(a);
	Serial_SendByte_USART1(0X00);
	Serial_SendByte_USART1(speed);		//速度
	Serial_SendByte_USART1(0X00);		//加速度
	Serial_SendByte_USART1(0X00);		
	Serial_SendByte_USART1(0X6B);

	delay(20);
	Serial_SendByte_USART1(0X04);
	Serial_SendByte_USART1(0XF6);
	Serial_SendByte_USART1(b);
	Serial_SendByte_USART1(0X00);
	Serial_SendByte_USART1(speed);		//速度
	Serial_SendByte_USART1(0X00);		//加速度
	Serial_SendByte_USART1(0X00);		
	Serial_SendByte_USART1(0X6B);
	delay(20);
	Mortor_Start();
	
}

uint8_t a,b;
/**				
  * 函    数：直走速度式
  * 参    数：
  * 返 回 值：无
  */

void Mortor_Zhi_Xing_Speed(int8_t speed_x,int8_t speed_y)
{
	
	
	if (speed_x > 0 && speed_y > 0)					//方向
	{
		a = 0; b = 1;
	}
	else if(speed_x < 0 && speed_y < 0)
	{
		a = 1; b = 0;
	}
	else if (speed_x < 0 && speed_y > 0)					//方向
	{
		a = 1; b = 0;
	}
	else if(speed_x > 0 && speed_y < 0)					//方向
	{
		a = 0; b = 1;
	}

	
	if((speed_x+speed_y)<0)							//取绝对值
	{
		speed_x = -speed_x;speed_y=-speed_y;
	}
	Serial_SendByte_USART1(0X01);
	Serial_SendByte_USART1(0XF6);
	Serial_SendByte_USART1(a);
	Serial_SendByte_USART1(0X00);
	Serial_SendByte_USART1(speed_x+speed_y);		//速度
	Serial_SendByte_USART1(0X00);		//加速度
	Serial_SendByte_USART1(0X00);		
	Serial_SendByte_USART1(0X6B);

	delay(20);
	Serial_SendByte_USART1(0X04);
	Serial_SendByte_USART1(0XF6);
	Serial_SendByte_USART1(b);
	Serial_SendByte_USART1(0X00);
	Serial_SendByte_USART1(speed_x+speed_y);		//速度
	Serial_SendByte_USART1(0X00);		//加速度
	Serial_SendByte_USART1(0X00);		
	Serial_SendByte_USART1(0X6B);

	if((speed_x-speed_y)<0)
	{
		speed_x = -speed_x;speed_y=-speed_y;
	}
	delay(20);
	Serial_SendByte_USART1(0X02);
	Serial_SendByte_USART1(0XF6);
	Serial_SendByte_USART1(b);
	Serial_SendByte_USART1(0X00);
	Serial_SendByte_USART1(speed_x-speed_y);		//速度
	Serial_SendByte_USART1(0X00);		//加速度
	Serial_SendByte_USART1(0X00);		
	Serial_SendByte_USART1(0X6B);

	delay(20);
	Serial_SendByte_USART1(0X03);
	Serial_SendByte_USART1(0XF6);
	Serial_SendByte_USART1(a);
	Serial_SendByte_USART1(0X00);
	Serial_SendByte_USART1(speed_x-speed_y);		//速度
	Serial_SendByte_USART1(0X00);		//加速度
	Serial_SendByte_USART1(0X00);		
	Serial_SendByte_USART1(0X6B);

	delay(20);
	Mortor_Start();
	
}



/**					调用会立即执行
  * 函    数：原地旋转90度，当转过90度后，使能PID算法，进一步进行校正
  * 参    数：方向：0X01/0X00
  * 返 回 值：无
  */
void Mortor_Zi_Zhuan_dir(uint8_t dir)
{
	
	Serial_SendByte_USART1(0X00);
	Serial_SendByte_USART1(0XFD);
	Serial_SendByte_USART1(dir);
	Serial_SendByte_USART1(0X01);
	Serial_SendByte_USART1(0XF4);		//速度
	Serial_SendByte_USART1(0X78);		//加速度
	Serial_SendByte_USART1(0X00);
	Serial_SendByte_USART1(0X00);
	Serial_SendByte_USART1(0X11);
	Serial_SendByte_USART1(0X40);
	Serial_SendByte_USART1(0X00);
	Serial_SendByte_USART1(0X00);
	Serial_SendByte_USART1(0X6B);
	delay(200);
	while(rxFrameFlag == false); 
	rxFrameFlag = false;

}

/**					调用会立即执行
  * 函    数：原地旋转,用于PID算法
  * 参    数：方向：0X01/0X00
  * 返 回 值：无
  */
void Mortor_Zi_Zhuan_speed(int16_t speed)
{
	uint8_t dir;
	if(speed < 0)
	{dir = 0x00; speed = -speed;}
	else {dir = 0x01; }
	
	Serial_SendByte_USART1(0X00);
	Serial_SendByte_USART1(0XF6);
	Serial_SendByte_USART1(dir);
	Serial_SendByte_USART1(0X00);
	Serial_SendByte_USART1(speed);		//速度
	Serial_SendByte_USART1(0X04);		//加速度
	Serial_SendByte_USART1(0X00);
	Serial_SendByte_USART1(0X6B);
	delay(20);
}

///**			  
//  * 函    数：自转PID
//  * 参    数：
//  * 返 回 值：无
//  */
//void Mortor_Zhuan_PID(uint8_t dir)
//{
//	Mortor_Zi_Zhuan_dir(dir);
//	while(RX_Flag ==0);
//	RX_Flag = 0;
//	TIM_Cmd(TIM1,ENABLE);


//}

/**			  
  * 函    数：使能电机5
  * 参    数：
  * 返 回 值：无
  */
void Mortor_5_Start(void)
{
	
	Emm_V5_En_Control(5,1,0);
	delay(600);	
}

/**			  
  * 函    数：失能电机5
  * 参    数：
  * 返 回 值：无
  */
void Mortor_5_Stop(void)
{
	
	Emm_V5_En_Control(5,0,0);
	delay(600);	
}


/**			  
  * 函    数：控制上升的步进电机，1为上升，0为下降
  * 参    数：
  * 返 回 值：无
  */
void Mortor_5(uint8_t dir,uint16_t Mai_Chong)
{
	
	Emm_V5_Pos_Control(5, dir, 400, 0, Mai_Chong, 0, 0);
	while(rxFrameFlag == false); 
	rxFrameFlag = false;

}

/**			  
  * 函    数：直行
  * 参    数：
  * 返 回 值：无
  */
void Mortor_Straight(int16_t Mai_Chong)
{
	uint8_t a,b;
	if(Mai_Chong > 0){a = 0x00;b=0x01;}
	else {a = 0x01;b=0x00;Mai_Chong = -Mai_Chong;}
	
	
	Emm_V5_Pos_Control(1, a, 170, 180, Mai_Chong, 0, 0);delay(10);
	Emm_V5_Pos_Control(2, b, 170, 180, Mai_Chong, 0, 0);delay(10);
	Emm_V5_Pos_Control(3, a, 170, 180, Mai_Chong, 0, 0);delay(10);
	Emm_V5_Pos_Control(4, b, 170, 180, Mai_Chong, 0, 0);
	while(rxFrameFlag == false); 
	rxFrameFlag = false;

}

/**			  
  * 函    数：慢直行
  * 参    数：
  * 返 回 值：无
  */
void Mortor_Straight_slow(int16_t Mai_Chong)
{
	uint8_t a,b;
	if(Mai_Chong > 0){a = 0x00;b=0x01;}
	else {a = 0x01;b=0x00;Mai_Chong = -Mai_Chong;}
	
	
	Emm_V5_Pos_Control(1, a, 150, 10, Mai_Chong, 0, 0);delay(10);
	Emm_V5_Pos_Control(2, b, 150, 10, Mai_Chong, 0, 0);delay(10);
	Emm_V5_Pos_Control(3, a, 150, 10, Mai_Chong, 0, 0);delay(10);
	Emm_V5_Pos_Control(4, b, 150, 10, Mai_Chong, 0, 0);
	while(rxFrameFlag == false); 
	rxFrameFlag = false;

}

