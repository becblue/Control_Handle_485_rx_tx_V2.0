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
*�������ƣ�u16 CRC_16(u8 *puchMsg,u8 count) 
*��ڲ�����*puchMsg ������׵�ַ��count Ҫ������ֽڸ���
*���ڲ�����Verify_Data�� ���ص�CRCУ��ֵ
*����˵����CRC���㺯����     
*******************************************************************************************/
u16 CRC_16(u8 *puchMsg,u8 count)  //����crcУ����
{
   	u8 i ,j;	
	u8 XORResult; 
	u16 xorCRC = 0xA001; //����CRC�Ķ���ʽ //
	u16 Verify_Data=0xffff; 
	for ( i = 0; i <count; i++) 
	{ 
		Verify_Data ^= puchMsg[i]; 	   //��0xffff��8λ���������
		for (j = 0; j < 8; j++) 
		{ 
			XORResult = Verify_Data & 1; 	 //�жϽ�Ҫ�Ƴ���һλ�Ƿ�Ϊһ
			Verify_Data >>= 1; 
			if (XORResult) 	   //�Ƴ�λΪ1��crc�����ʽ����
			{
				Verify_Data ^= xorCRC; 	
			}
		}
	}	
	return  Verify_Data;			//����16λ��crcУ����
}

