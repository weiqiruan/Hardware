#ifndef _AD7799_H__
#define _AD7799_H__

#include <msp430.h>
#include "system.h"
#include "type.h"

extern unsigned char Channel,Gain,FS;      //全局设置变量

#define ADPortDIR    P2DIR
#define ADPort       P2IN
#define ADPortOut    P2OUT
#define AD7799_SCLK  BIT4
#define AD7799_CS    BIT5
#define AD7799_DIN   BIT6
#define AD7799_DOUT  BIT7
#define AD7799_RDY   BIT7
#define ADPinShift   7      //注意修改与DOUT一致

#define AD7799_CS0       ADPortOut &= ~AD7799_CS
#define AD7799_CS1       ADPortOut |=  AD7799_CS
#define AD7799_SCLK0     ADPortOut &= ~AD7799_SCLK 
#define AD7799_SCLK1     ADPortOut |=  AD7799_SCLK 
#define AD7799_IN0       ADPortOut &= ~AD7799_DIN
#define AD7799_IN1       ADPortOut |=  AD7799_DIN

/*******************************************
 AD7799寄存器定义
*******************************************/
#define  Mode_reg     0x08     //Mode register
#define  Config_Reg   0x10     //CONFIGURATION Register
#define  Data_reg     0x58     //Data register

#define  Config_Reg_BO_1        0x20   //开电流源100nA 
#define  Config_Reg_BO_0        0x00   //关电流源100nA 

#define  Config_Reg_UB_1        0x10   //输入单极性
#define  Config_Reg_UB_0        0x00   //输入双极性 

#define  Config_Reg_Gain_1    0x00     //Gain = 1 (in-amp not used)
#define  Config_Reg_Gain_2    0x01     //Gain = 2 (in-amp not used)
#define  Config_Reg_Gain_4    0x02     //Gain = 4 
#define  Config_Reg_Gain_8    0x03     //Gain = 8 
#define  Config_Reg_Gain_16   0x04     //Gain = 16
#define  Config_Reg_Gain_32   0x05     //Gain = 32
#define  Config_Reg_Gain_64   0x06     //Gain = 64
#define  Config_Reg_Gain_128  0x07     //Gain = 128

#define  Config_Reg_REF_DET_1   0x20     //检测参考
#define  Config_Reg_REF_DET_0   0x00     //不检测参考

#define  Config_Reg_Buf_1       0x10     //开缓冲
#define  Config_Reg_Buf_0       0x00     //关缓冲

#define  Config_Reg_Channel1   0x00     //AIN1(+) – AIN1(–)
#define  Config_Reg_Channel2   0x01     //AIN2(+) – AIN2(–)
#define  Config_Reg_Channel3   0x02     //AIN3(+) – AIN3(–)

#define  Mode_reg_MD_0        0x00     //方式0，Continuous-Conversion Mode
#define  Mode_reg_MD_1        0x20     //方式1，Single-Conversion Mode
#define  Mode_reg_MD_2        0x40     //方式2, Idle Mode. 
#define  Mode_reg_MD_3        0x60     //方式3, Power-Down Mode
#define  Mode_reg_MD_4        0x80     //方式4, Internal Zero-Scale Calibration.
#define  Mode_reg_MD_5        0xA0     //方式5, Internal Full-Scale Calibration.
#define  Mode_reg_MD_6        0xC0     //方式6, System Zero-Scale Calibration.
#define  Mode_reg_MD_7        0xE0     //方式7, System Full-Scale Calibration

#define  Mode_reg_PSW_1       0x10     //Power Switch Control Bit. =1 set
#define  Mode_reg_PSW_0       0x00     //Power Switch Control Bit. =0 not set

#define  Mode_reg_FS_0        0x00
#define  Mode_reg_FS_1        0x01
#define  Mode_reg_FS_2        0x02
#define  Mode_reg_FS_3        0x03
#define  Mode_reg_FS_4        0x04
#define  Mode_reg_FS_5        0x05
#define  Mode_reg_FS_6        0x06
#define  Mode_reg_FS_7        0x07
#define  Mode_reg_FS_8        0x08
#define  Mode_reg_FS_9        0x09
#define  Mode_reg_FS_10       0x0A
#define  Mode_reg_FS_11       0x0B
#define  Mode_reg_FS_12       0x0B
#define  Mode_reg_FS_13       0x0D
#define  Mode_reg_FS_14       0x0E
#define  Mode_reg_FS_15       0x0F

void AD7799_WriteByte(unsigned char WriteData);
unsigned char AD7799_ReadByte(void);
unsigned long ReadAd7799ConversionData(void);
void AD7799_Config(unsigned char Config_Reg_Channel,unsigned char Config_Reg_Gain,unsigned char Mode_reg_FS);
char WaitRDY(void);
void ADC24_Init(void);
unsigned int GetADCHn(unsigned char Channeln);

#endif  //_AD7799_H__
