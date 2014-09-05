#ifndef __I2C_H
#define __I2C_H

#include <msp430.h>
#include "system.h"
#include "type.h"

#define SCL_Out P2DIR |=BIT0  //SCL����Ϊ���ģʽ
#define SCL_H  P2OUT |= BIT0   //P2^0�ߵ�ƽ
#define SCL_L  P2OUT &= ~BIT0  //P2^0�͵�ƽ

#define SDA_H  P2OUT |= BIT1   //P2^1�ߵ�ƽ
#define SDA_L  P2OUT &= ~BIT1  //P2^1�͵�ƽ

#define SDA_In  P2DIR &=~BIT1 //SDA�ĳ�����ģʽ
#define SDA_Out P2DIR |=BIT1  //SDA������ģʽ

void start();
void stop();
void Send1byte(unsigned int  byte);
unsigned int Read1byte();

#endif
