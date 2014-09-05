#include "AD7799.h"

unsigned char Channel,Gain,FS;      //全局设置变量

void delay_AD(int num)
{
  int i;
  for (i= 0; i < num ; i++)
    _NOP();
}

/***************************************
函数名：AD7799_WriteByte
功  能：AD7799写寄存器函数 
参  数：要写的数据
返回值：无
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
函数名：AD7799_ReadByte
功  能：AD7799读寄存器函数 
参  数：无
返回值：寄存器值
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
函数名：ReadAd7799ConversionData
功  能：AD7799读取值 
参  数：无
返回值：ADC的值
***************************************/ 
unsigned long ReadAd7799ConversionData(void)   
{ 
  unsigned long ConverData ;
  unsigned char ADSAT ; 
  unsigned char ErrNUM=0;
  WaitRDY();              //等待数据READY
  AD7799_WriteByte(0x40);  //0100 0000 配置下一操作为：读状态寄存器
  ADSAT = AD7799_ReadByte();   //读出状态 8位
  while((ADSAT&0x40)||(!(ADSAT&0x08)))    //出错或者读写异常    
  {
    AD7799_WriteByte(0xff);         //复位  
    AD7799_WriteByte(0xff);   
    AD7799_WriteByte(0xff);  
    AD7799_WriteByte(0xff);   
    AD7799_Config(Channel,Gain,FS);        //初始化  
    WaitRDY();                     //读状态   
    AD7799_WriteByte(0x40);        //下一操作：读状态   
    ADSAT = AD7799_ReadByte();     //读状态    
    
    ErrNUM++;  
    if(ErrNUM>5)return(0xffffff);//if(ErrNUM>5)break;              //连续5次都读出错 
  }

  AD7799_WriteByte(0x58);  //0101 1000 配置下一操作：读数据寄存器，000 
  
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
函数名：AD7799_Config
功  能：配置AD7799
参  数：通道号(0,1,2)，增益(1,2,4,8,16,32,64,128)
返回值：无
***************************************/
void AD7799_Config(unsigned char Config_Reg_Channel,unsigned char Config_Reg_Gain,unsigned char Mode_reg_FS)
{
  AD7799_WriteByte(0xff);   
  AD7799_WriteByte(0xff);   
  AD7799_WriteByte(0xff);   
  AD7799_WriteByte(0xff);
  
  AD7799_WriteByte(Config_Reg);                                                        //0x10
  AD7799_WriteByte(Config_Reg_BO_1 + Config_Reg_UB_1 + Config_Reg_Gain);              //开电流源100nA + 输入单极性 + 增 益
  AD7799_WriteByte(Config_Reg_REF_DET_0 + Config_Reg_Buf_0 + Config_Reg_Channel);     //检测参考 + 开缓冲 + 通道号 
  
  AD7799_WriteByte(Mode_reg);
  AD7799_WriteByte(0x80);       //零刻度校准
  AD7799_WriteByte(0x00);
  delay_us(200);
  
  AD7799_WriteByte(Mode_reg);
  AD7799_WriteByte(0xa0);       //满量程校准
  AD7799_WriteByte(0x00);
  delay_us(200);
  
  AD7799_WriteByte(Mode_reg);
  AD7799_WriteByte(Mode_reg_MD_0+ Mode_reg_PSW_1);   //
  AD7799_WriteByte(Mode_reg_FS);
  delay_ms(50);     //延时等待寄存器,随滤波阶数变化调整
}

/***************************************
函数名：WaitRDY
功  能：AD7799判断忙 
参  数：无
返回值：1--忙，0--正常
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
函数名：ADC24_Init
功  能：ADC7799管脚设置及初始化，默认通道1增益为1
参  数：无
返回值：无
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
函数名：GetADCHn
功  能：读取ADC各通道
参  数：通道数（0,1,2）
返回值：测量电压
****************************************************************/
unsigned int GetADCHn(unsigned char Channeln)
{
  unsigned int ADCValue;
  if(Channeln!=Channel)
  {
    Channel = Channeln;
    AD7799_WriteByte(Config_Reg);                                            //0x10
    AD7799_WriteByte(Config_Reg_BO_1 + Config_Reg_UB_1 + Gain);              //开电流源100nA + 输入单极性 + 增 益
    AD7799_WriteByte(Config_Reg_REF_DET_0 + Config_Reg_Buf_0 + Channel);     //检测参考 + 开缓冲 + 通道号
    delay_ms(50);     //切换通道时需要延时等待寄存器
  }
  ADCValue = (unsigned int)(3300.0*(float)ReadAd7799ConversionData()/16777215.0);
  return(ADCValue);
}

