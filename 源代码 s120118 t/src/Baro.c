
/*******************************Copyright (c)***************************/
/*
 *                         ������֮����Ӽ������޹�˾
 *                         �������������̡�����������
 *                         http://www.sinocomopto.com
 */
/***********************************************************************/
#define   __BARO_C__
#include  "_Include.h"

/***********************************************************************/
/*
 *���ܣ�
 *��ڣ�
 *���ڣ�
 *��ע��
 *�������ʣ�
 */
void Scp_AbtCntProc (void)
{	int16u temp;
	int32u baro;
	
	
	if(Scp_BusyTest()==0)return;
	
	temp=Scp_DirRegRd(0x21,2);							//���¶�
	temp=(0x3FFF&temp)>>1;
	if(0==(temp&0x1000)){ TempVal=temp; }
	else                { TempVal=temp-0x2000; }
	
	baro=(int32u)((0x0007)&Scp_DirRegRd(0x1F,1));		//����ѹ
	baro=(baro<<16);
	baro|=(int32u)Scp_DirRegRd(0x20,2);
	baro=(baro>>2);
	
	BaroVal=baro+BaroOffset;
	AltiVal=Alti_Baro2High(BaroVal+SEALEVEL_REF-SeaLevel);
	
}

