#include "includes.h"

static u8 CH375_ReadData()
{
	return (u8)GPIO_ReadInputData(GPIOA);
}

static void CH375_WriteData(u8 dat)
{
	GPIO_WriteBit(GPIOA, (u16)dat, Bit_SET);
	GPIO_WriteBit(GPIOA, (u16)~dat, Bit_RESET);	
}

#define CH375_Set_A0(enable)	\
	GPIO_WriteBit(GPIOB, GPIO_Pin_0, (enable) ? Bit_SET : Bit_RESET)

#define CH375_Set_RD(enable)	\
	GPIO_WriteBit(GPIOB, GPIO_Pin_2, (enable) ? Bit_SET : Bit_RESET)

#define CH375_Set_WR(enable)	\
	GPIO_WriteBit(GPIOB, GPIO_Pin_1, (enable) ? Bit_SET : Bit_RESET)

#define CH375_Set_CS(enable)	\
	GPIO_WriteBit(GPIOB, GPIO_Pin_13, (enable) ? Bit_SET : Bit_RESET)

#define CH375_Read_INT()	\
	GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12)

#define NO_DEFAULT_DELAY_100US	1
#define NO_DEFAULT_DELAY_WRITE	1
//#define CH375_INT_WIRE CH375_Read_INT()
	
#include "CH375LIB\ARM32\FILELIBM_CX\CH375HFM.h"

static void Delay_Us(u16 myUs)   
{
  u16 i;
  while(myUs--)
  {
    i=6;
    while(i--);
  }
}
 
static void Delay_Ms(u16 myMs)
{
  u16 i;
  while(myMs--)
  {
    i=7200;
    while(i--);
  }
}

void xDelay100uS( void )
{
	Delay_Us(200);
}

void xDelayAfterWrite( void )			/* Ð´²Ù×÷ºóÑÓÊ± */
{
	Delay_Us(2000);
}

void xWriteCH375Cmd( UINT8 mCmd )
{
	Delay_Us(4);
	CH375_WriteData(mCmd);
	CH375_Set_A0(1);
	CH375_Set_CS(0);
	CH375_Set_WR(0);
	Delay_Us(10);
	CH375_Set_WR(1);
	CH375_Set_CS(1);
	CH375_Set_A0(0);
	CH375_WriteData(0xff);
	Delay_Us(4);
}

void xWriteCH375Data( UINT8 mData )
{
	Delay_Us(4);
	CH375_WriteData(mData);
	CH375_Set_A0(0);
	CH375_Set_CS(0);
	CH375_Set_WR(0);
	Delay_Us(10);
	CH375_Set_WR(1);
	CH375_Set_CS(1);
	CH375_Set_A0(0);
	CH375_WriteData(0xff);
	Delay_Us(4);	
}

UINT8 xReadCH375Data( void )
{
	UINT8 data;

	Delay_Us(4);
	CH375_WriteData(0xff);
	CH375_Set_A0(0);
	CH375_Set_CS(0);
	CH375_Set_RD(0);
	Delay_Us(10);
	data = CH375_ReadData();
	CH375_Set_RD(1);
	CH375_Set_CS(1);
	Delay_Us(4);
	return data;
}

u8 xReadCH375Cmd(void)
{
	UINT8 data;

	Delay_Us(4);
	CH375_WriteData(0xff);
	CH375_Set_A0(1);
	CH375_Set_CS(0);
	CH375_Set_RD(0);
	Delay_Us(10);
	data = CH375_ReadData();
	CH375_Set_RD(1);
	CH375_Set_CS(1);
	Delay_Us(4);
	return data;	
}

UINT8 gErr = 0;
UINT32 gDiskSize = 0;

static int mStrcpy(char *dst, char *src)
{
	char *p = dst,*q = src;

	while(*p++ = *q++);

	return p - dst - 1;
}

void CH375_PortInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | 
						   RCC_APB2Periph_GPIOB,
						   ENABLE  );

	/* Configure GPIOA Open Drian Output */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 |
								  GPIO_Pin_1 |
								  GPIO_Pin_2 |
								  GPIO_Pin_3 |
								  GPIO_Pin_4 |
								  GPIO_Pin_5 |
								  GPIO_Pin_6 |
								  GPIO_Pin_7;								  			
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Configure CS#, RD#, WR#, A0 to PP output */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 |
								  GPIO_Pin_1 |
								  GPIO_Pin_2 |
								  GPIO_Pin_13;
							  			
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* Configure INT# to IPU input */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;							  			
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	CH375_Set_CS(1);
	CH375_Set_RD(1);
	CH375_Set_WR(1);
	CH375_Set_A0(1);	
}

void CH375_Test(void)
{
	UINT8 err = 0;
	UINT32 i = 0;

	CH375_PortInit();

//	xWriteCH375Cmd(CMD_RESET_ALL);
//	OSTimeDlyHMSM(0, 0, 1, 0);

	err = CH375LibInit();

	gErr = err;

//	OSTimeDlyHMSM(0, 0, 1, 0);

	while(1)
	{
	  	while(CH375DiskStatus != DISK_CONNECT) 
			xQueryInterrupt();
		OSTimeDlyHMSM(0, 0, 0, 200);
#if 1		
		for(i = 0; i < 5; i++)
		{
			Delay_Ms(100);
			if(CH375DiskReady() == ERR_SUCCESS)
			{
				break;
			}			
		}
#endif
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

