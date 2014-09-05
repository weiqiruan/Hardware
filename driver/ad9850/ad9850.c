#include "ad9850.h"

/****************************************************************************
* 名    称：AD9850_Port_Init
* 功    能：AD9850端口初始化
* 入口参数：无
* 出口参数：无
****************************************************************************/
void AD9850_Port_Init(void)
{
  P6SEL = 0x00;
  P6DIR = 0xFF;
  P2SEL = 0x00;
  P2DIR|= BIT0 + BIT1 + BIT2;
}

/****************************************************************************
* 名    称：AD9850_Init_Parallel
* 功    能：AD9850并口模式初始化
* 入口参数：无
* 出口参数：无
****************************************************************************/
void AD9850_Init_Parallel(void)
{
	W_CLK_CLR;
	FQ_UP_CLR;
	REST_CLR;
	REST_SET;
	delay_us(10);
	REST_CLR;
}

/****************************************************************************
* 名    称：AD9850_WR_Parallel
* 功    能：AD9850并口数据写入
* 入口参数：w0--w0值；frequence--频率（单位：HZ）
* 出口参数：无
****************************************************************************/
void AD9850_WR_Parallel(uchar w0,double frequence)
{
	uchar w;
	lint y;
	double x;

	x=4294967295/125;

	frequence=frequence*x/1000000+0.5;
	y=frequence;

	FQ_UP_CLR;
	W_CLK_CLR;
	
	//写w0数据
	w=w0;   
	AD9850_DATAPORT=w;      //w0
	W_CLK_SET;
	W_CLK_CLR;
	
	//写w1数据
	w=(y>>24);
	AD9850_DATAPORT=w;      //w1
	W_CLK_SET;
	W_CLK_CLR;
	
	//写w2数据
	w=(y>>16);
	AD9850_DATAPORT=w;      //w2
	W_CLK_SET;
	W_CLK_CLR;
	
	//写w3数据
	w=(y>>8);
	AD9850_DATAPORT=w;      //w3
	W_CLK_SET;
	W_CLK_CLR;
	
	//写w4数据
	w=(y>>=0);
	AD9850_DATAPORT=w;      //w4
	W_CLK_SET;
	W_CLK_CLR;
	
	//移入始能
	FQ_UP_SET;

}
