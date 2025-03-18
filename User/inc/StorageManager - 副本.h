/*
 * StorageManager.h
 *
 *  Created on: 2014年11月23日
 *      Author: xiao70
 */

#ifndef STORAGEMANAGER_H_
#define STORAGEMANAGER_H_
#include "includes.h"

#define StartCalPosition 		0x55
#define HaveSavedPosition 	0x55
#define ResUnitMaxNum				5
typedef struct _CONTROLLER_STORAGE
{
		u32 SaveFlag;
		u32 MiddlePositionHaveSaved;
		u32 HPositionValue[ResUnitMaxNum];
		u32 MPositionValue[ResUnitMaxNum];
		u32 LPositionValue[ResUnitMaxNum];
} CONTROLLER_STORAGE, *PCONTROLLER_STORAGE;

#define DEFAULT_STORAGE_ADDRESS 	0x800A000 //40k开始作为数据保存区
#define PAGE_SIZE 1024

#define CONTROLLER_ADDRESS 		DEFAULT_STORAGE_ADDRESS + PAGE_SIZE * 1

extern CONTROLLER_STORAGE Controller_Storage;

bool ReadcontrollerInfo(PCONTROLLER_STORAGE pControllerStorage);
bool WritecontrollerInfo(void);
	
#endif /* STORAGEMANAGER_H_ */
