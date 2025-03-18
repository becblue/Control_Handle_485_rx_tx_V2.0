#ifndef _AD_APP_C
#define _AD_APP_C
#include "ADApp.h"
void AD_App_Init(void)
{
	AD_Init();
}
void AD_App_Get(uint8_t * TxBuff)
{
	TxBuff[0]=(AD_Value[0]>>8)&0xFF;
	TxBuff[1]=(AD_Value[0]>>0)&0xFF;
	TxBuff[2]=(AD_Value[1]>>8)&0xFF;
	TxBuff[3]=(AD_Value[1]>>0)&0xFF;
}
#endif
