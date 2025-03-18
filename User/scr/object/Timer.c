

#include "includes.h"

void CloseTimer(u16 *tim);
void SetTimerN10ms(u16 *tim,u16 N10ms);
bool AceeptStatus(u16 *time);

TIMER timer =
{
  5,false,
  10,true,
	
  CloseTimer,
  SetTimerN10ms,
  AceeptStatus
};


/*********************************************************************************
Function:     CoseTimer
Description:  time2 overflow interrupt.

Input:        u16 tim
Output:       None
Return:       None
Others:	      None
*********************************************************************************/
void CloseTimer(u16 *tim)
{
  *tim = 0;
  return;
}

void SetTimerN10ms(u16 *tim,u16 N10ms)
{
  *tim =  N10ms + 1;
  return;
}

bool AceeptStatus(u16 *time)
{
  if(*time != 0)
    return true;// timer is running.
  else
    return true;//timer has been closed.
}

/*********************************************************************************
Function:     timerCount
Description:  calculate delay timer.

Input:        point of timer struct
Output:       None
Return:       None
Others:	      None
*********************************************************************************/
void timerCount(pTIMER ptim)
{
  u8 *Count = (u8*)ptim;
  u16 top = (u16)(sizeof(TIMER)-0x0C);
	u16 numb = 0;
  for(numb = 0;numb < top; numb +=4 )
  {
    if( (*(u16*)Count) > 1)
    {
      (*(u16*)Count)  --;
    }
    else if((*(u16*)Count) == 1 )
    {
      (*(u16*)Count) = 0; // close timer Count
      Count += 2;
      *Count = true; // ALarm set true
      Count += 2;
      continue;
    }
    Count += 4;
  }
}

