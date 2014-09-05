/*
 * ssd1963.c
 *
 *  Created on: 2014年8月20日
 *      Author: dell
 */
#include "ssd1963.h"
#include "font.h"

//LCD的画笔颜色和背景色	   
u16 POINT_COLOR=0x0000;	//画笔颜色
u16 BACK_COLOR=0xFFFF;  //背景色 

//管理LCD重要参数
//默认为竖屏
_lcd_dev lcddev;

u16  HDP=799;
u16  HT=928;
u16  HPS=46;
u16  LPS=15;
u8   HPW=48;

u16  VDP=479;
u16  VT=525;
u16  VPS=16;
u16  FPS=8;
u8   VPW=16;
	
		   
//写寄存器函数
//regval:寄存器值
void LCD_WR_REG(u16 regval)
{ 
	u8 CH=0,CL=0;
	CH = regval>>8;
	CL = regval;
	TFT_RD_1;// 读控制命令，"0"为读有效，写时一直为高电平
	TFT_CS_0; //低电平有效，片选
	TFT_RS_0;//"0"为命令，"1"为数据
	TFT_DATA_H=CH;//传输的命令，高8位
	TFT_DATA_L=CL;//传输的命令，低8位
	TFT_WR_0;
	TFT_WR_1;//低电平到高电平变化完成数据传输
	TFT_CS_1;//高电平无效，结束片选
}
//写LCD数据
//data:要写入的值
void LCD_WR_DATA(u16 data)
{										    	   
	u8 CH=0,CL=0;
	CH = data>>8;
	CL = data;
	TFT_RD_1;// 读控制命令，"0"为读有效，写时一直为高电平
	TFT_CS_0; //低电平有效，片选
	TFT_RS_1;//"0"为命令，"1"为数据
	TFT_DATA_H=CH;//传输的命令，高8位
	TFT_DATA_L=CL;//传输的命令，低8位
	TFT_WR_0;
	TFT_WR_1;//低电平到高电平变化完成数据传输
	TFT_CS_1;//高电平无效，结束片选
}					   
//写寄存器
//LCD_Reg:寄存器地址
//LCD_RegValue:要写入的数据
void LCD_WriteReg(u8 LCD_Reg, u16 LCD_RegValue)
{	
	LCD_WR_REG(LCD_Reg);		//写入要写的寄存器序号
	LCD_WR_DATA(LCD_RegValue);  //写入数据
}
//开始写GRAM
void LCD_WriteRAM_Prepare(void)
{
	LCD_WR_REG(lcddev.wramcmd);
}	 


void LCD_Init(void)
{
	P2DIR = 0xFF;
	P3DIR = 0xFF;
	P1DIR = 0xFF;
		
	LCD_WR_REG(0x00E2);	//PLL multiplier, set PLL clock to 120M
	LCD_WR_DATA(0x0023);	    //N=0x36 for 6.5M, 0x23 for 10M crystal
	LCD_WR_DATA(0x0002);
	LCD_WR_DATA(0x0004);
	
	LCD_WR_REG(0x00E0);  // PLL enable
	LCD_WR_DATA(0x0001);
	delay_ms(1);
	LCD_WR_REG(0x00E0);
	LCD_WR_DATA(0x0003);
	delay_ms(5);
	LCD_WR_REG(0x0001);  // software reset
	delay_ms(5);
	LCD_WR_REG(0x00E6);	//PLL setting for PCLK, depends on resolution
	LCD_WR_DATA(0x0003);//LCD_WR_DATA(0x0003);
	LCD_WR_DATA(0x00FF);//LCD_WR_DATA(0x0033);
	LCD_WR_DATA(0x00FF);//LCD_WR_DATA(0x0032);


	LCD_WR_REG(0x00B0);	//LCD SPECIFICATION
	LCD_WR_DATA(0x0027);
	LCD_WR_DATA(0x0000);
	LCD_WR_DATA((HDP>>8)&0X00FF);  //Set HDP
	LCD_WR_DATA(HDP&0X00FF);
  LCD_WR_DATA((VDP>>8)&0X00FF);  //Set VDP
	LCD_WR_DATA(VDP&0X00FF);
  LCD_WR_DATA(0x0000);

	LCD_WR_REG(0x00B4);	//HSYNC
	LCD_WR_DATA((HT>>8)&0X00FF);  //Set HT
	LCD_WR_DATA(HT&0X00FF);
	LCD_WR_DATA((HPS>>8)&0X00FF);  //Set HPS
	LCD_WR_DATA(HPS&0X00FF);
	LCD_WR_DATA(HPW);			   //Set HPW
	LCD_WR_DATA((LPS>>8)&0X00FF);  //Set LPS
	LCD_WR_DATA(LPS&0X00FF);
	LCD_WR_DATA(0x0000);

	LCD_WR_REG(0x00B6);	//VSYNC
	LCD_WR_DATA((VT>>8)&0X00FF);   //Set VT
	LCD_WR_DATA(VT&0X00FF);
	LCD_WR_DATA((VPS>>8)&0X00FF);  //Set VPS
	LCD_WR_DATA(VPS&0X00FF);
	LCD_WR_DATA(VPW);			   //Set VPW
	LCD_WR_DATA((FPS>>8)&0X00FF);  //Set FPS
	LCD_WR_DATA(FPS&0X00FF);

	LCD_WR_REG(0x00BA);
	LCD_WR_DATA(0x000F);    //GPIO[3:0] out 1

	LCD_WR_REG(0x00B8);
	LCD_WR_DATA(0x0007);    //GPIO3=input, GPIO[2:0]=output
	LCD_WR_DATA(0x0001);    //GPIO0 normal

	/*LCD_WR_REG(0x0036); //rotation
	LCD_WR_DATA(0x0000);*/

	LCD_WR_REG(0x00F0); //pixel data interface
	LCD_WR_DATA(0x0003);

	delay_ms(5);

	LCD_WR_REG(0x0029); //display on

	LCD_WR_REG(0x00d0);
	LCD_WR_DATA(0x000d);

	delay_ms(50);
  LCD_Display_Dir(1);		 	//默认为横屏
	//LCD_LED=1;							//点亮背光
	LCD_Clear(WHITE);
		
}

