/*
 * ILI9325.c
 *
 *  Created on: 2014年8月20日
 *      Author: dell
 */
#include "ILI9325.h"
#include "font.h"

u16 POINT_COLOR=0x0000;	//画笔颜色
u16 BACK_COLOR=0xFFFF;  //背景色

//管理LCD重要参数
//默认为竖屏
_lcd_dev lcddev;

//端口初始化函数
void InitPort(void)
{
  P2DIR=0XFF;    //控制引脚，输出方向，不同硬件请更改，配合头文件引脚分配
  P3DIR=0XFF;    //数据引脚，输出方向，不同硬件请更改，配合头文件引脚分配
}

//=============================================================
//写命令
//功能：写一个命令到TFT控制器。
//入口参数: CH,CL分别为命令字的高8位和低8位，指定要执行的命令。
//出口参数: 无
void write_cmd(unsigned char CH,unsigned char CL)
{
    TFT_RD_1;// 读控制命令，"0"为读有效，写时一直为高电平
	TFT_CS_0; //低电平有效，片选
	TFT_RS_0;//"0"为命令，"1"为数据
	TFT_DATA=CH;//传输的命令，高8位
	TFT_WR_0;//低电平到高电平变化完成命令传输
	TFT_WR_1;//
	TFT_DATA=CL;//传输的命令，低8位
	TFT_WR_0;
	TFT_WR_1;//低电平到高电平变化完成数据传输
	TFT_CS_1;//高电平无效，结束片选
}

//===================================================================
//写数据
//功能：写一个数据到TFT控制器。
//入口参数: DH,DL分别为数据字的高8位和低8位，指定要写入的数据。
//出口参数: 无
void write_data(unsigned char DH,unsigned char DL)
{
	TFT_RD_1;  // 读控制命令，"0"为读有效
    TFT_CS_0;  //低电平有效，片选
	TFT_RS_1;  //"1"为数据，"0"为命令
	TFT_DATA=DH;  //传输的数据，高8位
	TFT_WR_0;  // 低电平
	TFT_WR_1;  //变化到高电平，启动传输
	TFT_DATA=DL;  //传输的数据，低8位
	TFT_WR_0;  // 低电平
	TFT_WR_1; // 低电平
	TFT_CS_1;//高电平无效，结束片选
}

//==============================================================
//写16位命令
//功能： 写一个命令到TFT控制器。
//入口参数: cmd为要写入的16位命令。
//出口参数: 无
void write_cmd_u16(unsigned int cmd)
{
	unsigned char m,n;
	m=cmd>>8;   //高8位，右移8位
	n=cmd;     //低8位
	write_cmd(m,n);
}

//==============================================================
//写16位数据
//功能： 写一个数据到TFT控制器。
//入口参数: dat为要写入的16位数据。
//出口参数: 无
void write_data_u16(unsigned int dat)
{
	unsigned char m,n;
	m=dat>>8;   //高8位，右移8位
	n=dat;     //低8位
	write_data(m,n);
}

//===============================================================
//写命令数据
//功能： 写命令数据到TFT控制器。
//入口参数: cmd为要写入的命令字节；dat为要写入的16位数据。
//出口参数: 无
void write_cmd_data (unsigned int cmd,unsigned int dat)
{
	write_cmd_u16(cmd);
	write_data_u16(dat);
}

