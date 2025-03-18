/*
 * Controller.h
 *
 *  Created on: 2014��11��23��
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
		u16 physicsAddress;//�����ַ
		u16 product;
		u32 productNumber;//��Ʒ���к�
} CONTROLLER_STORAGE, *PCONTROLLER_STORAGE;

typedef struct _ENERGY_STORAGE
{
	
		//��Ԫ�� ���ܲ�ѯ 3.6��ͳ��һ�Σ�ÿ������ֵΪ����ֵ����32λ����ÿ��10000����32λ��������+1������2��32λ�������ݶ�Ҫ���档
		//������ ���ܲ�ѯ ���������ݳ���10��Ȼ��ֵ����32λ�������ݣ�Ȼ���ϴ�����32λ��������ʼ��Ϊ0.
		u32 Slaver_Electrical_Energy_H;//�й����ܸ�32λ
		u16 Slaver_Electrical_Energy_L;//�й����ܵ�32λ
		u16 Slaver_Electrical_Data_Flag;//���ֵ������0xAA,���ʾFLASH��Ҫ��ʼ��
}ENERGY_STORAGE, *PENERGY_STORAGE;

typedef struct _TIME_STORAGE//��ʼʱ��ͽ���ʱ��
{
		u8 	GroupStartHour;//�鿪ʼСʱ
		u8  GroupStartMinute;//�鿪ʼ����
		u8	GroupStopHour;//�����Сʱ
		u8  GroupStopMinute;//���������
}TIME_STORAGE,*PTIME_STORAGE;

typedef struct _DAY_STORAGE//ÿ��12��ʱ���
{
		TIME_STORAGE time_storage[12];
}DAY_STORAGE,*PDAY_STORAGE;

typedef struct _WEEK_STORAGE//ÿ��7��
{
		DAY_STORAGE day_storage[7];//����Ϊday_storage[0]����Ϊday_storage[6]
}WEEK_STORAGE,*PWEEK_STORAGE;


typedef struct _DAY_RANGE//��ʼ���ںͽ�������
{
		u8		StartMonth;//ʱ�䷶Χ���Ӽ��µ����£���2λ��ʾ�ڼ���ǿ����ǿ�ػ��Զ�
		u8		StartDay;//ʱ�䷶Χ���Ӽ��ŵ����ţ�
		u8		StopMonth;//ʱ�䷶Χ���Ӽ��µ����£�
		u8		StopDay;//ʱ�䷶Χ���Ӽ��ŵ����ţ�
}DAY_RANGE,PDAY_RANGE;

typedef struct _FESTIVAL_Storage
{
		DAY_RANGE			day_range;
		TIME_STORAGE 	time_storage;
}FESTIVAL_Storage,*PFESTIVAL_Storage;

typedef struct _REGULAR_storage
{
		DAY_RANGE			day_range;
		WEEK_STORAGE 	weekStorage;//һ�ܿ����ر�ʱ��
}REGULAR_storage,*PREGULAR_storage;

typedef struct _ROOM_STORAGE
{
		u8		hasFlag;//��ʱ���Ա�־,BIT0 0x00��ʶ��ʱ������Ч,0x01��ʶ��ʱ������Ч��//�¶Ȳ��Ա�־,BIT1 0x00��ʶ�¶Ȳ�����Ч,0x02��ʶ�¶Ȳ�����Ч��
		u8		WorkMode;//����0x01��������0x02���ǳ�ʪ0x03�����ͷ�0x04
		u8		opentemp;//�����¶�
		u8		closetemp;//�ر��¶�
		u32		AutoOpenSetting;//�Զ��������á�
		FESTIVAL_Storage 	festival_storage[FestivalMaxNum];
		REGULAR_storage		regular_storage;
}ROOM_STORAGE,*PROOM_STORAGE;

typedef struct _THEORY_STORAGE
{
		u32		Data_Flag;//���ֵ������0xAA,���ʾFLASH��Ҫ��ʼ��
		ROOM_STORAGE		Room_Storage[RoomMaxNum];//ƽ���Ŀ���ʱ�����
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
		u8	EventNC;//������ʱ����
		u8	EventMonth;//�¼������·�
		u8	EventDay;//�¼���������
		u8	EventHour;//�¼�����Сʱ
		u8	EventMinute;//�¼���������
		u8	EventSecond;//�¼���������
		u16	EventContext;//�¼���������
}EVENT_STRUCT,PEVENT_STRUCT;

//60���¼�
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
		u8  target_open_status;//Ŀ���趨�յ�����״̬
		u8  target_setting_temper;//Ŀ���趨�¶ȣ���Ҫ���õģ�
		u8  target_work_mode;//Ŀ������ģʽ
		u8	target_FAN_Speed;//Ŀ�����÷���
		s8  current_indoor_temper;//��ǰ������¶�
		u8	current_compressor_status;//��ǰѹ��������״̬
		u8	current_open_status;//��ǰ�յ�����״̬
		u8  current_setting_temper;//��ǰ�趨�¶ȣ��յ����صģ�
		u8	current_work_mode;//��ǰ�յ�����ģʽ ���仹�����ȣ��ͷ绹�ǳ�ʪ
		u8	current_FAN_Speed;//��ǰ�յ�����״̬
		u8  Check_On_Count;//������˿�ָ����ǵ�ǰ״̬Ϊ�أ���+1����������3ʱ������������
		u8  Check_Off_Count;//������˹�ָ����ǵ�ǰ״̬Ϊ������+1����������3ʱ�����رչ���
		u8	Current_Force_Mode;//��ǰǿ��״̬��
		u8 	Backup_Word;//����~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		u16	current_Err_Code;//Ŀ��������
		u16 last_Hand_Status;//�ϴ��ֶ�״̬
		u16 last_open_status;//�ϴ��Զ�״̬
		u32 tempresultlast;//�����ϴ��¶ȿ���״̬���ر�ʱҪ���㡣
}SLAVER_STRUCT,*PSLAVER_STRUCT;

typedef struct _SLAVER_STORAGE
{
		//u8	HanResume[16];//�ֶ��ָ���0x01��ʾ�رգ�0x10��ʾ���������16��
		u16 Data_Flag;//���ֵ������0xAA,���ʾFLASH��Ҫ��ʼ��
		u16	slaverType;//�յ����� 0x00��ʾ��Ԫ����0x01��ʾ������
		u16 heatPower;//���ȹ���
		u16 coolPower;//���书��
		u32 roomSubNum[RoomMaxNum];//�����ڵĿյ��š����֧��16����
		u16 last_Hand_Status[AirControllerMaxNum];//�ϴ��ֶ�״̬
		u16 last_open_status[AirControllerMaxNum];//�ϴ��Զ�״̬
		u8	Current_Force_Mode[AirControllerMaxNum];//�ϴ�ǿ��״̬��
}SLAVER_STORAGE,*PSLAVER_STORAGE;

typedef struct _SL_CONTROLLER
{
		CONTROLLER_STORAGE	Controller_storage;
		ENERGY_STORAGE			Energy_storage;
		THEORY_STORAGE			Theory_storage;
		SLAVER_STORAGE			Slaver_storage;
		SLAVER_STRUCT				slaver_struct[AirControllerMaxNum];
		STORAGE_TYPE				storage_type;
		//EVENT_DATA					Event_data;//��ʱ��ʹ��
		TIME_S 							currentTime;
		RX_BUFF							MER_RX_Buff;
		u32 								MER_Err_Time;
		RX_BUFF							KT_RX_Buff;//����ͨѶ���ͻ�����
		u8									KT_TX_Buff[64];//����ͨѶ���ͻ�����
		RX_BUFF							MBUS_RX_Buff;//����ͨѶ���ջ�����
		u8									MBUS_TX_Buff[64];//����ͨѶ���ͻ�����
		u8									DOWNLEDBlinkCount;
		u8									UPLEDBlinkCount;
		u8									MERLEDBlinkCount;
		u8									Reserve;
}	SL_CONTROLLER, *PSL_CONTROLLER;

#endif /* CONTROLLER_H_ */
