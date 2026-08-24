#ifndef __T_BUJIN_SP_H
#define __T_BUJIN_SP_H
#include "math.h"
 
 
typedef struct
{
	 uint8_t run_state;    //电机旋转状态
	 uint8_t dir ;         //电机旋转方向
	 int step_delay;   //下个脉冲周期（时间间隔）启动时为加速度
	 int decel_start;  //启动减速位置
	 int decel_val;    //减速阶段步数
	 int min_delay;    //最小脉冲周期（最大速度，即匀速段的速度）
	 int accel_count;  //加速阶段计数值
}speedRampData;
 
 
 
#define TRUE     1
#define FALSE    0
 
 
/*电机速度决策中的四个状态*/
#define STOP              0 // 停止状态
#define ACCEL             1 // 加速状态
#define DECEL             2 // 减速状态
#define RUN               3 // 匀速状态
#define TIM_PRESCALER      32
#define T1_FREQ            (SystemCoreClock/(TIM_PRESCALER+1))     //定时器频率
 
/*电机单圈参数*/
#define STEP_ANGLE				1.8									//步进电机的步距角 单位：度
#define FSPR              200        //步进电机单圈步数
 
#define MICRO_STEP        8         				//细分器细分数 
#define SPR               (FSPR*MICRO_STEP)  //16细分的步数
 
//数学常数，用于MSD_MOVE函数的简化计算
#define ALPHA             ((float)(2*3.14159/SPR))       // α= 2*pi/spr    
#define A_T_x10           ((float)(10*ALPHA*T1_FREQ))
#define T1_FREQ_148       ((float)((T1_FREQ*0.676)/10)) // 0.69为误差修正值(计算过程，文档中有写)
#define A_SQ              ((float)(2*100000*ALPHA)) 
#define A_x200            ((float)(200*ALPHA))
extern int SPEET_BUSHU_STOP;

void MOROR_MOVE_MAX(int xd,int spee,int acc);

void MOTOR_Move(int32_t step, uint32_t accel, uint32_t decel, uint32_t speed);				
extern void speed_decision(void);	

					
#endif
                 