//=============================================================
//TFT初始化
//功能：该函数用于初始化TFT彩屏模块。
//入口参数: 无
//出口参数: 无
void ILI9325_initial(void)
{
    InitPort();
	TFT_CS_1;
	delay_ms(5);
	//TFT_RST_0;
	//delay_ms(5);
	//TFT_RST_1;
	//delay_ms(20);//可以适当调整延时，保障稳定显示
	write_cmd_data(0x0001,0x0100); //源扫描设置SS=0, 输出的移动方向是从S1到S720
	//（源扫描设置SS=1, 输出的移动方向是从S720到S1）
	//SM=0，逐行扫描（SM=1，隔行扫描）

	write_cmd_data(0x0002,0x0700); //B/C=1、EOR=1：行倒置（B/C=0：帧/场倒置）
	write_cmd_data(0x0003,0x1030); //AM=0，地址在水平方向自动加1
	//I/D[1:0] = 11：水平垂直方向均增加
	//BGR=1:采用BGR格式
	//TRI=0、DFM=*：8位机、两次数据传送
	write_cmd_data(0x0004,0x0000); //比例缩放设置：不缩放

	//显示控制
	write_cmd_data(0x0008,0x0207); //设置后边沿和前沿
	write_cmd_data(0x0009,0x0000);//设置非显示区时间间隔ISC[3:0]
	write_cmd_data(0x000A,0x0000); //帧标记功能

	write_cmd_data(0x000C,0x0000);//RGB显示接口控制1
	write_cmd_data(0x000D,0x0000);//帧标记位置
	write_cmd_data(0x000F,0x0000);//RGB显示接口控制2

	//电源配置
	write_cmd_data(0x0010,0x0000);
	write_cmd_data(0x0011,0x0007);
	write_cmd_data(0x0012,0x0000);
	write_cmd_data(0x0013,0x0000);
	//vgh
	write_cmd_data(0x0010,0x1290);
	write_cmd_data(0x0011,0x0227);
	//delay_ms(100);
	//vregiout
	write_cmd_data(0x0012,0x001d); //0x001b
	//delay_ms(100);
	//vom amplitude
	write_cmd_data(0x0013,0x1500);
	//delay_ms(100);
	//vom H
	write_cmd_data(0x0029,0x0018);
	write_cmd_data(0x002B,0x000D);

	//伽马校正
	write_cmd_data(0x0030,0x0004);
	write_cmd_data(0x0031,0x0307);
	write_cmd_data(0x0032,0x0002);//0006
	write_cmd_data(0x0035,0x0206);
	write_cmd_data(0x0036,0x0408);
	write_cmd_data(0x0037,0x0507);
	write_cmd_data(0x0038,0x0204);//0200
	write_cmd_data(0x0039,0x0707);
	write_cmd_data(0x003C,0x0405);//0504
	write_cmd_data(0x003D,0x0F02);

	//窗口设置
	write_cmd_data(0x0050,0x0000);//水平起始位置
	write_cmd_data(0x0051,0x00EF);//水平终止位置
	write_cmd_data(0x0052,0x0000);//垂直起始位置
	write_cmd_data(0x0053,0x013F);//垂直终止位置

	write_cmd_data(0x0060,0xA700);//门扫描设置，GS=1：从G320扫描到G1，320线
	write_cmd_data(0x0061,0x0001);
	write_cmd_data(0x006A,0x0000);
	//
	write_cmd_data(0x0080,0x0000);
	write_cmd_data(0x0081,0x0000);
	write_cmd_data(0x0082,0x0000);
	write_cmd_data(0x0083,0x0000);
	write_cmd_data(0x0084,0x0000);
	write_cmd_data(0x0085,0x0000);
	//
	write_cmd_data(0x0090,0x0010);
	write_cmd_data(0x0092,0x0600);
	write_cmd_data(0x0093,0x0003);
	write_cmd_data(0x0095,0x0110);
	write_cmd_data(0x0097,0x0000);
	write_cmd_data(0x0098,0x0000);
	write_cmd_data(0x0007,0x0133);
	//write_cmd_data(0x0022);

	LCD_Display_Dir(0);		 	//默认为竖屏
	LCD_Clear(WHITE);
}

//设置LCD显示方向
//dir:0,竖屏；1,横屏
void LCD_Display_Dir(u8 dir)
{
	lcddev.id==0X9325;
	if(dir==0)//竖屏
	{
		lcddev.dir=0;//竖屏
		lcddev.width=240;
		lcddev.height=320;

		lcddev.wramcmd=R34;
		lcddev.setxcmd=R32;
		lcddev.setycmd=R33;

	}else if(dir==1)//6804不支持横屏显示
	{
		lcddev.dir=1;//横屏
		lcddev.width=320;
		lcddev.height=240;

		lcddev.wramcmd=R34;
		lcddev.setxcmd=R33;
		lcddev.setycmd=R32;

	}
	LCD_Scan_Dir(DFT_SCAN_DIR);	//默认扫描方向
}

