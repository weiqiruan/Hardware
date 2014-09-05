/*
 * ILI9325.h
 *
 *  Created on: 2014��8��20��
 *      Author: dell
 */
#ifndef ILI9325_H_
#define ILI9325_H_

#include <msp430.h>
#include "system.h"
#include "type.h"

//LCD��Ҫ������
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

//����TFT����ģ��ӿ�
#define TFT_RS_1  P2OUT |= BIT0;     //����/����ѡ��  p3.0
#define TFT_RS_0  P2OUT &= ~BIT0;

#define TFT_WR_1  P2OUT |= BIT5;	//дp3.1���ţ�����ʵ��Ӳ����Ҫ����
#define TFT_WR_0  P2OUT &= ~BIT5;

#define TFT_RD_1  P2OUT |= BIT2;	//��p3.2���ţ�����ʵ��Ӳ����Ҫ����
#define TFT_RD_0  P2OUT &= ~BIT2;

#define TFT_CS_1  P2OUT |= BIT3;	//Ƭѡp3.3���ţ�����ʵ��Ӳ����Ҫ����
#define TFT_CS_0  P2OUT &= ~BIT3;

//#define TFT_RST_1 P2OUT |= BIT4;	//��λp3.4���ţ�����ʵ��Ӳ����Ҫ����
//#define TFT_RST_0 P2OUT &= ~BIT4;

#define TFT_DATA P3OUT	//���ݿڣ�P4�˿�Ϊ8λ���ݣ�����ʵ��Ӳ����Ҫ����

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
#define BLUE         	 0x001F
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //��ɫ
#define BRRED 			 0XFC07 //�غ�ɫ
#define GRAY  			 0X8430 //��ɫ
//GUI��ɫ

#define DARKBLUE      	 0X01CF	//����ɫ
#define LIGHTBLUE      	 0X7D7C	//ǳ��ɫ
#define GRAYBLUE       	 0X5458 //����ɫ
//������ɫΪPANEL����ɫ

#define LIGHTGREEN     	 0X841F //ǳ��ɫ
//#define LIGHTGRAY        0XEF5B //ǳ��ɫ(PANNEL)
#define LGRAY 			 0XC618 //ǳ��ɫ(PANNEL),���屳��ɫ

#define LGRAYBLUE        0XA651 //ǳ����ɫ(�м����ɫ)
#define LBBLUE           0X2B12 //ǳ����ɫ(ѡ����Ŀ�ķ�ɫ)


//9320/9325 LCD�Ĵ���
#define R0             0x00
#define R1             0x01
#define R2             0x02
#define R3             0x03
#define R4             0x04
#define R5             0x05
#define R6             0x06
#define R7             0x07
#define R8             0x08
#define R9             0x09
#define R10            0x0A
#define R12            0x0C
#define R13            0x0D
#define R14            0x0E
#define R15            0x0F
#define R16            0x10
#define R17            0x11
#define R18            0x12
#define R19            0x13
#define R20            0x14
#define R21            0x15
#define R22            0x16
#define R23            0x17
#define R24            0x18
#define R25            0x19
#define R26            0x1A
#define R27            0x1B
#define R28            0x1C
#define R29            0x1D
#define R30            0x1E
#define R31            0x1F
#define R32            0x20
#define R33            0x21
#define R34            0x22
#define R36            0x24
#define R37            0x25
#define R40            0x28
#define R41            0x29
#define R43            0x2B
#define R45            0x2D
#define R48            0x30
#define R49            0x31
#define R50            0x32
#define R51            0x33
#define R52            0x34
#define R53            0x35
#define R54            0x36
#define R55            0x37
#define R56            0x38
#define R57            0x39
#define R59            0x3B
#define R60            0x3C
#define R61            0x3D
#define R62            0x3E
#define R63            0x3F
#define R64            0x40
#define R65            0x41
#define R66            0x42
#define R67            0x43
#define R68            0x44
#define R69            0x45
#define R70            0x46
#define R71            0x47
#define R72            0x48
#define R73            0x49
#define R74            0x4A
#define R75            0x4B
#define R76            0x4C
#define R77            0x4D
#define R78            0x4E
#define R79            0x4F
#define R80            0x50
#define R81            0x51
#define R82            0x52
#define R83            0x53
#define R96            0x60
#define R97            0x61
#define R106           0x6A
#define R118           0x76
#define R128           0x80
#define R129           0x81
#define R130           0x82
#define R131           0x83
#define R132           0x84
#define R133           0x85
#define R134           0x86
#define R135           0x87
#define R136           0x88
#define R137           0x89
#define R139           0x8B
#define R140           0x8C
#define R141           0x8D
#define R143           0x8F
#define R144           0x90
#define R145           0x91
#define R146           0x92
#define R147           0x93
#define R148           0x94
#define R149           0x95
#define R150           0x96
#define R151           0x97
#define R152           0x98
#define R153           0x99
#define R154           0x9A
#define R157           0x9D
#define R192           0xC0
#define R193           0xC1
#define R229           0xE5



//�˿ڳ�ʼ������
void InitPort(void);

