
/*******************************Copyright (c)***************************/
/*
 *                         深圳铭之光电子技术有限公司
 *                         工　　　　　程　　　　　部
 *                         http://www.sinocomopto.com
 */
/***********************************************************************/
#define  __COMP_C__
#include  "_Include.h"
/***********************************************************************/
	#define PNI_DRDY_IN()		DDRB_DDB0=0             //write
	#define PNI_MISO_IN()		DDRB_DDB4=0
	
	#define PNI_DRDY_RD			PINB_PINB0
	#define PNI_MISO_RD			PINB_PINB4
	
	#define PNI_DRDY_OUT()		DDRB_DDB1=1
	#define PNI_SSNOT_OUT()		DDRB_DDB2=1
	#define PNI_MOSI_OUT()		DDRB_DDB3=1
	#define PNI_SCK_OUT()		DDRB_DDB5=1
	
	#define PNI_RST_ON()		PORTB_PORTB1=1          
	#define PNI_SSNOT_ON()		PORTB_PORTB2=1
	#define PNI_MOSI_ON()		PORTB_PORTB3=1
	#define PNI_SCK_ON()		PORTB_PORTB5=1
	
	#define PNI_RST_OFF()		PORTB_PORTB1=0
	#define PNI_SSNOT_OFF()		PORTB_PORTB2=0
	#define PNI_MOSI_OFF()		PORTB_PORTB3=0
	#define PNI_SCK_OFF()		PORTB_PORTB5=0


	int16s	XGain;
	int16s	YGain;
	int16s	XOffset;
	int16s	YOffset;
	float	SinPhi;
	float	OneOverCosPhi;
	static	int16s	XAdcCalBuf[3];
	static	int16s	YAdcCalBuf[3];
	
/***********************************************************************/
/*
 *功能：
 *入口：
 *出口：
 *备注：
 *函数性质：
 */
void Comp_HardwareInit(void)
{
	PNI_SSNOT_ON();
	PNI_SCK_ON();
	PNI_MOSI_ON();
	PNI_RST_OFF();
	
	PNI_SSNOT_OUT();
	PNI_DRDY_OUT();
	PNI_SCK_OUT();
	PNI_MOSI_OUT();
	PNI_MISO_IN();
	PNI_DRDY_IN();
}


/*
 *功能：
 *入口：
 *出口：
 *备注：
 *函数性质：内部函数
 */
void Comp_SpiSta (void)
{
	PNI_SSNOT_OFF();
	PNI_RST_OFF();
	PNI_SCK_OFF();
	PNI_RST_ON();
	PNI_RST_OFF();
}

/*
 *功能：
 *入口：
 *出口：
 *备注：
 *函数性质：
 */
int8u Comp_SpiBusyTest (void)
{
	return(PNI_DRDY_RD&0x01);
	//delay_ms(10);
	//return(1);
}

/*
 *功能：
 *入口：
 *出口：
 *备注：
 *函数性质：
 */
void Comp_SpiByteSend (int8u val)
{	int8u i;
	
	
	for(i=0x80;i;i>>=1)
	{	if(val&i)PNI_MOSI_ON();
		else     PNI_MOSI_OFF();
		PNI_SCK_ON();
		PNI_SCK_OFF();
	}
}

/*
 *功能：
 *入口：
 *出口：
 *备注：
 *函数性质：
 */
int16s Comp_SpiByteGet (void)
{	int16s val=0;
	int16u i;
	
	
	PNI_SCK_OFF();
	for(i=0x8000;i;i>>=1)
	{	PNI_SCK_ON();
		if(PNI_MISO_RD&0x01){val|=i;}
		PNI_SCK_OFF();
	}
	return val;
}

/*
 *功能：
 *入口：
 *出口：
 *备注：
 *函数性质：内部函数
 */
void Comp_SpiEnd (void)
{
	PNI_SSNOT_ON();
}

/*
 *功能：
 *入口：
 *出口：
 *备注：
 *函数性质：
 */
int16s Comp_GetAdc (int8u cmd)
{	int16s val;
	
	
	Comp_SpiSta();
	Comp_SpiByteSend(cmd);		//0x51,0x52
	while(!Comp_SpiBusyTest());
	val=Comp_SpiByteGet();
	Comp_SpiEnd();
	return val;
}

