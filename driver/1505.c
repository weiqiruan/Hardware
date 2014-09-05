/*
 * ILI9325.c
 *
 *  Created on: 2014��8��20��
 *      Author: dell
 */
#include "1505.h"
#include "font.h"

//LCD�Ļ�����ɫ�ͱ���ɫ	   
u16 POINT_COLOR=0x0000;	//������ɫ
u16 BACK_COLOR=0xFFFF;  //����ɫ 

//����LCD��Ҫ����
//Ĭ��Ϊ����
_lcd_dev lcddev;

//�˿ڳ�ʼ������
void InitPort(void)
{
  P2DIR=0XFF;    //�������ţ�������򣬲�ͬӲ������ģ����ͷ�ļ����ŷ���
  P3DIR=0XFF;    //�������ţ�������򣬲�ͬӲ������ģ����ͷ�ļ����ŷ���
}

//=============================================================
//TFT��ʼ��
//���ܣ��ú������ڳ�ʼ��TFT����ģ�顣
//��ڲ���: ��
//���ڲ���: ��
void R61505_init(void)
{
  InitPort();
  TFT_CS_1;
  delay_ms(5);
  //TFT_RST_0;
  //delay_ms(5);
  //TFT_RST_1;
  //delay_ms(20);//�����ʵ�������ʱ�������ȶ���ʾ
  //write_cmd_data(0x0001,0x0100); //Դɨ������SS=0, ������ƶ������Ǵ�S1��S720
  //��Դɨ������SS=1, ������ƶ������Ǵ�S720��S1��
  //SM=0������ɨ�裨SM=1������ɨ�裩
  
  
  // second release on 3/5  ,luminance is acceptable,water wave appear during camera preview
	write_cmd_data(0x0007,0x0000);
	delay_ms(50);
	write_cmd_data(0x0012,0x011C);//0x011A   why need to set several times?
	write_cmd_data(0x00A4,0x0001);//NVM
	write_cmd_data(0x0008,0x000F);
	write_cmd_data(0x000A,0x0008);
	write_cmd_data(0x000D,0x0008);
	//٤��У��
	write_cmd_data(0x0030,0x0707);
	write_cmd_data(0x0031,0x0007); //0x0707
	write_cmd_data(0x0032,0x0603);
	write_cmd_data(0x0033,0x0700);
	write_cmd_data(0x0034,0x0202);
	write_cmd_data(0x0035,0x0002); //?0x0606
	write_cmd_data(0x0036,0x1F0F);
	write_cmd_data(0x0037,0x0707); //0x0f0f  0x0105
	write_cmd_data(0x0038,0x0000);
	write_cmd_data(0x0039,0x0000);
	write_cmd_data(0x003A,0x0707);
	write_cmd_data(0x003B,0x0000); //0x0303
	write_cmd_data(0x003C,0x0007); //?0x0707
	write_cmd_data(0x003D,0x0000); //0x1313//0x1f08
	delay_ms(50);
	write_cmd_data(0x0007,0x0001);
	write_cmd_data(0x0017,0x0001);//������Դ
	delay_ms(50);
	//��Դ����
	write_cmd_data(0x0010,0x17A0);
	write_cmd_data(0x0011,0x0217);//reference voltage VC[2:0]   Vciout = 1.00*Vcivl
	write_cmd_data(0x0012,0x011E);//0x011c  //Vreg1out = Vcilvl*1.80   is it the same as Vgama1out ?
	write_cmd_data(0x0013,0x0F00);//VDV[4:0]-->VCOM Amplitude VcomL = VcomH - Vcom Ampl
	write_cmd_data(0x002A,0x0000);
	write_cmd_data(0x0029,0x000A);//0x0001F  Vcomh = VCM1[4:0]*Vreg1out    gate source voltage??
	write_cmd_data(0x0012,0x013E);// 0x013C  power supply on
	//Coordinates Control//
	write_cmd_data(0x0050,0x0000);//0x0e00
	write_cmd_data(0x0051,0x00EF);
	write_cmd_data(0x0052,0x0000);
	write_cmd_data(0x0053,0x013F);
	//Pannel Image Control//
	write_cmd_data(0x0060,0x2700);
	write_cmd_data(0x0061,0x0001);
	write_cmd_data(0x006A,0x0000);
	write_cmd_data(0x0080,0x0000);
	//Partial Image Control//
	write_cmd_data(0x0081,0x0000);
	write_cmd_data(0x0082,0x0000);
	write_cmd_data(0x0083,0x0000);
	write_cmd_data(0x0084,0x0000);
	write_cmd_data(0x0085,0x0000);
	//Panel Interface Control//
	write_cmd_data(0x0090,0x0013);//0x0010 frenqucy
	write_cmd_data(0x0092,0x0300);
	write_cmd_data(0x0093,0x0005);
	write_cmd_data(0x0095,0x0000);
	write_cmd_data(0x0097,0x0000);
	write_cmd_data(0x0098,0x0000);

	write_cmd_data(0x0001,0x0100);
	write_cmd_data(0x0002,0x0700);
	write_cmd_data(0x0003,0x1038);//ɨ�跽�� ��->��  ��->��
	write_cmd_data(0x0004,0x0000);
	write_cmd_data(0x000C,0x0000);
	write_cmd_data(0x000F,0x0000);
	write_cmd_data(0x0020,0x0000);
	write_cmd_data(0x0021,0x0000);
	write_cmd_data(0x0007,0x0021);
	delay_ms(20);
	write_cmd_data(0x0007,0x0061);
	delay_ms(20);
	write_cmd_data(0x0007,0x0173);
	delay_ms(20);

	//Ĭ������
	LCD_Display_Dir(0);		 	//Ĭ��Ϊ����
	//LCD_LED_1;					//��������
	LCD_Clear(WHITE);
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
		LCD_WriteRAM(color);	   
	}
} 

