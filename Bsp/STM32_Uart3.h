#ifndef __STM32_UART3_H
#define __STM32_UART3_H
#include "stm32f10x.h"
#include "Timer.h"

typedef struct _UART3_STRUCT
{
		u8 	rx_data[63];
		u8 	tx_data[63];
		u8  Receive_Complete;	
}UART3_STRUCT;
extern UART3_STRUCT	Uart3_Struct;
extern u8 Rev485Address;
void Usart3_Init(unsigned int bound);
void USART3_SendData(u8 *str,u8 length);
#endif
