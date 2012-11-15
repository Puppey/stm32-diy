#ifndef _H_24C64_H
#define _H_24C64_H
void AT24C64_Init(void);
void AT24C64_Write(unsigned char ch,unsigned short address);
unsigned char AT24C64_Read(unsigned short address);
#endif
