
/*******************************Copyright (c)***************************/
/*
 *                         深圳铭之光电子技术有限公司
 *                         工　　　　　程　　　　　部
 *                         http://www.sinocomopto.com
 */
/***********************************************************************/
#define	__MAIN_C__
#include  "_Include.h"



/*
 *功能：
 *入口：
 *出口：
 *备注：
 *函数性质：
 */
void Main_Led0Proc (void)
{	static int8u i=0;
	
	if(++i==2)i=0;
	if(i==0)LED0_ON();
	else    LED0_OFF();
}

/*
 *功能：
 *入口：
 *出口：
 *备注：
 *函数性质：
 */
int main (void)
{	int8u buf[5];
	
	
	delay_ms(10);
	
	Init_All();
	
	LED0_OUT();	LED1_OUT();
	LED0_OFF();	LED1_OFF();
	
	SysTag=0;
	CompCalTag=0;
	if(Main_SysInitChk()==1){ SysTag=1; }
	SysTag=1;
	SysPowOnTag=1;
	_SEI();
	while(1)
	{	if(SysTag==1)
		{	
			if(SysPowOnTag==1)
			{	Scp_AbtCntProc();
				if(CompCalTag==0){ Comp_AngCntProc();  }
				else             { Comp_CalSampProc(); }
			}
			
		}
		else
		{	delay_ms(20);
		}
		if(1==Twi_GetDataFromTransceiver(buf,4)){ Twi_CmdDeal(buf); }
		Main_Led0Proc();
		__watchdog_reset();
	}
}
