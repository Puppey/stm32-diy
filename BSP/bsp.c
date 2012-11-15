#define BSP_GLOBALS
#include "includes.h"

static volatile ErrorStatus HSEStartUpStatus = SUCCESS;
static void SysTick_Configuration(void);
void GPIO_Configuration(void);
void NVIC_Configuration(void);
void RCC_Configuration(void);
void WWDG_Configuration(void);
void USART1_Configuration(void);
void USART4_Configuration(void);
void TIM1_Configuration(void);

/*
********************************************************************************
** 函数名称 ： BSP_Init(void)
** 函数功能 ： 系统初始化
** 输    入	： 无
** 输    出	： 无
** 返    回	： 无
********************************************************************************
*/
void BSP_Init(void)
{
    RCC_Configuration();
    GPIO_Configuration();
    NVIC_Configuration();
    SysTick_Configuration();
//	USART1_Configuration();
//	USART4_Configuration();
	TIM1_Configuration();
}
/*
********************************************************************************
** 函数名称 ： SysTick_Configuration(void)
** 函数功能 ： 时钟初始化
** 输    入	： 无
** 输    出	： 无
** 返    回	： 无
********************************************************************************
*/
static void SysTick_Configuration(void)
{
    RCC_ClocksTypeDef  rcc_clocks;
    u32  cnts;

    RCC_GetClocksFreq(&rcc_clocks);
    
    cnts = (u32)rcc_clocks.HCLK_Frequency/OS_TICKS_PER_SEC;

    SysTick_SetReload(cnts);
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
    SysTick_CounterCmd(SysTick_Counter_Enable);
    SysTick_ITConfig(ENABLE);
}

/*
********************************************************************************
** 函数名称 ： GPIO_Configuration(void)
** 函数功能 ： 端口初始化
** 输    入	： 无
** 输    出	： 无
** 返    回	： 无
********************************************************************************
*/
void GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | 
							RCC_APB2Periph_GPIOB | 
							RCC_APB2Periph_GPIOC | 
							RCC_APB2Periph_GPIOD | 
							RCC_APB2Periph_GPIOE, ENABLE  );
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);	 //释放JTAG脚作为普通IO口
   	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);	 //根据使用的引脚修改 

	//配置GPIOA为推挽输出，驱动LCD1602
	// PA0->E
	// PA1->RW
	// PA2->RS
	// PA4-7 -> DB4-7

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | 
								  GPIO_Pin_1 | 
								  GPIO_Pin_2 | 
								  GPIO_Pin_3 | 
								  GPIO_Pin_4 | 
								  GPIO_Pin_5 | 
								  GPIO_Pin_6 ;
   	GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		   // 最高输出速率50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);			       // 选择A端口

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 ;
   	GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		   // 最高输出速率50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);			       // 选择A端口

#if 0
     // 17路指示灯显示
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 ; 	  
  	GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_Out_PP;		   // 推挽输出
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		   // 最高输出速率50MHz
  	GPIO_Init(GPIOA, &GPIO_InitStructure);			       // 选择A端口
    
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8| GPIO_Pin_9 ; 	  
  	GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_Out_PP;		   // 推挽输出
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		   // 最高输出速率50MHz
  	GPIO_Init(GPIOC, &GPIO_InitStructure);			       // 选择C端口
  
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9|  GPIO_Pin_10| GPIO_Pin_11| 
                                  GPIO_Pin_12| GPIO_Pin_13| GPIO_Pin_14| GPIO_Pin_15 ;  
  	GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_Out_PP;		   // 推挽输出
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		   // 最高输出速率50MHz
  	GPIO_Init(GPIOD, &GPIO_InitStructure);			       // 选择C端口
 
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12| GPIO_Pin_13| GPIO_Pin_14| GPIO_Pin_15 ; 
  	GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_Out_PP;		   // 推挽输出
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		   // 最高输出速率50MHz
  	GPIO_Init(GPIOB, &GPIO_InitStructure);			       // 选择B端口
#endif
}

