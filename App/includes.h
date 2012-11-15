#ifndef _INCLUDES_H
#define _INCLUDES_H
#include "stm32f10x_lib.h"
#include "ucos_ii.h"
#include "bsp.h"
#endif

#define   BAUDRATE1  115200		// 串口1波特率
#define   BAUDRATE4	 115200	    // 串口4波特率

#define	D1_H	    GPIO_SetBits(GPIOA, GPIO_Pin_8)		 // D1指示灯 熄灭
#define	D1_L	    GPIO_ResetBits(GPIOA, GPIO_Pin_8)	 // D1指示灯 点亮 

#define	D2_H	    GPIO_SetBits(GPIOC, GPIO_Pin_9)		 // D2指示灯 熄灭
#define	D2_L	    GPIO_ResetBits(GPIOC, GPIO_Pin_9)	 // D2指示灯 点亮  

#define	D3_H	    GPIO_SetBits(GPIOC, GPIO_Pin_8)		 // D3指示灯 熄灭
#define	D3_L	    GPIO_ResetBits(GPIOC, GPIO_Pin_8)	 // D3指示灯 点亮  

#define	D4_H	    GPIO_SetBits(GPIOC, GPIO_Pin_7)		 // D4指示灯 熄灭
#define	D4_L	    GPIO_ResetBits(GPIOC, GPIO_Pin_7)	 // D4指示灯 点亮  

#define	D5_H	    GPIO_SetBits(GPIOC, GPIO_Pin_6)		 // D5指示灯 熄灭
#define	D5_L	    GPIO_ResetBits(GPIOC, GPIO_Pin_6)	 // D5指示灯 点亮  

#define	D6_H	    GPIO_SetBits(GPIOD, GPIO_Pin_15)     // D6指示灯 熄灭
#define	D6_L	    GPIO_ResetBits(GPIOD, GPIO_Pin_15)	 // D6指示灯 点亮  

#define	D7_H	    GPIO_SetBits(GPIOD, GPIO_Pin_14)	 // D7指示灯 熄灭
#define	D7_L	    GPIO_ResetBits(GPIOD, GPIO_Pin_14)	 // D7指示灯 点亮

#define	D8_H	    GPIO_SetBits(GPIOD, GPIO_Pin_13)     // D8指示灯 熄灭
#define	D8_L	    GPIO_ResetBits(GPIOD, GPIO_Pin_13)	 // D8指示灯 点亮  

#define	D9_H	    GPIO_SetBits(GPIOD, GPIO_Pin_12)	 // D9指示灯 熄灭
#define	D9_L	    GPIO_ResetBits(GPIOD, GPIO_Pin_12)	 // D9指示灯 点亮

#define	D10_H	    GPIO_SetBits(GPIOD, GPIO_Pin_11)     // D10指示灯 熄灭
#define	D10_L	    GPIO_ResetBits(GPIOD, GPIO_Pin_11)	 // D10指示灯 点亮  

#define	D11_H	    GPIO_SetBits(GPIOD, GPIO_Pin_10)	 // D11指示灯 熄灭
#define	D11_L	    GPIO_ResetBits(GPIOD, GPIO_Pin_10)	 // D11指示灯 点亮

#define	D12_H	    GPIO_SetBits(GPIOD, GPIO_Pin_9)      // D12指示灯 熄灭
#define	D12_L	    GPIO_ResetBits(GPIOD, GPIO_Pin_9)	 // D12指示灯 点亮  

#define	D13_H	    GPIO_SetBits(GPIOD, GPIO_Pin_8)	     // D13指示灯 熄灭
#define	D13_L	    GPIO_ResetBits(GPIOD, GPIO_Pin_8)	 // D13指示灯 点亮

#define	D14_H	    GPIO_SetBits(GPIOB, GPIO_Pin_15)     // D14指示灯 熄灭
#define	D14_L	    GPIO_ResetBits(GPIOB, GPIO_Pin_15)	 // D14指示灯 点亮  

#define	D15_H	    GPIO_SetBits(GPIOB, GPIO_Pin_14)	 // D15指示灯 熄灭
#define	D15_L	    GPIO_ResetBits(GPIOB, GPIO_Pin_14)	 // D15指示灯 点亮

#define	D16_H	    GPIO_SetBits(GPIOB, GPIO_Pin_13)     // D16指示灯 熄灭
#define	D16_L	    GPIO_ResetBits(GPIOB, GPIO_Pin_13)	 // D16指示灯 点亮  

#define	D18_H	    GPIO_SetBits(GPIOB, GPIO_Pin_12)	 // D18指示灯 熄灭  程序运行指示灯
#define	D18_L	    GPIO_ResetBits(GPIOB, GPIO_Pin_12)	 // D18指示灯 点亮


