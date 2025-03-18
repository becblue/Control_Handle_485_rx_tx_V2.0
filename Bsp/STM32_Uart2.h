#ifndef __STM32_UART2_H
#define __STM32_UART2_H
#include "stm32f10x.h"
#include "Timer.h"

typedef struct _UART2_STRUCT
{
		u8 	rx_data[63];
		u8 	tx_data[63];
		u8  Receive_Complete;	
}UART2_STRUCT;
extern UART2_STRUCT	Uart2_Struct;
void Usart2_Init(unsigned int bound);
void USART2_SendData(u8 *str,u8 length);
#endif
