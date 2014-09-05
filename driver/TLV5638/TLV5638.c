#include"TLV5638.h"

void TLV5638_port_init()
{
  P1SEL=0X00;
  P1DIR=0XFF;
}
void write_5638(uint data)
{
  uchar i;
  CS_0;
  SCLK_1;
  for(i=0;i<16;i++)
  {
    if(0x8000&data) DIN_1;
    else            DIN_0;
    delay_us(1);
    SCLK_0;
    delay_us(1);
    SCLK_1;
    data=data<<1;
  }
  delay_us(1);
  CS_1;
}
