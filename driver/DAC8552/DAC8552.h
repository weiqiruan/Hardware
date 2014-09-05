#ifndef __DAC8552_H
#define __DAC8552_H

#include <msp430.h>
#include "system.h"
#include "type.h"

#define SYNC_0   P2OUT&=~BIT3
#define SYNC_1   P2OUT|=BIT3
#define SCLK_0 P2OUT&=~BIT4
#define SCLK_1 P2OUT|=BIT4
#define DIN_0  P2OUT&=~BIT5
#define DIN_1  P2OUT|=BIT5

void DAC8852_port_init();
void write_8552(ulong data);

void DAC_A(ulong data);//启用A通道
void DAC_B(ulong data);//启用B通道
void DAC_AB(ulong data_A,ulong data_B);//启用A、B通道，且同时触发

#endif
