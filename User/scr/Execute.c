/*
 * Execute.c
 *
 *  Created on: 2014��11��29��
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
	u8  RoomIndex=0;//��ʾ��ǰ�յ������ķ���š�
	u8  ZeroCount=0;//��¼����������ͬ�յ���λ��Ϊ0�ĸ�����
	u8  i;
	if(timer.TimMBUSTimeOutAlarm==TRUE)
	{
		return;
	}
	if(sl_controller.Slaver_storage.slaverType==0x00)
	{
		//��Ԫ��
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
						//����ֶ�����10��󣬵�ǰ��ȡ״̬���ϴ��趨״̬��ͬ��������豸��ʱ��������ң�������������ֶ��ж���Ч��
						sl_controller.slaver_struct[ControllerNum].last_Hand_Status=0x00;//�����Զ�
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
			//����ֶ������ˣ���û�лָ�����ִ�в��ԡ�
			if(sl_controller.slaver_struct[ControllerNum].last_open_status==0x00)
			{
				sl_controller.slaver_struct[ControllerNum].target_open_status=0x00;
			}
			continue;
		}
		if(sl_controller.Theory_storage.Room_Storage[RoomIndex].WorkMode==MODE_COOL)
		{
				//����ģʽ�������ǰ�����¶�С�ڿ����¶ȣ��������¶ȸĻؿ����¶�
				if(sl_controller.slaver_struct[ControllerNum].current_setting_temper<sl_controller.Theory_storage.Room_Storage[RoomIndex].closetemp)
				{
						sl_controller.slaver_struct[ControllerNum].target_setting_temper=sl_controller.Theory_storage.Room_Storage[RoomIndex].closetemp;
				}
		}
		if(sl_controller.Theory_storage.Room_Storage[RoomIndex].WorkMode==MODE_HEAT)
		{
				//����ģʽ�������ǰ�����¶ȸ��ڿ����¶ȣ��������¶ȸĻؿ����¶�
				if(sl_controller.slaver_struct[ControllerNum].current_setting_temper>sl_controller.Theory_storage.Room_Storage[RoomIndex].closetemp)
				{
						sl_controller.slaver_struct[ControllerNum].target_setting_temper=sl_controller.Theory_storage.Room_Storage[RoomIndex].closetemp;
				}
		}
		if((InTimeTheory(RoomIndex)==ForceClose))
		{
				sl_controller.slaver_struct[ControllerNum].target_open_status=0x00;//�������������ֱ�ӹرա�
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
					//����Զ�������������ǰһ��״̬Ϊ�رգ����������������ٿ�����
					//����Զ������رգ��򲻲�����
					timer.TimONRepeatTimer[ControllerNum].Alarm = FALSE;
					if(sl_controller.Slaver_storage.slaverType==0x01)
					{
							timer.SetTimer(&timer.TimONRepeatTimer[ControllerNum].Count,6000);//���������������ǰ״̬��Ϊ������������10���ӿ�
					}
					else
					{
							timer.SetTimer(&timer.TimONRepeatTimer[ControllerNum].Count,3000);//���������������ǰ״̬��Ϊ������������5���ӿ�
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
			sl_controller.slaver_struct[ControllerNum].target_open_status=0x00;//�������������ֱ�ӹرա�
			sl_controller.slaver_struct[ControllerNum].last_open_status=0x00;
			sl_controller.slaver_struct[ControllerNum].tempresultlast=FALSE;
			//sl_controller.slaver_struct[ControllerNum].target_work_mode=sl_controller.Theory_storage.Room_Storage[RoomIndex].WorkMode;
					
		}
	}
	
}

//�ж��¶��Ƿ����㣬����й��ϾͲ��ж�
//����1��������������0����������
static s8 InTempTheory(u8 RoomNum ,u8 ControllerNum)
{
	if((sl_controller.Theory_storage.Room_Storage[RoomNum].hasFlag & HAS_TEMP) !=HAS_TEMP)
	{
			//����¶Ȳ��Բ���Ч�����¶Ȳ�������ȡ��
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
				//������ꡣ
				if((sl_controller.currentTime.month<=endMonth_Hex)||(sl_controller.currentTime.month>startMonth_Hex))
				{
						//�����ǰʱ��Ƚ���ʱ���磬���߱Ƚ���ʱ������Ҫ��12Сʱ��
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
				//�����ǰʱ�䳬��24Сʱ ���� ���ӳ���60����
				return FALSE;
		}
		if(IsInvalidTimeMinute(StartMinute_Hex)==TRUE || IsInvalidTimeHour(StartHour_Hex))
		{
				//�����ʼʱ�䳬��24Сʱ ���� ���ӳ���60����
				return FALSE;
		}
		if(IsInvalidTimeMinute(StopMinute_Hex)==TRUE || IsInvalidTimeHour(StopHour_Hex))
		{
				//�������ʱ�䳬��24Сʱ ���� ���ӳ���60����
				return FALSE;
		}
		if(stopminutecount==0)
		{
				stopminutecount=23<<8 | 59;//�������ʱ��Ϊ0��0�֣�����Ϊ��23��59�֣����Ҫ������Ǳ���΢��ͨһ�¡�
		}
		if(currentminutecount>=startminutecount && currentminutecount<stopminutecount)
		{
				return TRUE;
		}
		return FALSE;
}

//�жϷ������Ƿ����0~60�ķ�Χ
static bool IsInvalidTimeMinute(u8 timeMinute)
{
    if (timeMinute > 60 )
    {
        return TRUE;
    }
    return FALSE;
}
//�ж�Сʱ���Ƿ����0~24�ķ�Χ
static bool IsInvalidTimeHour(u8 timeHour)
{
    if (timeHour > 24 )
    {
        return TRUE;
    }
    return FALSE;
}
//�ж�Сʱ���Ƿ����1~12�ķ�Χ
static bool IsInvalidMonth(u32 month)
{
    if (month > 12 || month < 1)
    {
        return TRUE;
    }
    return FALSE;
}
//�ж�Сʱ���Ƿ����1~31�ķ�Χ
static bool IsInvalidDay(u32 day)
{
    if (day > 31 || day < 1)
    {
        return TRUE;
    }
    return FALSE;
}
//�ж�ʱ���Ƿ����㣬����й��ϾͲ��ж�
//����1����������0�ر�
static s8 InTimeTheory(u8 RoomNum)
{
		PWEEK_STORAGE pweek_storage_temp;
		u8 timeNum=0,festivalNum=0;
		//���ʱ�����δ���õĻ�������1��
		if((sl_controller.Theory_storage.Room_Storage[RoomNum].hasFlag & HAS_TIME) !=HAS_TIME)
		{
				//���ʱ����Բ���Ч����ʱ���������ȡ��
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
					return AutoTime;//�ڼ���ʱ�����Ҫ��ʱ����Է��ϡ�
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
						//�����ʼʱ��ͽ���ʱ����ͬ�������ȫ��ʱ����Է��ϡ�
						return AutoTime;
				}
				else
				{
						//�����ʼʱ��ͽ���ʱ�䲻��ͬ�����жϵ�ǰʱ���Ƿ���ʱ����ڡ�
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
						//�����һ��ʱ���Ϊ0XFF�����ж���һ�����ݡ�
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
				//����ڷ�Χ���򷵻�	TRUE��
				return TRUE;
		}
		else
		{
				//����ڷ�Χ���򷵻�	FALSE��
				return FALSE;
		}
}


