
/*******************************Copyright (c)***************************/
/*
 *                         ������֮����Ӽ������޹�˾
 *                         �������������̡�����������
 *                         http://www.sinocomopto.com
 */
/***********************************************************************/
#define   __TWI_SLAVE_C__
#include  "_Include.h"
/***********************************************************************/
#define TWI_BUF_SIZE 5

int8u TwiRxTag;
int8u TwiRxBuf[TWI_BUF_SIZE];		// Transceiver buffer. Set the size in the header file
int8u TwiRxBufPtr;

int8u TwiTxBuf[TWI_BUF_SIZE];		// Transceiver buffer. Set the size in the header file
int8u TwiTxBufPtr;

#define TWI_ENABLE()  TWCR=(1<<TWEN)|(0<<TWWC)|(1<<TWIE)|(1<<TWINT)|(1<<TWEA)|(0<<TWSTA)|(0<<TWSTO)

#define TWI_DISABLE() TWCR=(1<<TWEN)|(0<<TWWC)|(0<<TWIE)|(1<<TWINT)|(0<<TWEA)|(0<<TWSTA)|(0<<TWSTO)
/****************************************************************************/
/*
 *���ܣ�
 *��ڣ�
 *���ڣ�
 *��ע��
 *�������ʣ�
 */
void Twi_SlaveInit (void)
{
	TWAR=0x20;
	TWDR=0xFF;
	TWI_ENABLE();
}

/*
 *���ܣ�
 *��ڣ�
 *���ڣ�
 *��ע��
 *�������ʣ�
 */
void Twi_TransceiverStaWithData (int8u* buf,int8u n)
{	int8u i;
	
	TWI_DISABLE();
	if(n>TWI_BUF_SIZE)return;
	for(i=0;i<n;i++)
	{	TwiTxBuf[i]=buf[i];
	}
	TWI_ENABLE();
}

/*
 *���ܣ�
 *��ڣ�
 *���ڣ�
 *��ע��
 *�������ʣ�
 */
int8u Twi_GetDataFromTransceiver (int8u* buf,int8u n)
{	int8u i;
	
	
	if(3!=TwiRxTag)return 0;
	TwiRxTag=0;
	for(i=0;i<n;i++)
	{	buf[i]=TwiRxBuf[i];
	}
	return 1;
}

/*
 *���ܣ�
 *��ڣ�
 *���ڣ�
 *��ע��
 *�������ʣ�
 */
#pragma vector=TWI_vect
__interrupt void TWI_ISR (void)
{	int8u state=TWSR&0xFC;
	
	
	switch(state)
	{	case 0xA8:
				TwiTxBufPtr=0;				
		case 0xB8:
				if(TwiTxBufPtr<TWI_BUF_SIZE)
					TWDR=TwiTxBuf[TwiTxBufPtr++];
				else
					TWDR=0xaa;
				break;
		case 0x60:
				TwiRxTag=1;
				TwiRxBufPtr=0;
				break;
		case 0x80:
				if(TwiRxTag==1)TwiRxTag=2;
				TwiRxBuf[TwiRxBufPtr++]=TWDR;
				break;
		case 0xA0:
				if(TwiRxTag==2)TwiRxTag=3;
				break;
		default:
				break;
	}
	TWI_ENABLE();
}