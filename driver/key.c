#include "key.h"

uchar key = 0xFF;

/****************************************************************************
* ��    �ƣ�Key_Port_Init
* ��    �ܣ�Key�˿ڳ�ʼ��
* ��ڲ�������
* ���ڲ�������
****************************************************************************/
void Key_Port_Init(void)
{
    P1SEL = 0x00;                        //���̵�IO������Ϊ��ͨI/Oģʽ���˾��ʡ      
    P1DIR = 0x0F;                        //����λ����ģʽ������λ���ģʽ���ⲿ��������
    P1OUT = 0xF0;                        //��ʼֵ0xF0��ΪʲôҪ������׼�����жϵ�����
    P1IES = 0xf0;                        //P1.4~P1.7�½��ش����ж�
    P1IE  = 0xf0;                        //P1.4~P1.7�����ж�
    P1IFG = 0x00;                        //�жϱ�־��0
	_EINT();
}

/****************************************************************************
* ��    �ƣ�Key_Scan
* ��    �ܣ�Keyɨ��
* ��ڲ�������
* ���ڲ�����key--�������λ��
****************************************************************************/
uchar Key_Scan(void)
{
  uchar key_checkin,key_check;
  
   P1OUT=0xf0;                       //��IO���������ʽ��Ϊ���뷽ʽʱҪ�ӳ�һ������
   P1OUT=0xf0;                       //��ȡд���εķ�����ʱ
   key_checkin=P1IN; 
  // while(P1IN!=0xf0);                                     //��ȡIO��״̬���ж��Ƿ��м�����
  if(key_checkin!=0xf0)               //IO��ֵ�����仯���ʾ�м�����
  {
    delay_ms(20);                     //������������ʱ20MS
    key_checkin=P1IN;
    if(key_checkin!=0xf0)
    {
      
      P1OUT=0xfe;                    //����ɨ�迪ʼ��1111 1110
      P1OUT=0xfe;                    //д������ʱ����ͬ
      key_check=P1IN;
      switch (key_check)               //���ݶ�����IO��ֵ�жϰ����İ������
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
  {                    //�ް���ʱ����ֵ
    key=0xFF;
  }
    return(key);
}

/*
	//�˿�1�����ж�
#pragma vector=PORT1_VECTOR
__interrupt void Port(void)
{
  if((P1IFG&0xf0)!=0)
  {  
    Key_Scan();                     //����ɨ�裬���Ƿ��а������� 
  }
   if(key!=0xff)                   //����а������£�����ʾ�ð�����ֵ0~15
       {
          LCD_Write_Com(0x99);
          switch(key)
        {
          case 0:  LCD_Write_Data(0x00+0x30);statevalue=0x00+0x30;break;   //���ݼ�ֵ��ʾ��Һ����
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
      Write_Data(statevalue);              //û�а�����ʱ����ʾ�ϴεļ�ֵ
   } 
      P1IFG=0;
      P1OUT=0xf0;    //�ܹؼ������ã�ע�����(׼�����жϵ�����)
}
*/