//��ʼдGRAM
void LCD_WriteRAM_Prepare(void)
{
 	write_cmd_u16(lcddev.wramcmd);	  
}	 
//LCDдGRAM
//RGB_Code:��ɫֵ
void LCD_WriteRAM(u16 RGB_Code)
{							    
	write_data_u16(RGB_Code);//дʮ��λGRAM
}

//LCD������ʾ
void LCD_DisplayOn(void)
{					   
    write_cmd_data(R7,0x0173); 			//������ʾ
}	 
//LCD�ر���ʾ
void LCD_DisplayOff(void)
{	   
    write_cmd_data(R7,0x0);//�ر���ʾ 
}   
//���ù��λ��
//Xpos:������
//Ypos:������
void LCD_SetCursor(u16 Xpos, u16 Ypos)
{
	if(lcddev.dir==1)Xpos=lcddev.width-1-Xpos;//������ʵ���ǵ�תx,y����
	write_cmd_data(lcddev.setxcmd, Xpos);
	write_cmd_data(lcddev.setycmd, Ypos);
 
} 

//����LCD���Զ�ɨ�跽��
//һ������ΪL2R_U2D����,�������Ϊ����ɨ�跽ʽ,���ܵ�����ʾ������.
//dir:0~7,����8������(���嶨���lcd.h)   	   
void LCD_Scan_Dir(u8 dir)
{
	u16 regval=0;
	u8 dirreg=0;
	if(lcddev.dir==1)//����ʱ
	{			   
		switch(dir)//����ת��
		{
			case 0:dir=6;break;
			case 1:dir=7;break;
			case 2:dir=4;break;
			case 3:dir=5;break;
			case 4:dir=1;break;
			case 5:dir=0;break;
			case 6:dir=3;break;
			case 7:dir=2;break;	     
		}
	}

	switch(dir)
	{
		case L2R_U2D://������,���ϵ���
			regval|=(1<<5)|(1<<4)|(0<<3);
			break;
		case L2R_D2U://������,���µ���
			regval|=(0<<5)|(1<<4)|(0<<3);
			break;
		case R2L_U2D://���ҵ���,���ϵ���
			regval|=(1<<5)|(0<<4)|(0<<3);
			break;
		case R2L_D2U://���ҵ���,���µ���
			regval|=(0<<5)|(0<<4)|(0<<3);
			break;
		case U2D_L2R://���ϵ���,������
			regval|=(1<<5)|(1<<4)|(1<<3);
			break;
		case U2D_R2L://���ϵ���,���ҵ���
			regval|=(1<<5)|(0<<4)|(1<<3);
			break;
		case D2U_L2R://���µ���,������
			regval|=(0<<5)|(1<<4)|(1<<3);
			break;
		case D2U_R2L://���µ���,���ҵ���
			regval|=(0<<5)|(0<<4)|(1<<3);
			break;
	}

	dirreg=0X03;
	regval|=1<<12;

	write_cmd_data(dirreg,regval);
	
}  

