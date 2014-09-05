#include "DAC8552.h"

void DAC8852_port_init()
{
  P2SEL=0X00;
  P2DIR=0XFF;
}
void write_8552(ulong data)
{
  uchar i;
  SYNC_1;
  delay_us(1);
  SCLK_1;
  SYNC_0;
  for(i=0;i<24;i++)
  {
    if(0x800000&data) DIN_1;
    else              DIN_0;
    SCLK_0;
    delay_us(1);
    SCLK_1;
    data=data<<1;
  }
  delay_us(1);
  SYNC_1;
}
void DAC_A(ulong data)
{
  ulong ctr=0x10;
  data=data|(ctr<<16);
  write_8552(data);
}
void DAC_B(ulong data)
{
  ulong ctr=0x24;
  data=data|(ctr<<16);
  write_8552(data);
}
void DAC_AB(ulong data_A,ulong data_B)
{
  ulong ctr=0x00;
  data_A=data_A|(ctr<<16);
  write_8552(data_A);
  
  ctr=0x34;
  data_B=data_B|(ctr<<16);
  write_8552(data_B);
}
