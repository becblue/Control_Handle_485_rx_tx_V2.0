
#include "stm32f10x.h"
#include "PWM.h"
u8 DA_Flush;
double Input_Voltage0=0,Input_Voltage1=0; 

void PWM_Config(void)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_OCInitTypeDef  TIM_OCInitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
	
	//------????TIMx
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); //?????3??
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //?????3??
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //?????3??

	//------????GPIOx
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//??GPIO???? 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//??GPIO???? 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);//??AFIO????????,???????? 
	
	
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_10 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_11 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_1 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_7 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
  TIM_DeInit(TIM1);
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
  TIM_TimeBaseStructure.TIM_Period = duty;     
	TIM_TimeBaseStructure.TIM_Prescaler = 8999;
  TIM_TimeBaseStructure.TIM_ClockDivision = 1;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;        
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 0;//ռ�ձ�0%������͵�ƽ
  TIM_OC3Init(TIM1, &TIM_OCInitStructure);
  TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);

  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;        
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 0;//ռ�ձ�0%������͵�ƽ
  TIM_OC4Init(TIM1, &TIM_OCInitStructure);
  TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);
	
	TIM_CtrlPWMOutputs(TIM1, ENABLE);
  TIM_ARRPreloadConfig(TIM1, ENABLE);
  TIM_Cmd(TIM1, ENABLE);   /* TIM2 enable counter */
	
	TIM_SetCompare3(TIM1,0);
	TIM_SetCompare4(TIM1,0);
	
  TIM_DeInit(TIM3);
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
  TIM_TimeBaseStructure.TIM_Period = duty;     
	TIM_TimeBaseStructure.TIM_Prescaler = 8999;
  TIM_TimeBaseStructure.TIM_ClockDivision = 1;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;        
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 0;//ռ�ձ�0%������͵�ƽ
  TIM_OC3Init(TIM3, &TIM_OCInitStructure);
  TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);

  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;        
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 0;//ռ�ձ�0%������͵�ƽ
  TIM_OC4Init(TIM3, &TIM_OCInitStructure);
  TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);
	
	TIM_CtrlPWMOutputs(TIM3, ENABLE);
  TIM_ARRPreloadConfig(TIM3, ENABLE);
  TIM_Cmd(TIM3, ENABLE);   /* TIM2 enable counter */
	
	TIM_SetCompare3(TIM3,0);
	TIM_SetCompare4(TIM3,0);
	
  TIM_DeInit(TIM4);
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
  TIM_TimeBaseStructure.TIM_Period = duty;     
	TIM_TimeBaseStructure.TIM_Prescaler = 8999;
  TIM_TimeBaseStructure.TIM_ClockDivision = 1;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;        
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 0;//ռ�ձ�0%������͵�ƽ
  TIM_OC1Init(TIM4, &TIM_OCInitStructure);
  TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);

  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;        
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 0;//ռ�ձ�0%������͵�ƽ
  TIM_OC2Init(TIM4, &TIM_OCInitStructure);
  TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);
	
	TIM_CtrlPWMOutputs(TIM4, ENABLE);
  TIM_ARRPreloadConfig(TIM4, ENABLE);
  TIM_Cmd(TIM4, ENABLE);   /* TIM2 enable counter */
	
	TIM_SetCompare1(TIM4,0);
	TIM_SetCompare2(TIM4,0);
}

void PWM_Duty_Set(u16 Duty1)
{

	if(Duty1>=1000)
	{
		Duty1 = 1000;
	}
	TIM_SetCompare3(TIM1,Duty1);
	
	//TIM_CtrlPWMOutputs(TIM1, ENABLE);
}
/**********************************************************************
* ��    �ƣ�RCC_Configuration()
* ��    �ܣ�ʱ������
* ��ڲ�����
* ���ڲ�����
-----------------------------------------------------------------------
* ˵����
***********************************************************************/

void RCC_Configuration(void)
{
    ErrorStatus HSEStartUpStatus;

    //ʹ���ⲿ����
    RCC_HSEConfig(RCC_HSE_ON);
    //�ȴ��ⲿ�����ȶ�
    HSEStartUpStatus = RCC_WaitForHSEStartUp();
    //����ⲿ���������ɹ����������һ������
    if(HSEStartUpStatus==SUCCESS)
    {
        //����HCLK��AHBʱ�ӣ�=SYSCLK
        RCC_HCLKConfig(RCC_SYSCLK_Div1);

        //PCLK1(APB1) = HCLK/2
        RCC_PCLK1Config(RCC_HCLK_Div2);

        //PCLK2(APB2) = HCLK
        RCC_PCLK2Config(RCC_HCLK_Div1);
        
 
        RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_4);
        //����PLL
        RCC_PLLCmd(ENABLE);
        //�ȴ�PLL�ȶ�
        while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
        //ϵͳʱ��SYSCLK����PLL���
        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
        //�л�ʱ�Ӻ�ȴ�ϵͳʱ���ȶ�
        while(RCC_GetSYSCLKSource()!=0x08);  
     }
	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_USART1|RCC_APB2Periph_TIM1, ENABLE);	  //��ӳ��ʱ��
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3 , ENABLE); //USART3ʱ��
		
		GPIO_PinRemapConfig(GPIO_PartialRemap_USART3, ENABLE); //?USART3??????PC10,PC11
		 
		RCC_ADCCLKConfig(RCC_PCLK2_Div2);  
}