//设置LCD的自动扫描方向
//dir:0~7,代表8个方向(具体定义见ILI9325.h)
void LCD_Scan_Dir(u8 dir)
{
	u16 regval=0;
	u8 dirreg=0;

	switch(dir)
	{
		case L2R_U2D://从左到右,从上到下
			regval|=(1<<5)|(1<<4)|(0<<3);
			break;
		case L2R_D2U://从左到右,从下到上
			regval|=(0<<5)|(1<<4)|(0<<3);
			break;
		case R2L_U2D://从右到左,从上到下
			regval|=(1<<5)|(0<<4)|(0<<3);
			break;
		case R2L_D2U://从右到左,从下到上
			regval|=(0<<5)|(0<<4)|(0<<3);
			break;
		case U2D_L2R://从上到下,从左到右
			regval|=(1<<5)|(1<<4)|(1<<3);
			break;
		case U2D_R2L://从上到下,从右到左
			regval|=(1<<5)|(0<<4)|(1<<3);
			break;
		case D2U_L2R://从下到上,从左到右
			regval|=(0<<5)|(1<<4)|(1<<3);
			break;
		case D2U_R2L://从下到上,从右到左
			regval|=(0<<5)|(0<<4)|(1<<3);
			break;
	}

	dirreg=0X03;
	regval|=1<<12;

	write_cmd_data(dirreg,regval);

}

//设置光标位置
//Xpos:横坐标
//Ypos:纵坐标
void LCD_SetCursor(u16 Xpos, u16 Ypos)
{
	if(lcddev.dir==1)
		Xpos=lcddev.width-1-Xpos;//横屏其实就是调转x,y坐标
	write_cmd_data(lcddev.setxcmd, Xpos);
	write_cmd_data(lcddev.setycmd, Ypos);
}

//开始写GRAM
void LCD_WriteRAM_Prepare(void)
{
 	write_cmd_u16(lcddev.wramcmd);
}

//清屏函数
//color:要清屏的填充色
void LCD_Clear(u16 color)
{
	u32 index=0;
	u32 totalpoint=lcddev.width;
	totalpoint*=lcddev.height; 	//得到总点数
	LCD_SetCursor(0x00,0x0000);	//设置光标位置
	LCD_WriteRAM_Prepare();     //开始写入GRAM
	for(index=0;index<totalpoint;index++)
	{
		write_data_u16(color);
	}
}

//LCD开启显示
void LCD_DisplayOn(void)
{
	write_cmd_data(R7,0x0173); 			//开启显示
}

//LCD关闭显示
void LCD_DisplayOff(void)
{
	write_cmd_data(R7,0x0);//关闭显示
}

//画点
//x,y:坐标
//POINT_COLOR:此点的颜色
void LCD_DrawPoint(u16 x,u16 y)
{
	LCD_SetCursor(x,y);		//设置光标位置
	LCD_WriteRAM_Prepare();	//开始写入GRAM
	write_data_u16(POINT_COLOR);
}

//在指定区域内填充单个颜色
//(sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex-sx+1)*(ey-sy+1)
//color:要填充的颜色
void LCD_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color)
{
	u16 i,j;
	u16 xlen=0;
	xlen=ex-sx+1;
	for(i=sy;i<=ey;i++)
	{
	 	LCD_SetCursor(sx,i);      				//设置光标位置
		LCD_WriteRAM_Prepare();     			//开始写入GRAM
		for(j=0;j<xlen;j++)
			write_data_u16(color);
	}
}

//在指定区域内填充指定颜色块
//(sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex-sx+1)*(ey-sy+1)
//color:要填充的颜色
void LCD_Color_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 *color)
{
	u16 height,width;
	u16 i,j;
	width=ex-sx+1; 		//得到填充的宽度
	height=ey-sy+1;		//高度
 	for(i=0;i<height;i++)
	{
 		LCD_SetCursor(sx,sy+i);   	//设置光标位置
		LCD_WriteRAM_Prepare();     //开始写入GRAM
		for(j=0;j<width;j++)
			write_data_u16(color[i*height+j]);//写入数据
	}
}

