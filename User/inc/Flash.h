#ifndef _FALSE_H_ 
#define _FALSE_H_

#include "includes.h"

extern u32 WriteFlash(u32 startAddr, u8 *pData, u32 size);
extern u32 ReadFlash(u32 startAddr, u8 *pData, u32 size);
extern bool EraseFlash(u32 startAddr, u8 erasePage);

#endif

