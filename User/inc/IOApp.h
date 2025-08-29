#ifndef _IO_APP_H
#define _IO_APP_H
#include "includes.h"
#define DEVICE_MODE_SEND	0
#define DEVICE_MODE_RECV	1
#define DEVICE_MODE			DEVICE_MODE_RECV




#if DEVICE_MODE==DEVICE_MODE_SEND
	#define GPIO_KEY_NUM		24
	#define GPIO_LED_NUM		12
#else
	#define GPIO_OUT_NUM		24
	#define GPIO_IN_NUM			12
#endif
typedef struct _IO_APP_STRUCT
{
	GPIO_TypeDef* GPIOx;
	uint16_t GPIO_Pin;
}IO_APP_STRUCT;
void IO_App_Init(void);
void IO_APP_Get_IO_Value(uint8_t * UartTxBuff);
void IO_APP_Set_IO_Value(uint8_t * UartRxBuff);
void IO_APP_Get_IO_Value_Send(uint8_t * UartTxBuff);
#endif
