#include "Communication.h"

void SPI_Port_Init(void)
{
  P1DIR = 0x07;//0°¢1°¢2 ‰≥ˆ£¨3 ‰»Î  
  P1OUT = 0xFF;
}

void ReadFromSPI(uchar *buf, uchar count)
{
	uchar i=0,j=0;
	uchar RotateData = 0;
	uchar iTemp;

	//CS_CLR;
	//delay_ms(1);
	SCLK_SET;
	delay_ms(1);

	for(j=0; j<count; j++)
	{
		for(i=0; i<8; i++)
		{
			SCLK_CLR;
			RotateData <<= 1;
			delay_ms(1);
			iTemp = DOUT_RDY;
			SCLK_SET;
			if(iTemp)
				RotateData |= 1;
			delay_ms(1);
		}
		*(buf + j) = RotateData;
	}

	//CS_SET;
}

/*void ReadFromAD7799(uchar *buf, uchar count)
{
  uchar i=0,j=0;
  uchar RotateData = 0;
  uchar iTemp;
  
  //CS_CLR;
  //delay_ms(1);
  SCLK_SET;
  delay_ms(1);
  
  for(j=count; j>0; j--)
  {
    for(i=0; i<8; i++)
    {
      SCLK_CLR;
      RotateData <<= 1;
      delay_ms(1);
      iTemp = DOUT_RDY;
      SCLK_SET;
      if(iTemp)
        RotateData |= 1;
      delay_ms(1);
    }
    *(buf + j - 1) = RotateData;
  }
  
  //CS_SET;
}*/

void WriteToSPI(uchar *buf, uchar count)
{
	uchar ValueToWrite = 0;
	uchar i=0,j=0;

	//CS_CLR;
	//delay_ms(1);
	SCLK_SET;
	delay_ms(1);

	for(i=0; i<count; i++)
	{
		ValueToWrite = *(buf + i);
		for(j=0; j<8; j++)
		{
			SCLK_CLR;
			if((ValueToWrite & 0x80)==0x80)
				DIN_SET;
			else
				DIN_CLR;
			delay_ms(1);
			SCLK_SET;
			delay_ms(1);
			ValueToWrite <<= 1;
		}
	}

	//CS_SET;
}

/*void WriteToAD7799(uchar *buf, uchar count)
{
	uchar ValueToWrite = 0;
	uchar i=0,j=0;

	//CS_CLR;
	//delay_ms(1);
	SCLK_SET;
	delay_ms(1);

	for(i=count; i>0; i--)
	{
		ValueToWrite = *(buf + i - 1);
		for(j=0; j<8; j++)
		{
			SCLK_CLR;
			if((ValueToWrite & 0x80)==0x80)
				DIN_SET;
			else
				DIN_CLR;
			delay_ms(1);
			SCLK_SET;
			delay_ms(1);
			ValueToWrite <<= 1;
		}
	}

	//CS_SET;
}*/
