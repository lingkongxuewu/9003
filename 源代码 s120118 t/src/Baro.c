
/*******************************Copyright (c)***************************/
/*
 *                         深圳铭之光电子技术有限公司
 *                         工　　　　　程　　　　　部
 *                         http://www.sinocomopto.com
 */
/***********************************************************************/
#define   __BARO_C__
#include  "_Include.h"

/***********************************************************************/
/*
 *功能：
 *入口：
 *出口：
 *备注：
 *函数性质：
 */
void Scp_AbtCntProc (void)
{	int16u temp;
	int32u baro;
	
	
	if(Scp_BusyTest()==0)return;
	
	temp=Scp_DirRegRd(0x21,2);							//读温度
	temp=(0x3FFF&temp)>>1;
	if(0==(temp&0x1000)){ TempVal=temp; }
	else                { TempVal=temp-0x2000; }
	
	baro=(int32u)((0x0007)&Scp_DirRegRd(0x1F,1));		//读气压
	baro=(baro<<16);
	baro|=(int32u)Scp_DirRegRd(0x20,2);
	baro=(baro>>2);
	
	BaroVal=baro+BaroOffset;
	AltiVal=Alti_Baro2High(BaroVal+SEALEVEL_REF-SeaLevel);
	
}

