#include "AD.h"
void DMA_Configuration(void);
void ADC_Conf(void);
u16 AD_Value[2];
void AD_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
		ADC_Conf();
		DMA_Configuration();
}
/********************************************************************************************
 *函数名称：void ADC_Conf(void)
 *入口参数：无
 *出口参数：无
 *功能说明：ADC配置，两个通道，使用DMA功能
 *******************************************************************************************/
void ADC_Conf(void)
{
		volatile int i=0,j=0;
    ADC_InitTypeDef ADC_InitStructure;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
    /* ADC1 Configuration ------------------------------------------------------*/
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	 //	 //独立模式
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;			 //	 //连续多通道模式

    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;	   //  //连续转换

    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//	//转换不受外界决定

    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; //	//右对齐
    ADC_InitStructure.ADC_NbrOfChannel = 2;  //扫描通道数
    ADC_Init(ADC1, &ADC_InitStructure);
    RCC_ADCCLKConfig(RCC_PCLK2_Div2);  //时钟分频 1mhz

    ADC_TempSensorVrefintCmd(ENABLE);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_1,  1, ADC_SampleTime_55Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_2,  2, ADC_SampleTime_55Cycles5);
    //ADC_RegularChannelConfig(ADC1, ADC_Channel_0,  6, ADC_SampleTime_239Cycles5);

    ADC_Cmd(ADC1, ENABLE);              //使能或者失能指定的ADC
	  for(i=0;i<1000;i++)
		{
				for(j=0;j<1000;j++)
				{
				}
		}
    ADC_DMACmd(ADC1, ENABLE);
    ADC_ResetCalibration(ADC1);
    while (ADC_GetResetCalibrationStatus(ADC1));

    ADC_StartCalibration(ADC1);
    while (ADC_GetCalibrationStatus(ADC1));

    ADC_SoftwareStartConvCmd(ADC1, ENABLE);

}

/********************************************************************************************
 *函数名称：void DMA_Configuration(void)
 *入口参数：无
 *出口参数：无
 *功能说明：DMA配置，使序列1结果放在AD_Value[0]，序列2结果放在AD_Value[1]
 *******************************************************************************************/
void DMA_Configuration(void)
{
    DMA_InitTypeDef DMA_InitStructure;
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);

    DMA_DeInit(DMA1_Channel1);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&ADC1->DR;
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&AD_Value;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize = 2;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);
    DMA_Cmd(DMA1_Channel1, ENABLE);
}
