#ifndef _COMMUNICATION_H_
#define _COMMUNICATION_H_


#include <msp430x16x.h>
#include "type.h"
#include "system.h"

#define CS_CLR	        P1OUT &= ~BIT0
#define CS_SET	        P1OUT |=  BIT0 

#define SCLK_CLR		P1OUT &= ~BIT1
#define SCLK_SET		P1OUT |=  BIT1

#define DIN_CLR	        P1OUT &= ~BIT2
#define DIN_SET	        P1OUT |=  BIT2

#define DOUT_RDY        ((P1IN & 0x08)>>3)

void SPI_Port_Init(void);
void ReadFromSPI(uchar *buf, uchar count);
void WriteToSPI(uchar *buf, uchar count);

#endif	// _COMMUNICATION_H_
