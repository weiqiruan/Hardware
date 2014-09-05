#ifndef __I2C_H
#define __I2C_H

#include <msp430.h>
#include "system.h"
#include "type.h"

#define SCL_Out P2DIR |=BIT0  //SCL设置为输出模式
#define SCL_H  P2OUT |= BIT0   //P2^0高电平
#define SCL_L  P2OUT &= ~BIT0  //P2^0低电平

#define SDA_H  P2OUT |= BIT1   //P2^1高电平
#define SDA_L  P2OUT &= ~BIT1  //P2^1低电平

#define SDA_In  P2DIR &=~BIT1 //SDA改成输入模式
#define SDA_Out P2DIR |=BIT1  //SDA变回输出模式

void start();
void stop();
void Send1byte(unsigned int  byte);
unsigned int Read1byte();

#endif
