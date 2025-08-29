#ifndef _IO_APP_C
#define _IO_APP_C
#include "IOApp.h"
#include "Key.h"

#if DEVICE_MODE==DEVICE_MODE_RECV
static uint32_t g_LastReceivedData = 0;  // ���������յ���ԭʼ���ݣ�����ģʽר�ã�
#endif

#if DEVICE_MODE==DEVICE_MODE_SEND
// ����ģʽ�µĵ��Լ�ʱ��������500ms = 500��1ms�жϣ�
uint16_t g_DebugTimer = 0;
// ����Ƿ����µ����ݰ���Ҫ���
uint8_t g_HasNewDebugData = 0;
// �洢���µĽ��յ�������
uint8_t g_LatestRxData[14];
uint16_t g_LatestCheckSum = 0;
uint8_t g_IsCrcError = 0;
// �������ݵ������
uint8_t g_HasNewTxData = 0;
uint8_t g_LatestTxData[14];
// PA0,PA3,PA4,PA5��˸������أ�50ms = 50��1ms�жϣ�
uint16_t g_BlinkTimer = 0;
uint8_t g_BlinkState = 0;  // 0=��1=��
uint8_t g_PA_BlinkEnable[4] = {0, 0, 0, 0};  // PA0,PA3,PA4,PA5��˸ʹ��
uint8_t g_PA_NormalEnable[4] = {0, 0, 0, 0}; // PA0,PA3,PA4,PA5������λʹ��
#endif

