
/*******************************Copyright (c)***************************/
/*
 *                         ������֮����Ӽ������޹�˾
 *                         �������������̡�����������
 *                         http://www.sinocomopto.com
 */
/***********************************************************************/
#include	"_Type.h"
/***********************************************************************/
#ifndef __BARO_DRV_H__
	#define __BARO_DRV_H__
	/*-----�궨��---------------------*/
	/*-----�ṹ���������Ͷ���---------*/
	/*-----���������ȣ�����---------*/
	/*-----��������-------------------*/
	void Scp_DirRegWr(int8u addr,int8u temp);
	int8u Scp_BusyTest(void);
	int16u Scp_DirRegRd (int8u addr,int8u tag);
	int16u Scp_IndirRegRd(int8u addr);
	void Scp_IndirRegWr(int8u addr,int8u temp);
	int8u Scp_FlashByteRd(int8u addr);
	void Scp_FlashByteWr(int8u addr,int8u temp);
	void fail(void);
	void Scp_Init(void);
	void Scp_ReInit(void);
	void Scp_PowDown(void);
#endif
/***********************************************************************/
/***********************************************************************/
/***********************************************************************/
#ifdef   __BARO_DRV_C__
#define  BARO_DRV_EXT
#else
#define  BARO_DRV_EXT  extern
#endif
/*-------------��������--------------*/


