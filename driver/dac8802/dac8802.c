#include "dac8802.h"

void dac8802_init(void)
{
  P2DIR |= BIT0+BIT1+BIT2+BIT3+BIT4;
}

void write_16bits(uint data)
{
  uchar i;
  RST_1;
  LDAC_1;
  CS_1;
  
  CLK_0;
  delay_us(1);
  CS_0;
  delay_us(1);//Tcss
  for(i=0;i<16;i++)
  {
    if(data & 0x8000) SDI_1;
    else              SDI_0;
    delay_us(1);//data set time
    CLK_1;//clocked into serial shift register
    delay_us(1);//data hod time
    CLK_0;  
    
    data<<=1;
  }
  delay_us(1);//Tchs
  CS_1;//load data into input register
  delay_us(1);//Tlds
  LDAC_0;//transfer data from input register to the DAC register
  delay_us(1);//Tldac
  LDAC_1;
  delay_us(1);//Tldh
  CS_0;
}

void DAC8802_A(uint data)
{
	data |= 0x4000;
	write_16bits(data);
}

void DAC8802_B(uint data)
{
	data |= 0x8000;
	write_16bits(data);
}

void DAC8802_AB(uint data)
{
	data |= 0xC000;
	write_16bits(data);
}

