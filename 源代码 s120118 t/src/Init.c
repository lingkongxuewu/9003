
/*******************************Copyright (c)***************************/
/*
 *                         深圳铭之光电子技术有限公司
 *                         工　　　　　程　　　　　部
 *                         http://www.sinocomopto.com
 */
/***********************************************************************/

#include  "_Include.h"

/***********************************************************************/

/*
 *功能：
 *入口：
 *出口：
 *备注：
 *函数性质：
 */
//void WDTTurnOn (void)
//{
//	__disable_interrupt();
//	__watchdog_reset();
//	/*Clear WDRF in MCUSR*/
//	MCUSR_WDRF  = 0;
//	/*Start timed equence*/
//	WDTCSR_WDE  = 1;
//	WDTCSR_WDCE   = 1;
//	WDTCSR = 0x29;  //Set prescaler(time-out) value = 1024K cycles (~8.0S)
//	__enable_interrupt();
//}

/*
 *功能：
 *入口：
 *出口：
 *备注：
 *函数性质：
 */
void Init_GpioPort (void)
{
	DDRB  = 0x00;          //set write
	PORTB = 0xFF;          //write data register
	DDRD  = 0x00;
	PORTD = 0xFF;
	DDRC  = 0x00;
	PORTC = 0xFF;
}

/*
 *功能：
 *入口：
 *出口：
 *备注：
 *函数性质：
 */
void Init_GpioPortRe (void)
{
//	DDRB  = 0x00;
//	PORTB = 0xFF;
	DDRD  = 0x00;
	PORTD = 0xFF;
	DDRC  = 0x00;
	PORTC = 0xFF;
}

/*
 *功能：
 *入口：
 *出口：
 *备注：
 *函数性质：
 */
void Init_PowerManager (void)
{
	ADCSRA = 0x7F;	//Close AD_Converter
	ACSR = 0x80;    //Close Analog_comparetor
	SPCR_SPIE = 0;  //Close SPI
	PRR = 0x4B;
			        //7bit PRTWI  0
			           //6bit PRTIM2 1 shut down Timer/Counter2
			           //5bit PRTIM0 0
			           //4bit Reserved bit
			           //3bit PRTIM1 1 shut down Timer/Counter1
			           //2bit PRSPI  0 If using debugWIRE On-chip Debug System,this bit not be written
			           //1bit PRUSART0 1 shut down USART clock
			           //0bit PRADC    1 shut down ADC
}

/*
 *功能：
 *入口：
 *出口：
 *备注：
 *函数性质：
 */
void Init_PowerOff (void)
{
	SMCR_SM1 = 1;
	SMCR_SE  = 1;
//	SMCR = 0x40; //bits3..1:SM2..0:Sleep Mode Selet
	             //000  Idle
	             //001  ADC Noise Reduction
	             //010  Power-down
	             //011  Power-save
	             //100  Reserved
	             //101  Reserved
	             //110  Standby
	             //111  Reserved
}

/*
 *功能：
 *入口：
 *出口：
 *备注：
 *函数性质：
 */
void Init_PowerOn (void)
{
	SMCR_SE = 0;
}

/*
 *功能：
 *入口：
 *出口：
 *备注：
 *函数性质：
 */
void Init_All (void)
{
	Init_GpioPort();
	Init_PowerManager();
	Scp_Init();                     //气压                     
	Comp_HardwareInit();           //罗盘
	Flash_DataInit();
	Twi_SlaveInit();              //I2C
}
