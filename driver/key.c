#include "key.h"

uchar key = 0xFF;

/****************************************************************************
* 名    称：Key_Port_Init
* 功    能：Key端口初始化
* 入口参数：无
* 出口参数：无
****************************************************************************/
void Key_Port_Init(void)
{
    P1SEL = 0x00;                        //键盘的IO口设置为普通I/O模式，此句可省      
    P1DIR = 0x0F;                        //高四位输入模式，低四位输出模式，外部上拉电阻
    P1OUT = 0xF0;                        //初始值0xF0，为什么要这样？准备可中断的条件
    P1IES = 0xf0;                        //P1.4~P1.7下降沿触发中断
    P1IE  = 0xf0;                        //P1.4~P1.7允许中断
    P1IFG = 0x00;                        //中断标志清0
	_EINT();
}

/****************************************************************************
* 名    称：Key_Scan
* 功    能：Key扫描
* 入口参数：无
* 出口参数：key--矩阵键盘位置
****************************************************************************/
uchar Key_Scan(void)
{
  uchar key_checkin,key_check;
  
   P1OUT=0xf0;                       //在IO口由输出方式变为输入方式时要延迟一个周期
   P1OUT=0xf0;                       //采取写两次的方法延时
   key_checkin=P1IN; 
  // while(P1IN!=0xf0);                                     //读取IO口状态，判断是否有键按下
  if(key_checkin!=0xf0)               //IO口值发生变化则表示有键按下
  {
    delay_ms(20);                     //键盘消抖，延时20MS
    key_checkin=P1IN;
    if(key_checkin!=0xf0)
    {
      
      P1OUT=0xfe;                    //逐行扫描开始，1111 1110
      P1OUT=0xfe;                    //写两次延时，下同
      key_check=P1IN;
      switch (key_check)               //根据读到的IO口值判断按键的按下情况
      {
        case 0xee:key=15;break;
        case 0xde:key=11;break;
        case 0xbe:key=8;break;
        case 0x7e:key=4;break;
       }
         
     P1OUT=0xf7;    //1111 0111  
     P1OUT=0xf7;
     key_check=P1IN;
     switch (key_check)
       {
        case 0xe7:key=12;break;
        case 0xd7:key=9;break;
        case 0xb7:key=5;break;
        case 0x77:key=1;break;
        }
       
       P1OUT=0xfd; //1111 1101
       P1OUT=0xfd;
       key_check=P1IN;
       switch (key_check)
         {
         case 0xed:key=14;break;
         case 0xdd:key=10;break;
         case 0xbd:key=7;break;
         case 0x7d:key=3;break;
         }
       
        P1OUT=0xfb;   //1111 1011
        P1OUT=0xfb;
        key_check=P1IN;
        switch (key_check)
        {
         case 0xeb:key=13;break;
         case 0xdb:key=0;break;
         case 0xbb:key=6;break;
         case 0x7b:key=2;break;
        }   
     }
   }
  else
  {                    //无按键时返回值
    key=0xFF;
  }
    return(key);
}

/*
	//端口1按键中断
#pragma vector=PORT1_VECTOR
__interrupt void Port(void)
{
  if((P1IFG&0xf0)!=0)
  {  
    Key_Scan();                     //键盘扫描，看是否有按键按下 
  }
   if(key!=0xff)                   //如果有按键按下，则显示该按键键值0~15
       {
          LCD_Write_Com(0x99);
          switch(key)
        {
          case 0:  LCD_Write_Data(0x00+0x30);statevalue=0x00+0x30;break;   //根据键值显示到液晶上
          case 1:  LCD_Write_Data(0x01+0x30);statevalue=0x01+0x30;break;
          case 2:  LCD_Write_Data(0x02+0x30);statevalue=0x02+0x30;break;
          case 3:  LCD_Write_Data(0x03+0x30);statevalue=0x03+0x30;break;
	  case 4:  LCD_Write_Data(0x04+0x30);statevalue=0x04+0x30;break;
          case 5:  LCD_Write_Data(0x05+0x30);statevalue=0x05+0x30;break;
          case 6:  LCD_Write_Data(0x06+0x30);statevalue=0x06+0x30;break;
          case 7:  LCD_Write_Data(0x07+0x30);statevalue=0x07+0x30;break;
          case 8:  LCD_Write_Data(0x08+0x30);statevalue=0x08+0x30;break;
	  case 9:  LCD_Write_Data(0x09+0x30);statevalue=0x09+0x30;break;
          case 10: LCD_Write_Data(0x11+0x30);statevalue=0x11+0x30;break;
	  case 11: LCD_Write_Data(0x12+0x30);statevalue=0x12+0x30;break;
	  case 12: LCD_Write_Data(0x13+0x30);statevalue=0x13+0x30;break;
	  case 13: LCD_Write_Data(0x14+0x30);statevalue=0x14+0x30;break;
	  case 14: LCD_Write_Data(0x15+0x30);statevalue=0x15+0x30;break;
          case 15: LCD_Write_Data(0x16+0x30);statevalue=0x16+0x30;break;
         }
      
      }
  else
   {
      LCD_Write_Com(0x99);
      Write_Data(statevalue);              //没有按键的时候显示上次的键值
   } 
      P1IFG=0;
      P1OUT=0xf0;    //很关键的设置，注意理解(准备可中断的条件)
}
*/
