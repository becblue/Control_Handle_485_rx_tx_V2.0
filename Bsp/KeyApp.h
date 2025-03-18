#ifndef _KEY_APP_H
#define _KEY_APP_H
#include "gpio.h"

#define WAKETIME	60
extern uint8_t FirstFlag;
void Key_Process(uint8_t SW_Value);
void Send_Unlock_N_Time(uint16_t Input);
#endif
