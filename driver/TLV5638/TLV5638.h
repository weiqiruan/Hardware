#ifndef __TLV5638_H
#define __TLV5638_H

#include <msp430.h>
#include "system.h"
#include "type.h"

//说明：
//单通道：端口初始化-->设置参考电压-->写通道数据数据
//双通道：端口初始化-->设置参考电压-->写BUFFER(为B通道准备数据）-->写A通道数据(自动将BUFFER内容更新到B通道)
#define CS_0   P1OUT&=~BIT0
#define CS_1   P1OUT|=BIT0
#define SCLK_0 P1OUT&=~BIT1
#define SCLK_1 P1OUT|=BIT1
#define DIN_0  P1OUT&=~BIT2
#define DIN_1  P1OUT|=BIT2

//参考电压 满量程输出电压为参考电压的2倍
#define FAST_REF1  0XD001   //内部1.024V
#define SLOW_REF1  0X9001   
#define FAST_REF2  0XD002   //内部2.048V
#define SLOW_REF2  0X9002
#define FAST_REF_E 0XD000   //外部
#define SLOW_REF_E 0X9000

void TLV5638_port_init();
void write_5638(uint data);

#endif
