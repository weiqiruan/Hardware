#include "system.h"
#include <msp430.h>
/*
 * sysytem.c
 *
 *  Created on: 2014年8月18日
 *      Author: dell
 */

/*
 *不设置时钟时，默认为：
//   Description: Toggle P1.0 by xor'ing P1.1 inside of a software loop.
//   ACLK is rought out on pin P1.0, SMCLK is brought out on P2.2, and MCLK
//   is brought out on pin P7.7.
//   ACLK = REFO = 32.768kHz, MCLK = SMCLK = Default 1MHz
//
//                 MSP430F552x
//             -----------------
//         /|\|                 |
//          | |             P1.0|-->ACLK
//          --|RST          P7.7|-->MCLK
//            |             P2.2|-->SMCLK
 */

/*
 * 关闭看门狗
 */
void WDT_Init(void)
{
   WDTCTL = WDTPW | WDTHOLD;       //关闭看门狗
}
/****************************************************************************
* 名    称：ClkInit
* 功    能：时钟系统初始化  MCLK为8MHz，SMCLK为1MHz
* 入口参数：无
* 出口参数：无
****************************************************************************/
/*void Clock_Init(void)
{
  uchar i;
  BCSCTL1&=~XT2OFF;                 //打开XT2振荡器
  BCSCTL2|=SELM1+SELS;              //MCLK为8MHZ，SMCLK为8MHZ
  do{
    IFG1&=~OFIFG;                   //清除振荡器错误标志
    for(i=0;i<100;i++)
       _NOP();
  }
  while((IFG1&OFIFG)!=0);           //如果标志位1，则继续循环等待
  IFG1&=~OFIFG;
}*/


void SetVcoreUp (unsigned int level)
{
  // Open PMM registers for write
  PMMCTL0_H = PMMPW_H;
  // Set SVS/SVM high side new level
  SVSMHCTL = SVSHE + SVSHRVL0 * level + SVMHE + SVSMHRRL0 * level;
  // Set SVM low side to new level
  SVSMLCTL = SVSLE + SVMLE + SVSMLRRL0 * level;
  // Wait till SVM is settled
  while ((PMMIFG & SVSMLDLYIFG) == 0);
  // Clear already set flags
  PMMIFG &= ~(SVMLVLRIFG + SVMLIFG);
  // Set VCore to new level
  PMMCTL0_L = PMMCOREV0 * level;
  // Wait till new level reached
  if ((PMMIFG & SVMLIFG))
    while ((PMMIFG & SVMLVLRIFG) == 0);
  // Set SVS/SVM low side to new level
  SVSMLCTL = SVSLE + SVSLRVL0 * level + SVMLE + SVSMLRRL0 * level;
  // Lock PMM registers for write access
  PMMCTL0_H = 0x00;
}

