/*
 * system.h
 *
 *  Created on: 2014Äê8ÔÂ18ÈÕ
 *      Author: dell
 */

#ifndef SYSTEM_H_
#define SYSTEM_H_

#include <msp430.h>
#include "type.h"

#define CPU_F ((double)25000000)
#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0))
#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0))

void WDT_Init(void);


//   ACLK = REFO = 32kHz, MCLK = SMCLK = 25MHz
//
//                MSP430F5529
//             -----------------
//         /|\|                 |
//          | |             P1.0|-->ACLK
//          --|RST          P7.7|-->MCLK
//            |             P2.2|-->SMCLK
void SET_FLL_25MHZ(void);

//   MSP430F552x Demo - Software Toggle P1.1 with 12MHz DCO
//
//   Description: Toggle P1.1 by xor'ing P1.1 inside of a software loop.
//   ACLK is rought out on pin P1.0, SMCLK is brought out on P2.2, and MCLK
//   is brought out on pin P7.7.
//   ACLK = REFO = 32kHz, MCLK = SMCLK = 12MHz
//
//                 MSP430F552x
//             -----------------
//         /|\|                 |
//          | |             P1.0|-->ACLK
//          --|RST          P7.7|-->MCLK
//            |             P2.2|-->SMCLK
void SET_FLL_12MHZ(void);

//   MSP430F552x Demo - Software Toggle P1.1 with 8MHz DCO
//
//   Description: Toggle P1.1 by xor'ing P1.1 inside of a software loop.
//   ACLK is rought out on pin P1.0, SMCLK is brought out on P2.2, and MCLK
//   is brought out on pin P7.7.
//   ACLK = REFO = 32kHz, MCLK = SMCLK = 8MHz
//
//                MSP430F5529
//             -----------------
//         /|\|                 |
//          | |             P1.0|-->ACLK
//          --|RST          P7.7|-->MCLK
//            |             P2.2|-->SMCLK
void SET_FLL_8MHZ(void);

//               MSP430F552x
//             -----------------
//        /|\ |                 |
//         |  |                 |
//         ---|RST              |
//            |            XT2IN|-
//            |                 | HF XTAL (455kHz - 16MHz)
//            |           XT2OUT|-
//            |                 |
//            |             P7.7|--> MCLK = XT2
//            |             P2.2|--> SMCLK = XT2
void SET_XT2(void);

//  MSP430F552x Demo - FLL+, Runs Internal DCO at 2.45MHz
//
//  Description: This program demonstrates setting the internal DCO to run at
//  2.45MHz with auto-calibration by the FLL+ circuitry.
//  ACLK = REFO = 32768Hz, MCLK = SMCLK = DCO = (74+1) x REFO = 2457600Hz
//
//                MSP430F552x
//             -----------------
//         /|\|                 |
//          | |             P1.0|-->ACLK
//          --|RST          P7.7|-->MCLK
void SET_AEX_FLL_2_45MHZ(void);

//  MSP430F552x Demo - XT1 sources ACLK
//
//  Description: This program demonstrates using XT1 to source ACLK
//  ACLK = LFXT1 = 32,768Hz
//  //* An external watch crystal between XIN & XOUT is required for ACLK *//
//
//               MSP430F552x
//             -----------------
//        /|\ |              XIN|-
//         |  |                 | 32kHz
//         ---|RST          XOUT|-
//            |                 |
//            |             P1.0|-->ACLK = ~32kHz
void SET_ACLK_EX(void);

//  MSP430F552x Demo - FLL+, Output 32kHz Xtal + HF Xtal + Internal DCO
//
//  Description:  This program demonstrates using an external 32kHz crystal to
//  supply ACLK, and using a high speed crystal or resonator to supply SMCLK.
//  MLCK for the CPU is supplied by the internal DCO.  The 32kHz crystal
//  connects between pins Xin and Xout. The high frequency crystal or
//  resonator connects between pins XT2IN and XT2OUT.  The DCO clock is
//  generated internally and calibrated from the 32kHz crystal.
//  ACLK is rought out on pin P1.0, SMCLK is brought out on P2.2, and MCLK
//  is brought out on pin P7.7.
//  ACLK = LFXT1 = 32768Hz, MCLK = default DCO = 32 x ACLK = 1048576Hz
//  SMCLK = HF XTAL
//  //* An external watch crystal between XIN & XOUT is required for ACLK *//
//
//  NOTE: External matching capacitors must be added for the high
//       speed crystal or resonator as required.
//
//                MSP430F552x
//             -----------------
//        /|\ |              XIN|-
//         |  |                 | 32kHz
//         ---|RST          XOUT|-
//            |                 |
//            |                 |
//            |            XT2IN|-
//            |                 | HF XTAL or Resonator (add capacitors)
//            |           XT2OUT|-
//            |                 |
//            |             P1.0|--> ACLK = 32kHz Crystal Out
//            |                 |
//            |             P2.2|--> SMCLK = High Freq Xtal or Resonator Out
//            |                 |
//            |             P7.7|--> MCLK = Default DCO Frequency
void SET_AM_EX(void);

#endif /* SYSTEM_H_ */
