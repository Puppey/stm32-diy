#ifndef _BSP_H_
#define _BSP_H_


#ifdef  BSP_GLOBALS
#define BSP_EXT
#else
#define BSP_EXT  extern
#endif


/************************************************************************************/
void BSP_Init(void);

#endif


