#ifndef _KEY_APP_C
#define _KEY_APP_C
#include "KeyApp.h"
#include "Key.h"
#include "LoraApp.h"
#include "Storage.h"
#include "main.h"
#include "oled.h"
#include "oledApp.h"
void Send_N_Times(void);
uint8_t ResetSleepCountAfterSendFlag=0;
static uint8_t LastAddress=0;
static uint16_t SendCount=0;
static uint8_t SendOnceFlag=0;
static uint8_t buff[8]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
void Key_Process(uint8_t SW_Value)
{
	static uint32_t LoraTimeOut=0;

	if(Key_Struct.KeyBuzzerCount>0)
	{
			Key_Struct.KeyBuzzerCount--;
			Gpio_SetIO(BUZZER_PORT, BUZZER_PIN);
	}
	else
	{
			Gpio_ClrIO(BUZZER_PORT, BUZZER_PIN);
	}
	LoraTimeOut++;
	if(LoraTimeOut>2000)
	{
		LoraTimeOut=0;
		LoraInit(433000000+(1000000*SW_Value));
	}
	if(GetLoraRecvFlag()!=0)
	{
		LoraTimeOut=0;
 		if(GetLoraRecvFlag()==1)
		{
			if(RX_Buffer[7]== CheckSum(RX_Buffer,7))
			{
				if(RX_Buffer[0]==0xF0)
				{
					Storage_Struct.RollingCode=(RX_Buffer[3]<<24) + (RX_Buffer[4]<<16) + (RX_Buffer[5]<<8) + (RX_Buffer[6]<<0);
					Storage_Write();
					delay1ms(100);
					__NVIC_SystemReset();
				}
				else
				{
					if(RX_Buffer[0] == 0x01 
						&& Storage_Struct.RollingCode ==((RX_Buffer[3]<<24) + (RX_Buffer[4]<<16) + (RX_Buffer[5]<<8) + (RX_Buffer[6]<<0))
					  && Storage_Struct.RollingCode!=0xFFFFFFFF
						)
					{
						UpdateRSSI();
						if(((RX_Buffer[1]>>6)&0x03)!=0x03)
						{
							Total_Struct.CurrentDir=(RX_Buffer[1]>>6)&0x3;
							if(Total_Struct.CurrentDir == DIR_STOP)
							{
								if(ResetSleepCountAfterSendFlag==1)
								{
									ResetSleepCountAfterSendFlag=0;
									Total_Struct.SleepCount = 5;
								}
							}
							Total_Struct.CurrentLockFlag=(RX_Buffer[1]>>5)&0x1;
							if(Key_Struct.KeyPressFlag==1)
							{
								Key_Struct.KeyPressFlag=0;
								if(((Total_Struct.CurrentLockFlag&0x01)==0)||(((RX_Buffer[1]>>6)&0x03)==0x01))
								{
									Key_Struct.KeyBuzzerCount=50;
								}
							}
							Total_Struct.CurrentTimeOut=((RX_Buffer[1]&0x1F)<<8) + RX_Buffer[2];
						}
					}
				}
			}
		}
		ClrLoraRecvFlag();
	}
	if(SendCount==0)
	{
		SendCount=400;
		if(SendOnceFlag>0 && (LastAddress==Total_Struct.CurrentAddress))
		{
			SendOnceFlag--;
			ResetSleepCountAfterSendFlag=1;
			SetLoraRSSI((int16_t)(-150));
			UpdateRSSI();
			buff[0] = 0x80+(Total_Struct.CurrentAddress-1);
			buff[3] = (Storage_Struct.RollingCode>>24)&0xFF;
			buff[4] = (Storage_Struct.RollingCode>>16)&0xFF;
			buff[5] = (Storage_Struct.RollingCode>>8)&0xFF;
			buff[6] = (Storage_Struct.RollingCode>>0)&0xFF;
			buff[7] = CheckSum(buff,7);
			LoraSend(buff,8);
		}
		else
		{
			SendOnceFlag=0;
			LastAddress=Total_Struct.CurrentAddress;
			SetLoraRSSI((int16_t)(-150));
			UpdateRSSI();
			buff[1] = DIR_INIT;
			buff[2] = 0x01;
			buff[0] = 0x80+(Total_Struct.CurrentAddress-1);
			buff[3] = (Storage_Struct.RollingCode>>24)&0xFF;
			buff[4] = (Storage_Struct.RollingCode>>16)&0xFF;
			buff[5] = (Storage_Struct.RollingCode>>8)&0xFF;
			buff[6] = (Storage_Struct.RollingCode>>0)&0xFF;
			buff[7] = CheckSum(buff,7);
			LoraSend(buff,8);
		}
	}
	if(SendCount>0)
	{
		SendCount--;
	}
	Radio.IrqProcess();
	ReadKey();
	if(GetKeyCount(KEY3)>ShortPress)
	{
		Total_Struct.SleepCount=WAKETIME;
		buff[1] = DIR_FORWARD;
		buff[2] = 0x00;
		Send_N_Times();
		ClrKeyCount(KEY3);
	}
	if(GetKeyValue(KEY3)==1)
	{
		buff[1] = DIR_FORWARD;
		buff[2] = 0x00;
		Total_Struct.SleepCount=WAKETIME;
		ClrKeyValue(KEY3);
		ClrKeyCount(KEY3);
	}
	if(GetKeyCount(KEY4)>ShortPress)
	{
		Total_Struct.SleepCount=WAKETIME;
		buff[1] = DIR_STOP;
		buff[2] = 0x00;
		Send_N_Times();
		ClrKeyCount(KEY4);
	}
	if(GetKeyValue(KEY4)==1)
	{
		Total_Struct.SleepCount=WAKETIME;
		buff[1] = DIR_STOP;
		buff[2] = 0x00;
		ClrKeyValue(KEY4);
		ClrKeyCount(KEY4);
	}
	if(GetKeyCount(KEY5)>ShortPress)
	{
		Total_Struct.SleepCount=WAKETIME;
		buff[1] = DIR_REVERSE;
		buff[2] = 0x00;
		Send_N_Times();
		ClrKeyCount(KEY5);
	}
	if(GetKeyValue(KEY5)==1)
	{
		Total_Struct.SleepCount=WAKETIME;
		buff[1] = DIR_REVERSE;
		buff[2] = 0x00;
		ClrKeyValue(KEY5);
		ClrKeyCount(KEY5);
	}
	if(GetKeyValue(KEY6)==1)
	{
		Total_Struct.SleepCount=WAKETIME;
		buff[1] = DIR_STOP;
		buff[2] = 0x00;
		ClrKeyValue(KEY6);
		ClrKeyCount(KEY6);
	}
	if((GetKeyCount(KEY6)>ShortPress))
	{
		Total_Struct.SleepCount=WAKETIME;
		buff[1] = DIR_STOP;
		buff[2] = 0x00;
		ClrKeyCount(KEY6);
		Send_N_Times();
	}
	if(GetKeyValue(KEY7)==1)
	{
		Total_Struct.SleepCount=WAKETIME;
		buff[1] = DIR_INIT;
		buff[2] = 0x02;
		ClrKeyValue(KEY7);
		ClrKeyCount(KEY7);
	}
	if((GetKeyCount(KEY7)>ShortPress))
	{
		Total_Struct.SleepCount=WAKETIME;
		buff[1] = DIR_INIT;
		buff[2] = 0x02;
		ClrKeyCount(KEY7);
		Send_N_Times();
	}
	if(GetKeyValue(KEY8)==1)
	{
		Total_Struct.SleepCount=WAKETIME;
		buff[1] = DIR_INIT;
		buff[2] = 0x04;
		ClrKeyValue(KEY8);
		ClrKeyCount(KEY8);
	}
	if(GetKeyCount(KEY8)>ShortPress)
	{
		Total_Struct.SleepCount=WAKETIME;
		buff[1] = DIR_INIT;
		buff[2] = 0x04;
		ClrKeyCount(KEY8);
		Send_N_Times();
	}
	if(GetKeyValue(KEY9)==1)
	{
		Total_Struct.SleepCount=WAKETIME;
		if((Total_Struct.CurrentAddress+10)>99)
		{
			Total_Struct.CurrentAddress=99;
		}
		else
		{
			Total_Struct.CurrentAddress=Total_Struct.CurrentAddress+10;
		}
		ClrKeyValue(KEY9);
	}
	if(GetKeyValue(KEY10)==1)
	{
		Total_Struct.SleepCount=WAKETIME;
		if(Total_Struct.CurrentAddress>10)
		{
			Total_Struct.CurrentAddress=Total_Struct.CurrentAddress-10;
		}
		else
		{
			Total_Struct.CurrentAddress=1;
		}
		ClrKeyValue(KEY10);
	}
	if(GetKeyValue(KEY11)==1)
	{
		Total_Struct.SleepCount=WAKETIME;
		if((Total_Struct.CurrentAddress+1)>99)
		{
			Total_Struct.CurrentAddress=99;
		}
		else
		{
			Total_Struct.CurrentAddress=Total_Struct.CurrentAddress+1;
		}
		ClrKeyValue(KEY11);
	}
	if(GetKeyValue(KEY12)==1)
	{
		Total_Struct.SleepCount=WAKETIME;
		if(Total_Struct.CurrentAddress>1)
		{
			Total_Struct.CurrentAddress=Total_Struct.CurrentAddress-1;
		}
		else
		{
			Total_Struct.CurrentAddress=1;
		}
		ClrKeyValue(KEY12);
	}
}
void Send_N_Times(void)
{
	LastAddress=Total_Struct.CurrentAddress;
	SendOnceFlag=3;
}
void Send_Unlock_N_Time(uint16_t Input)
{
	SendCount=0;
	if(((1<<KEY5Pin)&Input)==0)
	{
		buff[1] = DIR_REVERSE;
		buff[2] = 0x00;
	}
	if(((1<<KEY4Pin)&Input)==0)
	{
		buff[1] = DIR_STOP;
		buff[2] = 0x00;
	}
	if(((1<<KEY3Pin)&Input)==0)
	{
		buff[1] = DIR_FORWARD;
		buff[2] = 0x00;
	}
	if(((1<<KEY6Pin)&Input)==0)
	{
		buff[1] = DIR_STOP;
		buff[2] = 0x00;
	}
	if(((1<<KEY7Pin)&Input)==0)
	{
		buff[1] = DIR_INIT;
		buff[2] = 0x02;
	}
	Send_N_Times();
}
#endif
