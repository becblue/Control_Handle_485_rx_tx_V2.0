

#ifndef CRC_H
#define CRC_H

#include "includes.h"
extern u8 BacnetCRC8(u8 *databuf,u8 datalen);
extern u16 BacnetCRC16(u8 *databuf,u8 datalen);
u16 Modbus_CRC_16(u8 *puchMsg, u8 count);
#endif
