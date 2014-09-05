#include "12864.h"

/****************************************************************************
* 名    称：LCD_Port_Init
* 功    能：LCD端口初始化
* 入口参数：无
* 出口参数：无
****************************************************************************/
void LCD_Port_Init(void)
{
  P5SEL = 0x00;
  P5DIR = 0xFF;
  P4SEL = 0x00;
  P4DIR|= BIT0 + BIT4 + BIT5 + BIT6 + BIT7;
  //PSB_SET;		          							//液晶并口方式
  RST_SET;		          							//复位脚RST置高
}

/****************************************************************************
* 名    称：LCD_Init
* 功    能：LCD硬件初始化
* 入口参数：无
* 出口参数：无
****************************************************************************/
void LCD_Init(void) 
{
  LCD_Write_Com(FUN_MODE);				//显示模式设置
  delay_ms(5);
  LCD_Write_Com(FUN_MODE);				//显示模式设置
  delay_ms(5);
  LCD_Write_Com(DISPLAY_ON);			//显示开
  delay_ms(5);
  LCD_Write_Com(CLEAR_SCREEN);			//清屏
  delay_ms(5);
}

/****************************************************************************
* 名    称：LCD_Write_Com
* 功    能：LCD命令写入
* 入口参数：com--控制命令
* 出口参数：无
****************************************************************************/
void LCD_Write_Com(uchar com) 
{	
  RS_CLR;
  RW_CLR;
  EN_SET;
  DATAPORT = com;
  delay_ms(5);
  EN_CLR;
}

/****************************************************************************
* 名    称：LCD_Write_Data
* 功    能：LCD数据写入
* 入口参数：data--显示数据
* 出口参数：无
****************************************************************************/
void LCD_Write_Data(uchar data) 
{
  RS_SET;
  RW_CLR;
  EN_SET;
  DATAPORT = data;
  delay_ms(5);
  EN_CLR;
}

/****************************************************************************
* 名    称：DisplayCharter
* 功    能：LCD显示汉字或字符
* 入口参数：addr--地址；hz--汉字指针
* 出口参数：无
****************************************************************************/
void DisplayCharter(uchar addr,uchar *hz)
{
  LCD_Write_Com(addr);
  delay_ms(5);
  while(*hz != '\0')  
  {
    LCD_Write_Data(*hz);
    hz++;
    delay_ms(5);
  }
	
} 

/****************************************************************************
* 名    称：DisplayNumber
* 功    能：LCD显示数字
* 入口参数：addr--地址；number--数字
* 出口参数：无
****************************************************************************/
void DisplayNumber(uchar addr,uint number)
{
  LCD_Write_Com(addr);
  delay_ms(5);
  LCD_Write_Data(0x30+number);
  delay_ms(5);
}

/****************************************************************************
* 名    称：LCD12864_Clear
* 功    能：LCD基本指令清屏
* 入口参数：无
* 出口参数：无
****************************************************************************/
void LCD12864_Clear(void)
{
  LCD_Write_Com(0x01);
  delay_ms(5);
}

/****************************************************************************
* 名    称：Display_Photo1
* 功    能：LCD上半部分图片显示
* 入口参数：img1--图片指针或者数组名
* 出口参数：无
****************************************************************************/
void Display_Photo1(const uchar *img1)
{
   int i,j;     
   
   LCD_Write_Com(FUN_MODEK);                 	  //扩展指令，开绘图显示
   delay_ms(1);

   for(i=0;i<32;i++)                              //上半屏32行
    {  

 		LCD_Write_Com(Yaddress+i);                //先写垂直地址，即Y地址，不能自动加一
        delay_ms(1);

        LCD_Write_Com(X1address);                 //再写水平地址，即X地址
        delay_ms(1);
      
		for(j=0;j<8;j++)                              //连续写入2个字节的数据，一共8次，为一行，即16*8位数据
			{  			                                     
			LCD_Write_Data(img1[i*16+j*2]);
			delay_ms(2);                              //这个延时是必须的，因为没有对液晶进行判忙操作，所以进行延时
			LCD_Write_Data(img1[i*16+j*2+1]); 
			delay_ms(2);          
			}

    }
}

/****************************************************************************
* 名    称：Display_Photo2
* 功    能：LCD下半部分图片显示
* 入口参数：img2--图片指针或者数组名
* 出口参数：无
****************************************************************************/
void Display_Photo2(const uchar *img2)          
{
	int i,j;

	for(i=0;i<32;i++)                           //下半屏32行
	{  

	 	LCD_Write_Com(Yaddress+i);              //先写垂直地址，即Y地址，不能自动加一
	    delay_ms(1);
		LCD_Write_Com(X2address);               //再写水平地址，即X地址
	    delay_ms(1);
	 	 for(j=0;j<8;j++)                      //连续写入2个字节的数据，一共8次，为一行，即16*8位数据
	    {
			
			LCD_Write_Data(img2[i*16+j*2]);
			delay_ms(2);                    
			LCD_Write_Data(img2[i*16+j*2+1]);  
			delay_ms(2);  
      
		}

	}

}

/****************************************************************************
* 名    称：LCD_Photo_Clear
* 功    能：LCD扩展指令清屏
* 入口参数：无
* 出口参数：无
****************************************************************************/
void LCD_Photo_Clear(void)           
{
	int i,j;
	
	
	LCD_Write_Com(FUN_MODEK);           //扩展指令，开绘图显示
	delay_ms(1);
     
        for(i=0;i<32;i++)                   //上半屏32行
    {  
 		LCD_Write_Com(Yaddress+i);          //先写垂直地址，即Y地址，不能自动加一
        delay_ms(1);

        LCD_Write_Com(X1address);           //再写水平地址，即X地址
        delay_ms(1);
      
		for(j=0;j<8;j++)                        //连续写入2个字节的数据，一共8次，为一行，即16*8位数据
			{  			                                     
			LCD_Write_Data(0x00);
			delay_us(10);                       //这个延时是必须的，因为没有对液晶进行判忙操作，所以进行延时
			LCD_Write_Data(0x00); 
			delay_us(10);          
			}

    }
    
    
	for(i=0;i<32;i++)                     //下半屏32行
	{  

	 	LCD_Write_Com(Yaddress+i);        //先写垂直地址，即Y地址，不能自动加一
	    delay_ms(1);
		LCD_Write_Com(X2address);         //再写水平地址，即X地址
	    delay_ms(1);
	 	 for(j=0;j<8;j++)                 //连续写入2个字节的数据，一共8次，为一行，即16*8位数据
	    {
			
			LCD_Write_Data(0x00);
			delay_ms(1);                    
			LCD_Write_Data(0x00);  
			delay_ms(1);  
      
		}

	}

}
