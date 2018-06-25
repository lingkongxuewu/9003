
/*******************************Copyright (c)***************************/
/*
 *                         深圳铭之光电子技术有限公司
 *                         工　　　　　程　　　　　部
 *                         http://www.sinocomopto.com
 */
/***********************************************************************/
#include	"_Type.h"
/***********************************************************************/
#ifndef __COMP_H__
	#define __COMP_H__
	/*-----宏定义---------------------*/
	/*-----结构体数据类型定义---------*/
	/*-----常量（表格等）声明---------*/
	/*-----函数声明-------------------*/
	void Comp_HardwareInit (void);
	void Comp_AngCntProc (void);
	void Comp_CalInit (void);
	void Comp_CalSampProc (void);
	void Comp_CalPrmtCnt (void);
	
void Comp_SpiEnd (void);
#endif
/***********************************************************************/
/***********************************************************************/
/***********************************************************************/
#ifdef   __COMP_C__
#define  COMP_EXT
#else
#define  COMP_EXT  extern
#endif
/*-------------变量声明--------------*/
