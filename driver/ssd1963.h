/*
 * ssd1963.h
 *
 *  Created on: 2014年8月20日
 *      Author: dell
 */
#ifndef __SSD1963_H
#define __SSD1963_H

#include <msp430.h>
#include "system.h"
#include "type.h"

typedef struct  
{										    
	u16 width;			//LCD 宽度
	u16 height;			//LCD 高度
	u16 id;				//LCD ID
	u8  dir;			//横屏还是竖屏控制：0，竖屏；1，横屏。	
	u8	wramcmd;		//开始写gram指令
	u8  setxcmd;		//设置x坐标指令
	u8  setycmd;		//设置y坐标指令	 
}_lcd_dev; 	  

//LCD参数
extern _lcd_dev lcddev;	//管理LCD重要参数
//LCD的画笔颜色和背景色	   
extern u16  POINT_COLOR;//默认红色    
extern u16  BACK_COLOR; //背景颜色.默认为白色

//-----------------LCD端口定义---------------- 
//#define	LCD_LED PAout(1) //LCD背光    		 PB0

//定义TFT彩屏模块接口
#define TFT_RS_1  P1OUT |= BIT2;     //数据/命令选择  p3.0
#define TFT_RS_0  P1OUT &= ~BIT2;

#define TFT_WR_1  P1OUT |= BIT3;	//写p3.1引脚，根据实际硬件需要更改
#define TFT_WR_0  P1OUT &= ~BIT3;

#define TFT_RD_1  P1OUT |= BIT4;	//读p3.2引脚，根据实际硬件需要更改
#define TFT_RD_0  P1OUT &= ~BIT4;

#define TFT_CS_1  P1OUT |= BIT5;	//片选p3.3引脚，根据实际硬件需要更改
#define TFT_CS_0  P1OUT &= ~BIT5;

#define TFT_DATA_H P3OUT	//数据口,高位
#define TFT_DATA_L P2OUT	//数据口,低位

//扫描方向定义
#define L2R_U2D  0 //从左到右,从上到下
#define L2R_D2U  1 //从左到右,从下到上
#define R2L_U2D  2 //从右到左,从上到下
#define R2L_D2U  3 //从右到左,从下到上

#define U2D_L2R  4 //从上到下,从左到右
#define U2D_R2L  5 //从上到下,从右到左
#define D2U_L2R  6 //从下到上,从左到右
#define D2U_R2L  7 //从下到上,从右到左	 

#define DFT_SCAN_DIR  L2R_U2D  //默认的扫描方向

//画笔颜色
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
#define BROWN 			     0XBC40 //棕色
#define BRRED 			     0XFC07 //棕红色
#define GRAY  			     0X8430 //灰色
//GUI颜色

#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色  
#define GRAYBLUE       	 0X5458 //灰蓝色
//以上三色为PANEL的颜色 
 
#define LIGHTGREEN     	 0X841F //浅绿色
#define LIGHTGRAY        0XEF5B //浅灰色(PANNEL)
#define LGRAY 			     0XC618 //浅灰色(PANNEL),窗体背景色

#define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)
			   																			 
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
void LCD_Scan_Dir(u8 dir);							//设置屏扫描方向
void LCD_Display_Dir(u8 dir);						//设置屏幕显示方向    dir:0,竖屏；1,横屏

void LCD_DrawPoint(u16 x,u16 y);																				//画点
void LCD_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color);                   //填充单色
void LCD_Color_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 *color);						//填充指定颜色
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2);											//画线
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2);									//画矩形
void LCD_DrawCircle(u16 x0,u16 y0,u8 r);																//画圆

void LCD_ShowChar(u16 x,u16 y,u8 num,u8 size,u8 mode);            			//显示一个字符
void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 size);  									//显示一个数字
void LCD_ShowxNum(u16 x,u16 y,u32 num,u8 len,u8 size,u8 mode);				  //显示 数字
void LCD_ShowString(u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p);		//显示一个字符串,12/16字体
//void LCD_Chinese(u16 x,u16 y, u8 str[],u8 len);											//显示一串汉字

#endif
