#include "MAX262.h"


void max262_Init(void)
{
  P1DIR |= BIT0 + BIT1 + BIT2 + BIT3 + BIT4 + BIT5 + BIT6 + BIT7;
}

/***************************************************************    
д���������ݵ�ĳ��ַ
��ڲ�����A3��A0�ĵ�ַadd,D1��D0������dat2bit
D1��D0ȡdat2bit�������λ����������PDL�ڵİ�λд����
***************************************************************/    
void write_1(u8 add,u8 dat2bit)   
{  
   uchar data;
   uchar i; 
     
     data = P1OUT;
     data = (data&0xf0)|(add&0x0f);  //ȡ4λ��ַ
     data = (data&0xcf)|((dat2bit<<4)&0x30);//ȡ��λ����
     P1OUT= data;
     resetWr_1;   
     for(i=0;i<255;i++);     //����250ns
     setWr_1;   
     for(i=0;i<100;i++);     //����10ns
}

void write_2(u8 add,u8 dat2bit)   
{  
   uchar data;
   uchar i; 
     
     data = P1OUT;
     data = (data&0xf0)|(add&0x0f);  //ȡ4λ��ַ
     data = (data&0xcf)|((dat2bit<<4)&0x30);//ȡ��λ����
     P1OUT= data;
     resetWr_2;   
     for(i=0;i<255;i++);     //����250ns
     setWr_2;   
     for(i=0;i<100;i++);     //����10ns
}


    
//����Aͨ���Ĺ�����ʽ
void setAmode_1(u8 mode)
{
	write_1(0,mode);
}

void setAmode_2(u8 mode)
{
	write_2(0,mode);
}

//����Aͨ����Fֵ  datF��Χ[0 63]
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

//����Aͨ����Qֵ  datQ��Χ[0 127]
void setAQ_1(u8 datQ)   
{   
     write_1(4,datQ);   
     datQ = datQ>>2;   
     write_1(5,datQ);   
     datQ = datQ>>2;   
     write_1(6,datQ);   
     datQ = (datQ>>2)&1;   //�������һλ��0
     write_1(7,datQ);   
}

void setAQ_2(u8 datQ)   
{   
     write_2(4,datQ);   
     datQ = datQ>>2;   
     write_2(5,datQ);   
     datQ = datQ>>2;   
     write_2(6,datQ);   
     datQ = (datQ>>2)&1;   //�������һλ��0
     write_2(7,datQ);   
}

//����Bͨ���Ĺ�����ʽ
void setBmode_1(u8 mode)
{
	write_1(8,mode);
} 

void setBmode_2(u8 mode)
{
	write_2(8,mode);
}  

//����Bͨ����Fֵ
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

//����Bͨ����Qֵ
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
��ڲ���  :mode��Χ [0-3],
					fclk��λΪHZ,��Χ[40-4000000],
          f0��λHZ,��Χ[1-100000]
���ڲ���  :u8����Ƶ�ʶ�Ӧ�����֣���Χ[0-63]
********************************************/
uchar ComputeFN(u8 mode,long int fclk,long int f0)
{
  if(mode==1)
    return ( (uchar)((fclk*2.8284271247/f0)/PI)-26 );
  else
    return ( (uchar)((fclk*2/f0)/PI)-26 );
}
/*******************************************
��ڲ���  :mode��Χ [0-3],
					Q��Χ[0.5 64]
���ڲ���  :QN[0-127]
********************************************/
uchar ComputeQN(u8 mode,float Q)
{
  if(mode==1)
    return(uchar)(128-(float)(90.51/Q));
  else
    return (uchar)(128-(float)(64/Q));
}   


//***************************************************************
//���ݽ���Ƶ��fc��Ʒ������Q��ͨ��ģʽpassMode�����˲���������Ƶ��f0
//�������ڵ�ͨ�͸�ͨ   
float lh_Getf0(float fc,float Q,u8 passMode)   
{   
     float temp;
        temp = 1-0.5/pow(Q,2);
		temp = sqrt(temp+sqrt(pow(temp,2)+1));

     if(passMode==lowPass)			//��ͨ����Ƶ�ʼ��㹫ʽ
      {    
        return  fc/temp;   
      }
     else						    //��ͨ����Ƶ�ʼ��㹫ʽ
      {
        return  fc*temp;
      }
}

  




