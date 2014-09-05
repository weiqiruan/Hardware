/*
 * ssd1963.c
 *
 *  Created on: 2014��8��20��
 *      Author: dell
 */
#include "ssd1963.h"
#include "font.h"

//LCD�Ļ�����ɫ�ͱ���ɫ	   
u16 POINT_COLOR=0x0000;	//������ɫ
u16 BACK_COLOR=0xFFFF;  //����ɫ 

//����LCD��Ҫ����
//Ĭ��Ϊ����
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
	
		   
//д�Ĵ�������
//regval:�Ĵ���ֵ
void LCD_WR_REG(u16 regval)
{ 
	u8 CH=0,CL=0;
	CH = regval>>8;
	CL = regval;
	TFT_RD_1;// ���������"0"Ϊ����Ч��дʱһֱΪ�ߵ�ƽ
	TFT_CS_0; //�͵�ƽ��Ч��Ƭѡ
	TFT_RS_0;//"0"Ϊ���"1"Ϊ����
	TFT_DATA_H=CH;//����������8λ
	TFT_DATA_L=CL;//����������8λ
	TFT_WR_0;
	TFT_WR_1;//�͵�ƽ���ߵ�ƽ�仯������ݴ���
	TFT_CS_1;//�ߵ�ƽ��Ч������Ƭѡ
}
//дLCD����
//data:Ҫд���ֵ
void LCD_WR_DATA(u16 data)
{										    	   
	u8 CH=0,CL=0;
	CH = data>>8;
	CL = data;
	TFT_RD_1;// ���������"0"Ϊ����Ч��дʱһֱΪ�ߵ�ƽ
	TFT_CS_0; //�͵�ƽ��Ч��Ƭѡ
	TFT_RS_1;//"0"Ϊ���"1"Ϊ����
	TFT_DATA_H=CH;//����������8λ
	TFT_DATA_L=CL;//����������8λ
	TFT_WR_0;
	TFT_WR_1;//�͵�ƽ���ߵ�ƽ�仯������ݴ���
	TFT_CS_1;//�ߵ�ƽ��Ч������Ƭѡ
}					   
//д�Ĵ���
//LCD_Reg:�Ĵ�����ַ
//LCD_RegValue:Ҫд�������
void LCD_WriteReg(u8 LCD_Reg, u16 LCD_RegValue)
{	
	LCD_WR_REG(LCD_Reg);		//д��Ҫд�ļĴ������
	LCD_WR_DATA(LCD_RegValue);  //д������
}
//��ʼдGRAM
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
  LCD_Display_Dir(1);		 	//Ĭ��Ϊ����
	//LCD_LED=1;							//��������
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


//����LCD��ʾ����
//dir:0,������1,����
void LCD_Display_Dir(u8 dir)
{
	if(dir==1)
	{
		LCD_WR_REG(0x0036); //rotation
		LCD_WR_DATA(0x0000);
		
		lcddev.dir=1;//����
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
		
		lcddev.dir=0;//����
		lcddev.width=480;
		lcddev.height=800;

		lcddev.wramcmd=0x2C;
		lcddev.setxcmd=0X2B;
		lcddev.setycmd=0X2A;  
			
	} 
	
}	 


//��������
//color:Ҫ���������ɫ
void LCD_Clear(u16 color)
{
	u32 index=0;      
	u32 totalpoint=lcddev.width;
	totalpoint*=lcddev.height; 	//�õ��ܵ���
	LCD_SetCursor(0x00,0x0000);	//���ù��λ�� 
	LCD_WriteRAM_Prepare();     //��ʼд��GRAM	 	 
	for(index=0;index<totalpoint;index++)
	{
		LCD_WR_DATA(color);
	}
}    

//���ù��λ��
//Xpos:������
//Ypos:������
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

//LCD������ʾ
void LCD_DisplayOn(void)
{					   
	LCD_WR_REG(0X29);	//������ʾ
}	 
//LCD�ر���ʾ
void LCD_DisplayOff(void)
{	   
	LCD_WR_REG(0X28);	//�ر���ʾ
}  

