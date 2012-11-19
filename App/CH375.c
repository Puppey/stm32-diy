#include "includes.h"

#define ARRAY_SIZE(x)	(sizeof(x) / sizeof(x[0]))

typedef enum _CH375_Pin
{
    CH375_PIN_D0 = 0,
    CH375_PIN_D1,
    CH375_PIN_D2,
    CH375_PIN_D3,
    CH375_PIN_D4,
    CH375_PIN_D5,
    CH375_PIN_D6,
    CH375_PIN_D7,
    CH375_PIN_A0,
    CH375_PIN_RD,
    CH375_PIN_WR,
    CH375_PIN_CS,
    CH375_PIN_INT
}CH375_Pin;

struct CH375_Pin_Config
{
    CH375_Pin 		Pin;
    GPIO_TypeDef 	*Gpio;
    u16				Gpio_Pin;	
};

struct CH375_Pin_Config CH375_Pin_Map[] = {
    {CH375_PIN_D0, GPIOC,  GPIO_Pin_8},
    {CH375_PIN_D1, GPIOC,  GPIO_Pin_6},
    {CH375_PIN_D2, GPIOB,  GPIO_Pin_14},
    {CH375_PIN_D3, GPIOB,  GPIO_Pin_12},
    {CH375_PIN_D4, GPIOA,  GPIO_Pin_5},
    {CH375_PIN_D5, GPIOA,  GPIO_Pin_7},
    {CH375_PIN_D6, GPIOC,  GPIO_Pin_5},
    {CH375_PIN_D7, GPIOB,  GPIO_Pin_1},
    {CH375_PIN_A0, GPIOB,  GPIO_Pin_0},
    {CH375_PIN_RD, GPIOA,  GPIO_Pin_6},
    {CH375_PIN_WR, GPIOC,  GPIO_Pin_4},
    {CH375_PIN_CS, GPIOB,  GPIO_Pin_13},
    {CH375_PIN_INT, GPIOA,  GPIO_Pin_4},
};

static u8 CH375_ReadData()
{
    u8 data = 0, i;
    
    for(i = CH375_PIN_D0; i <= CH375_PIN_D7; i++)
    {
        if(GPIO_ReadInputDataBit(CH375_Pin_Map[i].Gpio, 
        		  	            CH375_Pin_Map[i].Gpio_Pin))
        {
            data |= (1 << i);
        }
    }
    return data;
}

static void CH375_WriteData(u8 dat)
{
    u8 i;
    
    for(i = CH375_PIN_D0; i <= CH375_PIN_D7; i++)
    {
        GPIO_WriteBit(CH375_Pin_Map[i].Gpio, 
        	        CH375_Pin_Map[i].Gpio_Pin, 
        	        (dat & (1 << i)) ? Bit_SET : Bit_RESET);
    }
}

#define CH375_Set_A0(enable)	\
	GPIO_WriteBit(CH375_Pin_Map[CH375_PIN_A0].Gpio, \
				  CH375_Pin_Map[CH375_PIN_A0].Gpio_Pin, \
				  (enable) ? Bit_SET : Bit_RESET)

#define CH375_Set_RD(enable)							\
	GPIO_WriteBit(CH375_Pin_Map[CH375_PIN_RD].Gpio, 	\
				  CH375_Pin_Map[CH375_PIN_RD].Gpio_Pin, \
				  (enable) ? Bit_SET : Bit_RESET)

#define CH375_Set_WR(enable)							\
    GPIO_WriteBit(CH375_Pin_Map[CH375_PIN_WR].Gpio, 	\
                  CH375_Pin_Map[CH375_PIN_WR].Gpio_Pin, \
                  (enable) ? Bit_SET : Bit_RESET)

#define CH375_Set_CS(enable)							\
    GPIO_WriteBit(CH375_Pin_Map[CH375_PIN_CS].Gpio, 	\
                  CH375_Pin_Map[CH375_PIN_CS].Gpio_Pin, \
                  (enable) ? Bit_SET : Bit_RESET)

#define CH375_Read_INT()								\
    GPIO_ReadInputDataBit(CH375_Pin_Map[CH375_PIN_INT].Gpio, \
                          CH375_Pin_Map[CH375_PIN_INT].Gpio_Pin)

#define NO_DEFAULT_DELAY_100US	1
#define NO_DEFAULT_DELAY_WRITE	1
#define CH375_INT_WIRE CH375_Read_INT()
	
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
	Delay_Us(100);
}

