#include "includes.h"

void USART_Config(void)
{
	USART_ClockInitTypeDef  USART_ClockInitStructure;
	USART_InitTypeDef USART_InitStructure;	
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 |
						   RCC_APB2Periph_TIM1 | 
						   RCC_APB2Periph_GPIOA | 
						   RCC_APB2Periph_GPIOB | 
						   RCC_APB2Periph_GPIOC | 
						   RCC_APB2Periph_GPIOD, 
						   ENABLE  );

	USART_ClockInitStructure.USART_Clock = USART_Clock_Disable;			// Ê±ÖÓµÍµçÆ½»î¶¯
	USART_ClockInitStructure.USART_CPOL = USART_CPOL_Low;				// Ê±ÖÓµÍµçÆ½
	USART_ClockInitStructure.USART_CPHA = USART_CPHA_2Edge;				// Ê±ÖÓµÚ¶þ¸ö±ßÑØ½øÐÐÊý¾Ý²¶»ñ
	USART_ClockInitStructure.USART_LastBit = USART_LastBit_Disable;		// ×îºóÒ»Î»Êý¾ÝµÄÊ±ÖÓÂö³å²»´ÓSCLKÊä³ö
	/* Configure the USART1 synchronous paramters */
	USART_ClockInit(USART1, &USART_ClockInitStructure);					// Ê±ÖÓ²ÎÊý³õÊ¼»¯ÉèÖÃ	

	/* Configure USART1 Tx (PA.09) as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;				 //	Ñ¡ÖÐ¹Ü½Å9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		 // ¸´ÓÃÍÆÍìÊä³ö
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 // ×î¸ßÊä³öËÙÂÊ50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);				 // Ñ¡ÔñA¶Ë¿Ú
	
	/* Configure USART1 Rx (PA.10) as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;			  //Ñ¡ÖÐ¹Ü½Å10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//GPIO_Mode_IPU;	  //¸¡¿ÕÊäÈë
	GPIO_Init(GPIOA, &GPIO_InitStructure);				  //Ñ¡ÔñA¶ËÚ
	
	USART_DeInit(USART1);
												 
	USART_InitStructure.USART_BaudRate = 9600;						  // ²¨ÌØÂÊÎª£º115200
	USART_InitStructure.USART_WordLength = USART_WordLength_9b;			  // 8Î»Êý¾Ý
	USART_InitStructure.USART_StopBits = USART_StopBits_1;				  // ÔÚÖ¡½áÎ²´«Êä1¸öÍ£Ö¹Î»
	USART_InitStructure.USART_Parity = USART_Parity_No ;				  // ÆæÅ¼Ê§ÄÜ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	// Ó²¼þÁ÷¿ØÖÆÊ§ÄÜ
	
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;		  // ·¢ËÍÊ¹ÄÜ+½ÓÊÕÊ¹ÄÜ
	/* Configure USART1 basic and asynchronous paramters */
	USART_Init(USART1, &USART_InitStructure);
	    
	  /* Enable USART1 */
	USART_ClearFlag(USART1, USART_IT_RXNE); 			//ÇåÖÐ¶Ï£¬ÒÔÃâÒ»ÆôÓÃÖÐ¶ÏºóÁ¢¼´²úÉúÖÐ¶Ï
	USART_ITConfig(USART1,USART_IT_RXNE, DISABLE);		//Ê¹ÄÜUSART1ÖÐ¶ÏÔ´
	USART_Cmd(USART1, ENABLE);
}

void USART_SendChar(unsigned short ch)
{
	USART_SendData(USART1, ch);
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET){}	
}

unsigned short USART_ReadChar(void)
{
	unsigned short ch = 0;

	while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET){}

	ch = USART_ReceiveData(USART1);

	return ch;
}

