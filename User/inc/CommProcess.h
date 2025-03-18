
#ifndef COMMPROCESS_H_
#define COMMPROCESS_H_
#include "stm32f10x.h"
typedef struct _COMM_DATA_STRUCT
{
		u8  Cmd_Head;
		u8	Position[10];
		u8	IO_Status;
		u16	CrcTemp;
}COMM_DATA_STRUCT;

union COMM_DATA_UNION
{
		u8 DataBuff[14];
		COMM_DATA_STRUCT Comm_Data_Struct;
};

extern union COMM_DATA_UNION Comm_Data_Union;
void CommInit(void);
#endif
