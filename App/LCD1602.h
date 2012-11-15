#ifndef _H_LCD1602_H
#define _H_LCD1602_H

void LCD_Init(void);
void LCD_Clear(void);
void LCD_Light(int enable);
void LCD_Write_String(unsigned char x,unsigned char y,unsigned char *s);
void LCD_Write_Char(unsigned char x,unsigned char y,unsigned char Data);

#endif
