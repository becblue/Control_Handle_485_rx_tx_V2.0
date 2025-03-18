#ifndef _AO_C
#define _AO_C
#include "AO.h"
#include "IOApp.h"
#define DAC_SYNC	0
#define DAC_CLK		1
#define DAC_DIN		2

IO_APP_STRUCT gpio_AO_Struct[2][3]=
{
	GPIOB,GPIO_Pin_15,
	GPIOB,GPIO_Pin_13,
	GPIOB,GPIO_Pin_14,
	GPIOD,GPIO_Pin_2,
	GPIOA,GPIO_Pin_1,
	GPIOA,GPIO_Pin_2,
};
void AO_Init(void)
{
	  GPIO_InitTypeDef  GPIO_InitStruct;
		uint8_t i=0;
	
		for(i=0;i<(sizeof(gpio_AO_Struct)/sizeof(IO_APP_STRUCT))/2;i++)
		{
				GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;      //IO速率50M
				GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;	       //复用推挽输出，用于串口2的发送
				GPIO_InitStruct.GPIO_Pin=gpio_AO_Struct[0][i].GPIO_Pin;
				GPIO_Init(gpio_AO_Struct[0][i].GPIOx, &GPIO_InitStruct);
				GPIO_InitStruct.GPIO_Pin=gpio_AO_Struct[1][i].GPIO_Pin;
				GPIO_Init(gpio_AO_Struct[1][i].GPIOx, &GPIO_InitStruct);
		}
		AO_output(0,0);
		AO_output(1,0);
}
void AO_output(uint8_t AO_Chn ,uint16_t Value)
{
	uint16_t dac;
	uint8_t  i; 
	if(Value>2886)
	{
		dac = 2886 << 2;  //数据需要向左移动两位
	}
	else
	{
		dac = Value << 2;  //数据需要向左移动两位
	}
	dac &= 0X3FFF;    //正常模式
	
	GPIO_SetBits(gpio_AO_Struct[AO_Chn][DAC_SYNC].GPIOx,gpio_AO_Struct[AO_Chn][DAC_SYNC].GPIO_Pin);
	//DAC7311_SYNC_H();
	GPIO_SetBits(gpio_AO_Struct[AO_Chn][DAC_CLK].GPIOx,gpio_AO_Struct[AO_Chn][DAC_CLK].GPIO_Pin);
	//DAC7311_SCL_H();
	
	GPIO_ResetBits(gpio_AO_Struct[AO_Chn][DAC_SYNC].GPIOx,gpio_AO_Struct[AO_Chn][DAC_SYNC].GPIO_Pin);
	//DAC7311_SYNC_L();
	
	for(i = 0;i< 16;i++)
	{
		GPIO_SetBits(gpio_AO_Struct[AO_Chn][DAC_CLK].GPIOx,gpio_AO_Struct[AO_Chn][DAC_CLK].GPIO_Pin);
		//DAC7311_SCL_H();
		
		if(0X8000 == (dac & 0x8000))
		{
			GPIO_SetBits(gpio_AO_Struct[AO_Chn][DAC_DIN].GPIOx,gpio_AO_Struct[AO_Chn][DAC_DIN].GPIO_Pin);
			//DAC7311_DIN_H();
		}
		else
		{
			GPIO_ResetBits(gpio_AO_Struct[AO_Chn][DAC_DIN].GPIOx,gpio_AO_Struct[AO_Chn][DAC_DIN].GPIO_Pin);
			//DAC7311_DIN_L(); 
		}
		
		//DAC_delay(10);	
		GPIO_ResetBits(gpio_AO_Struct[AO_Chn][DAC_CLK].GPIOx,gpio_AO_Struct[AO_Chn][DAC_CLK].GPIO_Pin);
		//DAC7311_SCL_L();
		//DAC_delay(10);
		dac <<= 1;   //向前移动一位
	}
	GPIO_SetBits(gpio_AO_Struct[AO_Chn][DAC_SYNC].GPIOx,gpio_AO_Struct[AO_Chn][DAC_SYNC].GPIO_Pin);
	//DAC7311_SYNC_H();

		
}

#endif
