
/*******************************Copyright (c)***************************/
/*
 *                         ������֮����Ӽ������޹�˾
 *                         �������������̡�����������
 *                         http://www.sinocomopto.com
 */
/***********************************************************************/
#include	"_Type.h"
/***********************************************************************/
#ifndef __MAIN_H__
	#define __MAIN_H__
	/*-----�궨��---------------------*/
	#define ALTI_RD				0xA0
	#define BARO_RD				0xB0
	#define COMP_RD				0xC0
	#define TEMP_RD				0x80
	#define SEA_LEVEL_RD		0xA2	//	*
	#define BARO_OFFSET_RD		0xB2	//	*
	#define COMP_OFFSET_RD		0xD1
	
	#define ALTI_WR				0xA3	//	*
	
	#define SEA_LEVEL_WR		0xA1
	#define BARO_OFFSET_WR		0xB1
	#define COMP_OFFSET_WR		0xD0

	#define COMP_CAL_STA		0xE0
	#define COMP_CAL_STOP		0xE5
	
	#define SCH9003_POW_ON		0x70
	#define SCH9003_POW_OFF		0x71
	
	#define LED0_OUT()			DDRD_DDD0=1
	#define LED0_ON()			PORTD_PORTD0=1
	#define LED0_OFF()			PORTD_PORTD0=0
	#define LED1_OUT()			DDRD_DDD1=1
	#define LED1_ON()			PORTD_PORTD1=1
	#define LED1_OFF()			PORTD_PORTD1=0
	
	/*-----�ṹ���������Ͷ���---------*/
	/*-----���������ȣ�����---------*/
	/*-----��������-------------------*/
	void Main_I2cSetTempDataS16(int16s val);
	void Main_I2cSetAtliDataS32(int32s val);
	void Main_I2cSetBaroDataU32(int32u val);
	void Main_I2cSetCompDataS16(int16s val);
	
#endif
/***********************************************************************/
/***********************************************************************/
/***********************************************************************/
#ifdef   __MAIN_C__
#define  MAIN_EXT
#else
#define  MAIN_EXT  extern
#endif
/*-------------��������--------------*/
MAIN_EXT	int8u	CompCalTag;                     //
MAIN_EXT	int16s	XMax,YMax,XMin,YMin;            //
MAIN_EXT	int16s	XAtYMin,XAtYMax;                //
MAIN_EXT	int32s	AltiVal;
MAIN_EXT	int32u	BaroVal;
MAIN_EXT	int16u	CompVal;                        //
MAIN_EXT	int16s	TempVal;
MAIN_EXT	int32u	SeaLevel;
MAIN_EXT	int16s	BaroOffset;
MAIN_EXT	int16u	CompOffset;                     //
MAIN_EXT	int8u	TxBuf[5];                       //

MAIN_EXT	int8u	SysTag;

MAIN_EXT	int8u	SysPowOnTag;