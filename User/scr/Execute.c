/*
 * Execute.c
 *
 *  Created on: 2014年11月29日
 *      Author: xiao70
 */
#include "includes.h"

extern SL_CONTROLLER sl_controller;
extern TIMER	timer;
static s8 InTempTheory(u8 RoomNum ,u8 ControllerNum);
static s8 InTimeTheory(u8 RoomNum);
static bool todayInDate(u32 startMonth, u32 startDay, u32 endMonth, u32 endDay);
static bool IsInvalidDay(u32 day);
static bool IsInvalidMonth(u32 month);
static bool IsInvalidTimeMinute(u8 timeMinute);
static bool IsInvalidTimeHour(u8 timeHour);
static u8 Festival_Judge(u8 RoomNum,PWEEK_STORAGE pweek_storage);

void ExecuteSlaverTheroy(void)
{
	u8 	ControllerNum=0;
	u8	ControllerMaxNum=0;
	u8  RoomIndex=0;//表示当前空调所处的房间号。
	u8  ZeroCount=0;//记录各个房间相同空调号位置为0的个数。
	u8  i;
	if(timer.TimMBUSTimeOutAlarm==TRUE)
	{
		return;
	}
	if(sl_controller.Slaver_storage.slaverType==0x00)
	{
		//单元机
		ControllerMaxNum=1;
	}
	else
	{
		ControllerMaxNum=AirControllerMaxNum;
	}
	for(ControllerNum=0;ControllerNum<ControllerMaxNum;ControllerNum++)
	{
		if(timer.TimHandTimeOutTimer[ControllerNum].Alarm == TRUE)
		{
				if(sl_controller.slaver_struct[ControllerNum].last_open_status!=sl_controller.slaver_struct[ControllerNum].current_open_status)
				{
						timer.TimHandTimeOutTimer[ControllerNum].Alarm = FALSE;
						//如果手动操作10秒后，当前读取状态和上次设定状态不同，则表明设备超时，或者有遥控器操作，则手动判定无效。
						sl_controller.slaver_struct[ControllerNum].last_Hand_Status=0x00;//返回自动
				}
		}
		ZeroCount=0;
		for(i=0;i<RoomMaxNum;i++)
		{
				if(((sl_controller.Slaver_storage.roomSubNum[i]>>ControllerNum)&0x01)==0x00 )
				{
					ZeroCount++;
				}
		}
		if(ZeroCount==RoomMaxNum)
		{
			return;
		}
		for(i=0;i<RoomMaxNum;i++)
		{
				if(((sl_controller.Slaver_storage.roomSubNum[i]>>ControllerNum)&0x01)==0x01 )
				{
					RoomIndex=i;
					break;
				}
		}
		if(sl_controller.slaver_struct[ControllerNum].last_Hand_Status==0x01)
		{
			//如果手动操作了，且没有恢复，则不执行策略。
			if(sl_controller.slaver_struct[ControllerNum].last_open_status==0x00)
			{
				sl_controller.slaver_struct[ControllerNum].target_open_status=0x00;
			}
			continue;
		}
		if(sl_controller.Theory_storage.Room_Storage[RoomIndex].WorkMode==MODE_COOL)
		{
				//制冷模式，如果当前设置温度小于开启温度，将设置温度改回开启温度
				if(sl_controller.slaver_struct[ControllerNum].current_setting_temper<sl_controller.Theory_storage.Room_Storage[RoomIndex].closetemp)
				{
						sl_controller.slaver_struct[ControllerNum].target_setting_temper=sl_controller.Theory_storage.Room_Storage[RoomIndex].closetemp;
				}
		}
		if(sl_controller.Theory_storage.Room_Storage[RoomIndex].WorkMode==MODE_HEAT)
		{
				//制热模式，如果当前设置温度高于开启温度，将设置温度改回开启温度
				if(sl_controller.slaver_struct[ControllerNum].current_setting_temper>sl_controller.Theory_storage.Room_Storage[RoomIndex].closetemp)
				{
						sl_controller.slaver_struct[ControllerNum].target_setting_temper=sl_controller.Theory_storage.Room_Storage[RoomIndex].closetemp;
				}
		}
		if((InTimeTheory(RoomIndex)==ForceClose))
		{
				sl_controller.slaver_struct[ControllerNum].target_open_status=0x00;//如果条件不符合直接关闭。
				sl_controller.slaver_struct[ControllerNum].last_open_status=0x00;
				sl_controller.slaver_struct[ControllerNum].target_work_mode=sl_controller.Theory_storage.Room_Storage[RoomIndex].WorkMode;
				continue;
		}
		if(InTempTheory(RoomIndex,ControllerNum)==TRUE)
		{
			if(sl_controller.Theory_storage.Room_Storage[RoomIndex].AutoOpenSetting==AutoOpenSetting_Auto)
			{
				if(sl_controller.slaver_struct[ControllerNum].last_open_status==0x00)
				{
					//如果自动启动开启，且前一个状态为关闭，则开启。开启后则不再开启。
					//如果自动启动关闭，则不操作。
					timer.TimONRepeatTimer[ControllerNum].Alarm = FALSE;
					if(sl_controller.Slaver_storage.slaverType==0x01)
					{
							timer.SetTimer(&timer.TimONRepeatTimer[ControllerNum].Count,6000);//多联机如果开启后当前状态不为开启，则连续10分钟开
					}
					else
					{
							timer.SetTimer(&timer.TimONRepeatTimer[ControllerNum].Count,3000);//多联机如果开启后当前状态不为开启，则连续5分钟开
					}
					sl_controller.slaver_struct[ControllerNum].target_open_status=0x01;
					sl_controller.slaver_struct[ControllerNum].last_open_status=0x01;
					sl_controller.slaver_struct[ControllerNum].target_work_mode=sl_controller.Theory_storage.Room_Storage[RoomIndex].WorkMode;
					if(sl_controller.Theory_storage.Room_Storage[RoomIndex].closetemp==0xff)
					{
							sl_controller.slaver_struct[ControllerNum].target_setting_temper=25;
					}
					else
					{
							sl_controller.slaver_struct[ControllerNum].target_setting_temper=sl_controller.Theory_storage.Room_Storage[RoomIndex].closetemp;
					}
					sl_controller.slaver_struct[ControllerNum].target_FAN_Speed=sl_controller.slaver_struct[RoomIndex].current_FAN_Speed;
				}
			}
		}
		else
		{
			sl_controller.slaver_struct[ControllerNum].target_open_status=0x00;//如果条件不符合直接关闭。
			sl_controller.slaver_struct[ControllerNum].last_open_status=0x00;
			sl_controller.slaver_struct[ControllerNum].tempresultlast=FALSE;
			//sl_controller.slaver_struct[ControllerNum].target_work_mode=sl_controller.Theory_storage.Room_Storage[RoomIndex].WorkMode;
					
		}
	}
	
}

