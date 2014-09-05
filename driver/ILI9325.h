/*
 * ILI9325.h
 *
 *  Created on: 2014年8月20日
 *      Author: dell
 */
#ifndef ILI9325_H_
#define ILI9325_H_

#include <msp430.h>
#include "system.h"
#include "type.h"

//LCD重要参数集
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

//定义TFT彩屏模块接口
#define TFT_RS_1  P2OUT |= BIT0;     //数据/命令选择  p3.0
#define TFT_RS_0  P2OUT &= ~BIT0;

#define TFT_WR_1  P2OUT |= BIT5;	//写p3.1引脚，根据实际硬件需要更改
#define TFT_WR_0  P2OUT &= ~BIT5;

#define TFT_RD_1  P2OUT |= BIT2;	//读p3.2引脚，根据实际硬件需要更改
#define TFT_RD_0  P2OUT &= ~BIT2;

#define TFT_CS_1  P2OUT |= BIT3;	//片选p3.3引脚，根据实际硬件需要更改
#define TFT_CS_0  P2OUT &= ~BIT3;

//#define TFT_RST_1 P2OUT |= BIT4;	//复位p3.4引脚，根据实际硬件需要更改
//#define TFT_RST_0 P2OUT &= ~BIT4;

#define TFT_DATA P3OUT	//数据口，P4端口为8位数据，根据实际硬件需要更改

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
#define BLUE         	 0x001F
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //棕色
#define BRRED 			 0XFC07 //棕红色
#define GRAY  			 0X8430 //灰色
//GUI颜色

#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色
#define GRAYBLUE       	 0X5458 //灰蓝色
//以上三色为PANEL的颜色

#define LIGHTGREEN     	 0X841F //浅绿色
//#define LIGHTGRAY        0XEF5B //浅灰色(PANNEL)
#define LGRAY 			 0XC618 //浅灰色(PANNEL),窗体背景色

#define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)


//9320/9325 LCD寄存器
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



//端口初始化函数
void InitPort(void);

//=============================================================
//写命令
//功能：写一个命令到TFT控制器。
//入口参数: CH,CL分别为命令字的高8位和低8位，指定要执行的命令。
//出口参数: 无
void write_cmd(unsigned char CH,unsigned char CL);

//===================================================================
//写数据
//功能：写一个数据到TFT控制器。
//入口参数: DH,DL分别为数据字的高8位和低8位，指定要写入的数据。
//出口参数: 无
void write_data(unsigned char DH,unsigned char DL);

//==============================================================
//写16位命令
//功能： 写一个命令到TFT控制器。
//入口参数: cmd为要写入的16位命令。
//出口参数: 无
void write_cmd_u16(unsigned int cmd);

//==============================================================
//写16位数据
//功能： 写一个数据到TFT控制器。
//入口参数: dat为要写入的16位数据。
//出口参数: 无
void write_data_u16(unsigned int dat);

//===============================================================
//写命令数据
//功能： 写命令数据到TFT控制器。
//入口参数: cmd为要写入的命令字节；dat为要写入的16位数据。
//出口参数: 无
void write_cmd_data (unsigned int cmd,unsigned int dat);

//=============================================================
//TFT初始化
//功能：该函数用于初始化TFT彩屏模块。
//入口参数: 无
//出口参数: 无
void ILI9325_initial(void);

//设置LCD显示方向
//dir:0,竖屏；1,横屏
void LCD_Display_Dir(u8 dir);

//设置LCD的自动扫描方向
//dir:0~7,代表8个方向(具体定义见ILI9325.h)
void LCD_Scan_Dir(u8 dir);

//设置光标位置
//Xpos:横坐标
//Ypos:纵坐标
void LCD_SetCursor(u16 Xpos, u16 Ypos);

//开始写GRAM
void LCD_WriteRAM_Prepare(void);

//清屏函数
//color:要清屏的填充色
void LCD_Clear(u16 color);

//LCD开启显示
void LCD_DisplayOn(void);

//LCD关闭显示
void LCD_DisplayOff(void);

//画点
//x,y:坐标
//POINT_COLOR:此点的颜色
void LCD_DrawPoint(u16 x,u16 y);

//在指定区域内填充单个颜色
//(sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex-sx+1)*(ey-sy+1)
//color:要填充的颜色
void LCD_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color);

//在指定区域内填充指定颜色块
//(sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex-sx+1)*(ey-sy+1)
//color:要填充的颜色
void LCD_Color_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 *color);

//画线
//x1,y1:起点坐标
//x2,y2:终点坐标
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2);

//画线
//x1,y1:起点坐标
//x2,y2:终点坐标
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2);

//在指定位置画一个指定大小的圆
//(x,y):中心点
//r    :半径
void Draw_Circle(u16 x0,u16 y0,u8 r);

//在指定位置显示一个字符
//x,y:起始坐标
//num:要显示的字符:" "--->"~"
//size:字体大小 12/16
//mode:叠加方式(1)还是非叠加方式(0)
void LCD_ShowChar(u16 x,u16 y,u8 num,u8 size,u8 mode);

//m^n函数
//返回值:m^n次方.
u32 LCD_Pow(u8 m,u8 n);

//显示数字,高位为0,则不显示
//x,y :起点坐标
//len :数字的位数
//size:字体大小
//color:颜色
//num:数值(0~4294967295);
void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 size);

//显示数字,高位为0,还是显示
//x,y:起点坐标
//num:数值(0~999999999);
//len:长度(即要显示的位数)
//size:字体大小
//mode:
//[7]:0,不填充;1,填充0.
//[6:1]:保留
//[0]:0,非叠加显示;1,叠加显示.
void LCD_ShowxNum(u16 x,u16 y,u32 num,u8 len,u8 size,u8 mode);

//显示字符串
//x,y:起点坐标
//width,height:区域大小
//size:字体大小
//*p:字符串起始地址
void LCD_ShowString(u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p);      //显示一个字符串,12/16字体

/****************************************************************************
* 名    称：void LCD_Chinese(u16 x, u16 y, u8 *str, u16 len)
* 功    能：在指定座标显示汉字文本
* 入口参数：x      行座标
*           y      列座标
*           *str   汉字串
*           len    字串长度
* 出口参数：无
* 说    明：一个汉字两个字符，记得哟，5个汉字len就是10
* 调用方法：LCD_Chinese(0,0,"成都贝一特",10);
****************************************************************************/
/*void LCD_Chinese(u16 x,u16 y, u8 str[],u8 len);*/

#endif /* ILI9325_H_ */
