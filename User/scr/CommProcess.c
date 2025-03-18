#include "CommProcess.h"
#include "stm32f10x.h"
union COMM_DATA_UNION Comm_Data_Union;
void CommInit(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;     //定义一个设置GPIO功能的变量
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);   //使能GPIOA时钟

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;              //准备设置PA2
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;      //IO速率50M
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	       //复用推挽输出，用于串口2的发送
		GPIO_Init(GPIOC, &GPIO_InitStructure);                 //设置PA2
	
		GPIO_ResetBits(GPIOC,GPIO_Pin_13);

		Comm_Data_Union.Comm_Data_Struct.Cmd_Head=0xAA;
}
