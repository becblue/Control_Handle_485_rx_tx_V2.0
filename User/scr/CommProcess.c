#include "CommProcess.h"
#include "stm32f10x.h"
union COMM_DATA_UNION Comm_Data_Union;
void CommInit(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;     //����һ������GPIO���ܵı���
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);   //ʹ��GPIOAʱ��

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;              //׼������PA2
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;      //IO����50M
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	       //����������������ڴ���2�ķ���
		GPIO_Init(GPIOC, &GPIO_InitStructure);                 //����PA2
	
		GPIO_ResetBits(GPIOC,GPIO_Pin_13);

		Comm_Data_Union.Comm_Data_Struct.Cmd_Head=0xAA;
}
