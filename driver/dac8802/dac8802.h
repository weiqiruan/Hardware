#ifndef __DAC8802_H
#define __DAC8802_H

#include <msp430.h>
#include "system.h"
#include "type.h"

#define SDI_0  P2OUT &=~ BIT0
#define SDI_1  P2OUT |=  BIT0
#define CLK_0  P2OUT &=~ BIT1
#define CLK_1  P2OUT |=  BIT1
#define CS_0   P2OUT &=~ BIT2
#define CS_1   P2OUT |=  BIT2
#define LDAC_0 P2OUT &=~ BIT3
#define LDAC_1 P2OUT |=  BIT3
#define RST_0  P2OUT &=~ BIT4
#define RST_1  P2OUT |=  BIT4


void dac8802_init(void);
void write_16bits(uint data);
void DAC8802_A(uint data);
void DAC8802_B(uint data);
void DAC8802_AB(uint data);

#endif