#if DEVICE_MODE==DEVICE_MODE_SEND
IO_APP_STRUCT IO_App_Out_Struct[GPIO_LED_NUM]=
#else
IO_APP_STRUCT IO_App_In_Struct[GPIO_IN_NUM]=
#endif
{
	GPIOA,
	GPIO_Pin_0,
	GPIOA,
	GPIO_Pin_3,
	GPIOA,
	GPIO_Pin_4,
	GPIOA,
	GPIO_Pin_5,
	GPIOA,
	GPIO_Pin_6,
	GPIOA,
	GPIO_Pin_7,
	GPIOA,
	GPIO_Pin_8,
	GPIOA,
	GPIO_Pin_12,
	GPIOA,
	GPIO_Pin_15,
	GPIOC,
	GPIO_Pin_0,
	GPIOC,
	GPIO_Pin_1,
	GPIOC,
	GPIO_Pin_2,
};
#if DEVICE_MODE==DEVICE_MODE_SEND
IO_APP_STRUCT IO_App_In_Struct[GPIO_KEY_NUM]=
#else
IO_APP_STRUCT IO_App_Out_Struct[GPIO_OUT_NUM]=
#endif
{
	GPIOC,
	GPIO_Pin_3,
	GPIOC,
	GPIO_Pin_4,
	GPIOC,
	GPIO_Pin_5,
	GPIOC,
	GPIO_Pin_6,
	GPIOC,
	GPIO_Pin_7,
	GPIOC,
	GPIO_Pin_8,
	GPIOC,
	GPIO_Pin_9,
	GPIOC,
	GPIO_Pin_10,
	GPIOC,
	GPIO_Pin_11,
	GPIOC,
	GPIO_Pin_12,
	GPIOC,
	GPIO_Pin_13,
	GPIOC,
	GPIO_Pin_14,
	GPIOC,
	GPIO_Pin_15,
	GPIOB,
	GPIO_Pin_0,
	GPIOB,
	GPIO_Pin_1,
	GPIOB,
	GPIO_Pin_3,
	GPIOB,
	GPIO_Pin_4,
	GPIOB,
	GPIO_Pin_5,
	GPIOB,
	GPIO_Pin_6,
	GPIOB,
	GPIO_Pin_7,
	GPIOB,
	GPIO_Pin_8,
	GPIOB,
	GPIO_Pin_9,
	GPIOB,
	GPIO_Pin_10,
	GPIOB,
	GPIO_Pin_11,
};
void IO_App_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	uint8_t i=0;
	IO_Init();
	
	for(i=0;i<sizeof(IO_App_In_Struct)/sizeof(IO_APP_STRUCT);i++)
	{
		GPIO_InitStructure.GPIO_Pin = IO_App_In_Struct[i].GPIO_Pin;              //׼������PA2
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;      //IO����50M
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	       //����������������ڴ���2�ķ���
		GPIO_Init(IO_App_In_Struct[i].GPIOx, &GPIO_InitStructure);                 //����PA2
	}
	for(i=0;i<sizeof(IO_App_Out_Struct)/sizeof(IO_APP_STRUCT);i++)
	{
		GPIO_InitStructure.GPIO_Pin = IO_App_Out_Struct[i].GPIO_Pin;              //׼������PA2
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;      //IO����50M
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	       //����������������ڴ���2�ķ���
		GPIO_Init(IO_App_Out_Struct[i].GPIOx, &GPIO_InitStructure);                 //����PA2
		GPIO_ResetBits(IO_App_Out_Struct[i].GPIOx,IO_App_Out_Struct[i].GPIO_Pin);
	}
}
void IO_APP_Get_IO_Value(uint8_t * UartTxBuff)
{
	uint32_t result = 0;
	static uint32_t ShiftValue[2]={0,0};
	uint8_t i=0;
#if DEVICE_MODE==DEVICE_MODE_RECV
	uint32_t originalResult = 0;  // ����ģʽ��ʹ�õı������Ƶ�������ͷ����
#endif
	for(i=0;i<sizeof(IO_App_In_Struct)/sizeof(IO_APP_STRUCT);i++)
	{
#if DEVICE_MODE==DEVICE_MODE_SEND
		if(GPIO_ReadInputDataBit(IO_App_In_Struct[i].GPIOx,IO_App_In_Struct[i].GPIO_Pin)==RESET
			&& (i>7))
		{
			result=result|(0x01<<i);
		}
		if(GetKeyValue(KEY1)==1)
		{
			ClrKeyCount(KEY1);
			ClrKeyValue(KEY1);
			if(ShiftValue[0]==0)
			{
				ShiftValue[0]=1;
			}
			else if(ShiftValue[0]==0x08)
			{
				ShiftValue[0]=0;
			}
			else
			{
				ShiftValue[0]=ShiftValue[0]<<1;
			}
		}
		if(GetKeyValue(KEY5)==1)
		{
			ClrKeyCount(KEY5);
			ClrKeyValue(KEY5);
			if(ShiftValue[1]==0)
			{
				ShiftValue[1]=0x10;
			}
			else if(ShiftValue[1]==0x80)
			{
				ShiftValue[1]=0;
			}
			else
			{
				ShiftValue[1]=ShiftValue[1]<<1;
			}
		}
		result=result&(0xFFFFFFFF-ShiftValue[0]);
		result=result|(ShiftValue[0]);
		result=result&(0xFFFFFFFF-ShiftValue[1]);
		result=result|(ShiftValue[1]);
#else
		if(GPIO_ReadInputDataBit(IO_App_In_Struct[i].GPIOx,IO_App_In_Struct[i].GPIO_Pin)==SET)
		{
			result=result|(0x01<<i);
		}
#endif
	}

    // === ���������ڽ���ģʽ�µ����������� ===
#if DEVICE_MODE==DEVICE_MODE_RECV
    // ����ԭʼresult��Ϊ����״̬
    originalResult = result;
    
    // ����PC3-PC6��bit0-3�������������յ���ԭʼ����״̬
    result = result & 0xFFFFFFF0;  // ���bit0-3
    result = result | (g_LastReceivedData & 0x0000000F);  // ����bit0-3Ϊԭʼ��������
    
    // bit4�����ϱ���ԭ���߼�������״̬��
#endif

	UartTxBuff[0]=((result>>0)&0xFF);
	UartTxBuff[1]=((result>>8)&0xFF);
	UartTxBuff[2]=((result>>16)&0xFF);
}
void IO_APP_Set_IO_Value(uint8_t * UartTxBuff)
{
	uint32_t result;
	uint8_t i=0;
#if DEVICE_MODE==DEVICE_MODE_SEND
	extern uint8_t g_LatestTxData[14];
	uint32_t tx_result = 0;
	uint8_t pb0_in_tx_data = 0;
#endif
	
	result=UartTxBuff[0] + (UartTxBuff[1]<<8) + (UartTxBuff[2]<<16);
	
#if DEVICE_MODE==DEVICE_MODE_SEND
	// ����ģʽ�£�PA0,PA3,PA4,PA5�߼������������ݰ�bit0-3 + �������ݰ�PB0״̬
	// �����������״̬
	for(i = 0; i < 4; i++) {
		g_PA_BlinkEnable[i] = 0;
		g_PA_NormalEnable[i] = 0;
	}
	
	// ��ⷢ�����ݰ���PB0��״̬�������·��������л�ȡ��
	
	// �����������ݰ��е�IO״̬�������Ч��
	if(g_LatestTxData[0] == 0xAA) {  // ��Ч�����ݰ�
		tx_result = g_LatestTxData[1] + (g_LatestTxData[2]<<8) + (g_LatestTxData[3]<<16);
		pb0_in_tx_data = (tx_result & (0x01<<13)) ? 1 : 0;  // ���bit13(PB0)
	}
	
	// ֻ����PA0,PA3,PA4,PA5 (����0-3)
	for(i = 0; i < 4; i++)
	{
		if((result&(0x01<<i))==(0x01<<i))  // �������ݰ���bit0-3����λ
		{
			if(pb0_in_tx_data)  // �������ݰ���PB0����λ
			{
				g_PA_BlinkEnable[i] = 1;   // ������˸ģʽ
			}
			else  // �������ݰ���PB0δ��λ
			{
				g_PA_NormalEnable[i] = 1;  // ����������λģʽ
			}
		}
		// ����������ݰ���bit0-3δ��λ���򱣳ֹر�״̬������������գ�
	}
	
	// ����Ӧ���µ�״̬��������˸��������
	PA_Blink_Process();
	
	// ����PA6,PA7,PA8,PA12������IO������4�����ϣ�
	// ��ЩIO����ԭ�еļ��߼���ֱ�Ӹ��ݽ������ݰ���Ӧbit����
	for(i = 4; i < sizeof(IO_App_Out_Struct)/sizeof(IO_APP_STRUCT); i++)
	{
		if((result&(0x01<<i))==(0x01<<i))
		{
			GPIO_SetBits(IO_App_Out_Struct[i].GPIOx,IO_App_Out_Struct[i].GPIO_Pin);
		}
		else
		{
			GPIO_ResetBits(IO_App_Out_Struct[i].GPIOx,IO_App_Out_Struct[i].GPIO_Pin);
		}
	}
#else

    // === ����������ԭʼ�������� ===
    g_LastReceivedData = result;

	// ����ģʽ�£�PC3-PC6��Ҫ��PB0�������߼��ж�
	for(i=0;i<sizeof(IO_App_Out_Struct)/sizeof(IO_APP_STRUCT);i++)
	{
		if((result&(0x01<<i))==(0x01<<i))
		{
			// PC3-PC6 (����0-3) ��Ҫ��PB0 (����13) �������߼��ж�
			if(i <= 3)  // PC3(0), PC4(1), PC5(2), PC6(3)
			{
				// ֻ�е�PB0��Ӧ��bit13Ҳ����λʱ��PC3-PC6��������ߵ�ƽ
				if((result&(0x01<<13))==(0x01<<13))  // PB0����λ
				{
					GPIO_SetBits(IO_App_Out_Struct[i].GPIOx,IO_App_Out_Struct[i].GPIO_Pin);
				}
				else
				{
					GPIO_ResetBits(IO_App_Out_Struct[i].GPIOx,IO_App_Out_Struct[i].GPIO_Pin);
				}
			}
			else  // ����IO���ű���ԭ�߼�
			{
				GPIO_SetBits(IO_App_Out_Struct[i].GPIOx,IO_App_Out_Struct[i].GPIO_Pin);
			}
		}
		else
		{
			GPIO_ResetBits(IO_App_Out_Struct[i].GPIOx,IO_App_Out_Struct[i].GPIO_Pin);
		}
	}
#endif
}