void Address_set(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2)
{ 
	LCD_WR_REG(lcddev.setxcmd);	
	LCD_WR_DATA(x1>>8);	    
	LCD_WR_DATA(x1&0x00ff);
	LCD_WR_DATA(x2>>8);	    
	LCD_WR_DATA(x2&0x00ff);
	
  LCD_WR_REG(lcddev.setycmd);	
	LCD_WR_DATA(y1>>8);	    
	LCD_WR_DATA(y1&0x00ff);
	LCD_WR_DATA(y2>>8);	    
	LCD_WR_DATA(y2&0x00ff);
	
	LCD_WR_REG(0x002c);					 		//write_memory_start				 
}


//设置LCD显示方向
//dir:0,竖屏；1,横屏
void LCD_Display_Dir(u8 dir)
{
	if(dir==1)
	{
		LCD_WR_REG(0x0036); //rotation
		LCD_WR_DATA(0x0000);
		
		lcddev.dir=1;//横屏
		lcddev.width=800;
		lcddev.height=480;

		lcddev.wramcmd=0x2C;
		lcddev.setxcmd=0X2A;
		lcddev.setycmd=0X2B;  
		
	}
	else
	{
		LCD_WR_REG(0x0036); //rotation
		LCD_WR_DATA(0x0001);
		
		lcddev.dir=0;//竖屏
		lcddev.width=480;
		lcddev.height=800;

		lcddev.wramcmd=0x2C;
		lcddev.setxcmd=0X2B;
		lcddev.setycmd=0X2A;  
			
	} 
	
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
		LCD_WR_DATA(color);
	}
}    

//设置光标位置
//Xpos:横坐标
//Ypos:纵坐标
void LCD_SetCursor(u16 Xpos, u16 Ypos)
{	 
	LCD_WR_REG(lcddev.setxcmd);	
	LCD_WR_DATA(Xpos>>8);	    
	LCD_WR_DATA(Xpos&0x00ff);
	if(lcddev.setxcmd == 0x2A)
	{
		LCD_WR_DATA(799>>8);	    
		LCD_WR_DATA(799&0x00ff);
	}else{
		LCD_WR_DATA(479>>8);	    
		LCD_WR_DATA(479&0x00ff);	
	}
	
	LCD_WR_REG(lcddev.setycmd);	
	LCD_WR_DATA(Ypos>>8);	    
	LCD_WR_DATA(Ypos&0x00ff);
	if(lcddev.setycmd == 0x2A)
	{
		LCD_WR_DATA(799>>8);	    
		LCD_WR_DATA(799&0x00ff);
	}else{
		LCD_WR_DATA(479>>8);	    
		LCD_WR_DATA(479&0x00ff);	
	}
} 

//LCD开启显示
void LCD_DisplayOn(void)
{					   
	LCD_WR_REG(0X29);	//开启显示
}	 
//LCD关闭显示
void LCD_DisplayOff(void)
{	   
	LCD_WR_REG(0X28);	//关闭显示
}  

//画点
//x,y:坐标
//POINT_COLOR:此点的颜色
void LCD_DrawPoint(u16 x,u16 y)
{
	LCD_SetCursor(x,y);		//设置光标位置 
	LCD_WriteRAM_Prepare();	//开始写入GRAM
	LCD_WR_DATA(POINT_COLOR);
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
			LCD_WR_DATA(color);	//设置光标位置
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
			LCD_WR_DATA(color[i*height+j]);//写入数据
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
void LCD_DrawCircle(u16 x0,u16 y0,u8 r)
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
* 名    称：void GUI_Text(u16 x, u16 y, u8 *str, u16 len)
* 功    能：在指定座标显示汉字文本
* 入口参数：x      行座标
*           y      列座标
*           *str   汉字串
*           len    字串长度
*           Color  字符颜色
*           bkColor字符背景颜色
* 出口参数：无
* 说    明：一个汉字两个字符，记得哟，5个汉字len就是10
* 调用方法：GUI_Text(0,0,"成都贝一特",10);
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
