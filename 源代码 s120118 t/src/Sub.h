
/*******************************Copyright (c)***************************/
/*
 *                         ������֮����Ӽ������޹�˾
 *                         �������������̡�����������
 *                         http://www.sinocomopto.com
 */
/***********************************************************************/
#include	"_Type.h"
/***********************************************************************/
#ifndef __SUB__
	#define __SUB__
	/*-----�궨��---------------------*/
	/*-----�ṹ���������Ͷ���---------*/
	/*-----���������ȣ�����---------*/
	/*-----��������-------------------*/
	void Cmd_SprintWord (int16u val);
	void Cmd_SprintLong (int32u val);
	void Flash_LongWr (int8u addr,int32u val);
	int32u Flash_LongRd (int8u addr);
	void Flash_WordWr(int8u adrr,int16u val);
	int16u Flash_WordRd(int8u adrr);
	void Flash_ByteWr (int8u adrr,int8u val);
	int8u Flash_ByteRd (int8u adrr);
	void Flash_DataUpdata(void);
	void Flash_DataInit(void);
	
	void Main_SysInit(void);
	int8u Main_SysInitChk(void);
#endif
/***********************************************************************/
/***********************************************************************/
/***********************************************************************/
#ifdef   __SUB_C__
#define  SUB_EXT
#else
#define  SUB_EXT  extern
#endif
/*-------------��������--------------*/