//判断温度是否满足，如果有故障就不判断
//返回1符合条件，返回0不符合条件
static s8 InTempTheory(u8 RoomNum ,u8 ControllerNum)
{
	if((sl_controller.Theory_storage.Room_Storage[RoomNum].hasFlag & HAS_TEMP) !=HAS_TEMP)
	{
			//如果温度策略不生效，则温度策略限制取消
			return TRUE;
	}
	if(((sl_controller.Slaver_storage.roomSubNum[RoomNum]>>ControllerNum)&0x01)==0x01)
	{
			if(sl_controller.Theory_storage.Room_Storage[RoomNum].WorkMode==MODE_COOL)
			{
					if(sl_controller.slaver_struct[ControllerNum].current_indoor_temper>=sl_controller.Theory_storage.Room_Storage[RoomNum].opentemp)
					{
							
							sl_controller.slaver_struct[ControllerNum].tempresultlast=TRUE;
							return TRUE;
					}
					else if(sl_controller.slaver_struct[ControllerNum].current_indoor_temper<=sl_controller.Theory_storage.Room_Storage[RoomNum].closetemp)
					{
							sl_controller.slaver_struct[ControllerNum].tempresultlast=FALSE;
							return FALSE;
					}
					else
					{
							return sl_controller.slaver_struct[ControllerNum].tempresultlast;
					}
			}
			if(sl_controller.Theory_storage.Room_Storage[RoomNum].WorkMode==MODE_HEAT)
			{
					if(sl_controller.slaver_struct[ControllerNum].current_indoor_temper<=sl_controller.Theory_storage.Room_Storage[RoomNum].opentemp)
					{
							sl_controller.slaver_struct[ControllerNum].tempresultlast=TRUE;
							return TRUE;
					}
					else if(sl_controller.slaver_struct[ControllerNum].current_indoor_temper>=sl_controller.Theory_storage.Room_Storage[RoomNum].closetemp)
					{
							sl_controller.slaver_struct[ControllerNum].tempresultlast=FALSE;
							return FALSE;
					}
					else
					{
							return sl_controller.slaver_struct[ControllerNum].tempresultlast;
					}
			}
	}
	else
	{
			return FALSE;
	}
	return FALSE;
}

