#ifndef _AO_APP_C
#define _AO_APP_C
#include "AOApp.h"
#include "AO.h"
void AO_App_Init(void)
{
	AO_Init();
}
void AO_App_Set(uint8_t * RxBuff)
{
	uint16_t InputValue=0;
	uint16_t OutputValue=0;
	// ====== PA1通道处理 ======
	InputValue =(RxBuff[0]<<8) + RxBuff[1];
	OutputValue = (uint16_t)((double)InputValue/3723*2886);
	AO_output(0,OutputValue);
	// ====== PA2通道处理 ======
	InputValue =(RxBuff[2]<<8) + RxBuff[3];
	InputValue = 3723 - InputValue;  // 反向处理：将0-3723翻转为3723-0
	OutputValue = (uint16_t)((double)InputValue/3723*2886);
	AO_output(1,OutputValue);
}
#endif
