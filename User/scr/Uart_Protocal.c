#include "Uart_Protocal.h"
u16 CRC_16(u8 *puchMsg,u8 count);
UARTSENDSTRUCT UartSendStruct;

void UartProtocal(u8 * DataBuff)
{
		u16 CRCTemp=0;
		u8 i=0;
		u32 DataTemp=0;
		CRCTemp=CRC_16(DataBuff,DataBuff[1]-2);
		
}
/********************************************************************************************
*函数名称：u16 CRC_16(u8 *puchMsg,u8 count) 
*入口参数：*puchMsg 数组的首地址；count 要计算的字节个数
*出口参数：Verify_Data： 返回的CRC校验值
*功能说明：CRC计算函数；     
*******************************************************************************************/
u16 CRC_16(u8 *puchMsg,u8 count)  //计算crc校验码
{
   	u8 i ,j;	
	u8 XORResult; 
	u16 xorCRC = 0xA001; //设置CRC的多项式 //
	u16 Verify_Data=0xffff; 
	for ( i = 0; i <count; i++) 
	{ 
		Verify_Data ^= puchMsg[i]; 	   //将0xffff和8位数据相异或
		for (j = 0; j < 8; j++) 
		{ 
			XORResult = Verify_Data & 1; 	 //判断将要移除的一位是否为一
			Verify_Data >>= 1; 
			if (XORResult) 	   //移除位为1则将crc与多项式相与
			{
				Verify_Data ^= xorCRC; 	
			}
		}
	}	
	return  Verify_Data;			//返回16位的crc校验码
}

