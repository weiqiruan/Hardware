#include "ad9850.h"

/****************************************************************************
* ��    �ƣ�AD9850_Port_Init
* ��    �ܣ�AD9850�˿ڳ�ʼ��
* ��ڲ�������
* ���ڲ�������
****************************************************************************/
void AD9850_Port_Init(void)
{
  P6SEL = 0x00;
  P6DIR = 0xFF;
  P2SEL = 0x00;
  P2DIR|= BIT0 + BIT1 + BIT2;
}

/****************************************************************************
* ��    �ƣ�AD9850_Init_Parallel
* ��    �ܣ�AD9850����ģʽ��ʼ��
* ��ڲ�������
* ���ڲ�������
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
* ��    �ƣ�AD9850_WR_Parallel
* ��    �ܣ�AD9850��������д��
* ��ڲ�����w0--w0ֵ��frequence--Ƶ�ʣ���λ��HZ��
* ���ڲ�������
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
	
	//дw0����
	w=w0;   
	AD9850_DATAPORT=w;      //w0
	W_CLK_SET;
	W_CLK_CLR;
	
	//дw1����
	w=(y>>24);
	AD9850_DATAPORT=w;      //w1
	W_CLK_SET;
	W_CLK_CLR;
	
	//дw2����
	w=(y>>16);
	AD9850_DATAPORT=w;      //w2
	W_CLK_SET;
	W_CLK_CLR;
	
	//дw3����
	w=(y>>8);
	AD9850_DATAPORT=w;      //w3
	W_CLK_SET;
	W_CLK_CLR;
	
	//дw4����
	w=(y>>=0);
	AD9850_DATAPORT=w;      //w4
	W_CLK_SET;
	W_CLK_CLR;
	
	//����ʼ��
	FQ_UP_SET;

}
