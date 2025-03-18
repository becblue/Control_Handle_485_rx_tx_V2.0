/*
 * Controller.h
 *
 *  Created on: 2014年11月23日
 *      Author: xiao70
 */

#ifndef CONTROLLER_H_
#define CONTROLLER_H_
#include "includes.h"

#define RoomMaxNum 						8
#define FestivalMaxNum				12
#define AirControllerMaxNum		32
#define CurrentProductName		0x8629
#define Data_Value						0xAA
#define Software_Version			0x1220

#define Operate_None					100

#define MODE_AUTO							0x00
#define MODE_HEAT							0x01
#define MODE_COOL							0x02
#define MODE_DRY							0x03
#define MODE_FAN							0x04

#define HAS_TIME							0x01
#define HAS_TEMP							0x02

#define AutoOpenSetting_Auto  0x10
#define AutoOpenSetting_Close	0x01

#define FestivalNoUSE					0x00
#define ForceOpen							0x01
#define ForceClose						0x02
#define	AutoTime							0x03

#pragma pack(push,4)
typedef struct _CONTROLLER_STORAGE
{
		u16 physicsAddress;//物理地址
		u16 product;
		u32 productNumber;//产品序列号
} CONTROLLER_STORAGE, *PCONTROLLER_STORAGE;

typedef struct _ENERGY_STORAGE
{
	
		//单元机 电能查询 3.6秒统计一次，每次增加值为功率值。低32位电能每满10000，高32位电能数据+1，掉电2个32位电能数据都要保存。
		//多联机 电能查询 读到的数据除以10，然后赋值给高32位电能数据，然后上传。低32位电能数据始终为0.
		u32 Slaver_Electrical_Energy_H;//有功电能高32位
		u16 Slaver_Electrical_Energy_L;//有功电能低32位
		u16 Slaver_Electrical_Data_Flag;//如果值不等于0xAA,则表示FLASH需要初始化
}ENERGY_STORAGE, *PENERGY_STORAGE;

typedef struct _TIME_STORAGE//开始时间和结束时间
{
		u8 	GroupStartHour;//组开始小时
		u8  GroupStartMinute;//组开始分钟
		u8	GroupStopHour;//组结束小时
		u8  GroupStopMinute;//组结束分钟
}TIME_STORAGE,*PTIME_STORAGE;

typedef struct _DAY_STORAGE//每天12个时间段
{
		TIME_STORAGE time_storage[12];
}DAY_STORAGE,*PDAY_STORAGE;

typedef struct _WEEK_STORAGE//每周7天
{
		DAY_STORAGE day_storage[7];//周日为day_storage[0]，周为day_storage[6]
}WEEK_STORAGE,*PWEEK_STORAGE;


typedef struct _DAY_RANGE//开始日期和结束日期
{
		u8		StartMonth;//时间范围（从几月到几月）高2位表示节假日强开或强关或自动
		u8		StartDay;//时间范围（从几号到几号）
		u8		StopMonth;//时间范围（从几月到几月）
		u8		StopDay;//时间范围（从几号到几号）
}DAY_RANGE,PDAY_RANGE;

typedef struct _FESTIVAL_Storage
{
		DAY_RANGE			day_range;
		TIME_STORAGE 	time_storage;
}FESTIVAL_Storage,*PFESTIVAL_Storage;

typedef struct _REGULAR_storage
{
		DAY_RANGE			day_range;
		WEEK_STORAGE 	weekStorage;//一周开启关闭时间
}REGULAR_storage,*PREGULAR_storage;

typedef struct _ROOM_STORAGE
{
		u8		hasFlag;//定时策略标志,BIT0 0x00标识定时策略无效,0x01标识定时策略有效。//温度策略标志,BIT1 0x00标识温度策略无效,0x02标识温度策略有效。
		u8		WorkMode;//制热0x01还是制冷0x02还是除湿0x03还是送风0x04
		u8		opentemp;//开启温度
		u8		closetemp;//关闭温度
		u32		AutoOpenSetting;//自动开启配置。
		FESTIVAL_Storage 	festival_storage[FestivalMaxNum];
		REGULAR_storage		regular_storage;
}ROOM_STORAGE,*PROOM_STORAGE;

typedef struct _THEORY_STORAGE
{
		u32		Data_Flag;//如果值不等于0xAA,则表示FLASH需要初始化
		ROOM_STORAGE		Room_Storage[RoomMaxNum];//平常的开关时间策略
}THEORY_STORAGE, *PTHEORY_STORAGE;

