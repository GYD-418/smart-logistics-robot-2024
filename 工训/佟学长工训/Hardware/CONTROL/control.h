#ifndef _CONTROL_H_
#define _CONTROL_H_

#include "stm32f10x.h"


void Main_Mission(void);

void test_1(void);
void test_2(void);
void test_3(void);
void test_4(void);
void test_5(void);
void test_6(void);

void test_7(void);
void ZhuanJiaoDu(void);   //转90度

void ZhuaQuZhuanPanWL(void);
void PanDuan(void);


void ZhuaWuKuaiEr(void);
void ZhiZou_2(void);  //从第一个转角前往粗加工区
void JiaoZheng(float x, float y);
void CJG_ZhuaWuKuaiEr_TongYong(void);  //可以用。
void PanDuan_2(int speedx, int speedy)  ;            //前往粗加工区并放物料的判断


void JJG_ZhiZou_1(void);
void JJG_JiaoZheng(int speedx,int speedy);
void JJG_FangWuKuaiEr_TongYong(void);
void PanDuan_3(int speedx, int speedy);              //前往粗加工区并放物料的判断
void CJG_FangWuKuaiEr_TongYong(void);
void test_8(void);   //
void test_9(void);   //向前走去粗加工区绿色环进行矫正。
void test_10(void);  //回转盘区时第一个转角
void test_11(void);  //后退到转盘
void test_12(void);  //横移
void test_13(void);   //转90度
void test_15(void);
void test_23(void);   //返回出发区

void PanDuan_4(int speedx, int speedy);              //前往转盘区并抓第二轮物料的判断

void Sec_PanDuan(void);   //第二轮物块儿判断
void Sec_ZhuaQuZhuanPanWL(void);   //抓取物块儿的任务

void New_PanDuan(void);

void TEST_JiaoZheng(float x, float y);



extern int CJG_state;
extern int Choose;
extern int JiaoDu;
extern int Mission_State;
extern int JZ_CLim;
#endif //_CONTROL_H_