//画线
//x1,y1:起点坐标
//x2,y2:终点坐标
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2)
{
	u16 t;
	int xerr=0,yerr=0,delta_x,delta_y,distance;
	int incx,incy,uRow,uCol;
	delta_x=x2-x1; //计算坐标增量
	delta_y=y2-y1;
	uRow=x1;
	uCol=y1;
	if(delta_x>0)incx=1; //设置单步方向
	else if(delta_x==0)incx=0;//垂直线
	else {incx=-1;delta_x=-delta_x;}
	if(delta_y>0)incy=1;
	else if(delta_y==0)incy=0;//水平线
	else{incy=-1;delta_y=-delta_y;}
	if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴
	else distance=delta_y;
	for(t=0;t<=distance+1;t++ )//画线输出
	{
		LCD_DrawPoint(uRow,uCol);//画点
		xerr+=delta_x ;
		yerr+=delta_y ;
		if(xerr>distance)
		{
			xerr-=distance;
			uRow+=incx;
		}
		if(yerr>distance)
		{
			yerr-=distance;
			uCol+=incy;
		}
	}
}

//画矩形
//(x1,y1),(x2,y2):矩形的对角坐标
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2)
{
	LCD_DrawLine(x1,y1,x2,y1);
	LCD_DrawLine(x1,y1,x1,y2);
	LCD_DrawLine(x1,y2,x2,y2);
	LCD_DrawLine(x2,y1,x2,y2);
}

//在指定位置画一个指定大小的圆
//(x,y):中心点
//r    :半径
void Draw_Circle(u16 x0,u16 y0,u8 r)
{
	int a,b;
	int di;
	a=0;b=r;
	di=3-(r<<1);             //判断下个点位置的标志
	while(a<=b)
	{
		LCD_DrawPoint(x0+a,y0-b);             //5
 		LCD_DrawPoint(x0+b,y0-a);             //0
		LCD_DrawPoint(x0+b,y0+a);             //4
		LCD_DrawPoint(x0+a,y0+b);             //6
		LCD_DrawPoint(x0-a,y0+b);             //1
 		LCD_DrawPoint(x0-b,y0+a);
		LCD_DrawPoint(x0-a,y0-b);             //2
  		LCD_DrawPoint(x0-b,y0-a);             //7
		a++;
		//使用Bresenham算法画圆
		if(di<0)di +=4*a+6;
		else
		{
			di+=10+4*(a-b);
			b--;
		}
	}
}

//在指定位置显示一个字符
//x,y:起始坐标
//num:要显示的字符:" "--->"~"
//size:字体大小 12/16
//mode:叠加方式(1)还是非叠加方式(0)
void LCD_ShowChar(u16 x,u16 y,u8 num,u8 size,u8 mode)
{
    u8 temp,t1,t;
	u16 y0=y;
	u16 colortemp=POINT_COLOR;
	//设置窗口
	num=num-' ';//得到偏移后的值
	if(!mode) //非叠加方式
	{
	    for(t=0;t<size;t++)
	    {
			if(size==12)temp=asc2_1206[num][t];  //调用1206字体
			else temp=asc2_1608[num][t];		 //调用1608字体
	        for(t1=0;t1<8;t1++)
			{
		        if(temp&0x80)POINT_COLOR=colortemp;
				else POINT_COLOR=BACK_COLOR;
				LCD_DrawPoint(x,y);
				temp<<=1;
				y++;
				if(x>=lcddev.width){POINT_COLOR=colortemp;return;}//超区域了
				if((y-y0)==size)
				{
					y=y0;
					x++;
					if(x>=lcddev.width){POINT_COLOR=colortemp;return;}//超区域了
					break;
				}
			}
	    }
	}else//叠加方式
	{
	    for(t=0;t<size;t++)
	    {
			if(size==12)temp=asc2_1206[num][t];  //调用1206字体
			else temp=asc2_1608[num][t];		 //调用1608字体
	        for(t1=0;t1<8;t1++)
			{
		        if(temp&0x80)LCD_DrawPoint(x,y);
				temp<<=1;
				y++;
				if(x>=lcddev.height){POINT_COLOR=colortemp;return;}//超区域了
				if((y-y0)==size)
				{
					y=y0;
					x++;
					if(x>=lcddev.width){POINT_COLOR=colortemp;return;}//超区域了
					break;
				}
			}
	    }
	}
	POINT_COLOR=colortemp;
}