/*-------------------------------------------------*/
/*��������USART3���Գ�ʼ������                     */
/*��  ����������                                   */
/*����ֵ����                                       */
/*˵  ������ʼ��USART3���ڵ��������PB10(TX),PB11(RX)*/
/*-------------------------------------------------*/
void USART3_Debug_Init(uint32_t baudrate)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	// ʹ��USART3��GPIOBʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	// ����PB10ΪUSART3_TX (���츴�����)
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	// ����PB11ΪUSART3_RX (��������)
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	// ����USART3����
	USART_InitStructure.USART_BaudRate = baudrate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_Init(USART3, &USART_InitStructure);
	
	// ʹ��USART3
	USART_Cmd(USART3, ENABLE);
}

/*-------------------------------------------------*/
/*��������USART3�����ַ�����                       */
/*��  �����ַ�                                     */
/*����ֵ����                                       */
/*˵  ����ͨ��USART3���͵����ַ�                   */
/*-------------------------------------------------*/
void USART3_SendChar(char ch)
{
	// �ȴ����ͼĴ�����
	while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
	
	// �����ַ�
	USART_SendData(USART3, ch);
	
	// �ȴ��������
	while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);
}

/*-------------------------------------------------*/
/*��������USART3�����ַ�������                     */
/*��  �����ַ���ָ��                               */
/*����ֵ����                                       */
/*˵  ����ͨ��USART3�����ַ���                     */
/*-------------------------------------------------*/
void Debug_Print(char* str)
{
	while(*str) {
		USART3_SendChar(*str);
		str++;
	}
}

