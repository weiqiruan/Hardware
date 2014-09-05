#include "AD7799.h"

unsigned char Channel,Gain,FS;      //ȫ�����ñ���

void delay_AD(int num)
{
  int i;
  for (i= 0; i < num ; i++)
    _NOP();
}

/***************************************
��������AD7799_WriteByte
��  �ܣ�AD7799д�Ĵ������� 
��  ����Ҫд������
����ֵ����
***************************************/
void AD7799_WriteByte(unsigned char WriteData)   
{ 
  unsigned char i;   
  AD7799_CS0;
  for(i = 0 ; i < 8 ;i++)   
  {   
    delay_AD(5);   
    AD7799_SCLK0 ;               
    delay_AD(5); 
    if (WriteData & 0x80)
      AD7799_IN1;   
    else 
      AD7799_IN0;   
    WriteData = WriteData << 1 ;    
    delay_AD(5);  
    AD7799_SCLK1 ;   
    delay_AD(5);  
  }   
  AD7799_CS1; 
}   
   
/***************************************
��������AD7799_ReadByte
��  �ܣ�AD7799���Ĵ������� 
��  ������
����ֵ���Ĵ���ֵ
***************************************/   
unsigned char AD7799_ReadByte(void)   
{   
  unsigned char i ;   
  unsigned char ReadData ;   
  AD7799_CS0;   
  ReadData = 0 ;   
  for(i = 0 ; i < 8 ; i++)   
  {   
    delay_AD(5);  
    AD7799_SCLK0;  
    delay_AD(5);
    ReadData = ReadData << 1;   
    ReadData |= (ADPort & AD7799_DOUT)>>ADPinShift;
    delay_AD(5);
    AD7799_SCLK1;  
    delay_AD(5);
  }   
  //AD7799_DOUT1;  
  AD7799_CS1;   
  return(ReadData);   
}   

/***************************************
��������ReadAd7799ConversionData
��  �ܣ�AD7799��ȡֵ 
��  ������
����ֵ��ADC��ֵ
***************************************/ 
unsigned long ReadAd7799ConversionData(void)   
{ 
  unsigned long ConverData ;
  unsigned char ADSAT ; 
  unsigned char ErrNUM=0;
  WaitRDY();              //�ȴ�����READY
  AD7799_WriteByte(0x40);  //0100 0000 ������һ����Ϊ����״̬�Ĵ���
  ADSAT = AD7799_ReadByte();   //����״̬ 8λ
  while((ADSAT&0x40)||(!(ADSAT&0x08)))    //������߶�д�쳣    
  {
    AD7799_WriteByte(0xff);         //��λ  
    AD7799_WriteByte(0xff);   
    AD7799_WriteByte(0xff);  
    AD7799_WriteByte(0xff);   
    AD7799_Config(Channel,Gain,FS);        //��ʼ��  
    WaitRDY();                     //��״̬   
    AD7799_WriteByte(0x40);        //��һ��������״̬   
    ADSAT = AD7799_ReadByte();     //��״̬    
    
    ErrNUM++;  
    if(ErrNUM>5)return(0xffffff);//if(ErrNUM>5)break;              //����5�ζ������� 
  }

  AD7799_WriteByte(0x58);  //0101 1000 ������һ�����������ݼĴ�����000 
  
  if(!WaitRDY())
  {
    ConverData = 0 ; 
    ConverData = AD7799_ReadByte(); 
    ConverData = ConverData<<8 ;
    ConverData = AD7799_ReadByte() + ConverData;
    ConverData = ConverData<<8 ;
    ConverData = AD7799_ReadByte() + ConverData;
  } 

  return(ConverData);   
}  

/***************************************
��������AD7799_Config
��  �ܣ�����AD7799
��  ����ͨ����(0,1,2)������(1,2,4,8,16,32,64,128)
����ֵ����
***************************************/
void AD7799_Config(unsigned char Config_Reg_Channel,unsigned char Config_Reg_Gain,unsigned char Mode_reg_FS)
{
  AD7799_WriteByte(0xff);   
  AD7799_WriteByte(0xff);   
  AD7799_WriteByte(0xff);   
  AD7799_WriteByte(0xff);
  
  AD7799_WriteByte(Config_Reg);                                                        //0x10
  AD7799_WriteByte(Config_Reg_BO_1 + Config_Reg_UB_1 + Config_Reg_Gain);              //������Դ100nA + ���뵥���� + �� ��
  AD7799_WriteByte(Config_Reg_REF_DET_0 + Config_Reg_Buf_0 + Config_Reg_Channel);     //���ο� + ������ + ͨ���� 
  
  AD7799_WriteByte(Mode_reg);
  AD7799_WriteByte(0x80);       //��̶�У׼
  AD7799_WriteByte(0x00);
  delay_us(200);
  
  AD7799_WriteByte(Mode_reg);
  AD7799_WriteByte(0xa0);       //������У׼
  AD7799_WriteByte(0x00);
  delay_us(200);
  
  AD7799_WriteByte(Mode_reg);
  AD7799_WriteByte(Mode_reg_MD_0+ Mode_reg_PSW_1);   //
  AD7799_WriteByte(Mode_reg_FS);
  delay_ms(50);     //��ʱ�ȴ��Ĵ���,���˲������仯����
}

/***************************************
��������WaitRDY
��  �ܣ�AD7799�ж�æ 
��  ������
����ֵ��1--æ��0--����
***************************************/ 
char WaitRDY(void)   
{   
  unsigned int iint ;  
  iint = 0 ;
  AD7799_CS0;
  while((ADPort&AD7799_RDY)==(unsigned char)AD7799_RDY)   
  {   
    iint++;   
    if(iint>65530)   
    {   
      AD7799_WriteByte(0xff);   
      AD7799_WriteByte(0xff);   
      AD7799_WriteByte(0xff);   
      AD7799_WriteByte(0xff);  
      AD7799_Config(Channel,Gain,FS);  
      return 1;
    }   
  }
  AD7799_CS1;  
  return 0;
} 

/****************************************************************
��������ADC24_Init
��  �ܣ�ADC7799�ܽ����ü���ʼ����Ĭ��ͨ��1����Ϊ1
��  ������
����ֵ����
****************************************************************/
void ADC24_Init()
{
  ADPortDIR |= AD7799_SCLK+AD7799_CS+AD7799_DIN;
  ADPortDIR &=~ AD7799_DOUT;
  Channel = 0;
  Gain = 0;
  FS = Mode_reg_FS_4;
  AD7799_Config(Channel,Gain,FS);
}


/****************************************************************
��������GetADCHn
��  �ܣ���ȡADC��ͨ��
��  ����ͨ������0,1,2��
����ֵ��������ѹ
****************************************************************/
unsigned int GetADCHn(unsigned char Channeln)
{
  unsigned int ADCValue;
  if(Channeln!=Channel)
  {
    Channel = Channeln;
    AD7799_WriteByte(Config_Reg);                                            //0x10
    AD7799_WriteByte(Config_Reg_BO_1 + Config_Reg_UB_1 + Gain);              //������Դ100nA + ���뵥���� + �� ��
    AD7799_WriteByte(Config_Reg_REF_DET_0 + Config_Reg_Buf_0 + Channel);     //���ο� + ������ + ͨ����
    delay_ms(50);     //�л�ͨ��ʱ��Ҫ��ʱ�ȴ��Ĵ���
  }
  ADCValue = (unsigned int)(3300.0*(float)ReadAd7799ConversionData()/16777215.0);
  return(ADCValue);
}

