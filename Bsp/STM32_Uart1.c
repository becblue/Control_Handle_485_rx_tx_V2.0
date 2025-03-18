#include "STM32_Uart1.h"

UART1_STRUCT	Uart1_Struct;

void USART1_IRQHandler(void)//���ڴ�������
{
		volatile static u8 RxCount=0;
		volatile static u8 DataLen=0;
		volatile u8 DataTemp=0;
    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
				USART_ClearITPendingBit(USART1, USART_IT_RXNE); 
				if(timer.TimDownlinkUartTimeOutAlarm==true)//��������ֽ�֮���ʱ�䳬��һ������������ӵ�0���ֽ����¿�ʼ���ա�
				{
						RxCount=0;
						DataLen=0;
				}
				timer.TimDownlinkUartTimeOutAlarm=false;
				timer.SetTimer(&timer.TimDownlinkUartTimeOutCount,5);
				Uart1_Struct.rx_data[RxCount]=USART_ReceiveData(USART1);
				RxCount++;
				////////////////////////////�����///////////////////////////
				if(Uart1_Struct.rx_data[0]!=0xAA)
				{
						RxCount=0;
						DataLen=0;
						return;
				}
				if(RxCount>=14)
				{
						USART_ITConfig(USART1, USART_IT_RXNE, DISABLE); 	//ȡ���жϽ���
						Uart1_Struct.Receive_Complete=true;
						RxCount=0;
						DataLen=0;
						return;
				}
		}
}

void Usart1_Init(unsigned int bound)
{  	 	
		GPIO_InitTypeDef GPIO_InitStructure;     //����һ������GPIO���ܵı���
		USART_InitTypeDef USART_InitStructure;   //����һ�����ô��ڹ��ܵı���
		NVIC_InitTypeDef NVIC_InitStructure;     //���ʹ�ܽ��չ��ܣ�����һ�������жϵı���
	
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //�����ж��������飺��2�� �������ȼ���0 1 2 3 �����ȼ���0 1 2 3
				
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);  //ʹ�ܴ���2ʱ��
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);   //ʹ��GPIOAʱ��
		USART_DeInit(USART1);                                  //����2�Ĵ�����������ΪĬ��ֵ
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;              //׼������PA2
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;      //IO����50M
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	       //����������������ڴ���2�ķ���
		GPIO_Init(GPIOA, &GPIO_InitStructure);                 //����PA2
		 
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;              //׼������PA3
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  //�������룬���ڴ���2�Ľ���
		GPIO_Init(GPIOA, &GPIO_InitStructure);                 //����PA3
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;              //׼������PA2
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;      //IO����50M
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	       //����������������ڴ���2�ķ���
		GPIO_Init(GPIOA, &GPIO_InitStructure);                 //����PA2
		
		USART_InitStructure.USART_BaudRate = bound;                                    //����������
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;                    //8������λ
		USART_InitStructure.USART_StopBits = USART_StopBits_1;                         //1��ֹͣλ
		USART_InitStructure.USART_Parity = USART_Parity_No;                            //����żУ��λ
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
		USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;	               //�շ�ģʽ
     
		USART_Init(USART1, &USART_InitStructure);                                      //���ô���2	
		USART_ClearFlag(USART1, USART_FLAG_RXNE);	            //������ձ�־λ
		USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);          //���������ж�
		NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;       //���ô���2�ж�
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0; //��ռ���ȼ�0
		NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//�����ȼ�0
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//�ж�ͨ��ʹ��
		NVIC_Init(&NVIC_InitStructure);	                        //���ô���2�ж�

		GPIO_ResetBits(GPIOA,GPIO_Pin_11);
		USART_Cmd(USART1, ENABLE);                              //ʹ�ܴ���2
}
/********************************************************************************************
*�������ƣ�void UART3_SandData(u8 *str,u8 length)
*��ڲ�����*str ������׵�ַ��length Ҫ���͵��ֽڸ���
*���ڲ�������
*����˵����USART3�Ͷ��ֽں�����     
*******************************************************************************************/
void USART1_SendData(u8 *str,u8 length)
{
	u8 i;
	GPIO_SetBits(GPIOA,GPIO_Pin_11);
	USART_ITConfig(USART1, USART_IT_RXNE, DISABLE); 	//ȡ���жϽ���
	for(i=0;i<length;i++)
	{
		USART_SendData(USART1, str[i]);  		     //��USART1����һ�ֽ�����  
		while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET){} 	//ͨ����鷢�ͼĴ�����ʶλ���ж������Ƿ������
	}	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);  //���������ݿ��������ж�
	GPIO_ResetBits(GPIOA,GPIO_Pin_11);
}