/*-------------------------------------------------*/
/*��������USART3����ʮ������������                 */
/*��  ����32λ��ֵ                                */
/*����ֵ����                                       */
/*˵  ������ʮ�����Ƹ�ʽ������ֵ (0x12345678)      */
/*-------------------------------------------------*/
void Debug_PrintHex(uint32_t value)
{
	char hex_chars[] = "0123456789ABCDEF";
	char buffer[11] = "0x";
	int i;
	
	// ת��Ϊʮ�������ַ���
	for(i = 7; i >= 0; i--) {
		buffer[2 + (7-i)] = hex_chars[(value >> (i*4)) & 0x0F];
	}
	buffer[10] = '\0';
	
	Debug_Print(buffer);
}

#if DEVICE_MODE==DEVICE_MODE_SEND
/*-------------------------------------------------*/
/*�����������������������                         */
/*��  ������                                       */
/*����ֵ����                                       */
/*˵  ����ÿ500ms���һ�ε�����Ϣ������ˢ��        */
/*-------------------------------------------------*/
void Debug_OutputData(void)
{
	int i;
	
	// ����������ݣ�������µķ������ݣ�
	if(g_HasNewTxData) {
		Debug_Print("TX Data: [");
		for(i = 0; i < 14; i++) {
			Debug_PrintHex(g_LatestTxData[i]);
			if(i < 13) Debug_Print(" ");
		}
		Debug_Print("]\r\n");
		
		// ������͵�IO״̬�ֽ���ϸ��Ϣ
		Debug_Print("TX IO Status: B0=");
		Debug_PrintHex(g_LatestTxData[1]);
		Debug_Print(" B1=");
		Debug_PrintHex(g_LatestTxData[2]);
		Debug_Print(" B2=");
		Debug_PrintHex(g_LatestTxData[3]);
		Debug_Print("\r\n");
		
		// ������͵ľ���IOλ״̬
		{
			uint32_t tx_io_result = (((uint32_t)g_LatestTxData[3])<<16) | 
			                        (((uint32_t)g_LatestTxData[2])<<8) | 
			                        ((uint32_t)g_LatestTxData[1]);
			Debug_Print("TX IO Bits: ");
			Debug_PrintHex(tx_io_result);
			Debug_Print(" -> KEY1=");
			Debug_PrintHex((tx_io_result>>0)&1);
			Debug_Print(" PC4=");
			Debug_PrintHex((tx_io_result>>1)&1);
			Debug_Print(" PC5=");
			Debug_PrintHex((tx_io_result>>2)&1);
			Debug_Print(" PC6=");
			Debug_PrintHex((tx_io_result>>3)&1);
			Debug_Print(" bit8=");
			Debug_PrintHex((tx_io_result>>8)&1);
			Debug_Print("\r\n");
		}
		g_HasNewTxData = 0;  // ����������ݱ��
	}
	
	// ����������ݣ�������µĽ������ݣ�
	if(g_HasNewDebugData) {
		if(g_IsCrcError) {
			// CRCУ��ʧ��ʱ�������Ϣ
			Debug_Print("RX CRC Error! Calc=");
			Debug_PrintHex(g_LatestCheckSum);
			Debug_Print(" Recv=");
			Debug_PrintHex(g_LatestRxData[12]+(g_LatestRxData[13]<<8));
			Debug_Print("\r\n");
		} else {
			// ��������������ݰ�
			Debug_Print("RX Data: [");
			for(i = 0; i < 14; i++) {
				Debug_PrintHex(g_LatestRxData[i]);
				if(i < 13) Debug_Print(" ");
			}
			Debug_Print("]\r\n");
			
			// ������յ�IO״̬�ֽڵ���ϸ��Ϣ
			Debug_Print("RX IO Status: B0=");
			Debug_PrintHex(g_LatestRxData[1]);
			Debug_Print(" B1=");
			Debug_PrintHex(g_LatestRxData[2]);
			Debug_Print(" B2=");
			Debug_PrintHex(g_LatestRxData[3]);
			Debug_Print("\r\n");
			
			// ������յľ���IOλ״̬
			{
				uint32_t rx_io_result = (((uint32_t)g_LatestRxData[3])<<16) | 
				                        (((uint32_t)g_LatestRxData[2])<<8) | 
				                        ((uint32_t)g_LatestRxData[1]);
				Debug_Print("RX IO Bits: ");
				Debug_PrintHex(rx_io_result);
				Debug_Print(" -> bit0=");
				Debug_PrintHex((rx_io_result>>0)&1);
				Debug_Print(" bit1=");
				Debug_PrintHex((rx_io_result>>1)&1);
				Debug_Print(" bit2=");
				Debug_PrintHex((rx_io_result>>2)&1);
				Debug_Print(" bit3=");
				Debug_PrintHex((rx_io_result>>3)&1);
				Debug_Print(" bit8=");
				Debug_PrintHex((rx_io_result>>8)&1);
				Debug_Print(" bit13=");
				Debug_PrintHex((rx_io_result>>13)&1);
				Debug_Print("\r\n");
			}
		}
		g_HasNewDebugData = 0;  // ����������ݱ��
	}
	
	Debug_Print("\r\n");  // ��ӷָ���
}

/*-------------------------------------------------*/
/*��������PA��˸������                           */
/*��  ������                                       */
/*����ֵ����                                       */
/*˵  ��������PA0,PA3,PA4,PA5����˸������״̬     */
/*-------------------------------------------------*/
void PA_Blink_Process(void)
{
	int i;
	
	for(i = 0; i < 4; i++) {
		if(g_PA_BlinkEnable[i]) {
			// ��˸ģʽ������g_BlinkState����GPIO״̬
			if(g_BlinkState) {
				GPIO_SetBits(IO_App_Out_Struct[i].GPIOx, IO_App_Out_Struct[i].GPIO_Pin);
			} else {
				GPIO_ResetBits(IO_App_Out_Struct[i].GPIOx, IO_App_Out_Struct[i].GPIO_Pin);
			}
		} else if(g_PA_NormalEnable[i]) {
			// ����ģʽ���ȶ���λ
			GPIO_SetBits(IO_App_Out_Struct[i].GPIOx, IO_App_Out_Struct[i].GPIO_Pin);
		} else {
			// �ر�״̬
			GPIO_ResetBits(IO_App_Out_Struct[i].GPIOx, IO_App_Out_Struct[i].GPIO_Pin);
		}
	}
}
#endif
#endif
