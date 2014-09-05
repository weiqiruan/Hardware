#ifndef __AD9850_H
#define __AD9850_H

#include <msp430.h>
#include "system.h"
#include "type.h"

#define FQ_UP_CLR       P2OUT &= ~BIT0
#define FQ_UP_SET       P2OUT |=  BIT0

#define W_CLK_CLR       P2OUT &= ~BIT1
#define W_CLK_SET       P2OUT |=  BIT1

#define REST_CLR        P2OUT &= ~BIT2
#define REST_SET        P2OUT |=  BIT2

#define AD9850_DATAPORT	P6OUT
#define AD9850_DATASEL	P6SEL
#define AD9850_DATADIR	P6DIR

/****************************************************************************
* 名    称：AD9850_Port_Init
* 功    能：AD9850端口初始化
* 入口参数：无
* 出口参数：无
****************************************************************************/
void AD9850_Port_Init(void);

/****************************************************************************
* 名    称：AD9850_Init_Parallel
* 功    能：AD9850并口模式初始化
* 入口参数：无
* 出口参数：无
****************************************************************************/
void AD9850_Init_Parallel(void);

/****************************************************************************
* 名    称：AD9850_WR_Parallel
* 功    能：AD9850并口数据写入
* 入口参数：w0--w0值；frequence--频率（单位：HZ）
* 出口参数：无
****************************************************************************/
void AD9850_WR_Parallel(uchar w0,double frequence);

#endif
