
/*******************************Copyright (c)***************************/
/*
 *                         深圳铭之光电子技术有限公司
 *                         工　　　　　程　　　　　部
 *                         http://www.sinocomopto.com
 */
/***********************************************************************/
#define  __CMD_C__
#include  "_Include.h"
/***********************************************************************/

/*
 *功能：
 *入口：
 *出口：
 *备注：
 *函数性质：
 */
void Twi_CmdDeal(int8u* buf)
{	int8u cmd;
	
	
	cmd = buf[0];
	switch(cmd)
	{
          /*
		case ALTI_RD:				//ok
			if(AltiVal>=0)
				Cmd_SprintLong(AltiVal);
			else
			{	Cmd_SprintLong(-AltiVal);
				TxBuf[0]|=0x80;
			}
			Twi_TransceiverStaWithData(TxBuf,3);
			break;
		case BARO_RD:
			Cmd_SprintLong(BaroVal);
			Twi_TransceiverStaWithData(TxBuf,3);
			break;
          */
		case COMP_RD:
			Cmd_SprintWord(CompVal);
			Twi_TransceiverStaWithData(TxBuf,2);
			break;
           /*
		case TEMP_RD:
			if(TempVal>=0)
				Cmd_SprintWord(TempVal);
			else
			{	Cmd_SprintWord(-TempVal);
				TxBuf[0]|=0x80;
			}
			Twi_TransceiverStaWithData(TxBuf,2);
			break;
          */
		case COMP_OFFSET_RD:
			Cmd_SprintWord(CompOffset);
			Twi_TransceiverStaWithData(TxBuf,2);
			break;
	/*	case SEA_LEVEL_RD:
			Cmd_SprintLong(SeaLevel);
			Twi_TransceiverStaWithData(TxBuf,3);
			break;
		case BARO_OFFSET_RD:
			if(BaroOffset>=0)
				Cmd_SprintWord(BaroOffset);
			else
			{	Cmd_SprintWord(-BaroOffset);
				TxBuf[0]|=0x80;
			}
			Twi_TransceiverStaWithData(TxBuf,2);
			break;
			
			
		case SEA_LEVEL_WR:
			SeaLevel=((int32u)buf[1]<<16)+((int16u)buf[2]<<8)+buf[3];
			if(SeaLevel<30000 )SeaLevel=30000;
			if(SeaLevel>120000)SeaLevel=120000;
			Flash_DataUpdata();
			break;
		case BARO_OFFSET_WR:
			cmd=buf[1];//借用
			if((cmd&0x80)==0)
				BaroOffset=((int16u)cmd<<8)+buf[2];
			else
			{	cmd&=0x7F;
				BaroOffset=-(((int16u)cmd<<8)+buf[2]);
			}
			Flash_DataUpdata();
			break;                                          */
		case COMP_OFFSET_WR:                                    
			CompOffset=((int16u)buf[1]<<8)+buf[2];
			if(CompOffset>360)CompOffset=360;
			Flash_DataUpdata();
			break;
			
			//	#define 				0xA3	//	*
	/*	case ALTI_WR:
			cmd=buf[1];//借用
			if((cmd&0x80)==0)
			{	AltiVal= (((int32u)cmd<<16)+((int16u)buf[2]<<8)+buf[3]);
			}
			else
			{	cmd&=0x7F;
				AltiVal=-(((int32u)cmd<<16)+((int16u)buf[2]<<8)+buf[3]);
			}
			SeaLevel = BaroVal+SEALEVEL_REF-Alti_High2Pres(AltiVal);
			Flash_DataUpdata();
	*/		break;
			//	#define 				0xA3	//	*
			
		case COMP_CAL_STA:
			TxBuf[0]=cmd;
			Twi_TransceiverStaWithData(TxBuf,1);
			Comp_CalInit();
			break;
		case COMP_CAL_STOP:
			TxBuf[0]=cmd;
			Twi_TransceiverStaWithData(TxBuf,1);
			Comp_CalPrmtCnt();
			Flash_DataUpdata();
			break;
         
		case 0x65:
			LED1_ON();
			if(0==SysTag)
			{	Main_SysInit();
				SysTag=1;
			}
			delay_ms(300);
			LED1_OFF();
			break;
			
			
			
			
		case SCH9003_POW_ON:
			Scp_ReInit();
			Comp_HardwareInit();
//			Init_PowerOn();
//			LED0_OUT();	LED1_OUT();
//			LED0_OFF();	LED1_OFF();

			SysPowOnTag=1;
			break;
			
		case SCH9003_POW_OFF:
			Scp_PowDown();
			
			Init_GpioPort();
			//Init_GpioPortRe();
			//Comp_SpiEnd();
			
			Init_PowerOff();
			SysPowOnTag=0;
			__sleep();
			break;
                        
                   
			
	}
}