static bool todayInDate(u32 startMonth, u32 startDay, u32 endMonth, u32 endDay)
{
		u16 currentdaycount=0;
		u16 startdaycount=0;
		u16	enddaycount=0;
		u32 startMonth_Hex;
		u32 startDay_Hex;
		u32 endMonth_Hex;
		u32 endDay_Hex;
		startMonth_Hex=BCDtoHEX(startMonth);
		startDay_Hex=BCDtoHEX(startDay);
		endMonth_Hex=BCDtoHEX(endMonth);
		endDay_Hex=BCDtoHEX(endDay);
    if(IsInvalidMonth(startMonth_Hex) || IsInvalidMonth(endMonth_Hex))
    {
        return FALSE;
    }
    if(IsInvalidDay(startDay_Hex) || IsInvalidDay(endDay_Hex))
    {
        return FALSE;
    }
		currentdaycount=(sl_controller.currentTime.month<<8) | sl_controller.currentTime.day;
		if(startMonth_Hex > endMonth_Hex)
		{
				//如果跨年。
				if((sl_controller.currentTime.month<=endMonth_Hex)||(sl_controller.currentTime.month>startMonth_Hex))
				{
						//如果当前时间比结束时间早，或者比结束时间晚，需要加12小时。
						currentdaycount=(sl_controller.currentTime.month+0x12)<<8 | sl_controller.currentTime.day;
						endMonth_Hex = endMonth_Hex + 0x12;
				}
				endMonth_Hex = endMonth_Hex + 0x12;
		}
		
		startdaycount=startMonth_Hex<<8| startDay_Hex;
		enddaycount=endMonth_Hex<<8 | endDay_Hex;
		if((currentdaycount>=startdaycount) && (currentdaycount<=enddaycount))
		{
				return TRUE;
		}
		else
		{
				return FALSE;
		}
}
static bool NowInMinuteRange(u32 StartHour, u32 StartMinute, u32 StopHour, u32 StopMinute)
{
		u16 currentminutecount=0;
		u16 startminutecount=0;
		u16 stopminutecount=0;
		u32 StartHour_Hex;
		u32 StartMinute_Hex;
		u32 StopHour_Hex;
		u32 StopMinute_Hex;
		StartHour_Hex=BCDtoHEX(StartHour);
		StartMinute_Hex=BCDtoHEX(StartMinute);
		StopHour_Hex=BCDtoHEX(StopHour);
		StopMinute_Hex=BCDtoHEX(StopMinute);
		currentminutecount=sl_controller.currentTime.hour<<8|sl_controller.currentTime.minute;
		startminutecount=StartHour_Hex<<8|StartMinute_Hex;
		stopminutecount=StopHour_Hex<<8|StopMinute_Hex;
		if(IsInvalidTimeMinute(sl_controller.currentTime.minute)==TRUE || IsInvalidTimeHour(sl_controller.currentTime.hour))
		{
				//如果当前时间超过24小时 或者 分钟超过60分钟
				return FALSE;
		}
		if(IsInvalidTimeMinute(StartMinute_Hex)==TRUE || IsInvalidTimeHour(StartHour_Hex))
		{
				//如果开始时间超过24小时 或者 分钟超过60分钟
				return FALSE;
		}
		if(IsInvalidTimeMinute(StopMinute_Hex)==TRUE || IsInvalidTimeHour(StopHour_Hex))
		{
				//如果结束时间超过24小时 或者 分钟超过60分钟
				return FALSE;
		}
		if(stopminutecount==0)
		{
				stopminutecount=23<<8 | 59;//如果结束时间为0点0分，则认为是23点59分，这个要和软件那边稍微沟通一下。
		}
		if(currentminutecount>=startminutecount && currentminutecount<stopminutecount)
		{
				return TRUE;
		}
		return FALSE;
}

