
/*******************************Copyright (c)***************************/
/*
 *                         ������֮����Ӽ������޹�˾
 *                         �������������̡�����������
 *                         http://www.sinocomopto.com
 */
/***********************************************************************/

#include  "_Include.h"

#define F_CPU_1MHz   1000000 //no operation time-consuming 1us

/***********************************************************************/
#ifdef F_CPU_1MHz
/*
 *���ܣ�
 *��ڣ�
 *���ڣ�
 *��ע��
 *�������ʣ�
 */
void delay_us(int8u n)
{
	for(int8u i=n;i>0;i--)
	{
	  __no_operation();
	}
}

/*
 *���ܣ�
 *��ڣ�
 *���ڣ�
 *��ע��
 *�������ʣ�
 */
void _delay1ms(void)
{
	for(int8u i=10;i>0;i--)
		delay_us(100);
}

/*
 *���ܣ�
 *��ڣ�
 *���ڣ�
 *��ע��
 *�������ʣ�
 */
void delay_ms(int16u n)
{
	for(int16u i=n;i>0;i--)
	{
	  _delay1ms();
	}
}

#else
/*
 *���ܣ�
 *��ڣ�
 *���ڣ�
 *��ע��
 *�������ʣ�
 */
void delay_us(int8u n)
{
	for(int8u i=n;i>0;i--)
	{
		for(int8u i=8;i>0;i--)
		{
			__no_operation();
		}
	}
}

/*
 *���ܣ�
 *��ڣ�
 *���ڣ�
 *��ע��
 *�������ʣ�
 */
void _delay1ms(void)
{
	for(int8u i=10;i>0;i--)
	  delay_us(100);
}

/*
 *���ܣ�
 *��ڣ�
 *���ڣ�
 *��ע��
 *�������ʣ�
 */
void delay_ms(int16u n)
{
	for(int16u i=n;i>0;i--)
	  _delay1ms();
}

#endif