//����fc��Q��passMode��workMode��ͨ��channel����MAX262�ĵ�ͨ�͸�ͨ�Ĺ�������
// д�������Mode��Q��F=63������ʱ��Ƶ��fclk
float lhp_WorkFclk_1(float fc,float Q,u8 passMode,u8 workMode,u8 channel)   
{  
    float f0;
	uchar Qn;
         f0 = lh_Getf0(fc,Q,passMode);//���������Ƶ��f0

    if (workMode==modeTwo)   
          Qn = (uchar)(128-90.51/Q);  
    else   
          Qn = (uchar)(128-64/Q);        //�����QN

    if(passMode==highPass)
	      workMode=modeThree;	 //��ֻͨ�й���ģʽ3

    if(channel==channelA)
      {
        write_1(0,workMode);//д������M
        setAf_1(63);		//д������F=63��Ҫ���ô�Χ�ɵ�,���ص��ݵĲ���Ƶ�ʲ���ȡ���ֵ.
        setAQ_1(Qn);    //д������Q
      }
    else
      {
        write_1(8,workMode);   //д������M
        setBf_1(63);	       //д������F=63��Ҫ���ô�Χ�ɵ�,���ص��ݵĲ���Ƶ�ʲ���ȡ���ֵ.
        setBQ_1(Qn);				//д������Q
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
         f0 = lh_Getf0(fc,Q,passMode);//���������Ƶ��f0

    if (workMode==modeTwo)   
          Qn = (uchar)(128-90.51/Q);  
    else   
          Qn = (uchar)(128-64/Q);        //�����QN

    if(passMode==highPass)
	      workMode=modeThree;	 //��ֻͨ�й���ģʽ3

    if(channel==channelA)
      {
        write_2(0,workMode);//д������M
        setAf_2(63);		//д������F=63��Ҫ���ô�Χ�ɵ�,���ص��ݵĲ���Ƶ�ʲ���ȡ���ֵ.
        setAQ_2(Qn);    //д������Q
      }
    else
      {
        write_2(8,workMode);   //д������M
        setBf_2(63);	       //д������F=63��Ҫ���ô�Χ�ɵ�,���ص��ݵĲ���Ƶ�ʲ���ȡ���ֵ.
        setBQ_2(Qn);				//д������Q
      }

    if(workMode==modeTwo)
       return  31.46625*PI*f0;
    else
       return  44.5*PI*f0;
}

//����f0��Q��workMode��ͨ��channel����MAX262���ݲ���������
//����ʱ��Ƶ��fclk
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
          setAf_1(63);		//Ҫ���ô�Χ�ɵ������ص��ݵĲ���Ƶ�ʲ���ȡ���ֵ.
          setAQ_1(Qn);
        }
        else
        {
          write_1(8,workMode);
          setBf_1(63);	    //Ҫ���ô�Χ�ɵ������ص��ݵĲ���Ƶ�ʲ���ȡ���ֵ.
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
          setAf_2(63);		//Ҫ���ô�Χ�ɵ������ص��ݵĲ���Ƶ�ʲ���ȡ���ֵ.
          setAQ_2(Qn);
        }
        else
        {
          write_2(8,workMode);
          setBf_2(63);	    //Ҫ���ô�Χ�ɵ������ص��ݵĲ���Ƶ�ʲ���ȡ���ֵ.
          setBQ_2(Qn);
        }

       if(workMode==modeTwo)
        return  31.46625*PI*f0;
       else
        return  44.5*PI*f0;
}

//����f0��Q��ͨ��channel����MAX262��ȫͨ��������
//����ʱ��Ƶ��fclk
float ap_WorkFclk_1(float f0,float Q,uchar channel)
{
   uchar Qn;
      Qn = (uchar)(128-64/Q);

      if(channel==channelA)
      {
        write_1(0,modeFour);
        setAf_1(63);		//Ҫ���ô�Χ�ɵ������ص��ݵĲ���Ƶ�ʲ���ȡ���ֵ.
        setAQ_1(Qn);
      }
    else
      {
        write_1(8,modeFour);
        setBf_1(63);	    //Ҫ���ô�Χ�ɵ������ص��ݵĲ���Ƶ�ʲ���ȡ���ֵ.
        setBQ_1(Qn);
      }

	return  44.5*PI*f0;
}

//����f0��Q��ͨ��channel����MAX262��ȫͨ��������
//����ʱ��Ƶ��fclk
float ap_WorkFclk_2(float f0,float Q,uchar channel)
{
   uchar Qn;
      Qn = (uchar)(128-64/Q);

      if(channel==channelA)
      {
        write_2(0,modeFour);
        setAf_2(63);		//Ҫ���ô�Χ�ɵ������ص��ݵĲ���Ƶ�ʲ���ȡ���ֵ.
        setAQ_2(Qn);
      }
    else
      {
        write_2(8,modeFour);
        setBf_2(63);	    //Ҫ���ô�Χ�ɵ������ص��ݵĲ���Ƶ�ʲ���ȡ���ֵ.
        setBQ_2(Qn);
      }

	return  44.5*PI*f0;
}




