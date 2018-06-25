
/*******************************Copyright (c)***************************/
/*
 *                         深圳铭之光电子技术有限公司
 *                         工　　　　　程　　　　　部
 *                         http://www.sinocomopto.com
 */
/********************************文件信息*******************************/
/*
 *文件名:　_Const.h
 *版　本:　v1.0
 *创建人:　曾永锋
 *日　期:　2008-12-11 15:23
 *描　述:　
 *
 *当前版本:v1.0
 *修改人:　曾永锋
 *日　期:　2008-12-11 15:24
 *描　述:　
 */
/***********************************************************************/



#ifndef   __CONST__
	#define  __CONST__
	
	//#define		LCM_X_MAX					128
	//#define		LCM_Y_MAX					64
	
	#define		LCM_X_MAX					100
	#define		LCM_Y_MAX					80
	/*
	#define		BIT0		0x01
	#define		BIT1		0x02
	#define		BIT2		0x04
	#define		BIT3		0x08
	#define		BIT4		0x10
	#define		BIT5		0x20
	#define		BIT6		0x40
	#define		BIT7		0x80
	#define		BIT8		0x0100
	#define		BIT9		0x0200
	#define		BIT10		0x0400
	#define		BIT11		0x0800
	#define		BIT12		0x1000
	#define		BIT13		0x2000
	#define		BIT14		0x4000
	#define		BIT15		0x8000
	*/
	
	//#define	HIGH		0xFF
	//#define	LOW			0x00
	//#define	SUCCESS		0xFF
	//#define	FAIL		0x00
	//#define	BUSY		0xFF
	//#define	FREE		0x00
	//#define	FULL		0xFF
	//#define	NULL		0x00
	//#define	YES			0xFF
	//#define	NO			0x00
	
	//#define		true		0xFF
	//#define		false		0x00
	
	#define		TRUE		1
	#define		FALSE		0
	#define		ON			TRUE
	#define		OFF			FALSE
	#define		OK			TRUE
	#define		ERR			FALSE
	#define		ESC			FALSE
	#define		CYC			TRUE
	#define		END			FALSE
	#define		T24H		TRUE
	#define		T12H		FALSE
	
	#define  	NORMAL		1
	#define  	SLEEP		2
	
	#define		CONST_GET_HIGH_BYTE(temp)	(byte)(temp>>8)
	#define		CONST_GET_LOW_BYTE(temp)	(byte)(temp)
	
#endif


#ifndef  NUL
#define  NUL 	0x00
#endif