void xDelayAfterWrite( void )			/* Ð´²Ù×÷ºóÑÓÊ± */
{
	Delay_Us(200);
}

void xWriteCH375Cmd( UINT8 mCmd )
{
    Delay_Us(2);
    CH375_WriteData(mCmd);
    CH375_Set_A0(1);
    CH375_Set_CS(0);
    CH375_Set_WR(0);
    Delay_Us(5);
    CH375_Set_WR(1);
    Delay_Us(0);
    CH375_Set_CS(1);
    Delay_Us(2);
}

u8 xReadCH375Cmd(void)
{
    UINT8 data;
    
    Delay_Us(2);
    CH375_WriteData(0xff);
    CH375_Set_A0(1);
    CH375_Set_CS(0);
    CH375_Set_RD(0);
    Delay_Us(0);
    data = CH375_ReadData();	
    CH375_Set_RD(1);
    CH375_Set_CS(1);
    Delay_Us(2);
    return data;	
}

void xWriteCH375Data( UINT8 mData )
{
    Delay_Us(2);
    CH375_WriteData(mData);
    CH375_Set_A0(0);
    CH375_Set_CS(0);
    CH375_Set_WR(0);
    Delay_Us(5);
    CH375_Set_WR(1);
    Delay_Us(0);
    CH375_Set_CS(1);
    Delay_Us(2);	
}

UINT8 xReadCH375Data( void )
{
    UINT8 data;
    
    Delay_Us(2);
    CH375_WriteData(0xff);
    CH375_Set_A0(0);
    CH375_Set_CS(0);
    CH375_Set_RD(0);
    Delay_Us(0);
    data = CH375_ReadData();	
    CH375_Set_RD(1);
    CH375_Set_CS(1);
    Delay_Us(2);
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

static void GPIO_Pin_Config(GPIO_TypeDef *gpio, u16 pin, GPIOMode_TypeDef mode, GPIOSpeed_TypeDef speed)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    GPIO_InitStructure.GPIO_Pin 	= pin;
    GPIO_InitStructure.GPIO_Mode 	= mode;
    GPIO_InitStructure.GPIO_Speed 	= speed;
    GPIO_Init(gpio, &GPIO_InitStructure);		
}

void CH375_PortInit(void)
{
	int i;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | 
						   RCC_APB2Periph_GPIOB |
						   RCC_APB2Periph_GPIOC ,
						   ENABLE  );
	GPIO_DeInit(GPIOA);
	GPIO_DeInit(GPIOB);
	GPIO_DeInit(GPIOC);

	for(i = CH375_PIN_D0; i <= CH375_PIN_D7; i++)
	{
		GPIO_Pin_Config(CH375_Pin_Map[i].Gpio, 
						CH375_Pin_Map[i].Gpio_Pin, 
						GPIO_Mode_Out_OD, 
						GPIO_Speed_50MHz);				
	}

	GPIO_Pin_Config(CH375_Pin_Map[CH375_PIN_CS].Gpio, 
				 	CH375_Pin_Map[CH375_PIN_CS].Gpio_Pin, 
					GPIO_Mode_Out_OD, 
					GPIO_Speed_50MHz);
	GPIO_Pin_Config(CH375_Pin_Map[CH375_PIN_RD].Gpio, 
				 	CH375_Pin_Map[CH375_PIN_RD].Gpio_Pin, 
					GPIO_Mode_Out_OD, 
					GPIO_Speed_50MHz);
	GPIO_Pin_Config(CH375_Pin_Map[CH375_PIN_WR].Gpio, 
				 	CH375_Pin_Map[CH375_PIN_WR].Gpio_Pin, 
					GPIO_Mode_Out_OD, 
					GPIO_Speed_50MHz);
	GPIO_Pin_Config(CH375_Pin_Map[CH375_PIN_A0].Gpio, 
				 	CH375_Pin_Map[CH375_PIN_A0].Gpio_Pin, 
					GPIO_Mode_Out_OD, 
					GPIO_Speed_50MHz);

	GPIO_Pin_Config(CH375_Pin_Map[CH375_PIN_INT].Gpio, 
				 	CH375_Pin_Map[CH375_PIN_INT].Gpio_Pin, 
					GPIO_Mode_IPU, 
					GPIO_Speed_50MHz);

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


	err = CH375LibInit();

	gErr = err;


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