//判断分钟数是否符合0~60的范围
static bool IsInvalidTimeMinute(u8 timeMinute)
{
    if (timeMinute > 60 )
    {
        return TRUE;
    }
    return FALSE;
}
//判断小时数是否符合0~24的范围
static bool IsInvalidTimeHour(u8 timeHour)
{
    if (timeHour > 24 )
    {
        return TRUE;
    }
    return FALSE;
}
//判断小时数是否符合1~12的范围
static bool IsInvalidMonth(u32 month)
{
    if (month > 12 || month < 1)
    {
        return TRUE;
    }
    return FALSE;
}
//判断小时数是否符合1~31的范围
static bool IsInvalidDay(u32 day)
{
    if (day > 31 || day < 1)
    {
        return TRUE;
    }
    return FALSE;
}
//判断时间是否满足，如果有故障就不判断
//返回1启动，返回0关闭
static s8 InTimeTheory(u8 RoomNum)
{
		PWEEK_STORAGE pweek_storage_temp;
		u8 timeNum=0,festivalNum=0;
		//如果时间策论未启用的话，返回1。
		if((sl_controller.Theory_storage.Room_Storage[RoomNum].hasFlag & HAS_TIME) !=HAS_TIME)
		{
				//如果时间策略不生效，则时间策略限制取消
				return AutoTime;
		}
		pweek_storage_temp=&sl_controller.Theory_storage.Room_Storage[RoomNum].regular_storage.weekStorage;
		for(festivalNum=0;festivalNum<FestivalMaxNum;festivalNum++)
		{
			if(((sl_controller.Theory_storage.Room_Storage[RoomNum].festival_storage[festivalNum].day_range.StartMonth&0xC0)>>6)==FestivalNoUSE)
			{
				continue;
			}
			if(todayInDate(
				(sl_controller.Theory_storage.Room_Storage[RoomNum].festival_storage[festivalNum].day_range.StartMonth &0x3F),
				sl_controller.Theory_storage.Room_Storage[RoomNum].festival_storage[festivalNum].day_range.StartDay,
				sl_controller.Theory_storage.Room_Storage[RoomNum].festival_storage[festivalNum].day_range.StopMonth,
				sl_controller.Theory_storage.Room_Storage[RoomNum].festival_storage[festivalNum].day_range.StopDay
				)==TRUE)
			{
				if(((sl_controller.Theory_storage.Room_Storage[RoomNum].festival_storage[festivalNum].day_range.StartMonth&0xC0)>>6)==ForceClose)
				{
					return ((sl_controller.Theory_storage.Room_Storage[RoomNum].festival_storage[festivalNum].day_range.StartMonth&0xC0)>>6);
				}
				if(Festival_Judge(RoomNum,pweek_storage_temp)==TRUE)
				{
					return AutoTime;//节假日时间符合要求，时间策略符合。
				}
			}
		}
		if(todayInDate(
		sl_controller.Theory_storage.Room_Storage[RoomNum].regular_storage.day_range.StartMonth,
		sl_controller.Theory_storage.Room_Storage[RoomNum].regular_storage.day_range.StartDay,
		sl_controller.Theory_storage.Room_Storage[RoomNum].regular_storage.day_range.StopMonth,
		sl_controller.Theory_storage.Room_Storage[RoomNum].regular_storage.day_range.StopDay
		)==FALSE)
		{
			return ForceClose;
		}
		for(timeNum=0;timeNum<(sizeof(DAY_STORAGE)/sizeof(TIME_STORAGE));timeNum++)
		{
				if((pweek_storage_temp->day_storage[sl_controller.currentTime.week].time_storage[timeNum].GroupStartHour==0xFF)
				||(pweek_storage_temp->day_storage[sl_controller.currentTime.week].time_storage[timeNum].GroupStopHour==0xFF)
				||(pweek_storage_temp->day_storage[sl_controller.currentTime.week].time_storage[timeNum].GroupStartMinute==0xFF)
				||(pweek_storage_temp->day_storage[sl_controller.currentTime.week].time_storage[timeNum].GroupStopMinute==0xFF))
				{
						continue;
				}
				if((pweek_storage_temp->day_storage[sl_controller.currentTime.week].time_storage[timeNum].GroupStartHour
					==pweek_storage_temp->day_storage[sl_controller.currentTime.week].time_storage[timeNum].GroupStopHour)
				&&(pweek_storage_temp->day_storage[sl_controller.currentTime.week].time_storage[timeNum].GroupStartMinute
					==pweek_storage_temp->day_storage[sl_controller.currentTime.week].time_storage[timeNum].GroupStopMinute)
				)
				{
						//如果开始时间和结束时间相同，则今天全天时间策略符合。
						return AutoTime;
				}
				else
				{
						//如果开始时间和结束时间不相同，则判断当前时间是否在时间段内。
						if(NowInMinuteRange(pweek_storage_temp->day_storage[sl_controller.currentTime.week].time_storage[timeNum].GroupStartHour,
																pweek_storage_temp->day_storage[sl_controller.currentTime.week].time_storage[timeNum].GroupStartMinute,
																pweek_storage_temp->day_storage[sl_controller.currentTime.week].time_storage[timeNum].GroupStopHour,
																pweek_storage_temp->day_storage[sl_controller.currentTime.week].time_storage[timeNum].GroupStopMinute)
																==TRUE)
						{
								return AutoTime;
						}
				}
		}
		return ForceClose;
}

