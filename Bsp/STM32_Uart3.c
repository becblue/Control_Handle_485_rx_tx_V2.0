#include "STM32_Uart3.h"
#include "StorageManager.h"

UART3_STRUCT	Uart3_Struct;
u8 Rev485Address=0;

void USART3_IRQHandler(void)//���ڴ�������
{
		volatile static u8 RxCount=0;
		volatile static u8 DataLen=0;
		volatile u8 DataTemp=0;
    if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
    {
				USART_ClearITPendingBit(USART3, USART_IT_RXNE); 
				if(timer.TimUplinkUartTimeOutAlarm==true)//��������ֽ�֮���ʱ�䳬��һ������������ӵ�0���ֽ����¿�ʼ���ա�
				{
						RxCount=0;
						DataLen=0;
				}
				timer.TimUplinkUartTimeOutAlarm=false;
				timer.SetTimer(&timer.TimUplinkUartTimeOutCount,5);
				Uart3_Struct.rx_data[RxCount]=USART_ReceiveData(USART3);
				if(Uart3_Struct.rx_data[0]==0xAA)
				{
						Uart3_Struct.Receive_Complete=1;
				}
				RxCount++;
				timer.TimReceiveTimeOutAlarm=false;
				timer.SetTimer(&timer.TimReceiveTimeOutCount,300);
		}
}

void Usart3_Init(unsigned int bound)
{  	 	
		GPIO_InitTypeDef GPIO_InitStructure;     //����һ������GPIO���ܵı���
		USART_InitTypeDef USART_InitStructure;   //����һ�����ô��ڹ��ܵı���
		NVIC_InitTypeDef NVIC_InitStructure;     //���ʹ�ܽ��չ��ܣ�����һ�������жϵı���
	
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);  //�����ж��������飺��2�� �������ȼ���0 1 2 3 �����ȼ���0 1 2 3
				
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);  //ʹ�ܴ���2ʱ��
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);   //ʹ��GPIOAʱ��
		USART_DeInit(USART3);                                  //����2�Ĵ�����������ΪĬ��ֵ
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;              //׼������PA2
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;      //IO����50M
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	       //����������������ڴ���2�ķ���
		GPIO_Init(GPIOB, &GPIO_InitStructure);                 //����PA2
		 
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;              //׼������PA3
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  //�������룬���ڴ���2�Ľ���
		GPIO_Init(GPIOB, &GPIO_InitStructure);                 //����PA3
		
		USART_InitStructure.USART_BaudRate = bound;                                    //����������
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;                    //8������λ
		USART_InitStructure.USART_StopBits = USART_StopBits_1;                         //1��ֹͣλ
		USART_InitStructure.USART_Parity = USART_Parity_No;                            //����żУ��λ
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
		USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;	               //�շ�ģʽ
     
		USART_Init(USART3, &USART_InitStructure);                                      //���ô���2	

		USART_ClearFlag(USART3, USART_FLAG_RXNE);	            //������ձ�־λ
		USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);          //���������ж�
		NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;       //���ô���2�ж�
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0; //��ռ���ȼ�0
		NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//�����ȼ�0
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//�ж�ͨ��ʹ��
		NVIC_Init(&NVIC_InitStructure);	                        //���ô���2�ж�

		USART_Cmd(USART3, ENABLE);                              //ʹ�ܴ���2
}
/********************************************************************************************
*�������ƣ�void UART3_SandData(u8 *str,u8 length)
*��ڲ�����*str ������׵�ַ��length Ҫ���͵��ֽڸ���
*���ڲ�������
*����˵����USART3�Ͷ��ֽں�����     
*******************************************************************************************/
void USART3_SendData(u8 *str,u8 length)
{
	u8 i;
	USART_ITConfig(USART3, USART_IT_RXNE, DISABLE); 	//ȡ���жϽ���
	for(i=0;i<length;i++)
	{
		USART_SendData(USART3, str[i]);  		     //��USART1����һ�ֽ�����  
		while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET){} 	//ͨ����鷢�ͼĴ�����ʶλ���ж������Ƿ������
	}	
	USART_ITConfig(USART3	, USART_IT_RXNE, ENABLE);  //���������ݿ��������ж�
}
