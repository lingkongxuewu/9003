
/*******************************Copyright (c)***************************/
/*
 *                         深圳铭之光电子技术有限公司
 *                         工　　　　　程　　　　　部
 *                         http://www.sinocomopto.com
 */
/***********************************************************************/
#include  "_Include.h"
/***********************************************************************/
/*
 *功能：
 *入口：
 *出口：
 *备注：
 *函数性质：
 */
void Cmd_SprintWord (int16u val)
{
	TxBuf[1]=val&0xFF;
	val>>=8;
	TxBuf[0]=val&0xFF;
}

/*
 *功能：
 *入口：
 *出口：
 *备注：
 *函数性质：
 */
void Cmd_SprintLong (int32u val)
{
	TxBuf[2]=val&0xFF;
	val>>=8;
	TxBuf[1]=val&0xFF;
	val>>=8;
	TxBuf[0]=val&0xFF;
}

/*
 *功能：
 *入口：
 *出口：
 *备注：
 *函数性质：
 */
void Flash_LongWr (int8u addr,int32u val)
{	int8u temp0 = val >> 16;
	int8u temp1 = val >> 8;
	int8u temp2 = val;
	
	
	__EEPUT(addr++,temp0);
	__EEPUT(addr++,temp1);
	__EEPUT(addr  ,temp2);
}

/*
 *功能：
 *入口：
 *出口：
 *备注：
 *函数性质：
 */
int32u Flash_LongRd (int8u addr)
{	int32u val;
	int8u  temp;
	
	
	__EEGET(temp,addr++);
	val = temp;
	__EEGET(temp,addr++);
	val <<= 8;
	val |= temp;
	__EEGET(temp,addr  );
	val <<= 8;
	val |= temp;
	return val;
}

/*
 *功能：
 *入口：
 *出口：
 *备注：
 *函数性质：
 */
void Flash_WordWr (int8u addr,int16u val)
{	int8u temp0 = val >> 8;
	int8u temp1 = val;
	
	
	__EEPUT(addr++,temp0);
	__EEPUT(addr,temp1);
}

/*
 *功能：
 *入口：
 *出口：
 *备注：
 *函数性质：
 */
int16u Flash_WordRd (int8u addr)
{	int16u val;
	int8u temp;
	
	
	__EEGET(temp,addr++);
	val = temp;
	__EEGET(temp,addr  );
	val <<= 8;
	val |= temp;
	return val;
}

/*
 *功能：
 *入口：
 *出口：
 *备注：
 *函数性质：
 */
void Flash_ByteWr (int8u addr,int8u val)
{
	__EEPUT(addr,val);
}

/*
 *功能：
 *入口：
 *出口：
 *备注：
 *函数性质：
 */
int8u Flash_ByteRd (int8u addr)
{	int8u temp;
	
	
	__EEGET(temp,addr);
	return temp;
}

#define		FLASH_DATA_CHK		0x30
/*
 *功能：
 *入口：
 *出口：
 *备注：
 *函数性质：
 */
void Flash_DataUpdata (void)
{
	Flash_WordWr(0x30,XMax);
	Flash_WordWr(0x32,XMin);
	Flash_WordWr(0x34,YMax);
	Flash_WordWr(0x36,YMin);
	Flash_WordWr(0x38,XAtYMin);
	Flash_WordWr(0x3A,XAtYMax);
	Flash_WordWr(0x40,CompOffset);
	Flash_WordWr(0x24,BaroOffset);
	Flash_LongWr(0x20,SeaLevel);
	
	Flash_ByteWr(0x3C,FLASH_DATA_CHK);
	delay_ms(10);
}

/*
 *功能：
 *入口：
 *出口：
 *备注：
 *函数性质：
 */
void Flash_DataInit (void)
{
	if(FLASH_DATA_CHK!=Flash_ByteRd(0x3C))
	{	XMax = -1000;
		XMin =  1000;
		YMax = -1000;
		YMin =  1000;
		XAtYMin    = 0;
		XAtYMax    = 0;
		CompOffset = 0;
		BaroOffset = 0;
		SeaLevel   = 101325;
		
		Flash_DataUpdata();
	}
	else
	{	XMax = Flash_WordRd(0x30);
		XMin = Flash_WordRd(0x32);
		YMax = Flash_WordRd(0x34);
		YMin = Flash_WordRd(0x36);
		XAtYMin = Flash_WordRd(0x38);
		XAtYMax = Flash_WordRd(0x3A);
		CompOffset = Flash_WordRd(0x40);
		BaroOffset=Flash_WordRd(0x24);
		SeaLevel  =Flash_LongRd(0x20);
	}
	Comp_CalPrmtCnt();
}



#ifdef 	MAIN_INIT
	#define		SCP_0X29_CHK		0x31
	#define		SCP_0X2A_CHK		0x32
	#define		SCP_0X2B_CHK		0x33
	#define		SCP_0X2C_CHK		0x34
	#define		OSC_OFFSET			0x0
#else
	#define		SCP_0X29_CHK		0x81
	#define		SCP_0X2A_CHK		0x82
	#define		SCP_0X2B_CHK		0x83
	#define		SCP_0X2C_CHK		0x84
	#define		OSC_OFFSET			0x0
#endif

/*
 *功能：
 *入口：
 *出口：
 *备注：
 *函数性质：
 */
void Main_SysInit (void)
{	
  int8u temp;
	
	
	temp=OSCCAL+OSC_OFFSET;	Flash_ByteWr(0x2E,temp);
	Scp_FlashByteWr(0x29,SCP_0X29_CHK);
	Scp_FlashByteWr(0x2A,SCP_0X2A_CHK);
	Scp_FlashByteWr(0x2B,SCP_0X2B_CHK);
	Scp_FlashByteWr(0x2C,SCP_0X2C_CHK);
}

/*
 *功能：
 *入口：
 *出口：
 *备注：
 *函数性质：
 */
int8u Main_SysInitChk(void)
{	int8u temp;
	
	
	temp=Flash_ByteRd(0x2E);	if(temp!=OSCCAL+OSC_OFFSET)return 0;
	//temp=Scp_FlashByteRd(0x29);	if(temp!=SCP_0X29_CHK)return 0;
	//temp=Scp_FlashByteRd(0x2A);	if(temp!=SCP_0X2A_CHK)return 0;
	//temp=Scp_FlashByteRd(0x2B);	if(temp!=SCP_0X2B_CHK)return 0;
	//temp=Scp_FlashByteRd(0x2C);	if(temp!=SCP_0X2C_CHK)return 0;
	return 1;
}
