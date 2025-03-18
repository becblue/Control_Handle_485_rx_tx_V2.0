
/************************************************************************
*@�ļ�: Bsp.h
*@����:
*@�汾:
*@����:
*@����:
*************************************************************************/
#include "includes.h"
#include "AD.h"
void TIM4_Config(void);
//#include "BSP_10X_IT.h"//�����������ñ���
/************************************************************************
  * @����:  Ӳ����������
  * @����:  None
  * @����ֵ: None
  **********************************************************************/
void Bsp_Init(void)
{
  //NVIC_Config(); 
	//IWDG_Configuration();
  SysTick_Config(720000);
	GPIO_Configuration();
	//IO_Init();
}


/********************************************************************************************
 *�������ƣ�void IWDG_Configuration(void)
 *��ڲ�������
 *���ڲ�������
 *����˵�������Ź���ʼ��
 *******************************************************************************************/
void IWDG_Configuration(void)
{
	/* д��0x5555,�����������Ĵ���д�빦�� */
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);

	/* ����ʱ�ӷ�Ƶ,40K/256=156HZ(6.4ms)*/
	IWDG_SetPrescaler(IWDG_Prescaler_256);

	/* ι��ʱ�� 5s/6.4MS=781 .ע�ⲻ�ܴ���0xfff*/
	IWDG_SetReload(60);

	/* ι��*/
	IWDG_ReloadCounter();

	/* ʹ��*/
	IWDG_Enable();
}
/********************************************************************************************
 *�������ƣ�void GPIO_Configuration(void)
 *��ڲ�������
 *���ڲ�������
 *����˵���������жϳ�ʼ��
 *******************************************************************************************/
void GPIO_Configuration(void)
{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);   //ʹ��GPIOAʱ��
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
		//GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);
		GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); 
		
		
		//Comm_Data_Union.Comm_Data_Struct.Cmd_Head=0xAA;
}