//����
//x,y:����
//POINT_COLOR:�˵����ɫ
void LCD_DrawPoint(u16 x,u16 y)
{
	LCD_SetCursor(x,y);		//���ù��λ�� 
	LCD_WriteRAM_Prepare();	//��ʼд��GRAM
	LCD_WR_DATA(POINT_COLOR);
}

//��ָ����������䵥����ɫ
//(sx,sy),(ex,ey):�����ζԽ�����,�����СΪ:(ex-sx+1)*(ey-sy+1)   
//color:Ҫ������ɫ
void LCD_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color)
{          
	u16 i,j;
	u16 xlen=0;
	xlen=ex-sx+1;	   
	for(i=sy;i<=ey;i++)
	{
	 	LCD_SetCursor(sx,i);      				//���ù��λ�� 
		LCD_WriteRAM_Prepare();     			//��ʼд��GRAM	  
		for(j=0;j<xlen;j++)
			LCD_WR_DATA(color);	//���ù��λ��
	}
}  
//��ָ�����������ָ����ɫ��			 
//(sx,sy),(ex,ey):�����ζԽ�����,�����СΪ:(ex-sx+1)*(ey-sy+1)   
//color:Ҫ������ɫ
void LCD_Color_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 *color)
{  
	u16 height,width;
	u16 i,j;
	width=ex-sx+1; 		//�õ����Ŀ��
	height=ey-sy+1;		//�߶�
 	for(i=0;i<height;i++)
	{
 		LCD_SetCursor(sx,sy+i);   	//���ù��λ�� 
		LCD_WriteRAM_Prepare();     //��ʼд��GRAM
		for(j=0;j<width;j++)
			LCD_WR_DATA(color[i*height+j]);//д������
	}	  
} 

//����
//x1,y1:�������
//x2,y2:�յ�����  
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	delta_x=x2-x1; //������������ 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //���õ������� 
	else if(delta_x==0)incx=0;//��ֱ�� 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//ˮƽ�� 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //ѡȡ�������������� 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//������� 
	{  
		LCD_DrawPoint(uRow,uCol);//���� 
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

//������	  
//(x1,y1),(x2,y2):���εĶԽ�����
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2)
{
	LCD_DrawLine(x1,y1,x2,y1);
	LCD_DrawLine(x1,y1,x1,y2);
	LCD_DrawLine(x1,y2,x2,y2);
	LCD_DrawLine(x2,y1,x2,y2);
}

//��ָ��λ�û�һ��ָ����С��Բ
//(x,y):���ĵ�
//r    :�뾶
void LCD_DrawCircle(u16 x0,u16 y0,u8 r)
{
	int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);             //�ж��¸���λ�õı�־
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
		//ʹ��Bresenham�㷨��Բ     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 						    
	}
} 

//��ָ��λ����ʾһ���ַ�
//x,y:��ʼ����
//num:Ҫ��ʾ���ַ�:" "--->"~"
//size:�����С 12/16
//mode:���ӷ�ʽ(1)���Ƿǵ��ӷ�ʽ(0)
void LCD_ShowChar(u16 x,u16 y,u8 num,u8 size,u8 mode)
{  							  
    u8 temp,t1,t;
	u16 y0=y;
	u16 colortemp=POINT_COLOR;      			     
	//���ô���		   
	num=num-' ';//�õ�ƫ�ƺ��ֵ
	if(!mode) //�ǵ��ӷ�ʽ
	{
	    for(t=0;t<size;t++)
	    {   
			if(size==12)temp=asc2_1206[num][t];  //����1206����
			else temp=asc2_1608[num][t];		 //����1608���� 	                          
	        for(t1=0;t1<8;t1++)
			{			    
		        if(temp&0x80)POINT_COLOR=colortemp;
				else POINT_COLOR=BACK_COLOR;
				LCD_DrawPoint(x,y);	
				temp<<=1;
				y++;
				if(x>=lcddev.width){POINT_COLOR=colortemp;return;}//��������
				if((y-y0)==size)
				{
					y=y0;
					x++;
					if(x>=lcddev.width){POINT_COLOR=colortemp;return;}//��������
					break;
				}
			}  	 
	    }    
	}else//���ӷ�ʽ
	{
	    for(t=0;t<size;t++)
	    {   
			if(size==12)temp=asc2_1206[num][t];  //����1206����
			else temp=asc2_1608[num][t];		 //����1608���� 	                          
	        for(t1=0;t1<8;t1++)
			{			    
		        if(temp&0x80)LCD_DrawPoint(x,y); 
				temp<<=1;
				y++;
				if(x>=lcddev.height){POINT_COLOR=colortemp;return;}//��������
				if((y-y0)==size)
				{
					y=y0;
					x++;
					if(x>=lcddev.width){POINT_COLOR=colortemp;return;}//��������
					break;
				}
			}  	 
	    }     
	}
	POINT_COLOR=colortemp;	    	   	 	  
} 

