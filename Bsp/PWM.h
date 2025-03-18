#ifndef __PWM_H
#define __PWM_H
#include "stm32f10x.h"

#define duty 100
void PWM_Config(void);
void PWM_Duty_Set(u16 Duty1);
#endif
