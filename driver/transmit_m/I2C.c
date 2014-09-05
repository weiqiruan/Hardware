#include "I2C.h"

/**************起始信号***************/
void start()
{       
    SDA_Out;
	SDA_L;
    SCL_Out;
	SCL_H;
	SDA_H;
	SDA_L;
	SCL_L;
}
/*************终止信号*************/
void stop()
{
	SDA_Out;
	SDA_L;
	SCL_H;
	SDA_H;
}
/*************向I2C总线发送一个字节************/
void Send1byte(uint byte)
{
	uint i;
        SDA_Out;
	for(i=0;i<8;i++)
	{
	   if((byte<<i) & 0x80)
		   SDA_H;
	   else
		   SDA_L;
	   SCL_H;
           SCL_L;
	 }
  SDA_H;
  SCL_H;
  SCL_L;
}
/**************读取一个字节**************/
uint Read1byte()
{
    uint temp=0,i;
    SDA_In;
    for(i=0;i<8;i++)
    {
    	temp=temp<<1;
    	SCL_L;
        SCL_H;
        if(P2IN&0x02)
        {
        	temp|=0x01;
        }

    }
    SDA_Out;
    SCL_L;
    SDA_L;
    SCL_H;
    SCL_L;
    SDA_H;
    return temp;
}