//m^n函数
//返回值:m^n次方.
u32 LCD_Pow(u8 m,u8 n)
{
	u32 result=1;
	while(n--)result*=m;
	return result;
}

//显示数字,高位为0,则不显示
//x,y :起点坐标
//len :数字的位数
//size:字体大小
//color:颜色
//num:数值(0~4294967295);
void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 size)
{
	u8 t,temp;
	u8 enshow=0;
	for(t=0;t<len;t++)
	{
		temp=(num/LCD_Pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				LCD_ShowChar(x+(size/2)*t,y,' ',size,0);
				continue;
			}else enshow=1;

		}
	 	LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,0);
	}
}

//显示数字,高位为0,还是显示
//x,y:起点坐标
//num:数值(0~999999999);
//len:长度(即要显示的位数)
//size:字体大小
//mode:
//[7]:0,不填充;1,填充0.
//[6:1]:保留
//[0]:0,非叠加显示;1,叠加显示.
void LCD_ShowxNum(u16 x,u16 y,u32 num,u8 len,u8 size,u8 mode)
{
	u8 t,temp;
	u8 enshow=0;
	for(t=0;t<len;t++)
	{
		temp=(num/LCD_Pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				if(mode&0X80)LCD_ShowChar(x+(size/2)*t,y,'0',size,mode&0X01);
				else LCD_ShowChar(x+(size/2)*t,y,' ',size,mode&0X01);
 				continue;
			}else enshow=1;

		}
	 	LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,mode&0X01);
	}
}

//显示字符串
//x,y:起点坐标
//width,height:区域大小
//size:字体大小
//*p:字符串起始地址
void LCD_ShowString(u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p)
{
	u8 x0=x;
	width+=x;
	height+=y;
    while((*p<='~')&&(*p>=' '))//判断是不是非法字符!
    {
        if(x>=width){x=x0;y+=size;}
        if(y>=height)break;//退出
        LCD_ShowChar(x,y,*p,size,0);
        x+=size/2;
        p++;
    }
}

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
/*void LCD_Chinese(u16 x,u16 y, u8 str[],u8 len)
{
    u16 i=0,b;
    u16 j=0;
	u16 x_add,y_add;
    u16 tmp_char=0,index=0;
	u16 colortemp=POINT_COLOR;
	x_add=x;
	y_add=y;

	for(b=0;b<len/2;b++)
	{
			index=(94*(str[b*2] - 0xa1)+(str[b*2+1] - 0xa1));	    //计算区位
			for (i=0;i<16;i++)
		    {
			    for (j=0;j<8;j++)
			    {
				    tmp_char=HzLib[index*32+i*2];					//查字库
			        if ( (tmp_char << j) & 0x80)
			        {
								POINT_COLOR = colortemp;
			          LCD_DrawPoint(x_add+j,y_add+i);  // 字符颜色
			        }
			        else
			        {
								POINT_COLOR = BACK_COLOR;
			          LCD_DrawPoint(x_add+j,y_add+i);   // 背景颜色
			        }
			    }
			   	for (j=0;j<8;j++)
			    {
				    tmp_char=HzLib[index*32+i*2+1];				    //查字库
			        if ( (tmp_char << j) & 0x80)
			        {
								POINT_COLOR = colortemp;
			          LCD_DrawPoint(x_add+j+8,y_add+i); // 字符颜色
			        }
			        else
			        {
								POINT_COLOR = BACK_COLOR;
			          LCD_DrawPoint(x_add+j+8,y_add+i);   // 背景颜色
			        }
			    }
		     }
			 x_add=x_add+17;										 //地址累加
	}

	POINT_COLOR = colortemp;
}*/
