#include "lcd_touch.h"

void SPI_Port_Init(void)
{
  P1DIR = 0xFE;//0、4、2输出，3输入
  CS_SET;
  SCLK_CLR;
}

uchar ReadFromSPI(void)
{
	uchar i=0;
	uchar Data = 0;
	uchar iTemp;

	//CS_CLR;
	//delay_us(10);
	//SCLK_SET;
	//delay_us(10);

	for(i=0; i<8; i++)
	{
		SCLK_CLR;
		Data <<= 1;
		SCLK_SET;
		SCLK_CLR;             //下降沿有效
		iTemp = DOUT_RDY;
		SCLK_SET;
		if(iTemp)
			Data |= 1;
		delay_us(10);
	}

	//CS_SET;
	return Data;
}

void WriteToSPI(uchar data)
{
	uchar ValueToWrite = data;
	uchar j=0;

	//CS_CLR;
	//delay_us(10);
	SCLK_CLR;
	delay_us(10);

	for(j=0; j<8; j++)
	{
		if((ValueToWrite & 0x80)==0x80)
			DIN_SET;
		else
			DIN_CLR;
		SCLK_CLR;               //上升沿有效
		SCLK_SET;
		ValueToWrite <<= 1;
	}

	//CS_SET;
}

void SpiDelay(unsigned int DelayCnt)
{
		unsigned int i;
		for(i=0;i<DelayCnt;i++);
}

u16 TPReadX(void)
{ 
	   u16 x=0;
	   CS_CLR;
	   SpiDelay(10);
	   WriteToSPI(0x90);
	   SCLK_SET;
	   SCLK_CLR;
	   //SpiDelay(10);
	   x=ReadFromSPI();
	   x<<=8;
	   x+=ReadFromSPI();
	   SpiDelay(10);
	   CS_SET;
	   x = x>>3;
	   return (x);
}

u16 TPReadY(void)
{
	  u16 y=0;
	  CS_CLR;
	  SpiDelay(10);
	  WriteToSPI(0xD0);
	  SCLK_SET;
	  SCLK_CLR;
	  //SpiDelay(10);
	  y=ReadFromSPI();
	  y<<=8;
	  y+=ReadFromSPI();
	  SpiDelay(10);
	  CS_SET;
	  y = y>>3; 
	  return (y);
}
