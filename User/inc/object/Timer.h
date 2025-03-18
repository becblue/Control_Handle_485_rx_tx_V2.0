/*
 * Controller.h
 *
 *  Created on: 2014Äê11ÔÂ23ÈÕ
 *      Author: xiao70
 */

#ifndef TIMER_H_
#define TIMER_H_
#include "includes.h"
#include <stdbool.h>

#pragma pack(push,4)

typedef struct _MEMBER_TIMER
{
  u16 Count;
  u16 Alarm;
} MEMBER_TIMER;

/************ time count ***********/
typedef struct _TIMER
{
	u16 TimDownlinkUartTimeOutCount;				u16 TimDownlinkUartTimeOutAlarm;
	u16 TimUart1SendCount;									u16 TimUart1SendAlarm;
  void (*CloseTimer)(u16 *tim);
  void (*SetTimer)(u16 *tim,u16 N10ms);
  bool (*AceeptStatus)(u16 *time);
}TIMER,*pTIMER;

#pragma pack(pop)

/*==================================================================================
@ Constants and defines
*/
extern TIMER timer;
void CloseTimer(u16 *tim);
void SetTimerN10ms(u16 *tim,u16 N10ms);
bool AceeptStatus(u16 *time);
void timerCount(pTIMER ptim);
#endif /* TIMER_H_*/
