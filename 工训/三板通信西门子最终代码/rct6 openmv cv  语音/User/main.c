#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Serial.h"
#include "Key.h"
#include "Serial2.h"
#include "Serial3.h"
#include "Serial4.h"

/* 32向电脑发送数据包    电脑向32发送数据包 */
uint16_t KeyNum;
uint16_t hexToDec_mv(uint16_t hexValue) {
  uint16_t decValue = 0;
  uint16_t base = 1;
  while (hexValue != 0) {
    uint16_t digit = hexValue % 10;
    decValue += digit * base;
    base *= 16;
    hexValue /= 10;
  }
  return decValue;
}
uint8_t hexToDec(uint8_t hexValue) {
  uint8_t decValue = 0;
  uint8_t base = 1;
  while (hexValue != 0) {
    uint8_t digit = hexValue % 10;
    decValue += digit * base;
    base *= 16;
    hexValue /= 10;
  }
  return decValue;
}
	int Music_RxData;

int main(void)
{
	OLED_Init();
	Key_Init();
	Serial_Init();	
	Serial_Init22();
	Serial_Init3();
	Serial_Init4();

//	OLED_ShowString(1, 1, "TxPacket");
//	OLED_ShowString(3, 1, "RxPacket");

		Serial_SendPacket4();

	

	while (1)
	{
		
//				Cx = (Serial_RxPacket[2]<<8) + (Serial_RxPacket[3]);
//				Cy = (Serial_RxPacket[4]<<8) + (Serial_RxPacket[5]);

//			OLED_ShowNum(2, 8,Cy, 3);
//			OLED_ShowNum(2, 1,Cx, 3);
			OLED_ShowNum(1, 8,Music_RxData, 4);
			OLED_ShowNum(2, 8,(hexToDec(Serial_RxPacket_2[3])*100)+(hexToDec(Serial_RxPacket_2[4])*10)+hexToDec(Serial_RxPacket_2[5]), 4);
			OLED_ShowNum(3, 8,(hexToDec(Serial_RxPacket_2[0])*100)+(hexToDec(Serial_RxPacket_2[1])*10)+hexToDec(Serial_RxPacket_2[2]), 4);

//			OLED_ShowNum(4, 8,(hexToDec(Serial_RxPacket[5])), 4);
/*********************串口1 openmv********************************************************************************/
     	if (Serial_GetRxFlag() == 1)
			{
					Serial_TxPacket_4[0] = Serial_RxPacket[2];
					Serial_SendPacket();
			}
/*****************************************************************************************************************/
/*********************串口2 opencv********************************************************************************/
			if (Serial_GetRxFlag_2() == 1)
			{
				  Serial_TxPacket_4[1] = Serial_RxPacket_2[0];    //x
				  Serial_TxPacket_4[2] = Serial_RxPacket_2[1];
				  Serial_TxPacket_4[3] = Serial_RxPacket_2[2];
				  Serial_TxPacket_4[5] = Serial_RxPacket_2[6];     //color
					Serial_TxPacket_4[6] = Serial_RxPacket_2[3];     //w
				  Serial_TxPacket_4[7] = Serial_RxPacket_2[4];
				  Serial_TxPacket_4[8] = Serial_RxPacket_2[5];

			}
			
/*****************************************************************************************************************/	
/*********************串口3 接收数据********************************************************************************/
			if (Serial_GetRxFlag3() == 1)    //不断判断RXNE的标志位 如果置1了 说明收到数据  TXE:发送数据寄存器空
			{
				  Music_RxData = Serial_GetRxData3();     //返回USART1最新接收到的数据 赋值给RxData
					Serial_TxPacket_4[4] = Music_RxData;   //串口四 发送第2位数据 0x13

//				  Serial_SendByte(0x13);
			}
		Serial_SendPacket4();

/******************************************************************************************************************/
	}
}
void TIM2_IRQHandler(void)
{
	
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)  //获取中断标志位
	{
//    if(Music_RxData == 0x01)   //开始启动 串口三收到0x13发送串口
//		{
//			Serial_TxPacket_4[4] = 0x01;   //串口四 发送第2位数据 0x13
//		}
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);  //清除中断标志位
	}
}
