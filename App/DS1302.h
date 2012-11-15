#ifndef _H_DS1302_H
#define _H_DS1302_H
void DS1302_Init(void);
void DS1302_ReadTime(unsigned char *buf);
void DS1302_WriteTime(unsigned char *buf); 
#endif