/*
********************************************************************************
** 函数名称 ： NVIC_Configuration(void)
** 函数功能 ： 中断初始化
** 输    入	： 无
** 输    出	： 无
** 返    回	： 无
********************************************************************************
*/
void NVIC_Configuration(void)
{
      NVIC_InitTypeDef NVIC_InitStructure; 

      //NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
      //NVIC_SystemHandlerPriorityConfig(SystemHandler_SysTick,0,0);
	  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
      //Step2.中断NVIC设置：允许中断，设置优先级 
  	  NVIC_InitStructure.NVIC_IRQChannel =  USART3_IRQChannel;    //更新事件 
	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;   //抢占优先级0 
	  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;          //响应优先级1 
	  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             //允许中断 
	  NVIC_Init(&NVIC_InitStructure);                             //写入设置 
	
	  NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQChannel;    //更新事件 
	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;   //抢占优先级0 
	  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;          //响应优先级2 
	  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             //允许中断 
	  NVIC_Init(&NVIC_InitStructure);                             //写入设置 
	
	  NVIC_InitStructure.NVIC_IRQChannel =  USART2_IRQChannel;    //更新事件 
	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;   //抢占优先级0 
	  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;          //响应优先级3 
	  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             //允许中断 
	  NVIC_Init(&NVIC_InitStructure);                             //写入设置
	
	  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQChannel;       //更新事件 
	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;   //抢占优先级0 
	  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;          //响应优先级4 
	  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             //允许中断 
	  NVIC_Init(&NVIC_InitStructure);                             //写入设置 
	 
	  NVIC_InitStructure.NVIC_IRQChannel = WWDG_IRQChannel;		  //更新事件
	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	  //抢占优先级0 
	  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5;		  //响应优先级5
	  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             //允许中断
	  NVIC_Init(&NVIC_InitStructure);							  //写入设置
												   

}

/*
********************************************************************************
** 函数名称 ： RCC_Configuration(void)
** 函数功能 ： 时钟初始化
** 输    入	： 无
** 输    出	： 无
** 返    回	： 无
********************************************************************************
*/
void RCC_Configuration(void)
{   
 	RCC_DeInit();

    RCC_HSEConfig(RCC_HSE_ON);
    HSEStartUpStatus = RCC_WaitForHSEStartUp();
    if(HSEStartUpStatus == SUCCESS)
    {
      FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
      FLASH_SetLatency(FLASH_Latency_2);
      RCC_HCLKConfig(RCC_SYSCLK_Div1);
      RCC_PCLK2Config(RCC_HCLK_Div1);
      RCC_PCLK1Config(RCC_HCLK_Div2);
      RCC_PLLConfig(RCC_PLLSource_HSE_Div1,RCC_PLLMul_9);

      RCC_PLLCmd(ENABLE);

      while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY)==RESET)
      {
       }

      RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
      
      while(RCC_GetSYSCLKSource() != 0x08)
      {
       }
      
     }
      
}

   /*
********************************************************************************
** 函数名称 ： WWDG_Configuration(void)
** 函数功能 ： 看门狗初始化
** 输    入	： 无
** 输    出	： 无
** 返    回	： 无
********************************************************************************
*/
void WWDG_Configuration(void)
{
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);
  	
  WWDG_SetPrescaler(WWDG_Prescaler_8);	              //  WWDG clock counter = (PCLK1/4096)/8 = 244 Hz (~4 ms)  
  WWDG_SetWindowValue(0x41);		                 // Set Window value to 0x41
  WWDG_Enable(0x50);		       // Enable WWDG and set counter value to 0x7F, WWDG timeout = ~4 ms * 64 = 262 ms 
  WWDG_ClearFlag();			       // Clear EWI flag
  WWDG_EnableIT();			       // Enable EW interrupt
}

