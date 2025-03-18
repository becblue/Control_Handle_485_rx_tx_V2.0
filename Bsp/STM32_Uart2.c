#include "STM32_Uart2.h"

UART2_STRUCT	Uart2_Struct;

void USART2_IRQHandler(void)//串口传送数据
{
		volatile static u8 RxCount=0;
		volatile static u8 DataLen=0;
		volatile u8 DataTemp=0;
    if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
    {
				USART_ClearITPendingBit(USART2, USART_IT_RXNE); 
				if(timer.TimDownlinkUartTimeOutAlarm==true)//如果两个字节之间的时间超过一定毫秒数，则从第0个字节重新开始接收。
				{
						RxCount=0;
						DataLen=0;
				}
				timer.TimDownlinkUartTimeOutAlarm=false;
				timer.SetTimer(&timer.TimDownlinkUartTimeOutCount,5);
				Uart2_Struct.rx_data[RxCount]=USART_ReceiveData(USART2);
				RxCount++;
				////////////////////////////单体机///////////////////////////
				if(Uart2_Struct.rx_data[0]!=0xAA)
				{
						RxCount=0;
						DataLen=0;
						return;
				}
				if(Uart2_Struct.rx_data[1]!=15 && RxCount==0x02)
				{
						RxCount=0;
						DataLen=0;
						return;
				}
				if(RxCount>=15)
				{
						Uart2_Struct.Receive_Complete=true;
						RxCount=0;
						DataLen=0;
						USART_ITConfig(USART2,USART_IT_RXNE,DISABLE);
						return;
				}
		}
}

void Usart2_Init(unsigned int bound)
{  	 	
		GPIO_InitTypeDef GPIO_InitStructure;     //定义一个设置GPIO功能的变量
		USART_InitTypeDef USART_InitStructure;   //定义一个设置串口功能的变量
		NVIC_InitTypeDef NVIC_InitStructure;     //如果使能接收功能，定义一个设置中断的变量
	
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //设置中断向量分组：第2组 抢先优先级：0 1 2 3 子优先级：0 1 2 3
				
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);  //使能串口2时钟
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);   //使能GPIOA时钟
		USART_DeInit(USART2);                                  //串口2寄存器重新设置为默认值
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;              //准备设置PA2
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;      //IO速率50M
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	       //复用推挽输出，用于串口2的发送
		GPIO_Init(GPIOA, &GPIO_InitStructure);                 //设置PA2
		 
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;              //准备设置PA3
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  //浮空输入，用于串口2的接收
		GPIO_Init(GPIOA, &GPIO_InitStructure);                 //设置PA3
		
		USART_InitStructure.USART_BaudRate = bound;                                    //波特率设置
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;                    //8个数据位
		USART_InitStructure.USART_StopBits = USART_StopBits_1;                         //1个停止位
		USART_InitStructure.USART_Parity = USART_Parity_No;                            //无奇偶校验位
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
		USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;	               //收发模式
     
		USART_Init(USART2, &USART_InitStructure);                                      //设置串口2	

		USART_ClearFlag(USART2, USART_FLAG_RXNE);	            //清除接收标志位
		USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);          //开启接收中断
		NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;       //设置串口2中断
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0; //抢占优先级0
		NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//子优先级0
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//中断通道使能
		NVIC_Init(&NVIC_InitStructure);	                        //设置串口2中断

		USART_Cmd(USART2, ENABLE);                              //使能串口2
}
/********************************************************************************************
*函数名称：void UART3_SandData(u8 *str,u8 length)
*入口参数：*str 数组的首地址；length 要发送的字节个数
*出口参数：无
*功能说明：USART3送多字节函数；     
*******************************************************************************************/
void USART2_SendData(u8 *str,u8 length)
{
	u8 i;
	USART_ITConfig(USART2, USART_IT_RXNE, DISABLE); 	//取消中断接收
	for(i=0;i<length;i++)
	{
		USART_SendData(USART2, str[i]);  		     //向USART1发送一字节数据  
		while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET){} 	//通过检查发送寄存器标识位来判断数据是否发送完毕
	}	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);  //发送完数据开启接收中断
}
