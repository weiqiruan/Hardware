#include "ADS1115.h"

//extern unsigned int Initdata[4];
uint result_gd=0;
uint Initdata[4]={0};
/*******************Config寄存器*********************/
void Confige_1115(uchar channel )
{
    uchar i=0;
    switch(channel)
    {
    case 0:  Initdata[2] = 0xc2; break;//0xc2
    case 1:  Initdata[2] = 0x52; break;
    case 2:  Initdata[2] = 0x62; break;
    case 3:  Initdata[2] = 0x72; 
    }
    Initdata[0] =0x90;       // 启动配置命令
    Initdata[1] =0x01;       // 指向配置寄存器
    Initdata[3] =0x83;       // 配置字低字节
    SCL_H;
    start();
    for(i=0;i<4;i++)
    {
    	  Send1byte(Initdata[i]);
    	  _NOP();
    }
    stop();
}
/************指向转换结果寄存器***************/
void Point_reg(void)
{
	uchar i=0;
	Initdata[0] =0x90;           // 地址 ＋ 写命令
	Initdata[1] =0x00;            // 指向转换结果寄存器
	  SDA_Out;
	  SCL_H;
	  start();
	  for(i=0;i<2;i++)
	    {
		  Send1byte(Initdata[i]);
	    }
	  stop();
}
/**************读取转换结果***************/
uint Read_1115(void)
{
  uchar   ResultL,ResultH;
  uint    Result;
  SCL_H;
  start();
  Send1byte(0x91);
  ResultH=Read1byte();
  ResultL=Read1byte();
  stop();
  Result=ResultH*256+ResultL;
  return   Result;
}
