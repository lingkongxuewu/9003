
/*******************************Copyright (c)***************************/
/*
 *                         ������֮����Ӽ������޹�˾
 *                         �������������̡�����������
 *                         http://www.sinocomopto.com
 */
/***********************************************************************/
#define	__MAIN_C__
#include  "_Include.h"



/*
 *���ܣ�
 *��ڣ�
 *���ڣ�
 *��ע��
 *�������ʣ�
 */
void Main_Led0Proc (void)
{	static int8u i=0;
	
	if(++i==2)i=0;
	if(i==0)LED0_ON();
	else    LED0_OFF();
}

/*
 *���ܣ�
 *��ڣ�
 *���ڣ�
 *��ע��
 *�������ʣ�
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
