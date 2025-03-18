#ifndef _IO_H
#define _IO_H
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
void IO_Init(void);
void InputData(u8 *DataBuff);
void OutputData(u8 * DataBuff);
void delay(u16 n);
#endif