//����LCD��ʾ����
//dir:0,������1,����
void LCD_Display_Dir(u8 dir)
{
	if(dir==0)//����
	{
		lcddev.dir=0;//����
		lcddev.width=240;
		lcddev.height=320;

		lcddev.wramcmd=R34;
		lcddev.setxcmd=R32;
		lcddev.setycmd=R33;
                
	}else if(dir==1)
	{ 
		
          
		lcddev.dir=1;//����
		lcddev.width=320;
		lcddev.height=240;

		lcddev.wramcmd=R34;
		lcddev.setxcmd=R33;
		lcddev.setycmd=R32;
	} 
	LCD_Scan_Dir(DFT_SCAN_DIR);	//Ĭ��ɨ�跽��
}


//����
//x,y:����
//POINT_COLOR:�˵����ɫ
void LCD_DrawPoint(u16 x,u16 y)
{
	LCD_SetCursor(x,y);		//���ù��λ�� 
	LCD_WriteRAM_Prepare();	//��ʼд��GRAM
	LCD_WriteRAM(POINT_COLOR); 
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
			LCD_WriteRAM(color);	//���ù��λ��
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
			LCD_WriteRAM(color[i*height+j]);//д������
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
void Draw_Circle(u16 x0,u16 y0,u8 r)
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
			if(size==12) temp=asc2_1206[num][t];  //����1206����
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
			if(size==12) temp=asc2_1206[num][t];  //����1206����
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



//=============================================================
//д����
//���ܣ�дһ�����TFT��������
//��ڲ���: CH,CL�ֱ�Ϊ�����ֵĸ�8λ�͵�8λ��ָ��Ҫִ�е����
//���ڲ���: ��
void write_cmd(unsigned char CH,unsigned char CL)
{
    TFT_RD_1;// ���������"0"Ϊ����Ч��дʱһֱΪ�ߵ�ƽ
	TFT_CS_0; //�͵�ƽ��Ч��Ƭѡ
	TFT_RS_0;//"0"Ϊ���"1"Ϊ����
	TFT_DATA=CH;//����������8λ
	TFT_WR_0;//�͵�ƽ���ߵ�ƽ�仯��������
	TFT_WR_1;//
	TFT_DATA=CL;//����������8λ
	TFT_WR_0;
	TFT_WR_1;//�͵�ƽ���ߵ�ƽ�仯������ݴ���
	TFT_CS_1;//�ߵ�ƽ��Ч������Ƭѡ
}

//=============================================================
//д16λ����
//���ܣ�дһ�����TFT��������
//��ڲ���: CH,CL�ֱ�Ϊ�����ֵĸ�8λ�͵�8λ��ָ��Ҫִ�е����
//���ڲ���: ��
void write_cmd_u16(unsigned int cmd)
{
	unsigned char m,n;  
	m=cmd>>8;   //��8λ������8λ
	n=cmd;     //��8λ
	write_cmd(m,n);
}

//===================================================================
//д����
//���ܣ�дһ�����ݵ�TFT��������
//��ڲ���: DH,DL�ֱ�Ϊ�����ֵĸ�8λ�͵�8λ��ָ��Ҫд������ݡ�
//���ڲ���: ��
void write_data(unsigned char DH,unsigned char DL)
{
	TFT_RD_1;  // ���������"0"Ϊ����Ч
        TFT_CS_0;  //�͵�ƽ��Ч��Ƭѡ
	TFT_RS_1;  //"1"Ϊ���ݣ�"0"Ϊ����
	TFT_DATA=DH;  //��������ݣ���8λ
	TFT_WR_0;  // �͵�ƽ
	TFT_WR_1;  //�仯���ߵ�ƽ����������
	TFT_DATA=DL;  //��������ݣ���8λ
	TFT_WR_0;  // �͵�ƽ
	TFT_WR_1; // �͵�ƽ
	TFT_CS_1;//�ߵ�ƽ��Ч������Ƭѡ
}


//==============================================================
//д16λ����
//���ܣ� дһ�����ݵ�TFT��������
//��ڲ���: datΪҪд���16λ���ݡ�
//���ڲ���: ��
void write_data_u16(unsigned int dat)
{
	unsigned char m,n;  
	m=dat>>8;   //��8λ������8λ
	n=dat;     //��8λ
	write_data(m,n);
}


//===============================================================
//д��������
//���ܣ� д�������ݵ�TFT��������
//��ڲ���: cmdΪҪд��������ֽڣ�datΪҪд���16λ���ݡ�
//���ڲ���: ��
void write_cmd_data (unsigned char cmd,unsigned int dat)
{
	unsigned char m,n;
	m=dat>>8;
	n=dat;
	write_cmd(0x00,cmd);  //д�������ĸ�8λΪ0x00,���ֽ�Ϊ����
	write_data(m,n); //д��������
}

