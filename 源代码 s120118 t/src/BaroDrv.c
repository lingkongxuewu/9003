
/*******************************Copyright (c)***************************/
/*
 *                         深圳铭之光电子技术有限公司
 *                         工　　　　　程　　　　　部
 *                         http://www.sinocomopto.com
 */
/***********************************************************************/
#include  "_Include.h"
/***********************************************************************/

	#define SCP_DRDY_IN()	DDR_DDRD2=0
	#define SCP_MISO_IN()	DDR_DDRD6=0
	
	#define SCP_DRDY_RD		PIND_PIND2
	#define SCP_MISO_RD		PIND_PIND6
	
	#define SCP_PD_OUT()	DDR_DDRD3=1
	#define SCP_CSB_OUT()	DDR_DDRD4=1
	#define SCP_MOSI_OUT()	DDR_DDRD5=1
	#define SCP_SCK_OUT()	DDR_DDRD7=1
	
	#define SCP_PD_ON()	PORTD_PORTD3=1
	#define SCP_CSB_ON()	PORTD_PORTD4=1
	#define SCP_MOSI_ON()	PORTD_PORTD5=1
	#define SCP_SCK_ON()	PORTD_PORTD7=1
	
	#define SCP_PD_OFF()	PORTD_PORTD3=0
	#define SCP_CSB_OFF()	PORTD_PORTD4=0
	#define SCP_MOSI_OFF()	PORTD_PORTD5=0
	#define SCP_SCK_OFF()	PORTD_PORTD7=0


/***********************************************************************/
/*
 *功能：
 *入口：
 *出口：
 *备注：
 *函数性质：
 */
void Scp_PowDown(void)
{
	SCP_CSB_ON();
	SCP_PD_ON();
}

/*
 *功能：
 *入口：
 *出口：
 *备注：
 *函数性质：
 */
int8u Scp_BusyTest(void)
{
	return(SCP_DRDY_RD&0x01);
}

/*
 *功能：
 *入口：
 *出口：
 *备注：
 *函数性质：
 */
void Scp_ByteSend (int8u val)
{	int8u i;
	
	
	for(i=0;i<8;i++)
	{	SCP_SCK_OFF();
		if((val&0x80)==0x80)            SCP_MOSI_ON();
		else				SCP_MOSI_OFF();
		val<<=1;
		SCP_SCK_ON();
	}
	SCP_SCK_OFF();
}

/*
 *功能：
 *入口：
 *出口：
 *备注：
 *函数性质：
 */
int8u Scp_ByteGet (void)
{	int8u i;
	int8u val=0;
	
	
	for(i=0;i<8;i++)
	{	SCP_SCK_ON();
		val<<=1;
		if(SCP_MISO_RD&0x01){val|=0x01;}		//if((P4IN&SCP_MISO)==SCP_MISO){val|=0x01;}
		SCP_SCK_OFF();
	}
	return (val);
}

/*
 *功能：
 *入口：
 *出口：
 *备注：
 *函数性质：
 */
int16u Scp_DirRegRd (int8u addr,int8u tag)
{	int16u val;
	
	
	addr = (addr << 2);							//#1, shift the SCP1000 reg addr to left by 2
	SCP_CSB_OFF();							//#3, set CSB to low
	Scp_ByteSend(addr);							//#4, write reg addr byte (8bits)
	val=Scp_ByteGet();							//#5, Read data from SPI bus.
	if(tag==2){ val<<=8; val+=Scp_ByteGet(); }
	SCP_CSB_ON();							//#6, Set CSB to HIGH
	return	val;
}

/*
 *功能：
 *入口：
 *出口：
 *备注：
 *函数性质：
 */
void Scp_DirRegWr (int8u addr,int8u val)
{
	addr = (addr << 2);							//#1, shift the SCP1000 reg addr to left by 2
	addr |= 0x02;								//#2, set write bit to one (RW=1)

	SCP_CSB_OFF();							//#3, set CSB to low
	Scp_ByteSend(addr);							//#4, write reg addr byte (8bits)
	Scp_ByteSend(val);							//#5, write data byte (8bit) to line
	SCP_CSB_ON();							//#6, Set CSB to HIGH
}

/*
 *功能：
 *入口：
 *出口：
 *备注：
 *函数性质：
 */
int16u Scp_IndirRegRd (int8u addr)
{	int16u val;
	
	
	Scp_DirRegWr(0x02,addr);
	Scp_DirRegWr(0x03,0x01);	delay_ms(10);
	val=Scp_DirRegRd(0x02,2);
	return val;
}

/*
 *功能：
 *入口：
 *出口：
 *备注：
 *函数性质：
 */
void Scp_IndirRegWr (int8u addr,int8u val)
{
	Scp_DirRegWr(0x02,addr);
	Scp_DirRegWr(0x01,val);
	Scp_DirRegWr(0x03,0x02);	delay_ms(50);
}

/*
 *功能：
 *入口：
 *出口：
 *备注：
 *函数性质：
 */
int8u Scp_FlashByteRd (int8u addr)
{	int8u val;
	
	
	Scp_DirRegWr(0x02,addr);
	Scp_DirRegWr(0x03,0x05);	delay_ms(20);
	val=Scp_DirRegRd(0x1F,1);
	return val;
}

/*
 *功能：
 *入口：
 *出口：
 *备注：
 *函数性质：
 */
void Scp_FlashByteWr (int8u addr,int8u val)
{
	Scp_DirRegWr(0x02,addr);
	Scp_DirRegWr(0x01,val);
	Scp_DirRegWr(0x03,0x06);	delay_ms(50);
}


/*
 *功能：
 *入口：
 *出口：
 *备注：
 *函数性质：
 */
void fail(void)
{
}

/*
 *功能：
 *入口：
 *出口：
 *备注：
 *函数性质：
 */
void Scp_Init (void)
{	int8u val;
	int8u i;
	
	
	DDRD=0xBA;
	PORTD=0x76;
	SCP_CSB_ON();
	SCP_PD_ON();	delay_ms(1);
	SCP_PD_OFF();	delay_ms(60);
	for(i=6;i>0;i--)
	{	val=Scp_DirRegRd(0x07,1);
		if(!(val&0x01))break;
		delay_ms(10);
	}
	if(i==0)fail();
	val=Scp_DirRegRd(0x1F,1);
	if(!(val&0x01))fail();
	
	
	Scp_ReInit();
}

/*
 *功能：
 *入口：
 *出口：
 *备注：
 *函数性质：
 */
void Scp_ReInit (void)
{	int16u val;
	int8u  i;
	
	
	DDRD=0xBA;
	PORTD=0x76;
	SCP_CSB_ON();
	SCP_PD_ON();	delay_ms(1);
	SCP_PD_OFF();	delay_ms(60);
	for(i=6;i>0;i--)
	{	val=Scp_DirRegRd(0x07,1);
		if(!(val&0x01))break;
		delay_ms(10);
	}
	if(i==0)fail();
	val=Scp_DirRegRd(0x1F,1);
	if(!(val&0x01))fail();
	
	Scp_IndirRegWr(0x2D,0x03);	delay_ms(100);
	Scp_DirRegWr(0x03,0x00);	delay_ms(10);
	Scp_DirRegWr(0x03,0x0A);	delay_ms(100);
}