/*
********************************************************************************
** 函数名称 ： TIM1_Configuration(void)
** 函数功能 ： 定时器1初始化
** 输    入	： 无
** 输    出	： 无
** 返    回	： 无
********************************************************************************
*/
void TIM1_Configuration(void)
{
  
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE );
   /* Time Base configuration */
  TIM_DeInit(TIM1);
  TIM_TimeBaseStructure.TIM_Prescaler = 71;                   //设置预分频器分频系数71，即APB2=72M, TIM1_CLK=72/72=1MHz ，
                                                               //它的取值必须在0x0000和0xFFFF之间
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; // 向上计数模式
  //TIM1_TimeBaseStructure.TIM_Period = 1000;	               // 1ms定时，计数器向上计数到1000后产生更新事件，计数值归零
  TIM_TimeBaseStructure.TIM_Period = 1000*15;	               // 15ms定时	 它的取值必须在0x0000和0xFFFF之间
  TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;			   //设置了定时器时钟分割，
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0;		  // 设置了周期计数器值，它的取值必须在0x00和0xFF之间。

  TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);	         // 根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
   
  TIM_ClearFlag(TIM1, TIM_FLAG_Update);      //清中断，以免一启用中断后立即产生中断   
  TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE); //使能TIM1中断源 
   
  TIM_Cmd(TIM1, ENABLE); 		             //TIM1总开关：开启
 
}


/*
********************************************************************************
** 函数名称 ： USART1_Configuration(void)
** 函数功能 ： 串口1初始化
** 输    入	： 无
** 输    出	： 无
** 返    回	： 无
********************************************************************************
*/
void USART1_Configuration(void)
{

USART_InitTypeDef USART_InitStructure;
USART_ClockInitTypeDef  USART_ClockInitStructure;
RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 , ENABLE  );

USART_ClockInitStructure.USART_Clock = USART_Clock_Disable;			// 时钟低电平活动
USART_ClockInitStructure.USART_CPOL = USART_CPOL_Low;				// 时钟低电平
USART_ClockInitStructure.USART_CPHA = USART_CPHA_2Edge;				// 时钟第二个边沿进行数据捕获
USART_ClockInitStructure.USART_LastBit = USART_LastBit_Disable;		// 最后一位数据的时钟脉冲不从SCLK输出
/* Configure the USART1 synchronous paramters */
USART_ClockInit(USART1, &USART_ClockInitStructure);					// 时钟参数初始化设置
																	 
USART_InitStructure.USART_BaudRate = BAUDRATE1;						  // 波特率为：115200
USART_InitStructure.USART_WordLength = USART_WordLength_8b;			  // 8位数据
USART_InitStructure.USART_StopBits = USART_StopBits_1;				  // 在帧结尾传输1个停止位
USART_InitStructure.USART_Parity = USART_Parity_No ;				  // 奇偶失能
USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	// 硬件流控制失能

USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;		  // 发送使能+接收使能
/* Configure USART1 basic and asynchronous paramters */
USART_Init(USART1, &USART_InitStructure);
    
  /* Enable USART1 */
USART_ClearFlag(USART1, USART_IT_RXNE); 			//清中断，以免一启用中断后立即产生中断
USART_ITConfig(USART1,USART_IT_RXNE, ENABLE);		//使能USART1中断源
USART_Cmd(USART1, ENABLE);							//USART1总开关：开启 


}

/*
********************************************************************************
** 函数名称 ： USART4_Configuration(void)
** 函数功能 ： 串口4初始化
** 输    入	： 无
** 输    出	： 无
** 返    回	： 无
********************************************************************************
*/

void USART4_Configuration(void)
{

USART_InitTypeDef USART_InitStructure;
RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE  );
												 
USART_InitStructure.USART_BaudRate = BAUDRATE4;						  // 波特率为：57600
USART_InitStructure.USART_WordLength = USART_WordLength_8b;			  // 8位数据
USART_InitStructure.USART_StopBits = USART_StopBits_1;				  // 在帧结尾传输1个停止位
USART_InitStructure.USART_Parity = USART_Parity_No ;				  // 奇偶失能
USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	// 硬件流控制失能

USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;		  // 发送使能+接收使能

USART_Init(UART4, &USART_InitStructure);
    
 
USART_ClearFlag(UART4, USART_IT_RXNE); 			//清中断，以免一启用中断后立即产生中断
USART_ITConfig(UART4,USART_IT_RXNE, ENABLE);		//使能USART4中断源
USART_Cmd(UART4, ENABLE);							//USART4总开关：开启 
}

