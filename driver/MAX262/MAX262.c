#include "MAX262.h"


void max262_Init(void)
{
  P1DIR |= BIT0 + BIT1 + BIT2 + BIT3 + BIT4 + BIT5 + BIT6 + BIT7;
}

/***************************************************************    
写两比特数据到某地址
入口参数：A3到A0的地址add,D1和D0的数据dat2bit
D1和D0取dat2bit的最低两位。仅适用于PDL口的八位写操作
***************************************************************/    
void write_1(u8 add,u8 dat2bit)   
{  
   uchar data;
   uchar i; 
     
     data = P1OUT;
     data = (data&0xf0)|(add&0x0f);  //取4位地址
     data = (data&0xcf)|((dat2bit<<4)&0x30);//取两位数据
     P1OUT= data;
     resetWr_1;   
     for(i=0;i<255;i++);     //至少250ns
     setWr_1;   
     for(i=0;i<100;i++);     //至少10ns
}

void write_2(u8 add,u8 dat2bit)   
{  
   uchar data;
   uchar i; 
     
     data = P1OUT;
     data = (data&0xf0)|(add&0x0f);  //取4位地址
     data = (data&0xcf)|((dat2bit<<4)&0x30);//取两位数据
     P1OUT= data;
     resetWr_2;   
     for(i=0;i<255;i++);     //至少250ns
     setWr_2;   
     for(i=0;i<100;i++);     //至少10ns
}


    
//设置A通道的工作方式
void setAmode_1(u8 mode)
{
	write_1(0,mode);
}

void setAmode_2(u8 mode)
{
	write_2(0,mode);
}

//设置A通道的F值  datF范围[0 63]
void setAf_1(u8 datF)   
{   
     write_1(1,datF);   
     datF = datF>>2;   
     write_1(2,datF);   
     datF = datF>>2;   
     write_1(3,datF);   
} 

void setAf_2(u8 datF)   
{   
     write_2(1,datF);   
     datF = datF>>2;   
     write_2(2,datF);   
     datF = datF>>2;   
     write_2(3,datF);   
} 

//设置A通道的Q值  datQ范围[0 127]
void setAQ_1(u8 datQ)   
{   
     write_1(4,datQ);   
     datQ = datQ>>2;   
     write_1(5,datQ);   
     datQ = datQ>>2;   
     write_1(6,datQ);   
     datQ = (datQ>>2)&1;   //多出来的一位置0
     write_1(7,datQ);   
}

void setAQ_2(u8 datQ)   
{   
     write_2(4,datQ);   
     datQ = datQ>>2;   
     write_2(5,datQ);   
     datQ = datQ>>2;   
     write_2(6,datQ);   
     datQ = (datQ>>2)&1;   //多出来的一位置0
     write_2(7,datQ);   
}

//设置B通道的工作方式
void setBmode_1(u8 mode)
{
	write_1(8,mode);
} 

void setBmode_2(u8 mode)
{
	write_2(8,mode);
}  

//设置B通道的F值
void setBf_1(u8 datF)   
{   
     write_1(9,datF);   
     datF = datF>>2;   
     write_1(10,datF);   
     datF = datF>>2;   
     write_1(11,datF);   
}

void setBf_2(u8 datF)   
{   
     write_2(9,datF);   
     datF = datF>>2;   
     write_2(10,datF);   
     datF = datF>>2;   
     write_2(11,datF);   
}

//设置B通道的Q值
void setBQ_1(u8 datQ)   
{   
     write_1(12,datQ);   
     datQ = datQ>>2;   
     write_1(13,datQ);   
     datQ = datQ>>2;   
     write_1(14,datQ);   
     datQ = (datQ>>2)&1;   
     write_1(15,datQ);   

}   

void setBQ_2(u8 datQ)   
{   
     write_2(12,datQ);   
     datQ = datQ>>2;   
     write_2(13,datQ);   
     datQ = datQ>>2;   
     write_2(14,datQ);   
     datQ = (datQ>>2)&1;   
     write_2(15,datQ);   
} 
/*******************************************
入口参数  :mode范围 [0-3],
					fclk单位为HZ,范围[40-4000000],
          f0单位HZ,范围[1-100000]
出口参数  :u8中心频率对应控制字，范围[0-63]
********************************************/
uchar ComputeFN(u8 mode,long int fclk,long int f0)
{
  if(mode==1)
    return ( (uchar)((fclk*2.8284271247/f0)/PI)-26 );
  else
    return ( (uchar)((fclk*2/f0)/PI)-26 );
}
/*******************************************
入口参数  :mode范围 [0-3],
					Q范围[0.5 64]
出口参数  :QN[0-127]
********************************************/
uchar ComputeQN(u8 mode,float Q)
{
  if(mode==1)
    return(uchar)(128-(float)(90.51/Q));
  else
    return (uchar)(128-(float)(64/Q));
}   


//***************************************************************
//根据截至频率fc、品质因数Q和通过模式passMode计算滤波器的中心频率f0
//仅适用于低通和高通   
float lh_Getf0(float fc,float Q,u8 passMode)   
{   
     float temp;
        temp = 1-0.5/pow(Q,2);
		temp = sqrt(temp+sqrt(pow(temp,2)+1));

     if(passMode==lowPass)			//低通中心频率计算公式
      {    
        return  fc/temp;   
      }
     else						    //高通中心频率计算公式
      {
        return  fc*temp;
      }
}

  




