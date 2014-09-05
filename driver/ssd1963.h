/*
 * ssd1963.h
 *
 *  Created on: 2014��8��20��
 *      Author: dell
 */
#ifndef __SSD1963_H
#define __SSD1963_H

#include <msp430.h>
#include "system.h"
#include "type.h"

typedef struct  
{										    
	u16 width;			//LCD ���
	u16 height;			//LCD �߶�
	u16 id;				//LCD ID
	u8  dir;			//���������������ƣ�0��������1��������	
	u8	wramcmd;		//��ʼдgramָ��
	u8  setxcmd;		//����x����ָ��
	u8  setycmd;		//����y����ָ��	 
}_lcd_dev; 	  

//LCD����
extern _lcd_dev lcddev;	//����LCD��Ҫ����
//LCD�Ļ�����ɫ�ͱ���ɫ	   
extern u16  POINT_COLOR;//Ĭ�Ϻ�ɫ    
extern u16  BACK_COLOR; //������ɫ.Ĭ��Ϊ��ɫ

//-----------------LCD�˿ڶ���---------------- 
//#define	LCD_LED PAout(1) //LCD����    		 PB0

//����TFT����ģ��ӿ�
#define TFT_RS_1  P1OUT |= BIT2;     //����/����ѡ��  p3.0
#define TFT_RS_0  P1OUT &= ~BIT2;

#define TFT_WR_1  P1OUT |= BIT3;	//дp3.1���ţ�����ʵ��Ӳ����Ҫ����
#define TFT_WR_0  P1OUT &= ~BIT3;

#define TFT_RD_1  P1OUT |= BIT4;	//��p3.2���ţ�����ʵ��Ӳ����Ҫ����
#define TFT_RD_0  P1OUT &= ~BIT4;

#define TFT_CS_1  P1OUT |= BIT5;	//Ƭѡp3.3���ţ�����ʵ��Ӳ����Ҫ����
#define TFT_CS_0  P1OUT &= ~BIT5;

#define TFT_DATA_H P3OUT	//���ݿ�,��λ
#define TFT_DATA_L P2OUT	//���ݿ�,��λ

//ɨ�跽����
#define L2R_U2D  0 //������,���ϵ���
#define L2R_D2U  1 //������,���µ���
#define R2L_U2D  2 //���ҵ���,���ϵ���
#define R2L_D2U  3 //���ҵ���,���µ���

#define U2D_L2R  4 //���ϵ���,������
#define U2D_R2L  5 //���ϵ���,���ҵ���
#define D2U_L2R  6 //���µ���,������
#define D2U_R2L  7 //���µ���,���ҵ���	 

#define DFT_SCAN_DIR  L2R_U2D  //Ĭ�ϵ�ɨ�跽��

//������ɫ
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	 	 0x001F  
#define BRED             0XF81F
#define GRED 			       0XFFE0
#define GBLUE			 			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			     0XBC40 //��ɫ
#define BRRED 			     0XFC07 //�غ�ɫ
#define GRAY  			     0X8430 //��ɫ
//GUI��ɫ

#define DARKBLUE      	 0X01CF	//����ɫ
#define LIGHTBLUE      	 0X7D7C	//ǳ��ɫ  
#define GRAYBLUE       	 0X5458 //����ɫ
//������ɫΪPANEL����ɫ 
 
#define LIGHTGREEN     	 0X841F //ǳ��ɫ
#define LIGHTGRAY        0XEF5B //ǳ��ɫ(PANNEL)
#define LGRAY 			     0XC618 //ǳ��ɫ(PANNEL),���屳��ɫ

#define LGRAYBLUE        0XA651 //ǳ����ɫ(�м����ɫ)
#define LBBLUE           0X2B12 //ǳ����ɫ(ѡ����Ŀ�ķ�ɫ)
			   																			 
void LCD_WR_REG(u16 regval);
void LCD_WR_DATA(u16 data);
void LCD_WriteReg(u8 LCD_Reg, u16 LCD_RegValue);
void LCD_WriteRAM_Prepare(void);
u32 LCD_Pow(u8 m,u8 n);

void LCD_Init(void);
void Address_set(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2);
void LCD_SetCursor(u16 Xpos, u16 Ypos);
void LCD_Clear(u16 color);
void LCD_DisplayOn(void);
void LCD_DisplayOff(void);
void LCD_Scan_Dir(u8 dir);							//������ɨ�跽��
void LCD_Display_Dir(u8 dir);						//������Ļ��ʾ����    dir:0,������1,����

void LCD_DrawPoint(u16 x,u16 y);																				//����
void LCD_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color);                   //��䵥ɫ
void LCD_Color_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 *color);						//���ָ����ɫ
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2);											//����
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2);									//������
void LCD_DrawCircle(u16 x0,u16 y0,u8 r);																//��Բ

void LCD_ShowChar(u16 x,u16 y,u8 num,u8 size,u8 mode);            			//��ʾһ���ַ�
void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 size);  									//��ʾһ������
void LCD_ShowxNum(u16 x,u16 y,u32 num,u8 len,u8 size,u8 mode);				  //��ʾ ����
void LCD_ShowString(u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p);		//��ʾһ���ַ���,12/16����
//void LCD_Chinese(u16 x,u16 y, u8 str[],u8 len);											//��ʾһ������

#endif
