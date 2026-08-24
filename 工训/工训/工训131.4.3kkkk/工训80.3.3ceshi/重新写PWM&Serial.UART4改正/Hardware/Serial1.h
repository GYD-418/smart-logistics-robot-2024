#ifndef __SERIAL1_H
#define __SERIAL1_H

#include "stdio.h"
void Serial_SendByte1(uint8_t Byte)  ;    //从TX引脚发送一个字节数据

void Serial_SendArray1(uint8_t *Array, uint16_t Length) ; //数组发送需要指针

uint8_t Serial_GetRxFlag1(void);  //自动清零自己的标志位

uint8_t Serial_GetRxData1(void); //返回USARTx外设最近收到的数据

void USART1_IRQHandler(void) ;   //USART1_IRQHandler中断名

#define FRAME_HEADER 0x55             //帧头
#define CMD_SERVO_MOVE 0x03           //舵机移动指令
#define CMD_ACTION_GROUP_RUN 0x06     //运行动作组指令
#define CMD_ACTION_GROUP_STOP 0x07    //停止动作组指令
#define CMD_ACTION_GROUP_SPEED 0x0B   //设置动作组运行速度
#define CMD_GET_BATTERY_VOLTAGE 0x0F  //获取电池电压指令

extern u8 USART_RX_BUF[64];     //接收缓冲,最大63个字节.末字节为换行符 
extern u8 USART_RX_STA;         //接收状态标记	
extern uint8_t LobotRxBuf[16];
extern uint16_t batteryVolt;
extern void receiveHandle(void);
typedef struct _lobot_servo_ {  //舵机ID,舵机目标位置
	uint8_t ID;
	uint16_t Position;
} LobotServo;
typedef enum {
	false = 0, true = !false
}bool;
extern bool isUartRxCompleted;
void moveServo(uint8_t servoID, uint16_t Position, uint16_t Time);
void moveServosByArray(LobotServo servos[], uint8_t Num, uint16_t Time);
void moveServos(uint8_t Num, uint16_t Time, ...);
void runActionGroup(uint8_t numOfAction, uint16_t Times);
void stopActionGroup(void);
void setActionGroupSpeed(uint8_t numOfAction, uint16_t Speed);
void setAllActionGroupSpeed(uint16_t Speed);
void getBatteryVoltage(void);
void uartInit(u32 bound);
void uartWriteBuf(uint8_t *buf, uint8_t len);

/************物料区抓取放车上*******************/
void ActionGroup1(void);//机械臂位置初始化
void ActionGroup2(void);//架起摄像头识别
void ActionGroup0(void);//放置
void ActionGroup3(void);//机械臂扫完二维码准备抓转盘
void ActionGroup5(void);//抓完第一次准备放置第一次
void ActionGroup6(void);//抓完第一次准备放置第一次
void ActionGroup7(void);//抓完第一次准备放置第一次
void ActionGroup4(void);//抓完第一次准备放置第一次
void ActionGroup11(void);//抓地
void ActionGroup12(void);//抓地
void ActionGroup13(void);//抓地
void ActionGroup21(void);//抓地
void ActionGroup22(void);//抓地
void ActionGroup23(void);//抓地
void ActionGroup31(void);//放车 红
void ActionGroup32(void);//放车 绿
void ActionGroup33(void);//放车 蓝
void ActionGroup91(void);//抓地红
void ActionGroup92(void);//抓地绿
void ActionGroup93(void);//抓地蓝
void ActionGroup393(void);//抓完准备矫正
void ActionGroup199(void);
void ActionGroup219(void);//码垛矫正

#endif

