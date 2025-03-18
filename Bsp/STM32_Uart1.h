#ifndef __STM32_UART2_H
#define __STM32_UART2_H
#include "stm32f10x.h"
#include "Timer.h"

typedef struct _UART1_STRUCT
{
		u8 	rx_data[63];
		u8 	tx_data[63];
		u8  Receive_Complete;	
}UART1_STRUCT;
extern UART1_STRUCT	Uart1_Struct;
void Usart1_Init(unsigned int bound);
void USART1_SendData(u8 *str,u8 length);
#endif
