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
	
	// 初始化USART3调试接口（发送端和接收端都使用）
	USART3_Debug_Init(115200);
	
#if DEVICE_MODE==DEVICE_MODE_SEND
	AD_App_Init();
	Debug_Print("Send Mode: USART3 Debug Init OK\r\n");
#else
	AO_App_Init();
	Debug_Print("Recv Mode: USART3 Debug Init OK\r\n");
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
			
			// 缓存发送数据用于调试输出
			{
				extern uint8_t g_HasNewTxData;
				extern uint8_t g_LatestTxData[14];
				int i;
				
				// 复制发送数据
				for(i = 0; i < 14; i++) {
					g_LatestTxData[i] = Uart1_Struct.tx_data[i];
				}
				g_HasNewTxData = 1;  // 标记有新发送数据
			}
			
			USART1_SendData(Uart1_Struct.tx_data,14);
			timer.SetTimer(&timer.TimUart1SendCount,15);
		}
#endif
		if(Uart1_Struct.Receive_Complete==1)
		{
			CheckSum=Modbus_CRC_16(Uart1_Struct.rx_data,12);
			if(CheckSum==(Uart1_Struct.rx_data[12]+(Uart1_Struct.rx_data[13]<<8)))
			{
#if DEVICE_MODE==DEVICE_MODE_SEND
				// 发送模式下缓存接收到的状态数据包，等待500ms后输出
				{
					extern uint8_t g_HasNewDebugData;
					extern uint8_t g_LatestRxData[14];
					extern uint8_t g_IsCrcError;
					int i;
					
					// 复制最新的接收数据
					for(i = 0; i < 14; i++) {
						g_LatestRxData[i] = Uart1_Struct.rx_data[i];
					}
					g_IsCrcError = 0;  // CRC正确
					g_HasNewDebugData = 1;  // 标记有新数据需要输出
				}
#endif
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
			else
			{
#if DEVICE_MODE==DEVICE_MODE_SEND
				// CRC校验失败时缓存错误信息，等待500ms后输出
				{
					extern uint8_t g_HasNewDebugData;
					extern uint8_t g_LatestRxData[14];
					extern uint16_t g_LatestCheckSum;
					extern uint8_t g_IsCrcError;
					int i;
					
					// 复制最新的接收数据
					for(i = 0; i < 14; i++) {
						g_LatestRxData[i] = Uart1_Struct.rx_data[i];
					}
					g_LatestCheckSum = CheckSum;
					g_IsCrcError = 1;  // CRC错误
					g_HasNewDebugData = 1;  // 标记有新数据需要输出
				}
#endif
			}
			Uart1_Struct.Receive_Complete=0;
		}
	}
}


