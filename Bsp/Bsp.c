
/************************************************************************
*@文件: Bsp.h
*@作者:
*@版本:
*@日期:
*@描述:
*************************************************************************/
#include "includes.h"
#include "AD.h"
void TIM4_Config(void);
//#include "BSP_10X_IT.h"//两个函数公用变量
/************************************************************************
  * @描述:  硬件驱动设置
  * @参数:  None
  * @返回值: None
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
 *函数名称：void IWDG_Configuration(void)
 *入口参数：无
 *出口参数：无
 *功能说明：看门狗初始化
 *******************************************************************************************/
void IWDG_Configuration(void)
{
	/* 写入0x5555,用于允许狗狗寄存器写入功能 */
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);

	/* 狗狗时钟分频,40K/256=156HZ(6.4ms)*/
	IWDG_SetPrescaler(IWDG_Prescaler_256);

	/* 喂狗时间 5s/6.4MS=781 .注意不能大于0xfff*/
	IWDG_SetReload(60);

	/* 喂狗*/
	IWDG_ReloadCounter();

	/* 使能*/
	IWDG_Enable();
}
/********************************************************************************************
 *函数名称：void GPIO_Configuration(void)
 *入口参数：无
 *出口参数：无
 *功能说明：掉电中断初始化
 *******************************************************************************************/
void GPIO_Configuration(void)
{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);   //使能GPIOA时钟
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
		//GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);
		GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); 
		
		
		//Comm_Data_Union.Comm_Data_Struct.Cmd_Head=0xAA;
}