//=============================================================
//д����
//���ܣ�дһ�����TFT��������
//��ڲ���: CH,CL�ֱ�Ϊ�����ֵĸ�8λ�͵�8λ��ָ��Ҫִ�е����
//���ڲ���: ��
void write_cmd(unsigned char CH,unsigned char CL);

//===================================================================
//д����
//���ܣ�дһ�����ݵ�TFT��������
//��ڲ���: DH,DL�ֱ�Ϊ�����ֵĸ�8λ�͵�8λ��ָ��Ҫд������ݡ�
//���ڲ���: ��
void write_data(unsigned char DH,unsigned char DL);

//==============================================================
//д16λ����
//���ܣ� дһ�����TFT��������
//��ڲ���: cmdΪҪд���16λ���
//���ڲ���: ��
void write_cmd_u16(unsigned int cmd);

//==============================================================
//д16λ����
//���ܣ� дһ�����ݵ�TFT��������
//��ڲ���: datΪҪд���16λ���ݡ�
//���ڲ���: ��
void write_data_u16(unsigned int dat);

//===============================================================
//д��������
//���ܣ� д�������ݵ�TFT��������
//��ڲ���: cmdΪҪд��������ֽڣ�datΪҪд���16λ���ݡ�
//���ڲ���: ��
void write_cmd_data (unsigned int cmd,unsigned int dat);

//=============================================================
//TFT��ʼ��
//���ܣ��ú������ڳ�ʼ��TFT����ģ�顣
//��ڲ���: ��
//���ڲ���: ��
void ILI9325_initial(void);

//����LCD��ʾ����
//dir:0,������1,����
void LCD_Display_Dir(u8 dir);

//����LCD���Զ�ɨ�跽��
//dir:0~7,����8������(���嶨���ILI9325.h)
void LCD_Scan_Dir(u8 dir);

//���ù��λ��
//Xpos:������
//Ypos:������
void LCD_SetCursor(u16 Xpos, u16 Ypos);

//��ʼдGRAM
void LCD_WriteRAM_Prepare(void);

//��������
//color:Ҫ���������ɫ
void LCD_Clear(u16 color);

//LCD������ʾ
void LCD_DisplayOn(void);

//LCD�ر���ʾ
void LCD_DisplayOff(void);

//����
//x,y:����
//POINT_COLOR:�˵����ɫ
void LCD_DrawPoint(u16 x,u16 y);

//��ָ����������䵥����ɫ
//(sx,sy),(ex,ey):�����ζԽ�����,�����СΪ:(ex-sx+1)*(ey-sy+1)
//color:Ҫ������ɫ
void LCD_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color);

//��ָ�����������ָ����ɫ��
//(sx,sy),(ex,ey):�����ζԽ�����,�����СΪ:(ex-sx+1)*(ey-sy+1)
//color:Ҫ������ɫ
void LCD_Color_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 *color);

//����
//x1,y1:�������
//x2,y2:�յ�����
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2);

//����
//x1,y1:�������
//x2,y2:�յ�����
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2);

//��ָ��λ�û�һ��ָ����С��Բ
//(x,y):���ĵ�
//r    :�뾶
void Draw_Circle(u16 x0,u16 y0,u8 r);

//��ָ��λ����ʾһ���ַ�
//x,y:��ʼ����
//num:Ҫ��ʾ���ַ�:" "--->"~"
//size:�����С 12/16
//mode:���ӷ�ʽ(1)���Ƿǵ��ӷ�ʽ(0)
void LCD_ShowChar(u16 x,u16 y,u8 num,u8 size,u8 mode);

//m^n����
//����ֵ:m^n�η�.
u32 LCD_Pow(u8 m,u8 n);

//��ʾ����,��λΪ0,����ʾ
//x,y :�������
//len :���ֵ�λ��
//size:�����С
//color:��ɫ
//num:��ֵ(0~4294967295);
void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 size);

//��ʾ����,��λΪ0,������ʾ
//x,y:�������
//num:��ֵ(0~999999999);
//len:����(��Ҫ��ʾ��λ��)
//size:�����С
//mode:
//[7]:0,�����;1,���0.
//[6:1]:����
//[0]:0,�ǵ�����ʾ;1,������ʾ.
void LCD_ShowxNum(u16 x,u16 y,u32 num,u8 len,u8 size,u8 mode);

//��ʾ�ַ���
//x,y:�������
//width,height:�����С
//size:�����С
//*p:�ַ�����ʼ��ַ
void LCD_ShowString(u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p);      //��ʾһ���ַ���,12/16����

/****************************************************************************
* ��    �ƣ�void LCD_Chinese(u16 x, u16 y, u8 *str, u16 len)
* ��    �ܣ���ָ��������ʾ�����ı�
* ��ڲ�����x      ������
*           y      ������
*           *str   ���ִ�
*           len    �ִ�����
* ���ڲ�������
* ˵    ����һ�����������ַ����ǵ�Ӵ��5������len����10
* ���÷�����LCD_Chinese(0,0,"�ɶ���һ��",10);
****************************************************************************/
/*void LCD_Chinese(u16 x,u16 y, u8 str[],u8 len);*/

#endif /* ILI9325_H_ */
