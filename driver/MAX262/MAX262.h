#ifndef _MAX262_H
#define _MAX262_H

#include <msp430.h>
#include "system.h"
#include "type.h"
#include <math.h>

#define PI 3.141592653
//PDL口(从高到低):Wr,空,D1,D0,A3,A2,A1,A0
#define setWr_1		 P1OUT |= BIT6
#define resetWr_1  	 P1OUT &= ~BIT6
#define setWr_2		 P1OUT |= BIT7
#define resetWr_2  	 P1OUT &= ~BIT7

extern enum {modeOne,modeTwo,modeThree,modeFour} workMode;
extern enum {lowPass,highPass,bandPass,allPass,bandStop} passMode;
extern enum {channelA,channelB} channel;

void max262_Init(void);//端口初始化

//底层函数
void write_1(u8 add,u8 dat2bit);  //单片机向MAX262写两位数据
void write_2(u8 add,u8 dat2bit);  //单片机向MAX262写两位数据

//中层函数：写控制字
void setAmode_1(u8 mode);
void setAf_1(u8 datF);
void setAQ_1(u8 datQ);      

void setAmode_2(u8 mode);
void setAf_2(u8 datF);
void setAQ_2(u8 datQ);      

//高层函数：计算FN、QN、f0
uchar ComputeFN(u8 mode,long int fclk,long int f0);
uchar ComputeQN(u8 mode,float Q);
float lh_Getf0(float fc,float Q,u8 passMode);

//GUI函数：固定FN=63，通过给定的fc、q、passMode、workMode、channel等参数，自动写入控制字mode、FN、QN并返回当前所需时钟频率
float lhp_WorkFclk_1(float fc,float Q,u8 passMode,u8 workMode,u8 channel);//返回时钟频率fclk  写入控制字Q、F=
float bs_WorkFclk_1(float f0,float Q,uchar workMode,uchar channel);

float lhp_WorkFclk_2(float fc,float Q,u8 passMode,u8 workMode,u8 channel);//返回时钟频率fclk  写入控制字Q、F=
float bs_WorkFclk_2(float f0,float Q,uchar workMode,uchar channel);//陷波

float ap_WorkFclk_1(float f0,float Q,uchar channel);
float ap_WorkFclk_2(float f0,float Q,uchar channel);

#endif

