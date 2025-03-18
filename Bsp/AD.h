#ifndef _AD_H
#define _AD_H
#include "stm32f10x.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_dma.h"
extern u16 AD_Value[2];
void AD_Init(void);
#endif
