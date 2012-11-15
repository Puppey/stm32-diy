#include "DS1302.h"
#include "includes.h"

#define ds1302_sec_add			0x80		//秒数据地址
#define ds1302_min_add			0x82		//分数据地址
#define ds1302_hr_add			0x84		//时数据地址
#define ds1302_date_add			0x86		//日数据地址
#define ds1302_month_add		0x88		//月数据地址
#define ds1302_day_add			0x8a		//星期数据地址
#define ds1302_year_add			0x8c		//年数据地址
#define ds1302_control_add		0x8e		//控制数据地址
#define ds1302_charger_add		0x90 					 
#define ds1302_clkburst_add		0xbe

#define HEX2BCD(v)	((v) % 10 + (v) / 10 * 16)
#define BCD2HEX(v)	((v) % 16 + (v) / 16 * 10)

#ifndef UBYTE
#define UBYTE unsigned char
#endif

#define DS1302_GPIO	GPIOB
#define DS1302_SCLK	GPIO_Pin_0
#define DS1302_SDA	GPIO_Pin_10
#define DS1302_RST	GPIO_Pin_1	

static void DS1302_WriteByte(UBYTE addr, UBYTE d)
{
	UBYTE i;

//	DS1302_RST = 1;
	GPIO_WriteBit(DS1302_GPIO, DS1302_RST, Bit_SET);	
	
	addr = addr & 0xFE;
	for (i = 0; i < 8; i ++) 
	{	
//		DS1302_SDA = (bit)(addr & 1); 
		GPIO_WriteBit(DS1302_GPIO, DS1302_SDA, (addr & 1) ? Bit_SET : Bit_RESET);
//		DS1302_SCK = 1;
		GPIO_WriteBit(DS1302_GPIO, DS1302_SCLK, Bit_SET);
//		DS1302_SCK = 0;
		GPIO_WriteBit(DS1302_GPIO, DS1302_SCLK, Bit_RESET);
		addr >>= 1;
	}
	
	for (i = 0; i < 8; i ++) 
	{
//		DS1302_SDA = (bit)(d & 1);
		GPIO_WriteBit(DS1302_GPIO, DS1302_SDA, (d & 1) ? Bit_SET : Bit_RESET);
//		DS1302_SCK = 1;
		GPIO_WriteBit(DS1302_GPIO, DS1302_SCLK, Bit_SET);
//		DS1302_SCK = 0;
		GPIO_WriteBit(DS1302_GPIO, DS1302_SCLK, Bit_RESET);
		d >>= 1;
	}
	
//	DS1302_RST = 0;
	GPIO_WriteBit(DS1302_GPIO, DS1302_RST, Bit_RESET);
}

static UBYTE DS1302_ReadByte(UBYTE addr) 
{
	UBYTE i;
	UBYTE temp = 0;

//	DS1302_RST = 1;
	GPIO_WriteBit(DS1302_GPIO, DS1302_RST, Bit_SET);	
	addr = addr | 0x01;//最低位置高

	for (i = 0; i < 8; i ++) 
	{
//		DS1302_SDA = (bit)(addr & 1);
		GPIO_WriteBit(DS1302_GPIO, DS1302_SDA, (addr & 1) ? Bit_SET : Bit_RESET);
//		DS1302_SCK = 1;
		GPIO_WriteBit(DS1302_GPIO, DS1302_SCLK, Bit_SET);
//		DS1302_SCK = 0; 
		GPIO_WriteBit(DS1302_GPIO, DS1302_SCLK, Bit_RESET);
		addr >>= 1;
	}
	
	for (i = 0; i < 8; i ++) 
	{
		temp >>= 1;
//		if(DS1302_SDA)
		if(GPIO_ReadInputDataBit(DS1302_GPIO, DS1302_SDA))
			temp |= 0x80;
//		DS1302_SCK = 1;
		GPIO_WriteBit(DS1302_GPIO, DS1302_SCLK, Bit_SET);
//		DS1302_SCK = 0;
		GPIO_WriteBit(DS1302_GPIO, DS1302_SCLK, Bit_RESET);
	}

//	DS1302_RST = 0;
	GPIO_WriteBit(DS1302_GPIO, DS1302_RST, Bit_RESET);
	return temp;
}

void DS1302_WriteTime(UBYTE *buf) 
{	
	DS1302_WriteByte(ds1302_control_add,0x00);			//关闭写保护 
	DS1302_WriteByte(ds1302_sec_add,0x80);				//暂停 
	//Ds1302_Write_Byte(ds1302_charger_add,0xa9);			//涓流充电 
	DS1302_WriteByte(ds1302_year_add,HEX2BCD(buf[1]));		//年 
	DS1302_WriteByte(ds1302_month_add,HEX2BCD(buf[2]));	//月 
	DS1302_WriteByte(ds1302_date_add,HEX2BCD(buf[3]));		//日  
	DS1302_WriteByte(ds1302_hr_add,HEX2BCD(buf[4]));		//时 
	DS1302_WriteByte(ds1302_min_add,HEX2BCD(buf[5]));		//分
	DS1302_WriteByte(ds1302_sec_add,HEX2BCD(buf[6]));		//秒
	DS1302_WriteByte(ds1302_day_add,HEX2BCD(buf[7]));		//周 
	DS1302_WriteByte(ds1302_control_add,0x80);			//打开写保护 
}

void DS1302_ReadTime(UBYTE *buf)  
{ 
   	UBYTE tmp;
	
	tmp = DS1302_ReadByte(ds1302_year_add); 	
	buf[1] = BCD2HEX(tmp);	 //年
		 
	tmp = DS1302_ReadByte(ds1302_month_add); 	
	buf[2] = BCD2HEX(tmp);		//月
	 
	tmp = DS1302_ReadByte(ds1302_date_add); 	
	buf[3] = BCD2HEX(tmp);		//日
	 
	tmp = DS1302_ReadByte(ds1302_hr_add);		
	buf[4] = BCD2HEX(tmp);		//时
	 
	tmp = DS1302_ReadByte(ds1302_min_add);		
	buf[5] = BCD2HEX(tmp);		//分
	 
	tmp = DS1302_ReadByte((ds1302_sec_add))&0x7F; 
	buf[6] = BCD2HEX(tmp);	//秒
	 
	tmp = DS1302_ReadByte(ds1302_day_add);		
	buf[7] = BCD2HEX(tmp);		//周 
}

void DS1302_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = DS1302_SCLK | DS1302_SDA | DS1302_RST;
   	GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		   // 最高输出速率50MHz
	GPIO_Init(DS1302_GPIO, &GPIO_InitStructure);			       // 选择B端口
		
//	DS1302_RST = 0;			//RST脚置低
	GPIO_WriteBit(DS1302_GPIO, DS1302_RST, Bit_RESET);
//	DS1302_SCK = 0;			//SCK脚置低
	GPIO_WriteBit(DS1302_GPIO, DS1302_SCLK, Bit_RESET);
    DS1302_WriteByte(ds1302_sec_add,0x00);				 
}