static u8 Festival_Judge(u8 RoomNum,PWEEK_STORAGE pweek_storage)
{
		PWEEK_STORAGE pweek_storage_temp;
		u8 timeNum=0;
		u16 EarlistTimeCount=0x2400;
		u16 LastTimeCount=0x0000;
		u16 CurrentTimeCount=0x0000;
		u16 startminutecount=0;
		u16 stopminutecount=0;
		u32 StartHour_Hex;
		u32 StartMinute_Hex;
		u32 StopHour_Hex;
		u32 StopMinute_Hex;
		CurrentTimeCount=sl_controller.currentTime.hour<<8|sl_controller.currentTime.minute;
		pweek_storage_temp=pweek_storage;
		for(timeNum=0;timeNum<(sizeof(DAY_STORAGE)/sizeof(TIME_STORAGE));timeNum++)
		{
			
				if((pweek_storage_temp->day_storage[sl_controller.currentTime.week].time_storage[timeNum].GroupStartHour==0xFF)
				||(pweek_storage_temp->day_storage[sl_controller.currentTime.week].time_storage[timeNum].GroupStopHour==0xFF)
				||(pweek_storage_temp->day_storage[sl_controller.currentTime.week].time_storage[timeNum].GroupStartMinute==0xFF)
				||(pweek_storage_temp->day_storage[sl_controller.currentTime.week].time_storage[timeNum].GroupStopMinute==0xFF))
				{
						//如果有一个时间点为0XFF，则判断下一个数据。
						continue;
				}
				StartHour_Hex=BCDtoHEX(pweek_storage_temp->day_storage[sl_controller.currentTime.week].time_storage[timeNum].GroupStartHour);
				StartMinute_Hex=BCDtoHEX(pweek_storage_temp->day_storage[sl_controller.currentTime.week].time_storage[timeNum].GroupStartMinute);
				StopHour_Hex=BCDtoHEX(pweek_storage_temp->day_storage[sl_controller.currentTime.week].time_storage[timeNum].GroupStopHour);
				StopMinute_Hex=BCDtoHEX(pweek_storage_temp->day_storage[sl_controller.currentTime.week].time_storage[timeNum].GroupStopMinute);
			
				startminutecount=StartHour_Hex<<8|StartMinute_Hex;
				stopminutecount=StopHour_Hex<<8|StopMinute_Hex;
				if((EarlistTimeCount>=startminutecount))
				{
						EarlistTimeCount=startminutecount;
				}
				if((LastTimeCount<=stopminutecount))
				{
						LastTimeCount=stopminutecount;
				}
		}
		if((CurrentTimeCount>=EarlistTimeCount) && (CurrentTimeCount<=LastTimeCount))
		{
				//如果在范围里则返回	TRUE。
				return TRUE;
		}
		else
		{
				//如果在范围里则返回	FALSE。
				return FALSE;
		}
}