//   ACLK = REFO = 32kHz, MCLK = SMCLK = 25MHz
//
//                MSP430F5529
//             -----------------
//         /|\|                 |
//          | |             P1.0|-->ACLK
//          --|RST          P7.7|-->MCLK
//            |             P2.2|-->SMCLK
void SET_FLL_25MHZ(void)
{
	// Increase Vcore setting to level3 to support fsystem=25MHz
	// NOTE: Change core voltage one level at a time..
	SetVcoreUp (0x01);
	SetVcoreUp (0x02);
	SetVcoreUp (0x03);

	UCSCTL3 = SELREF_2;                       // Set DCO FLL reference = REFO
	UCSCTL4 |= SELA_2;                        // Set ACLK = REFO

	__bis_SR_register(SCG0);                  // Disable the FLL control loop
	UCSCTL0 = 0x0000;                         // Set lowest possible DCOx, MODx
	UCSCTL1 = DCORSEL_7;                      // Select DCO range 50MHz operation
	UCSCTL2 = FLLD_0 + 762;                   // Set DCO Multiplier for 25MHz
	                                            // (N + 1) * FLLRef = Fdco
	                                            // (762 + 1) * 32768 = 25MHz
	                                            // Set FLL Div = fDCOCLK/2
	__bic_SR_register(SCG0);                  // Enable the FLL control loop

	// Worst-case settling time for the DCO when the DCO range bits have been
	// changed is n x 32 x 32 x f_MCLK / f_FLL_reference. See UCS chapter in 5xx
	// UG for optimization.
	// 32 x 32 x 25 MHz / 32,768 Hz ~ 780k MCLK cycles for DCO to settle
	__delay_cycles(782000);

	// Loop until XT1,XT2 & DCO stabilizes - In this case only DCO has to stabilize
	do
	{
	  UCSCTL7 &= ~(XT2OFFG + XT1LFOFFG + DCOFFG);
	                                            // Clear XT2,XT1,DCO fault flags
	  SFRIFG1 &= ~OFIFG;                      // Clear fault flags
	}while (SFRIFG1&OFIFG);                   // Test oscillator fault flag
}

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
void SET_XT2(void)
{
	P5SEL |= BIT2+BIT3;                       // Port select XT2

	UCSCTL6 &= ~XT2OFF;                       // Enable XT2
	UCSCTL3 |= SELREF_2;                      // FLLref = REFO
	                                            // Since LFXT1 is not used,
	                                            // sourcing FLL with LFXT1 can cause
	                                            // XT1OFFG flag to set
	UCSCTL4 |= SELA_2;                        // ACLK=REFO,SMCLK=DCO,MCLK=DCO

	// Loop until XT1,XT2 & DCO stabilizes - in this case loop until XT2 settles
	do
	{
	  UCSCTL7 &= ~(XT2OFFG + XT1LFOFFG + DCOFFG);
	                                            // Clear XT2,XT1,DCO fault flags
	  SFRIFG1 &= ~OFIFG;                      // Clear fault flags
	}while (SFRIFG1&OFIFG);                   // Test oscillator fault flag

	UCSCTL6 &= ~XT2DRIVE0;                    // Decrease XT2 Drive according to
	                                            // expected frequency
	UCSCTL4 |= SELS_5 + SELM_5;               // SMCLK=MCLK=XT2
}

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
void SET_FLL_8MHZ(void)
{
	UCSCTL3 = SELREF_2;                       // Set DCO FLL reference = REFO
	UCSCTL4 |= SELA_2;                        // Set ACLK = REFO
	UCSCTL0 = 0x0000;                         // Set lowest possible DCOx, MODx

	// Loop until XT1,XT2 & DCO stabilizes - In this case only DCO has to stabilize
	do
	{
	  UCSCTL7 &= ~(XT2OFFG + XT1LFOFFG + DCOFFG);
	                                            // Clear XT2,XT1,DCO fault flags
	  SFRIFG1 &= ~OFIFG;                      // Clear fault flags
	}while (SFRIFG1&OFIFG);                   // Test oscillator fault flag

	__bis_SR_register(SCG0);                  // Disable the FLL control loop
	UCSCTL1 = DCORSEL_5;                      // Select DCO range 16MHz operation
	UCSCTL2 |= 249;                           // Set DCO Multiplier for 8MHz
	                                          // (N + 1) * FLLRef = Fdco
	                                          // (249 + 1) * 32768 = 8MHz
	__bic_SR_register(SCG0);                  // Enable the FLL control loop

	// Worst-case settling time for the DCO when the DCO range bits have been
	// changed is n x 32 x 32 x f_MCLK / f_FLL_reference. See UCS chapter in 5xx
	// UG for optimization.
	// 32 x 32 x 8 MHz / 32,768 Hz = 250000 = MCLK cycles for DCO to settle
	__delay_cycles(250000);
}

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
void SET_FLL_12MHZ(void)
{
	UCSCTL3 |= SELREF_2;                      // Set DCO FLL reference = REFO
	UCSCTL4 |= SELA_2;                        // Set ACLK = REFO

    __bis_SR_register(SCG0);                  // Disable the FLL control loop
	UCSCTL0 = 0x0000;                         // Set lowest possible DCOx, MODx
	UCSCTL1 = DCORSEL_5;                      // Select DCO range 24MHz operation
	UCSCTL2 = FLLD_1 + 374;                   // Set DCO Multiplier for 12MHz
	                                          // (N + 1) * FLLRef = Fdco
	                                          // (374 + 1) * 32768 = 12MHz
	                                          // Set FLL Div = fDCOCLK/2
	__bic_SR_register(SCG0);                  // Enable the FLL control loop

	// Worst-case settling time for the DCO when the DCO range bits have been
	// changed is n x 32 x 32 x f_MCLK / f_FLL_reference. See UCS chapter in 5xx
	// UG for optimization.
	// 32 x 32 x 12 MHz / 32,768 Hz = 375000 = MCLK cycles for DCO to settle
	__delay_cycles(375000);

	// Loop until XT1,XT2 & DCO fault flag is cleared
	do
	{
	  UCSCTL7 &= ~(XT2OFFG + XT1LFOFFG + DCOFFG);
	                                          // Clear XT2,XT1,DCO fault flags
	  SFRIFG1 &= ~OFIFG;                      // Clear fault flags
	}while (SFRIFG1&OFIFG);                   // Test oscillator fault flag
}

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
void SET_AEX_FLL_2_45MHZ(void)
{
	P5SEL |= BIT4+BIT5;                       // Port select XT1
	UCSCTL6 &= ~(XT1OFF);                     // XT1 On
	UCSCTL6 |= XCAP_3;                        // Internal load cap

	// Loop until XT1 fault flag is cleared
	do
	{
	  UCSCTL7 &= ~XT1LFOFFG;                  // Clear XT1 fault flags
	}while (UCSCTL7&XT1LFOFFG);               // Test XT1 fault flag

	// Initialize DCO to 2.45MHz
    __bis_SR_register(SCG0);                  // Disable the FLL control loop
	UCSCTL0 = 0x0000;                         // Set lowest possible DCOx, MODx
	UCSCTL1 = DCORSEL_3;                      // Set RSELx for DCO = 4.9 MHz
	UCSCTL2 = FLLD_1 + 74;                    // Set DCO Multiplier for 2.45MHz
	                                          // (N + 1) * FLLRef = Fdco
	                                          // (74 + 1) * 32768 = 2.45MHz
	                                          // Set FLL Div = fDCOCLK/2
	__bic_SR_register(SCG0);                  // Enable the FLL control loop

	// Worst-case settling time for the DCO when the DCO range bits have been
	// changed is n x 32 x 32 x f_MCLK / f_FLL_reference. See UCS chapter in 5xx
	// UG for optimization.
	// 32 x 32 x 2.45 MHz / 32,768 Hz = 76563 = MCLK cycles for DCO to settle
	__delay_cycles(76563);

	// Loop until XT1,XT2 & DCO fault flag is cleared
	do
	{
	  UCSCTL7 &= ~(XT2OFFG + XT1LFOFFG + DCOFFG);
	                                          // Clear XT2,XT1,DCO fault flags
	  SFRIFG1 &= ~OFIFG;                      // Clear fault flags
	}while (SFRIFG1&OFIFG);                   // Test oscillator fault flag
}

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
void SET_ACLK_EX(void)
{
	P5SEL |= BIT4+BIT5;                     // Select XT1

	UCSCTL6 &= ~(XT1OFF);                     // XT1 On
	UCSCTL6 |= XCAP_3;                        // Internal load cap
	UCSCTL3 = 0;                              // FLL Reference Clock = XT1

	// Loop until XT1,XT2 & DCO stabilizes - In this case loop until XT1 and DCo settle
	do
	{
	  UCSCTL7 &= ~(XT2OFFG + XT1LFOFFG + DCOFFG);
	                                          // Clear XT2,XT1,DCO fault flags
	  SFRIFG1 &= ~OFIFG;                      // Clear fault flags
	}while (SFRIFG1&OFIFG);                   // Test oscillator fault flag

	UCSCTL6 &= ~(XT1DRIVE_3);                 // Xtal is now stable, reduce drive strength

	UCSCTL4 |= SELA_0;                        // ACLK = LFTX1 (by default)
}

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
void SET_AM_EX(void)
{
	P5SEL |= BIT2+BIT3;                       // Port select XT2
	P5SEL |= BIT4+BIT5;                       // Port select XT1

	UCSCTL6 &= ~XT2OFF;                       // Set XT2 On
	UCSCTL6 &= ~XT1OFF;                       // Set XT1 On
	UCSCTL6 |= XCAP_3;                        // Internal load cap

	// XT1 is by default on as it is used default reference for the FLL - internal load caps?
	// Loop until XT1,XT2 & DCO stabilizes
	do
	{
	  UCSCTL7 &= ~(XT2OFFG + XT1LFOFFG + DCOFFG);
	                                          // Clear XT2,XT1,DCO fault flags
	  SFRIFG1 &= ~OFIFG;                      // Clear fault flags
	}while (SFRIFG1&OFIFG);                   // Test oscillator fault flag

	UCSCTL6 &= ~XT2DRIVE0;                    // Decrease XT2 Drive according to
	                                            // expected frequency
	UCSCTL4 |= SELA_0 + SELS_5;               // Select SMCLK, ACLK source and DCO source
}
