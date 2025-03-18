#include "includes.h"
#include "Bsp.h"
#include "STM32_Uart1.h"
#include "timer2.h"
#include "IO.h"
#include "ADApp.h"
#include "Average.h"
#include "crc.h"
#include "IOApp.h"
#include "AOApp.h"
#define Subdivision 500
#define Offset 600
void delay(u16 n);
//自动传数据SONG
int main(void)
{  
	uint16_t CheckSum=0;
	Bsp_Init();
	IO_App_Init();
	TIM2_Init(1000,71);
#if DEVICE_MODE==DEVICE_MODE_SEND
	AD_App_Init();
#else
	AO_App_Init();
#endif
	Usart1_Init(115200);
	while(1)
	{
#if DEVICE_MODE==DEVICE_MODE_SEND
		if(timer.TimUart1SendAlarm==true)
		{
			timer.TimUart1SendAlarm=false;
			Uart1_Struct.tx_data[0]=0xAA;
			IO_APP_Get_IO_Value(&Uart1_Struct.tx_data[1]);
			AD_App_Get(&Uart1_Struct.tx_data[4]);
			CheckSum=Modbus_CRC_16(Uart1_Struct.tx_data,12);
			Uart1_Struct.tx_data[12]=CheckSum&0xFF;
			Uart1_Struct.tx_data[13]=(CheckSum>>8)&0xFF;
			USART1_SendData(Uart1_Struct.tx_data,14);
			timer.SetTimer(&timer.TimUart1SendCount,15);
		}
#endif
		if(Uart1_Struct.Receive_Complete==1)
		{
			CheckSum=Modbus_CRC_16(Uart1_Struct.rx_data,12);
			if(CheckSum==(Uart1_Struct.rx_data[12]+(Uart1_Struct.rx_data[13]<<8)))
			{
				IO_APP_Set_IO_Value(&Uart1_Struct.rx_data[1]);
#if DEVICE_MODE==DEVICE_MODE_SEND
#else
				AO_App_Set(&Uart1_Struct.rx_data[4]);
				Uart1_Struct.tx_data[0]=0xAA;
				IO_APP_Get_IO_Value(&Uart1_Struct.tx_data[1]);
				CheckSum=Modbus_CRC_16(Uart1_Struct.tx_data,12);
				Uart1_Struct.tx_data[12]=CheckSum&0xFF;
				Uart1_Struct.tx_data[13]=(CheckSum>>8)&0xFF;
				USART1_SendData(Uart1_Struct.tx_data,14);
#endif
			}
			Uart1_Struct.Receive_Complete=0;
		}
	}
}


