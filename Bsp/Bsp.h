
#ifndef __BSP_H
#define __BSP_H
#include "stm32f10x.h"
#include "IO.h"
/****************************结构类型**********************************/

/****************************函数声明**********************************/
void Bsp_Init(void);
void NVIC_Config(void);
void TIM2_Config(u32 T2Delayms);
void IWDG_Configuration(void);
void GPIO_Configuration(void);
void Param_Init(void);
/**************************全局变量************************************/
extern  __IO u32 test;

#endif

