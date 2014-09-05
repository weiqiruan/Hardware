#ifndef __TLV5638_H
#define __TLV5638_H

#include <msp430.h>
#include "system.h"
#include "type.h"

//˵����
//��ͨ�����˿ڳ�ʼ��-->���òο���ѹ-->дͨ����������
//˫ͨ�����˿ڳ�ʼ��-->���òο���ѹ-->дBUFFER(ΪBͨ��׼�����ݣ�-->дAͨ������(�Զ���BUFFER���ݸ��µ�Bͨ��)
#define CS_0   P1OUT&=~BIT0
#define CS_1   P1OUT|=BIT0
#define SCLK_0 P1OUT&=~BIT1
#define SCLK_1 P1OUT|=BIT1
#define DIN_0  P1OUT&=~BIT2
#define DIN_1  P1OUT|=BIT2

//�ο���ѹ �����������ѹΪ�ο���ѹ��2��
#define FAST_REF1  0XD001   //�ڲ�1.024V
#define SLOW_REF1  0X9001   
#define FAST_REF2  0XD002   //�ڲ�2.048V
#define SLOW_REF2  0X9002
#define FAST_REF_E 0XD000   //�ⲿ
#define SLOW_REF_E 0X9000

void TLV5638_port_init();
void write_5638(uint data);

#endif
