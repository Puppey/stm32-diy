#include "LCD1602.h"
#include "includes.h"

#define LCD_GPIO	GPIOA
#define LCD_EN	GPIO_Pin_0
#define LCD_RW	GPIO_Pin_1
#define LCD_RS	GPIO_Pin_2
#define LCD_D7	GPIO_Pin_7
#define LCD_D6	GPIO_Pin_6
#define LCD_D5	GPIO_Pin_5
#define LCD_D4	GPIO_Pin_4
#define LCD_BLK	GPIO_Pin_3

#define EN_CLR \
	GPIO_WriteBit(LCD_GPIO, LCD_EN, Bit_RESET)
#define EN_SET \
	GPIO_WriteBit(LCD_GPIO, LCD_EN, Bit_SET)

#define RW_CLR \
	GPIO_WriteBit(LCD_GPIO, LCD_RW, Bit_RESET)
#define RW_SET \
	GPIO_WriteBit(LCD_GPIO, LCD_RW, Bit_SET)

#define RS_CLR \
	GPIO_WriteBit(LCD_GPIO, LCD_RS, Bit_RESET)
#define RS_SET \
	GPIO_WriteBit(LCD_GPIO, LCD_RS, Bit_SET)

#define BLK_ON \
	GPIO_WriteBit(LCD_GPIO, LCD_BLK, Bit_SET)

#define BLK_OFF \
	GPIO_WriteBit(LCD_GPIO, LCD_BLK, Bit_RESET)	

static unsigned char framebuffer[16 * 2] = {0,};

static int LCD_Check_Busy(void)
{
	unsigned char dat = 0;

	GPIO_WriteBit(LCD_GPIO, LCD_D7, Bit_SET);

	RS_CLR;
	RW_SET;
	EN_SET;
	
	dat = GPIO_ReadInputDataBit(LCD_GPIO, LCD_D7);
	
	return dat;	 	
}

static void LCD_Write_HalfByte(unsigned char data)
{
	RW_CLR;
	EN_SET;
	GPIO_WriteBit(LCD_GPIO, LCD_D4, (data & (1 << 4)) ? Bit_SET : Bit_RESET);
	GPIO_WriteBit(LCD_GPIO, LCD_D5, (data & (1 << 5)) ? Bit_SET : Bit_RESET);
	GPIO_WriteBit(LCD_GPIO, LCD_D6, (data & (1 << 6)) ? Bit_SET : Bit_RESET);
	GPIO_WriteBit(LCD_GPIO, LCD_D7, (data & (1 << 7)) ? Bit_SET : Bit_RESET);
	EN_CLR;
}

static void LCD_Write_Byte(unsigned command, unsigned data)
{
	while(LCD_Check_Busy());
	if(command)
		RS_CLR;
	else
		RS_SET;
		
	LCD_Write_HalfByte(data);

	LCD_Write_HalfByte(data << 4);
}

void LCD_Light(int enable)
{
	if(enable)
		BLK_ON;
	else
		BLK_OFF;
}

void LCD_Init(void)
{
	EN_CLR;
	RW_CLR;
	RS_CLR;
	
	GPIO_WriteBit(LCD_GPIO, LCD_BLK, Bit_SET);
		
	GPIO_WriteBit(LCD_GPIO, LCD_D4, Bit_RESET);	
	GPIO_WriteBit(LCD_GPIO, LCD_D5, Bit_RESET);	
	GPIO_WriteBit(LCD_GPIO, LCD_D6, Bit_RESET);	
	GPIO_WriteBit(LCD_GPIO, LCD_D7, Bit_RESET);

	LCD_Write_Byte(1, 0x33);	
	LCD_Write_Byte(1, 0x32);
	LCD_Write_Byte(1, 0x28);
	LCD_Write_Byte(1, 0x0c);
	LCD_Write_Byte(1, 0x01);
	LCD_Write_Byte(1, 0x06);
	LCD_Clear();
} 

void LCD_Clear(void)
{
	LCD_Write_Byte(1, 0x01);
	memset(framebuffer, 0, sizeof(framebuffer));
}

void LCD_Write_String(unsigned char x,unsigned char y,unsigned char *s) 
{     
	unsigned char *p = framebuffer + y * 16 + x;

	while(*s && (*p == *s))
	{
		s++;
		p++;
		x++;
	}

	if(!*s)
		return;

	LCD_Write_Byte(1, ((y == 0) ? 0x80 : 0xc0) + x);
     
	while (*s) 
	{     
		LCD_Write_Byte(0, *s);     
		*p = *s;
		s++;     
	}
}

void LCD_Write_Char(unsigned char x,unsigned char y,unsigned char Data) 
{
	if(framebuffer[y * 16 + x] != Data)
	{	     
		LCD_Write_Byte(1, ((y == 0) ? 0x80 : 0xc0) + x);        
		LCD_Write_Byte(0, Data);
		framebuffer[y * 16 + x] = Data; 
	} 
}
