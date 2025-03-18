#include "includes.h"
#include <stdbool.h>
#include "StorageManager.h"
#include "Flash.h"
#include <math.h>
#include "Average.h"
CONTROLLER_STORAGE Controller_Storage;
void Param_Init(void)
{
		u8 i=0;
		ReadcontrollerInfo(&Controller_Storage);
		//Controller_Storage.SaveFlag=HaveSavedPosition;
		if(Controller_Storage.SaveFlag!=HaveSavedPosition)
		{
				for(i=0;i<ResUnitMaxNum;i++)
				{
						Controller_Storage.HPositionValue[i]=0x00;
						Controller_Storage.LPositionValue[i]=0xFFFFF;
						Controller_Storage.MPositionValue[i]=0x00;
				}
				Controller_Storage.MiddlePositionHaveSaved=0;
				Controller_Storage.SaveFlag=HaveSavedPosition;
				WritecontrollerInfo();
		}
		else
		{
				Controller_Storage.MiddlePositionHaveSaved=1;
		}
		for(i=0;i<ResUnitMaxNum;i++)
		{
				Data_Array[i].ADValueInterval=(Controller_Storage.HPositionValue[i]-Controller_Storage.LPositionValue[i])/500;
		}
		
}
bool ReadcontrollerInfo(PCONTROLLER_STORAGE pControllerStorage)
{
	u32 readbyte;
	u32 startAddr = CONTROLLER_ADDRESS;
	readbyte = ReadFlash(startAddr, (u8*) pControllerStorage,
			sizeof(CONTROLLER_STORAGE));
	if (readbyte != sizeof(CONTROLLER_STORAGE))
	{
		return false;
	}
	return true;
}

bool WritecontrollerInfo(void)
{
	u32 writebyte;
	u32 startAddr = CONTROLLER_ADDRESS;
	if (!EraseFlash(startAddr, 1))
	{
		return false;
	}
	writebyte = WriteFlash(startAddr, (u8*)&Controller_Storage,
			sizeof(CONTROLLER_STORAGE));
	if (writebyte != sizeof(CONTROLLER_STORAGE))
	{
		return false;
	}
	return true;
}
