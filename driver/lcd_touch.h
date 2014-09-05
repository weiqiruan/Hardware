#ifndef __LCD_TOUCH_H
#define __LCD_TOUCH_H

#include <msp430.h>
#include "system.h"
#include "type.h"

#define CS_CLR	        P1OUT &= ~BIT4
#define CS_SET	        P1OUT |=  BIT4

#define SCLK_CLR		P1OUT &= ~BIT5
#define SCLK_SET		P1OUT |=  BIT5

#define DIN_CLR	        P1OUT &= ~BIT6
#define DIN_SET	        P1OUT |=  BIT6

#define DOUT_RDY        (P1IN & 0x01)

void SPI_Port_Init(void);
uchar ReadFromSPI(void);
void WriteToSPI(uchar data);
void SpiDelay(unsigned int DelayCnt);
u16 TPReadX(void);
u16 TPReadY(void);

#endif
