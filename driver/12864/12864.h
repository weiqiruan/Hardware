#ifndef __12864_H
#define __12864_H

#include <msp430.h>
#include "system.h"
#include "type.h"

#define DATAPORT		P5OUT                   //���ݿ����ڶ˿�P4
#define DATASEL         P5SEL                   //���ݿڹ��ܼĴ��������ƹ���ģʽ
#define DATADIR         P5DIR                   //���ݿڷ���Ĵ���

#define RS_CLR	        P4OUT &= ~BIT4           //RS�õ�
#define RS_SET	        P4OUT |=  BIT4           //RS�ø�

#define RW_CLR	        P4OUT &= ~BIT5           //RW�õ�
#define RW_SET	        P4OUT |=  BIT5           //RW�ø�

#define EN_CLR	        P4OUT &= ~BIT6           //E�õ�
#define EN_SET	        P4OUT |=  BIT6           //E�ø�

#define RST_CLR	        P4OUT &= ~BIT7            //RST�õ�
#define RST_SET	        P4OUT |=  BIT7            //RST�ø�

/*12864Ӧ��ָ��*/
#define CLEAR_SCREEN	0x01		          //����ָ�������ACֵΪ00H
#define AC_INIT			0x02		          //��AC����Ϊ00H�����α��Ƶ�ԭ��λ��
#define CURSE_ADD		0x06		          //�趨�α��Ƶ�����ͼ�������ƶ�����Ĭ���α����ƣ�ͼ�����岻����
#define FUN_MODE		0x30		          //����ģʽ��8λ����ָ�
#define FUN_MODEK		0x36		          //����ģʽ��8λ��չָ�
#define DISPLAY_ON		0x0c		          //��ʾ��,��ʾ�α꣬���α�λ�÷���
#define DISPLAY_OFF		0x08		          //��ʾ��
#define CURSE_DIR		0x14		          //�α������ƶ�:AC=AC+1
#define SET_CG_AC		0x40		          //����AC����ΧΪ��00H~3FH
#define SET_DD_AC		0x80                  //����DDRAM AC

#define X1address       0x80        //12864�ϰ���X����ʼ��ַ
#define X2address       0x88        //12864�°���X����ʼ��ַ
#define Yaddress        0x80        //Y����ʼ��ַ

#define firstcode		0x80		//��һ����ʼ��ַ
#define secondcode		0x90		//�ڶ�����ʼ��ַ
#define thirdcode		0x88		//��������ʼ��ַ
#define fourthcode		0x98		//��������ʼ��ַ

/****************************************************************************
* ��    �ƣ�LCD_Port_Init
* ��    �ܣ�LCD�˿ڳ�ʼ��
* ��ڲ�������
* ���ڲ�������
****************************************************************************/
void LCD_Port_Init(void);

/****************************************************************************
* ��    �ƣ�LCD_Init
* ��    �ܣ�LCDӲ����ʼ��
* ��ڲ�������
* ���ڲ�������
****************************************************************************/
void LCD_Init(void);
 
/****************************************************************************
* ��    �ƣ�LCD_Write_Com
* ��    �ܣ�LCD����д��
* ��ڲ�����com--��������
* ���ڲ�������
****************************************************************************/
void LCD_Write_Com(uchar com);
 
/****************************************************************************
* ��    �ƣ�LCD_Write_Data
* ��    �ܣ�LCD����д��
* ��ڲ�����data--��ʾ����
* ���ڲ�������
****************************************************************************/
void LCD_Write_Data(uchar data);
 
/****************************************************************************
* ��    �ƣ�DisplayCharter
* ��    �ܣ�LCD��ʾ���ֻ��ַ�
* ��ڲ�����addr--��ַ��hz--����ָ��
* ���ڲ�������
****************************************************************************/
void DisplayCharter(uchar addr,uchar *hz);

/****************************************************************************
* ��    �ƣ�DisplayNumber
* ��    �ܣ�LCD��ʾ����
* ��ڲ�����addr--��ַ��number--����
* ���ڲ�������
****************************************************************************/
void DisplayNumber(uchar addr,uint number);

/****************************************************************************
* ��    �ƣ�LCD12864_Clear
* ��    �ܣ�LCD����ָ������
* ��ڲ�������
* ���ڲ�������
****************************************************************************/
void LCD12864_Clear(void);
 
/****************************************************************************
* ��    �ƣ�Display_Photo1
* ��    �ܣ�LCD�ϰ벿��ͼƬ��ʾ
* ��ڲ�����img1--ͼƬָ�����������
* ���ڲ�������
****************************************************************************/
void Display_Photo1(const uchar *img1);

/****************************************************************************
* ��    �ƣ�Display_Photo2
* ��    �ܣ�LCD�°벿��ͼƬ��ʾ
* ��ڲ�����img2--ͼƬָ�����������
* ���ڲ�������
****************************************************************************/
void Display_Photo2(const uchar *img2);
 
/****************************************************************************
* ��    �ƣ�LCD_Photo_Clear
* ��    �ܣ�LCD��չָ������
* ��ڲ�������
* ���ڲ�������
****************************************************************************/
void LCD_Photo_Clear(void);

#endif