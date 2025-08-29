#ifndef _IO_APP_C
#define _IO_APP_C
#include "IOApp.h"
#include "Key.h"

#if DEVICE_MODE==DEVICE_MODE_SEND
IO_APP_STRUCT IO_App_Out_Struct[GPIO_LED_NUM]=
#else
IO_APP_STRUCT IO_App_In_Struct[GPIO_IN_NUM]=
#endif
{
	GPIOA,
	GPIO_Pin_0,
	GPIOA,
	GPIO_Pin_3,
	GPIOA,
	GPIO_Pin_4,
	GPIOA,
	GPIO_Pin_5,
	GPIOA,
	GPIO_Pin_6,
	GPIOA,
	GPIO_Pin_7,
	GPIOA,
	GPIO_Pin_8,
	GPIOA,
	GPIO_Pin_12,
	GPIOA,
	GPIO_Pin_15,
	GPIOC,
	GPIO_Pin_0,
	GPIOC,
	GPIO_Pin_1,
	GPIOC,
	GPIO_Pin_2,
};
#if DEVICE_MODE==DEVICE_MODE_SEND
IO_APP_STRUCT IO_App_In_Struct[GPIO_KEY_NUM]=
#else
IO_APP_STRUCT IO_App_Out_Struct[GPIO_OUT_NUM]=
#endif
{
	GPIOC,
	GPIO_Pin_3,
	GPIOC,
	GPIO_Pin_4,
	GPIOC,
	GPIO_Pin_5,
	GPIOC,
	GPIO_Pin_6,
	GPIOC,
	GPIO_Pin_7,
	GPIOC,
	GPIO_Pin_8,
	GPIOC,
	GPIO_Pin_9,
	GPIOC,
	GPIO_Pin_10,
	GPIOC,
	GPIO_Pin_11,
	GPIOC,
	GPIO_Pin_12,
	GPIOC,
	GPIO_Pin_13,
	GPIOC,
	GPIO_Pin_14,
	GPIOC,
	GPIO_Pin_15,
	GPIOB,
	GPIO_Pin_0,
	GPIOB,
	GPIO_Pin_1,
	GPIOB,
	GPIO_Pin_3,
	GPIOB,
	GPIO_Pin_4,
	GPIOB,
	GPIO_Pin_5,
	GPIOB,
	GPIO_Pin_6,
	GPIOB,
	GPIO_Pin_7,
	GPIOB,
	GPIO_Pin_8,
	GPIOB,
	GPIO_Pin_9,
	GPIOB,
	GPIO_Pin_10,
	GPIOB,
	GPIO_Pin_11,
};
void IO_App_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	uint8_t i=0;
	IO_Init();
	
	for(i=0;i<sizeof(IO_App_In_Struct)/sizeof(IO_APP_STRUCT);i++)
	{
		GPIO_InitStructure.GPIO_Pin = IO_App_In_Struct[i].GPIO_Pin;              //准备设置PA2
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;      //IO速率50M
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	       //复用推挽输出，用于串口2的发送
		GPIO_Init(IO_App_In_Struct[i].GPIOx, &GPIO_InitStructure);                 //设置PA2
	}
	for(i=0;i<sizeof(IO_App_Out_Struct)/sizeof(IO_APP_STRUCT);i++)
	{
		GPIO_InitStructure.GPIO_Pin = IO_App_Out_Struct[i].GPIO_Pin;              //准备设置PA2
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;      //IO速率50M
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	       //复用推挽输出，用于串口2的发送
		GPIO_Init(IO_App_Out_Struct[i].GPIOx, &GPIO_InitStructure);                 //设置PA2
		GPIO_ResetBits(IO_App_Out_Struct[i].GPIOx,IO_App_Out_Struct[i].GPIO_Pin);
	}
}
void IO_APP_Get_IO_Value(uint8_t * UartTxBuff)
{
	uint32_t result = 0;
	static uint32_t ShiftValue[2]={0,0};
	uint8_t i=0;
	for(i=0;i<sizeof(IO_App_In_Struct)/sizeof(IO_APP_STRUCT);i++)
	{
#if DEVICE_MODE==DEVICE_MODE_SEND
		if(GPIO_ReadInputDataBit(IO_App_In_Struct[i].GPIOx,IO_App_In_Struct[i].GPIO_Pin)==RESET
			&& (i>7))
		{
			result=result|(0x01<<i);
		}
		if(GetKeyValue(KEY1)==1)
		{
			ClrKeyCount(KEY1);
			ClrKeyValue(KEY1);
			if(ShiftValue[0]==0)
			{
				ShiftValue[0]=1;
			}
			else if(ShiftValue[0]==0x08)
			{
				ShiftValue[0]=0;
			}
			else
			{
				ShiftValue[0]=ShiftValue[0]<<1;
			}
		}
		if(GetKeyValue(KEY5)==1)
		{
			ClrKeyCount(KEY5);
			ClrKeyValue(KEY5);
			if(ShiftValue[1]==0)
			{
				ShiftValue[1]=0x10;
			}
			else if(ShiftValue[1]==0x80)
			{
				ShiftValue[1]=0;
			}
			else
			{
				ShiftValue[1]=ShiftValue[1]<<1;
			}
		}
		result=result&(0xFFFFFFFF-ShiftValue[0]);
		result=result|(ShiftValue[0]);
		result=result&(0xFFFFFFFF-ShiftValue[1]);
		result=result|(ShiftValue[1]);
#else
		if(GPIO_ReadInputDataBit(IO_App_In_Struct[i].GPIOx,IO_App_In_Struct[i].GPIO_Pin)==SET)
		{
			result=result|(0x01<<i);
		}
#endif
	}
	UartTxBuff[0]=((result>>0)&0xFF);
	UartTxBuff[1]=((result>>8)&0xFF);
	UartTxBuff[2]=((result>>16)&0xFF);
}
void IO_APP_Set_IO_Value(uint8_t * UartTxBuff)
{
	uint32_t result;
	uint8_t i=0;
	result=UartTxBuff[0] + (UartTxBuff[1]<<8) + (UartTxBuff[2]<<16);
	
#if DEVICE_MODE==DEVICE_MODE_SEND
	// 发送模式下：PA0-PA5直接根据接收数据控制，无特殊限制
	for(i=0;i<sizeof(IO_App_Out_Struct)/sizeof(IO_APP_STRUCT);i++)
	{
		if((result&(0x01<<i))==(0x01<<i))
		{
			GPIO_SetBits(IO_App_Out_Struct[i].GPIOx,IO_App_Out_Struct[i].GPIO_Pin);
		}
		else
		{
			GPIO_ResetBits(IO_App_Out_Struct[i].GPIOx,IO_App_Out_Struct[i].GPIO_Pin);
		}
	}
#else
	// 接收模式下：PC3-PC6需要与PB0进行与逻辑判断
	for(i=0;i<sizeof(IO_App_Out_Struct)/sizeof(IO_APP_STRUCT);i++)
	{
		if((result&(0x01<<i))==(0x01<<i))
		{
			// PC3-PC6 (索引0-3) 需要与PB0 (索引13) 进行与逻辑判断
			if(i <= 3)  // PC3(0), PC4(1), PC5(2), PC6(3)
			{
				// 只有当PB0对应的bit13也被置位时，PC3-PC6才能输出高电平
				if((result&(0x01<<13))==(0x01<<13))  // PB0被置位
				{
					GPIO_SetBits(IO_App_Out_Struct[i].GPIOx,IO_App_Out_Struct[i].GPIO_Pin);
				}
				else
				{
					GPIO_ResetBits(IO_App_Out_Struct[i].GPIOx,IO_App_Out_Struct[i].GPIO_Pin);
				}
			}
			else  // 其他IO引脚保持原逻辑
			{
				GPIO_SetBits(IO_App_Out_Struct[i].GPIOx,IO_App_Out_Struct[i].GPIO_Pin);
			}
		}
		else
		{
			GPIO_ResetBits(IO_App_Out_Struct[i].GPIOx,IO_App_Out_Struct[i].GPIO_Pin);
		}
	}
#endif
}
#endif
