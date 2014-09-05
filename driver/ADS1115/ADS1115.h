#ifndef __ADS1115_H
#define __ADS1115_H

#include <msp430.h>
#include "system.h"
#include "type.h"
#include "I2C.h"

extern uint result_gd;
extern unsigned int Initdata[4];

void Confige_1115(unsigned char channel);
void Point_reg(void);
unsigned int Read_1115(void);


#endif