//m^n����
//����ֵ:m^n�η�.
u32 LCD_Pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}

//��ʾ����,��λΪ0,����ʾ
//x,y :�������	 
//len :���ֵ�λ��
//size:�����С
//color:��ɫ 
//num:��ֵ(0~4294967295);	 
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
 
//��ʾ����,��λΪ0,������ʾ
//x,y:�������
//num:��ֵ(0~999999999);	 
//len:����(��Ҫ��ʾ��λ��)
//size:�����С
//mode:
//[7]:0,�����;1,���0.
//[6:1]:����
//[0]:0,�ǵ�����ʾ;1,������ʾ.
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
//��ʾ�ַ���
//x,y:�������
//width,height:�����С  
//size:�����С
//*p:�ַ�����ʼ��ַ		  
void LCD_ShowString(u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p)
{         
	u8 x0=x;
	width+=x;
	height+=y;
    while((*p<='~')&&(*p>=' '))//�ж��ǲ��ǷǷ��ַ�!
    {       
        if(x>=width){x=x0;y+=size;}
        if(y>=height)break;//�˳�
        LCD_ShowChar(x,y,*p,size,0);
        x+=size/2;
        p++;
    }  
}

/****************************************************************************
* ��    �ƣ�void GUI_Text(u16 x, u16 y, u8 *str, u16 len)
* ��    �ܣ���ָ��������ʾ�����ı�
* ��ڲ�����x      ������
*           y      ������
*           *str   ���ִ�
*           len    �ִ�����
*           Color  �ַ���ɫ
*           bkColor�ַ�������ɫ
* ���ڲ�������
* ˵    ����һ�����������ַ����ǵ�Ӵ��5������len����10
* ���÷�����GUI_Text(0,0,"�ɶ���һ��",10);
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
			index=(94*(str[b*2] - 0xa1)+(str[b*2+1] - 0xa1));	    //������λ
			for (i=0;i<16;i++)
		    {
			    for (j=0;j<8;j++)
			    {
				    tmp_char=HzLib[index*32+i*2];					//���ֿ�
			        if ( (tmp_char << j) & 0x80)
			        {
								POINT_COLOR = colortemp;
			          LCD_DrawPoint(x_add+j,y_add+i);  // �ַ���ɫ
			        }
			        else
			        {
								POINT_COLOR = BACK_COLOR;
			          LCD_DrawPoint(x_add+j,y_add+i);   // ������ɫ
			        }
			    }
			   	for (j=0;j<8;j++)
			    {
				    tmp_char=HzLib[index*32+i*2+1];				    //���ֿ�
			        if ( (tmp_char << j) & 0x80)
			        {
								POINT_COLOR = colortemp;
			          LCD_DrawPoint(x_add+j+8,y_add+i); // �ַ���ɫ
			        }
			        else
			        {
								POINT_COLOR = BACK_COLOR;
			          LCD_DrawPoint(x_add+j+8,y_add+i);   // ������ɫ
			        }
			    }
		     }
			 x_add=x_add+17;										 //��ַ�ۼ�
	}
	
	POINT_COLOR = colortemp;
}*/