//根据fc、Q、passMode、workMode和通道channel设置MAX262的低通和高通的工作数据
// 写入控制字Mode、Q、F=63并返回时钟频率fclk
float lhp_WorkFclk_1(float fc,float Q,u8 passMode,u8 workMode,u8 channel)   
{  
    float f0;
	uchar Qn;
         f0 = lh_Getf0(fc,Q,passMode);//计算出中心频率f0

    if (workMode==modeTwo)   
          Qn = (uchar)(128-90.51/Q);  
    else   
          Qn = (uchar)(128-64/Q);        //计算出QN

    if(passMode==highPass)
	      workMode=modeThree;	 //高通只有工作模式3

    if(channel==channelA)
      {
        write_1(0,workMode);//写控制字M
        setAf_1(63);		//写控制字F=63、要想获得大范围可调,开关电容的采样频率参数取最大值.
        setAQ_1(Qn);    //写控制字Q
      }
    else
      {
        write_1(8,workMode);   //写控制字M
        setBf_1(63);	       //写控制字F=63、要想获得大范围可调,开关电容的采样频率参数取最大值.
        setBQ_1(Qn);				//写控制字Q
      }

    if(workMode==modeTwo)
       return  31.46625*PI*f0;
    else
       return  44.5*PI*f0;
}

float lhp_WorkFclk_2(float fc,float Q,u8 passMode,u8 workMode,u8 channel)   
{  
    float f0;
	uchar Qn;
         f0 = lh_Getf0(fc,Q,passMode);//计算出中心频率f0

    if (workMode==modeTwo)   
          Qn = (uchar)(128-90.51/Q);  
    else   
          Qn = (uchar)(128-64/Q);        //计算出QN

    if(passMode==highPass)
	      workMode=modeThree;	 //高通只有工作模式3

    if(channel==channelA)
      {
        write_2(0,workMode);//写控制字M
        setAf_2(63);		//写控制字F=63、要想获得大范围可调,开关电容的采样频率参数取最大值.
        setAQ_2(Qn);    //写控制字Q
      }
    else
      {
        write_2(8,workMode);   //写控制字M
        setBf_2(63);	       //写控制字F=63、要想获得大范围可调,开关电容的采样频率参数取最大值.
        setBQ_2(Qn);				//写控制字Q
      }

    if(workMode==modeTwo)
       return  31.46625*PI*f0;
    else
       return  44.5*PI*f0;
}

//根据f0、Q、workMode和通道channel设置MAX262的陷波工作数据
//返回时钟频率fclk
float bs_WorkFclk_1(float f0,float Q,uchar workMode,uchar channel)
{
        uchar Qn;
	if (workMode==modeTwo)   
          Qn = (uchar)(128-90.51/Q);  
        else   
          Qn = (uchar)(128-64/Q);

	if(channel==channelA)
        {
          write_1(0,workMode);
          setAf_1(63);		//要想获得大范围可调，开关电容的采样频率参数取最大值.
          setAQ_1(Qn);
        }
        else
        {
          write_1(8,workMode);
          setBf_1(63);	    //要想获得大范围可调，开关电容的采样频率参数取最大值.
          setBQ_1(Qn);
        }

       if(workMode==modeTwo)
        return  31.46625*PI*f0;
       else
        return  44.5*PI*f0;
}

float bs_WorkFclk_2(float f0,float Q,uchar workMode,uchar channel)
{
        uchar Qn;
	if (workMode==modeTwo)   
          Qn = (uchar)(128-90.51/Q);  
        else   
          Qn = (uchar)(128-64/Q);

	if(channel==channelA)
        {
          write_2(0,workMode);
          setAf_2(63);		//要想获得大范围可调，开关电容的采样频率参数取最大值.
          setAQ_2(Qn);
        }
        else
        {
          write_2(8,workMode);
          setBf_2(63);	    //要想获得大范围可调，开关电容的采样频率参数取最大值.
          setBQ_2(Qn);
        }

       if(workMode==modeTwo)
        return  31.46625*PI*f0;
       else
        return  44.5*PI*f0;
}

//根据f0、Q和通道channel设置MAX262的全通工作数据
//返回时钟频率fclk
float ap_WorkFclk_1(float f0,float Q,uchar channel)
{
   uchar Qn;
      Qn = (uchar)(128-64/Q);

      if(channel==channelA)
      {
        write_1(0,modeFour);
        setAf_1(63);		//要想获得大范围可调，开关电容的采样频率参数取最大值.
        setAQ_1(Qn);
      }
    else
      {
        write_1(8,modeFour);
        setBf_1(63);	    //要想获得大范围可调，开关电容的采样频率参数取最大值.
        setBQ_1(Qn);
      }

	return  44.5*PI*f0;
}

//根据f0、Q和通道channel设置MAX262的全通工作数据
//返回时钟频率fclk
float ap_WorkFclk_2(float f0,float Q,uchar channel)
{
   uchar Qn;
      Qn = (uchar)(128-64/Q);

      if(channel==channelA)
      {
        write_2(0,modeFour);
        setAf_2(63);		//要想获得大范围可调，开关电容的采样频率参数取最大值.
        setAQ_2(Qn);
      }
    else
      {
        write_2(8,modeFour);
        setBf_2(63);	    //要想获得大范围可调，开关电容的采样频率参数取最大值.
        setBQ_2(Qn);
      }

	return  44.5*PI*f0;
}




