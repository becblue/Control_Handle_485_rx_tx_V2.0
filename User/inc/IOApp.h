#ifndef _IO_APP_H
#define _IO_APP_H
#include "includes.h"
#define DEVICE_MODE_SEND	0
#define DEVICE_MODE_RECV	1
#define DEVICE_MODE			DEVICE_MODE_SEND




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

// USART3调试功能（发送端和接收端都可用）
void USART3_Debug_Init(uint32_t baudrate);
void Debug_Print(char* str);
void Debug_PrintHex(uint32_t value);

#if DEVICE_MODE==DEVICE_MODE_SEND
// 发送模式下的调试计时器
extern uint16_t g_DebugTimer;
extern uint8_t g_HasNewDebugData;
extern uint8_t g_LatestRxData[14];
extern uint16_t g_LatestCheckSum;
extern uint8_t g_IsCrcError;
// 发送数据调试相关
extern uint8_t g_HasNewTxData;
extern uint8_t g_LatestTxData[14];
void Debug_OutputData(void);
// PA0,PA3,PA4,PA5闪烁功能相关
extern uint16_t g_BlinkTimer;
extern uint8_t g_BlinkState;
extern uint8_t g_PA_BlinkEnable[4];
extern uint8_t g_PA_NormalEnable[4];
void PA_Blink_Process(void);
#endif
#endif
