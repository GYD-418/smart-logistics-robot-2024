#ifndef __SHIQV_H
#define __SHIQV_H

void PWM_Initkk(int ARR , int PSC , int CCR);  //PWM初始化
void Servo1_Init(void);
void Servo2_Init(void);
void Servo3_Init(void);
void PWM_SetCompare2(uint16_t Compare);
void PWM_SetCompare3(uint16_t Compare);
void PWM_SetCompare4(uint16_t Compare);
void Servo1_SetAngle(float Angle1);
void Servo2_SetAngle(float Angle2);
void Servo3_SetAngle(float Angle3);
void PWMkk_ENABLE(void);
void PWMkk_DISABLE(void);
void Motor_InitC(void);
void Motor_SetSpeedkk(int8_t Speed);
void ShiQv(void);
void ShiQv2(void);
void ShiQv3(void);
void ShiQv4(void);  //抓地面物料的矫正位置 下降

extern int bb;
extern int aa;
void Servo4_Init(void);
void Servo4_SetAngle(float Angle44);
void FK4(uint16_t Compare);
void ShiQv5(void);   //抓地面物料的矫正位置 下降

#endif
