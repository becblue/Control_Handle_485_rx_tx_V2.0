#ifndef __UART_PROTOCAL_H
#define __UART_PROTOCAL_H
#include "stm32f10x.h"
#include "StorageManager.h"
#include "STM32_Uart2.h"

typedef struct _UARTSENDSTRUCT
{
		u8 SendBuff[31];
		u8 SendLen;
}UARTSENDSTRUCT;
extern UARTSENDSTRUCT UartSendStruct;
void UartProtocal(u8 * DataBuff);
#endif
