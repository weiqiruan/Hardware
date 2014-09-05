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

void DAC_A(ulong data);//����Aͨ��
void DAC_B(ulong data);//����Bͨ��
void DAC_AB(ulong data_A,ulong data_B);//����A��Bͨ������ͬʱ����

#endif
