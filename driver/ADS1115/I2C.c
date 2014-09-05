#include "I2C.h"

/**************��ʼ�ź�***************/
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
/*************��ֹ�ź�*************/
void stop()
{
	SDA_Out;
	SDA_L;
	SCL_H;
	SDA_H;
}
/*************��I2C���߷���һ���ֽ�************/
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
/**************��ȡһ���ֽ�**************/
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
