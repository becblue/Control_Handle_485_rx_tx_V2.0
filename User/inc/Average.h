#include "stm32f10x.h"
#define ArrayLen 4
#define ResUnitMaxNum 5
typedef struct _DATA_ARRAY
{
		u16 ADValueInterval;//ADֵ��Ϊ120�ݵ�ʱ��ÿ�ݵĲ�ֵ��
		u16 ADValueAfterAverage;
		u16 Data[ArrayLen];
		s8 Index;
}DATA_ARRAY;
extern DATA_ARRAY Data_Array[ResUnitMaxNum];
u16 Average(u16 *DataArray,u16 len);
