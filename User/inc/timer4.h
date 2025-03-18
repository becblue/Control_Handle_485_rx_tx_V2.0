/*-------------------------------------------------*/
/*            超纬电子STM32系列开发板              */
/*-------------------------------------------------*/
/*                                                 */
/*            实现定时器4功能的头文件              */
/*                                                 */
/*-------------------------------------------------*/

#ifndef _TIMER4_H
#define _TIMER4_H
#include "stm32f10x.h"
typedef struct _TIMER4_COUNTER
{
	u32 LORASendInterval;
	u32 RS485SendInterval;
}TIMER4_COUNTER;
extern TIMER4_COUNTER Timer4_Counter;
void TIM4_Init(unsigned short int, unsigned short int);

#endif
