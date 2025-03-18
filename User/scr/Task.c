
#include "includes.h"

extern SL_CONTROLLER	sl_controller;
extern TIMER	timer;
extern TIME_S	time_s;
void LED_Process(void);

void TaskProcess(void)
{
		TheoryProcess();
		IWDGProcess();
		MER_Process();
		UplinkandDownlink_Process();
		LED_Process();
}
void UplinkandDownlink_Process(void)
{
		Receive_Uplink_Data();
		if(sl_controller.Slaver_storage.slaverType==0x01)
		{
				Receive_MODBUS_Data();
				MODBUS_Protocal();
		}
		if(sl_controller.Slaver_storage.slaverType==0x00)
		{
				Receive_BACNET_Data();
				BACNET_Protocal();
		}
}

void LED_Process(void)
{
		static u8 MER_LED_Status=0,KT_LED_Status=0,MBUS_LED_Status=0;
		if(timer.TimLEDBlinkAlarm==TRUE)
		{
				if(MER_LED_Status==0)
				{
						MER_LED_OFF;
						if(sl_controller.MERLEDBlinkCount!=0x00)
						{
								MER_LED_Status=0x01;
						}
				}
				else
				{
						MER_LED_ON;
						MER_LED_Status=0x00;
						sl_controller.MERLEDBlinkCount=sl_controller.MERLEDBlinkCount-1;
				}
				
				
				if(KT_LED_Status==0)
				{
						KT_LED_ON;
						if(sl_controller.DOWNLEDBlinkCount!=0x00)
						{
								KT_LED_Status=0x01;
						}
				}
				else
				{
						KT_LED_OFF;
						KT_LED_Status=0x00;
						sl_controller.DOWNLEDBlinkCount=sl_controller.DOWNLEDBlinkCount-1;
				}
				
				
				
				if(MBUS_LED_Status==0)
				{
						MBUS_LED_OFF;
						if(sl_controller.UPLEDBlinkCount!=0x00)
						{
								MBUS_LED_Status=0x01;
						}
				}
				else
				{
						MBUS_LED_ON;
						MBUS_LED_Status=0x00;
						sl_controller.UPLEDBlinkCount=sl_controller.UPLEDBlinkCount-1;
				}

				timer.TimLEDBlinkAlarm=FALSE;
				timer.SetTimer(&timer.TimLEDBlinkCount,20);//400ms 闪灭一次。200ms为半个周期。
		}
		
}
void MER_Process(void)
{
		u16 crc_value=0;
		u32 Data_temp=0;
		u32 energy_value=0;
		u8 DataBuff[8]={0x01,0x03,0x00,0x1D,0x00,0x02,0x54,0x0D};//读取表计有功电能的指令。
		if(sl_controller.Slaver_storage.slaverType==0x00)
		{
				if(timer.TimMERRTimeoutAlarm==TRUE)
				{
						if(sl_controller.slaver_struct[0].current_compressor_status==0x01)
						{
								if(sl_controller.slaver_struct[0].current_work_mode==MODE_HEAT)
								{
										sl_controller.Energy_storage.Slaver_Electrical_Energy_L=sl_controller.Energy_storage.Slaver_Electrical_Energy_L
																																						+sl_controller.Slaver_storage.heatPower;
								}
								if((sl_controller.slaver_struct[0].current_work_mode==MODE_COOL) || (sl_controller.slaver_struct[0].current_work_mode==MODE_DRY))
								{
										sl_controller.Energy_storage.Slaver_Electrical_Energy_L=sl_controller.Energy_storage.Slaver_Electrical_Energy_L
																																						+sl_controller.Slaver_storage.coolPower;
								}
								Data_temp=(u16)(sl_controller.Energy_storage.Slaver_Electrical_Energy_L/10000);
								Data_temp=sl_controller.Energy_storage.Slaver_Electrical_Energy_H+Data_temp;
								sl_controller.Energy_storage.Slaver_Electrical_Energy_H=Data_temp;
								sl_controller.Energy_storage.Slaver_Electrical_Energy_L=sl_controller.Energy_storage.Slaver_Electrical_Energy_L%10000;
						}
				}
		}
		else
		{
				if(sl_controller.MER_RX_Buff.Receive_Complete==TRUE)
				{
						crc_value=(sl_controller.MER_RX_Buff.rx_data[8]<<8) + sl_controller.MER_RX_Buff.rx_data[7];
						if(crc_value == Modbus_CRC_16(sl_controller.MER_RX_Buff.rx_data,7))
						{
								sl_controller.MER_Err_Time=0;
								sl_controller.MERLEDBlinkCount=0x01;
								energy_value=sl_controller.MER_RX_Buff.rx_data[3];
								energy_value=energy_value<<8;
								energy_value=energy_value+sl_controller.MER_RX_Buff.rx_data[4];
								energy_value=energy_value<<8;
								energy_value=energy_value+sl_controller.MER_RX_Buff.rx_data[5];
								energy_value=energy_value<<8;
								energy_value=energy_value+sl_controller.MER_RX_Buff.rx_data[6];
								sl_controller.Energy_storage.Slaver_Electrical_Energy_H=(u32)(energy_value);
						}
						sl_controller.MER_RX_Buff.Receive_Complete=FALSE;
						sl_controller.MER_RX_Buff.rx_data[0]=0x00;
				}
				if(sl_controller.MER_Err_Time>=4)
				{
						sl_controller.MER_Err_Time=4;
						sl_controller.slaver_struct[0].current_Err_Code=sl_controller.slaver_struct[0].current_Err_Code | 0x0400;
				}
				else
				{
						sl_controller.slaver_struct[0].current_Err_Code=sl_controller.slaver_struct[0].current_Err_Code & 0xFBFF;
				}
		}
		if(timer.TimMERRTimeoutAlarm==TRUE)
		{
			
				sl_controller.MER_Err_Time++;
				timer.TimMERRTimeoutAlarm=FALSE;
				
				USART1_SendData(DataBuff,8);
				timer.SetTimer(&timer.TimMERRTimeoutCount,350);//3.6秒读一次
		}
}
void IWDGProcess(void)
{
		if(timer.TimWatchDogAlarm == TRUE)
		{
				timer.TimWatchDogAlarm = FALSE;
				timer.SetTimer(&timer.TimWatchDogCount,100);
				IWDG_ReloadCounter();//喂狗
		}
}
void TheoryProcess(void)
{
		if((timer.TimModbusStartWorkAlarm==FALSE) &&(sl_controller.Slaver_storage.slaverType==0x01))
		{
			return;
		}
		if(timer.TimTheoryAlarm == TRUE)
		{
				SD2068_I2CReadDate(&time_s);
				sl_controller.currentTime.year=time_s.year;
				sl_controller.currentTime.month=time_s.month;
				sl_controller.currentTime.day=time_s.day;
				sl_controller.currentTime.week=time_s.week;
				sl_controller.currentTime.hour=time_s.hour;
				sl_controller.currentTime.minute=time_s.minute;
				sl_controller.currentTime.second=time_s.second;
				timer.TimTheoryAlarm = FALSE;
				timer.SetTimer(&timer.TimTheoryCount,50);
				ExecuteSlaverTheroy();
		}
}
