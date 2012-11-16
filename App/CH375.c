#include "includes.h"

#define NO_DEFAULT_DELAY_100US	1
#define NO_DEFAULT_DELAY_WRITE	1
#define CH375_INT_WIRE GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_7)
	
#include "CH375LIB\ARM32\FILELIBM_CX\CH375HFM.h"


extern void USART_SendChar(unsigned short ch);
extern unsigned short USART_ReadChar(void);
extern void USART_Config(void);
extern void USART_ChangeBaudRate(unsigned int rate);

void xDelay100uS( void )
{
	OSTimeDlyHMSM(0, 0, 0, 10);
}

void xDelayAfterWrite( void )			/* д��������ʱ */
{
	OSTimeDlyHMSM(0, 0, 0, 10);
}

void xWriteCH375Cmd( UINT8 mCmd )
{
	USART_SendChar((1 << 8) | mCmd);
}

void xWriteCH375Data( UINT8 mData )
{
	USART_SendChar(mData);
}

UINT8 xReadCH375Data( void )
{
	return (UINT8)USART_ReadChar();
}

UINT8 gErr = 0;
UINT32 gDiskSize = 0;

static int mStrcpy(char *dst, char *src)
{
	char *p = dst,*q = src;

	while(*p++ = *q++);

	return p - dst - 1;
}

void CH375_Test(void)
{
	UINT8 err = 0;
	UINT32 i = 0;

	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
												 
	USART_InitStructure.USART_BaudRate = 115200;						  // ������Ϊ��115200
	USART_InitStructure.USART_WordLength = USART_WordLength_9b;			  // 8λ����
	USART_InitStructure.USART_StopBits = USART_StopBits_1;				  // ��֡��β����1��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No ;				  // ��żʧ��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	// Ӳ��������ʧ��
	
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;		  // ����ʹ��+����ʹ��
	/* Configure USART1 basic and asynchronous paramters */
//	USART_Init(USART1, &USART_InitStructure);
	    
	  /* Enable USART1 */
//	USART_ClearFlag(USART1, USART_IT_RXNE); 			//���жϣ�����һ�����жϺ����������ж�
//	USART_ITConfig(USART1,USART_IT_RXNE, DISABLE);		//ʹ��USART1�ж�Դ
//	USART_Cmd(USART1, ENABLE);	

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,
						   ENABLE  );

	/* Configure USART1 Tx (PA.09) as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;			
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;		 
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	USART_Config();	
//	xWriteCH375Cmd(CMD_RESET_ALL);
	OSTimeDlyHMSM(0, 0, 1, 0);

	err = CH375LibInit();
	
	xWriteCH375Cmd(CMD_SET_BAUDRATE);	// set Baudrate to 2000000
	xWriteCH375Data(0x3);

	xWriteCH375Data(0xcc);

	OSTimeDlyHMSM(0, 0, 1, 0);
	USART_Cmd(USART1, DISABLE);
	
	USART_Init(USART1, &USART_InitStructure);	
	USART_Cmd(USART1, ENABLE);
#if 0
	err = xReadCH375Data();

	xWriteCH375Cmd(0xc8);
	if(err != CMD_RET_SUCCESS)
	{
		 while(1);
	}
	else
	{
		 xWriteCH375Data(0xc1);
		 while(1);
	}
	gErr = err;
#endif
	while(1)
	{
	  	while(CH375DiskStatus != DISK_CONNECT) 
			xQueryInterrupt();
		OSTimeDlyHMSM(0, 0, 0, 200);
		
		for(i = 0; i < 5; i++)
		{
			if(CH375DiskReady() == ERR_SUCCESS)
			{
				break;
			}
			OSTimeDlyHMSM(0, 0, 0, 100);			
		}

		mStrcpy((char*)mCmdParam.Open.mPathName, "/TEST.TXT");
		err = CH375FileOpen();
		if(err != ERR_SUCCESS)
			break;

		mCmdParam.ByteLocate.mByteOffset = 0xffffffff;
		err = CH375ByteLocate();
	   	if(err != ERR_SUCCESS)
			break;

		i = mStrcpy(mCmdParam.ByteWrite.mByteBuffer, "This is a test");
		mCmdParam.ByteWrite.mByteCount = i;
		err = CH375ByteWrite();
	   	if(err != ERR_SUCCESS)
			break;

		err = CH375FileClose();
	   	if(err != ERR_SUCCESS)
			break;
		break;
	}
	while(1);
}