/***********************************************************************/
/*
 *功能：
 *入口：
 *出口：
 *备注：
 *函数性质：
 */
int16s Comp_MidFilter (int16s* buf)
{	int16s min=buf[0];
	int16s max=min;
	int16s sum=max;
	
	
	if(min>buf[1])min=buf[1];
	if(max<buf[1])max=buf[1];
	sum+=buf[1];
	if(min>buf[2])min=buf[2];
	if(max<buf[2])max=buf[2];
	sum+=buf[2];
	return (sum-min-max);
}

/*
 *功能：
 *入口：
 *出口：
 *备注：
 *函数性质：公共
 */
void Comp_AdcCorrectS (int16s* x_ptr,int16s* y_ptr)
{	float x,y;
	
	
	x = ((float)(*x_ptr - XOffset)) / XGain;
	y = ((float)(*y_ptr - YOffset)) / YGain;
	x = x - y * SinPhi;
	x = x * OneOverCosPhi;
	*x_ptr=(int16s)(x*XGain);
	*y_ptr=(int16s)(y*XGain);
}

#define OFFSET (1440+90)
/*
 *功能：
 *入口：
 *出口：
 *备注：
 *函数性质：
 */
void Comp_AngCntProc (void)
{	int16s adc_x;
	int16s adc_y;
	int8u  tag;
	int16s ang;
	
	
	adc_x=Comp_GetAdc(0x51);
	adc_y=Comp_GetAdc(0x52);
	Comp_AdcCorrectS(&adc_x,&adc_y);		//软硬磁校准
	if(0==adc_x)adc_x=1;
	if(0==adc_y)adc_y=1;
	if     ( (adc_x<0)&&(adc_y>0) ){ tag=2;	adc_x=-adc_x;					}
	else if( (adc_x<0)&&(adc_y<0) ){ tag=3;	adc_x=-adc_x;	adc_y=-adc_y;	}
	else if( (adc_x>0)&&(adc_y<0) ){ tag=4;					adc_y=-adc_y;	}
	else                           { tag=1;									}
	ang=(int16s)((atan( (float) adc_x/adc_y))/3.14*1800);
	switch(tag)
	{	case 4:		ang=1800-ang;	break;
		case 3:		ang=1800+ang;	break;
		case 2:		ang=3600-ang;	break;
		case 1:		ang=ang;
	}
	//ang=Comp_AdcSmooth_L(ang);
	CompVal=(OFFSET+CompOffset-ang/10)%360;
}

/*
 *功能：
 *入口：
 *出口：
 *备注：
 *函数性质：
 */
void Comp_CalInit (void)
{
	CompCalTag=1;
	
	XMax =-32767;
	YMax =-32767;
	XMin = 32767;
	YMin = 32767;
}

/*
 *功能：
 *入口：
 *出口：
 *备注：
 *函数性质：
 */
void Comp_CalSampProc (void)
{	int16s adc_x;
	int16s adc_y;
	int8u  i;
	
	
	for(i=0;i<3;i++)
	{	XAdcCalBuf[i]=Comp_GetAdc(0x51);
		YAdcCalBuf[i]=Comp_GetAdc(0x52);
	}
	adc_x=Comp_MidFilter(XAdcCalBuf);
	adc_y=Comp_MidFilter(YAdcCalBuf);
	if(adc_x > XMax) { 		        XMax = adc_x; }
	if(adc_x < XMin) { 			XMin = adc_x; }
	if(adc_y > YMax) { XAtYMax = adc_x;	YMax = adc_y; }
	if(adc_y < YMin) { XAtYMin = adc_x;	YMin = adc_y; }
}

/*
 *功能：
 *入口：
 *出口：
 *备注：
 *函数性质：
 */
void Comp_CalPrmtCnt (void)
{	float	Phi0;
	float	Phi1;
	float	Phi;
	
	
	CompCalTag=0;
	
	XGain   = (XMax-XMin)>>1;
	YGain   = (YMax-YMin)>>1;
	XOffset = (XMax+XMin)>>1;
	YOffset = (YMax+YMin)>>1;
	Phi0  = asin( ((float)XAtYMax-XOffset)/XGain );
	Phi1  = asin( ((float)XAtYMin-XOffset)/XGain );
	Phi    = (Phi0-Phi1)/2.0f;
	SinPhi = sin(Phi);
	OneOverCosPhi = 1.0f/cos(Phi);
	
}
