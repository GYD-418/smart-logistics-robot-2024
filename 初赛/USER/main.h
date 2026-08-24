#include "stm32f4xx.h"                  // Device header
#include "Mortor.h"
#include "HWT101.h"
#include "delay.h"
#include "PID_Zi_Zhuan.h"
#include "TIM_PID_Xuan.h"
#include "USART2.h"
#include "Servo.h"
#include "Action.h"
#include "OpenMV.h"
#include "Emm_V5.h"
#include "fifo.h"
#include "usart.h"
#include "PWM.h"


extern float yaw;
extern uint8_t test;
extern float yaw_1;
extern uint16_t Temp[16];
extern struct SAngle 	stcAngle;

//#define X_Value 199			//车向左移动增加
//#define Y_Value 183			//车向下移动减小

extern uint16_t X_Value_goal;
extern uint16_t Y_Value_goal;


//向下移动减少

