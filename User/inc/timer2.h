/*-------------------------------------------------*/
/*            ��γ����STM32ϵ�п�����              */
/*-------------------------------------------------*/
/*                                                 */
/*            ʵ�ֶ�ʱ��4���ܵ�ͷ�ļ�              */
/*                                                 */
/*-------------------------------------------------*/

#ifndef _TIMER2_H
#define _TIMER2_H
#include "stm32f10x.h"
typedef struct _TIMER2_COUNTER
{
		u8 Rsv;
}TIMER2_COUNTER;
extern TIMER2_COUNTER Timer2_Counter;
void TIM2_Init(unsigned short int, unsigned short int);

#endif
