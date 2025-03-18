#include "Average.h"
DATA_ARRAY Data_Array[ResUnitMaxNum];
u16 Average(u16 *DataArray,u16 len)
{
		volatile u32 WeightSum=0;
		volatile s32 DataArrayAfterSort[ArrayLen];
		volatile s32 TempValue=0;
		volatile double KTemp=0;
		int i=0,j=0;
		for(i=0;i<len;i++)
		{
				DataArrayAfterSort[i]=*(DataArray+i);
		}
		for(i=0;i<len-1;i++)
		{
				for(j=0;j<len-1-i;j++)
				{
						if(DataArrayAfterSort[j]>DataArrayAfterSort[j+1])
						{
								u32 tmp=DataArrayAfterSort[j+1];
								DataArrayAfterSort[j+1]=DataArrayAfterSort[j];
								DataArrayAfterSort[j]=tmp;
						}
				}
		}
		for(i=1;i<len-1;i++)
		{
				WeightSum=WeightSum+DataArrayAfterSort[i];
		}
		WeightSum=WeightSum/(len-2);
		return (u16)(WeightSum);
}