typedef struct _TIME_S
{
		u8 second;
		u8 minute;
		u8 hour;
		u8 day;
		u8 month;
		u8 year;
		u8 week;
		u8 reserve;
} TIME_S, *PTIME_S;

typedef struct _EVENT_STRUCT
{
		u8	EventNC;//备份暂时不用
		u8	EventMonth;//事件发生月份
		u8	EventDay;//事件发生日期
		u8	EventHour;//事件发生小时
		u8	EventMinute;//事件发生分钟
		u8	EventSecond;//事件发生秒钟
		u16	EventContext;//事件发生内容
}EVENT_STRUCT,PEVENT_STRUCT;

//60条事件
typedef struct _EVENT_DATA
{
		EVENT_STRUCT event_struct[60];
}EVENT_DATA,*PEVENT_DATA;

typedef enum _STORAGE_TYPE
{
	NOneedSTORAGE,
	CONTROLLERneedSTORAGE,
	THEORYneedSTORAGE,
	SLAVERneedSTORAGE,
	TIMEneedSTORAGE
}STORAGE_TYPE;

typedef struct _RX_BUFF
{
		u8 	rx_data[63];
		u8  Receive_Complete;
}RX_BUFF,*PRX_BUFF;

typedef struct _SLAVER_STRUCT
{
		u8  target_open_status;//目标设定空调开关状态
		u8  target_setting_temper;//目标设定温度（需要设置的）
		u8  target_work_mode;//目标设置模式
		u8	target_FAN_Speed;//目标设置风速
		s8  current_indoor_temper;//当前出风口温度
		u8	current_compressor_status;//当前压缩机工作状态
		u8	current_open_status;//当前空调开关状态
		u8  current_setting_temper;//当前设定温度（空调读回的）
		u8	current_work_mode;//当前空调工作模式 制冷还是制热，送风还是除湿
		u8	current_FAN_Speed;//当前空调风速状态
		u8  Check_On_Count;//如果发了开指令，但是当前状态为关，则+1，计数大于3时，报开启故障
		u8  Check_Off_Count;//如果发了关指令，但是当前状态为开，则+1，计数大于3时，报关闭故障
		u8	Current_Force_Mode;//当前强制状态。
		u8 	Backup_Word;//备用~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		u16	current_Err_Code;//目标错误代码
		u16 last_Hand_Status;//上次手动状态
		u16 last_open_status;//上次自动状态
		u32 tempresultlast;//保存上次温度开启状态，关闭时要清零。
}SLAVER_STRUCT,*PSLAVER_STRUCT;

typedef struct _SLAVER_STORAGE
{
		//u8	HanResume[16];//手动恢复，0x01表示关闭，0x10表示开启。最大16组
		u16 Data_Flag;//如果值不等于0xAA,则表示FLASH需要初始化
		u16	slaverType;//空调类型 0x00表示单元机，0x01表示多联机
		u16 heatPower;//制热功率
		u16 coolPower;//制冷功率
		u32 roomSubNum[RoomMaxNum];//房间内的空调号。最大支持16个。
		u16 last_Hand_Status[AirControllerMaxNum];//上次手动状态
		u16 last_open_status[AirControllerMaxNum];//上次自动状态
		u8	Current_Force_Mode[AirControllerMaxNum];//上次强制状态。
}SLAVER_STORAGE,*PSLAVER_STORAGE;

typedef struct _SL_CONTROLLER
{
		CONTROLLER_STORAGE	Controller_storage;
		ENERGY_STORAGE			Energy_storage;
		THEORY_STORAGE			Theory_storage;
		SLAVER_STORAGE			Slaver_storage;
		SLAVER_STRUCT				slaver_struct[AirControllerMaxNum];
		STORAGE_TYPE				storage_type;
		//EVENT_DATA					Event_data;//暂时不使用
		TIME_S 							currentTime;
		RX_BUFF							MER_RX_Buff;
		u32 								MER_Err_Time;
		RX_BUFF							KT_RX_Buff;//下行通讯发送缓冲区
		u8									KT_TX_Buff[64];//下行通讯发送缓冲区
		RX_BUFF							MBUS_RX_Buff;//上行通讯接收缓冲区
		u8									MBUS_TX_Buff[64];//上行通讯发送缓冲区
		u8									DOWNLEDBlinkCount;
		u8									UPLEDBlinkCount;
		u8									MERLEDBlinkCount;
		u8									Reserve;
}	SL_CONTROLLER, *PSL_CONTROLLER;

#endif /* CONTROLLER_H_ */
