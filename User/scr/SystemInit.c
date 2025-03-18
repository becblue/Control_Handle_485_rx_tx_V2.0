/*
 * SystemInit.c
 *
 *  Created on: 2014年11月23日
 *      Author: xiao70
 */
#include "Includes.h"

extern  SL_CONTROLLER sl_controller;

void initControllerObject(void)
{
    ReadcontrollerInfo(&sl_controller.Controller_storage);

    if(sl_controller.Controller_storage.product != CurrentProductName)
    {
        sl_controller.Controller_storage.physicsAddress=1;//地址！！！！！！！！！！！！！！！！！
				sl_controller.Controller_storage.product=CurrentProductName;
				sl_controller.Controller_storage.productNumber=0x00000001;
        WritecontrollerInfo();
    }
}

void initTheoryObject(void)
{
		u8 i=0,j=0;
    ReadtheoryInfo(&sl_controller.Theory_storage);
    if (sl_controller.Theory_storage.Data_Flag!=Data_Value)
    {
				sl_controller.Theory_storage.Data_Flag=Data_Value;
				for(i=0;i<RoomMaxNum;i++)
				{
						sl_controller.Theory_storage.Room_Storage[i].AutoOpenSetting=0x01;//默认自动运行设置为开启
						sl_controller.Theory_storage.Room_Storage[i].closetemp=20;//关闭温度20度
						sl_controller.Theory_storage.Room_Storage[i].opentemp=25;//开启温度25度
						sl_controller.Theory_storage.Room_Storage[i].WorkMode=0x02;//默认制冷
						for(j=0;j<FestivalMaxNum;j++)
						{
							sl_controller.Theory_storage.Room_Storage[i].festival_storage[j].day_range.StartMonth=0x05;//0x0c表示定时使用
							sl_controller.Theory_storage.Room_Storage[i].festival_storage[j].day_range.StartDay=0x07;
							sl_controller.Theory_storage.Room_Storage[i].festival_storage[j].day_range.StopMonth=0x09;
							sl_controller.Theory_storage.Room_Storage[i].festival_storage[j].day_range.StopDay=0x28;
							sl_controller.Theory_storage.Room_Storage[i].festival_storage[j].time_storage.GroupStartHour=0x08;
							sl_controller.Theory_storage.Room_Storage[i].festival_storage[j].time_storage.GroupStartMinute=0x00;
							sl_controller.Theory_storage.Room_Storage[i].festival_storage[j].time_storage.GroupStopHour=0x17;
							sl_controller.Theory_storage.Room_Storage[i].festival_storage[j].time_storage.GroupStopMinute=0x01;
							
							sl_controller.Theory_storage.Room_Storage[i].regular_storage.day_range.StartMonth=0x05;
							sl_controller.Theory_storage.Room_Storage[i].regular_storage.day_range.StartDay=0x01;
							sl_controller.Theory_storage.Room_Storage[i].regular_storage.day_range.StopMonth=0x09;
							sl_controller.Theory_storage.Room_Storage[i].regular_storage.day_range.StopDay=0x30;
						}
						for(j=0;j<sizeof(WEEK_STORAGE)/sizeof(DAY_STORAGE);j++)
						{
								sl_controller.Theory_storage.Room_Storage[i].regular_storage.weekStorage.day_storage[j]
								.time_storage[0].GroupStartHour=0x08;
								sl_controller.Theory_storage.Room_Storage[i].regular_storage.weekStorage.day_storage[j]
								.time_storage[0].GroupStartMinute=0x00;
								sl_controller.Theory_storage.Room_Storage[i].regular_storage.weekStorage.day_storage[j]
								.time_storage[0].GroupStopHour=0x08;
								sl_controller.Theory_storage.Room_Storage[i].regular_storage.weekStorage.day_storage[j]
								.time_storage[0].GroupStopMinute=0x30;
							
								sl_controller.Theory_storage.Room_Storage[i].regular_storage.weekStorage.day_storage[j]
								.time_storage[1].GroupStartHour=0x09;
								sl_controller.Theory_storage.Room_Storage[i].regular_storage.weekStorage.day_storage[j]
								.time_storage[1].GroupStartMinute=0x00;
								sl_controller.Theory_storage.Room_Storage[i].regular_storage.weekStorage.day_storage[j]
								.time_storage[1].GroupStopHour=0x09;
								sl_controller.Theory_storage.Room_Storage[i].regular_storage.weekStorage.day_storage[j]
								.time_storage[1].GroupStopMinute=0x30;
							
								sl_controller.Theory_storage.Room_Storage[i].regular_storage.weekStorage.day_storage[j]
								.time_storage[2].GroupStartHour=0x10;
								sl_controller.Theory_storage.Room_Storage[i].regular_storage.weekStorage.day_storage[j]
								.time_storage[2].GroupStartMinute=0x00;
								sl_controller.Theory_storage.Room_Storage[i].regular_storage.weekStorage.day_storage[j]
								.time_storage[2].GroupStopHour=0x10;
								sl_controller.Theory_storage.Room_Storage[i].regular_storage.weekStorage.day_storage[j]
								.time_storage[2].GroupStopMinute=0x30;
							
								sl_controller.Theory_storage.Room_Storage[i].regular_storage.weekStorage.day_storage[j]
								.time_storage[3].GroupStartHour=0x11;
								sl_controller.Theory_storage.Room_Storage[i].regular_storage.weekStorage.day_storage[j]
								.time_storage[3].GroupStartMinute=0x00;
								sl_controller.Theory_storage.Room_Storage[i].regular_storage.weekStorage.day_storage[j]
								.time_storage[3].GroupStopHour=0x11;
								sl_controller.Theory_storage.Room_Storage[i].regular_storage.weekStorage.day_storage[j]
								.time_storage[3].GroupStopMinute=0x30;
							
								sl_controller.Theory_storage.Room_Storage[i].regular_storage.weekStorage.day_storage[j]
								.time_storage[4].GroupStartHour=0x12;
								sl_controller.Theory_storage.Room_Storage[i].regular_storage.weekStorage.day_storage[j]
								.time_storage[4].GroupStartMinute=0x00;
								sl_controller.Theory_storage.Room_Storage[i].regular_storage.weekStorage.day_storage[j]
								.time_storage[4].GroupStopHour=0x12;
								sl_controller.Theory_storage.Room_Storage[i].regular_storage.weekStorage.day_storage[j]
								.time_storage[4].GroupStopMinute=0x30;
							
								sl_controller.Theory_storage.Room_Storage[i].regular_storage.weekStorage.day_storage[j]
								.time_storage[5].GroupStartHour=0x13;
								sl_controller.Theory_storage.Room_Storage[i].regular_storage.weekStorage.day_storage[j]
								.time_storage[5].GroupStartMinute=0x00;
								sl_controller.Theory_storage.Room_Storage[i].regular_storage.weekStorage.day_storage[j]
								.time_storage[5].GroupStopHour=0x13;
								sl_controller.Theory_storage.Room_Storage[i].regular_storage.weekStorage.day_storage[j]
								.time_storage[5].GroupStopMinute=0x30;
							
								sl_controller.Theory_storage.Room_Storage[i].regular_storage.weekStorage.day_storage[j]
								.time_storage[6].GroupStartHour=0x14;
								sl_controller.Theory_storage.Room_Storage[i].regular_storage.weekStorage.day_storage[j]
								.time_storage[6].GroupStartMinute=0x00;
								sl_controller.Theory_storage.Room_Storage[i].regular_storage.weekStorage.day_storage[j]
								.time_storage[6].GroupStopHour=0x14;
								sl_controller.Theory_storage.Room_Storage[i].regular_storage.weekStorage.day_storage[j]
								.time_storage[6].GroupStopMinute=0x30;
							
								sl_controller.Theory_storage.Room_Storage[i].regular_storage.weekStorage.day_storage[j]
								.time_storage[7].GroupStartHour=0x15;
								sl_controller.Theory_storage.Room_Storage[i].regular_storage.weekStorage.day_storage[j]
								.time_storage[7].GroupStartMinute=0x00;
								sl_controller.Theory_storage.Room_Storage[i].regular_storage.weekStorage.day_storage[j]
								.time_storage[7].GroupStopHour=0x15;
								sl_controller.Theory_storage.Room_Storage[i].regular_storage.weekStorage.day_storage[j]
								.time_storage[7].GroupStopMinute=0x30;
							
								sl_controller.Theory_storage.Room_Storage[i].regular_storage.weekStorage.day_storage[j]
								.time_storage[8].GroupStartHour=0x16;
								sl_controller.Theory_storage.Room_Storage[i].regular_storage.weekStorage.day_storage[j]
								.time_storage[8].GroupStartMinute=0x00;
								sl_controller.Theory_storage.Room_Storage[i].regular_storage.weekStorage.day_storage[j]
								.time_storage[8].GroupStopHour=0x16;
								sl_controller.Theory_storage.Room_Storage[i].regular_storage.weekStorage.day_storage[j]
								.time_storage[8].GroupStopMinute=0x30;
							
								sl_controller.Theory_storage.Room_Storage[i].regular_storage.weekStorage.day_storage[j]
								.time_storage[9].GroupStartHour=0x17;
								sl_controller.Theory_storage.Room_Storage[i].regular_storage.weekStorage.day_storage[j]
								.time_storage[9].GroupStartMinute=0x00;
								sl_controller.Theory_storage.Room_Storage[i].regular_storage.weekStorage.day_storage[j]
								.time_storage[9].GroupStopHour=0x17;
								sl_controller.Theory_storage.Room_Storage[i].regular_storage.weekStorage.day_storage[j]
								.time_storage[9].GroupStopMinute=0x30;
							
								sl_controller.Theory_storage.Room_Storage[i].regular_storage.weekStorage.day_storage[j]
								.time_storage[10].GroupStartHour=0x18;
								sl_controller.Theory_storage.Room_Storage[i].regular_storage.weekStorage.day_storage[j]
								.time_storage[10].GroupStartMinute=0x00;
								sl_controller.Theory_storage.Room_Storage[i].regular_storage.weekStorage.day_storage[j]
								.time_storage[10].GroupStopHour=0x18;
								sl_controller.Theory_storage.Room_Storage[i].regular_storage.weekStorage.day_storage[j]
								.time_storage[10].GroupStopMinute=0x30;
							
								sl_controller.Theory_storage.Room_Storage[i].regular_storage.weekStorage.day_storage[j]
								.time_storage[11].GroupStartHour=0x19;
								sl_controller.Theory_storage.Room_Storage[i].regular_storage.weekStorage.day_storage[j]
								.time_storage[11].GroupStartMinute=0x00;
								sl_controller.Theory_storage.Room_Storage[i].regular_storage.weekStorage.day_storage[j]
								.time_storage[11].GroupStopHour=0x19;
								sl_controller.Theory_storage.Room_Storage[i].regular_storage.weekStorage.day_storage[j]
								.time_storage[11].GroupStopMinute=0x30;
						}
				}
				WritetheoryInfo();
    }
}
void initSlaverObject(void)
{
		u8 i=0;
    ReadSlaverInfo(&sl_controller.Slaver_storage);
		if (sl_controller.Slaver_storage.Data_Flag!=Data_Value)
		{
			sl_controller.Slaver_storage.Data_Flag=Data_Value;
			sl_controller.Slaver_storage.coolPower=1000;
			sl_controller.Slaver_storage.heatPower=2000;
			sl_controller.Slaver_storage.roomSubNum[0]=0x00000001;
			sl_controller.Slaver_storage.roomSubNum[1]=0x00000000;
			sl_controller.Slaver_storage.roomSubNum[2]=0x00000000;
			sl_controller.Slaver_storage.roomSubNum[3]=0x00000000;
			sl_controller.Slaver_storage.roomSubNum[4]=0x00000000;
			sl_controller.Slaver_storage.roomSubNum[5]=0x00000000;
			sl_controller.Slaver_storage.roomSubNum[6]=0x00000000;
			sl_controller.Slaver_storage.roomSubNum[7]=0x00000000;
			sl_controller.Slaver_storage.slaverType=0x00;//0x01多联机，0x00单元机
			for(i=0;i<AirControllerMaxNum;i++)
			{
				sl_controller.Slaver_storage.last_Hand_Status[i]=0x00;
				sl_controller.Slaver_storage.last_open_status[i]=0x00;
				sl_controller.Slaver_storage.Current_Force_Mode[i]=0x10;
			}
			WriteSlaverInfo();
		}
		for(i=0;i<AirControllerMaxNum;i++)
		{
				sl_controller.slaver_struct[i].target_setting_temper=Operate_None;
				sl_controller.slaver_struct[i].target_open_status=Operate_None;
				sl_controller.slaver_struct[i].target_work_mode=Operate_None;
				sl_controller.slaver_struct[i].target_FAN_Speed=Operate_None;
				//以上4个操作非Operate_None时会向空调发送控制命令。控制完毕后，会恢复Operate_None状态。
				sl_controller.slaver_struct[i].last_open_status=sl_controller.Slaver_storage.last_open_status[i];
				sl_controller.slaver_struct[i].last_Hand_Status=sl_controller.Slaver_storage.last_Hand_Status[i];
				sl_controller.slaver_struct[i].Current_Force_Mode=sl_controller.Slaver_storage.Current_Force_Mode[i];
				sl_controller.slaver_struct[i].tempresultlast=FALSE;
				sl_controller.slaver_struct[i].Check_Off_Count=0;
				sl_controller.slaver_struct[i].Check_On_Count=0;
				//sl_controller.slaver_struct[i].Current_Force_Mode=0x10;
		}
}
void initControllerParam(void)
{
		sl_controller.MBUS_TX_Buff[0]=0xAA;
		sl_controller.DOWNLEDBlinkCount=0x00;
		sl_controller.UPLEDBlinkCount=0x00;
		sl_controller.MERLEDBlinkCount=0x00;
}
void initMERObject(void)
{
		ReadEnergyInfo(&sl_controller.Energy_storage);
		if(sl_controller.Energy_storage.Slaver_Electrical_Data_Flag!=Data_Value)
		{
			sl_controller.Energy_storage.Slaver_Electrical_Data_Flag=Data_Value;
			sl_controller.Energy_storage.Slaver_Electrical_Energy_H=0x00;
			sl_controller.Energy_storage.Slaver_Electrical_Energy_L=0x00;
			WriteEnergyInfo();
		}
		sl_controller.MER_Err_Time=0;
}
void Param_Init(void)
{
		initMERObject();
		initControllerParam();
		initTheoryObject();
		initControllerObject();
		initSlaverObject();
		initBACNETParam();
		initMODBUSParam();
}
