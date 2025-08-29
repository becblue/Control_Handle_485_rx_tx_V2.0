/*-------------------------------------------------*/
/*            超纬电子STM32系列开发板              */
/*-------------------------------------------------*/
/*                                                 */
/*            实现定时器4功能的源文件              */
/*                                                 */
/*-------------------------------------------------*/

#include "stm32f10x.h"  //包含需要的头文件
#include "timer2.h"
#include "Key.h"
#include "object/Timer.h"
#include "IOApp.h"

/*-------------------------------------------------*/
/*函数名：定时器4初始化                            */
/*参  数：arr：自动重装值   0~65535                */
/*参  数：psc：时钟预分频数 0~65535                */
/*返回值：无                                       */
/*说  明：定时时间：arr*psc*1000/72000000  单位ms  */
/*-------------------------------------------------*/
TIMER2_COUNTER Timer2_Counter;
void TIM2_Init(unsigned short int arr, unsigned short int psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;            //定义一个设置定时器的变量
	NVIC_InitTypeDef NVIC_InitStructure;                          //定义一个设置中断的变量
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);               //设置中断向量分组：第2组 抢先优先级：0 1 2 3 子优先级：0 1 2 3		
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);           //使能TIM4时钟	
	TIM_DeInit(TIM2);                                             //定时器4寄存器恢复默认值	
	TIM_TimeBaseInitStructure.TIM_Period = arr; 	              //设置自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;                  //设置定时器预分频数
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;     //1分频
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);            //设置TIM4
	
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);                    //清除溢出中断标志位
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);                      //使能TIM4溢出中断    
	TIM_Cmd(TIM2,ENABLE);                                        //先关闭TIM4                          
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn;                 //设置TIM4中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;       //抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;              //子优先级1
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;                 //中断通道使能
	NVIC_Init(&NVIC_InitStructure);                               //设置中断
}


/*-------------------------------------------------*/
/*函数名：定时器4中断服务函数                      */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void TIM2_IRQHandler(void)
{
#if DEVICE_MODE==DEVICE_MODE_SEND
	extern uint8_t g_HasNewDebugData;  // 接收数据标记
	extern uint8_t g_HasNewTxData;     // 发送数据标记
#endif

	if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
	{                //如果TIM_IT_Update置位，表示TIM4溢出中断，进入if	
		
		timerCount(&timer);
		ReadKey();
		
#if DEVICE_MODE==DEVICE_MODE_SEND
		// 发送模式下的调试计时器处理（每500ms输出一次）
		g_DebugTimer++;
		if(g_DebugTimer >= 500) {
			g_DebugTimer = 0;
			// 检查是否有新的调试数据需要输出（发送或接收数据）
			if(g_HasNewDebugData || g_HasNewTxData) {
				Debug_OutputData();
			}
		}
#endif
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);     			  //清除TIM4溢出中断标志 	
	}
}

