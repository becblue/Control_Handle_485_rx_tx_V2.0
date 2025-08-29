#ifndef _IO_APP_C
#define _IO_APP_C
#include "IOApp.h"
#include "Key.h"

#if DEVICE_MODE==DEVICE_MODE_RECV
static uint32_t g_LastReceivedData = 0;  // 保存最后接收到的原始数据（接收模式专用）
#endif

#if DEVICE_MODE==DEVICE_MODE_SEND
// 发送模式下的调试计时器变量（500ms = 500次1ms中断）
uint16_t g_DebugTimer = 0;
// 标记是否有新的数据包需要输出
uint8_t g_HasNewDebugData = 0;
// 存储最新的接收调试数据
uint8_t g_LatestRxData[14];
uint16_t g_LatestCheckSum = 0;
uint8_t g_IsCrcError = 0;
// 发送数据调试相关
uint8_t g_HasNewTxData = 0;
uint8_t g_LatestTxData[14];
// PA0,PA3,PA4,PA5闪烁功能相关（50ms = 50次1ms中断）
uint16_t g_BlinkTimer = 0;
uint8_t g_BlinkState = 0;  // 0=灭，1=亮
uint8_t g_PA_BlinkEnable[4] = {0, 0, 0, 0};  // PA0,PA3,PA4,PA5闪烁使能
uint8_t g_PA_NormalEnable[4] = {0, 0, 0, 0}; // PA0,PA3,PA4,PA5正常置位使能
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
		GPIO_InitStructure.GPIO_Pin = IO_App_In_Struct[i].GPIO_Pin;              //准备设置PA2
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;      //IO速率50M
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	       //复用推挽输出，用于串口2的发送
		GPIO_Init(IO_App_In_Struct[i].GPIOx, &GPIO_InitStructure);                 //设置PA2
	}
	for(i=0;i<sizeof(IO_App_Out_Struct)/sizeof(IO_APP_STRUCT);i++)
	{
		GPIO_InitStructure.GPIO_Pin = IO_App_Out_Struct[i].GPIO_Pin;              //准备设置PA2
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;      //IO速率50M
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	       //复用推挽输出，用于串口2的发送
		GPIO_Init(IO_App_Out_Struct[i].GPIOx, &GPIO_InitStructure);                 //设置PA2
		GPIO_ResetBits(IO_App_Out_Struct[i].GPIOx,IO_App_Out_Struct[i].GPIO_Pin);
	}
}
void IO_APP_Get_IO_Value(uint8_t * UartTxBuff)
{
	uint32_t result = 0;
	static uint32_t ShiftValue[2]={0,0};
	uint8_t i=0;
#if DEVICE_MODE==DEVICE_MODE_RECV
	uint32_t originalResult = 0;  // 接收模式下使用的变量，移到函数开头声明
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

    // === 新增：仅在接收模式下调整返回数据 ===
#if DEVICE_MODE==DEVICE_MODE_RECV
    // 保存原始result作为输入状态
    originalResult = result;
    
    // 对于PC3-PC6（bit0-3），返回最后接收到的原始激活状态
    result = result & 0xFFFFFFF0;  // 清除bit0-3
    result = result | (g_LastReceivedData & 0x0000000F);  // 设置bit0-3为原始接收数据
    
    // bit4及以上保持原有逻辑（输入状态）
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
	// 发送模式下，PA0,PA3,PA4,PA5逻辑：检测接收数据包bit0-3 + 发送数据包PB0状态
	// 首先清空所有状态
	for(i = 0; i < 4; i++) {
		g_PA_BlinkEnable[i] = 0;
		g_PA_NormalEnable[i] = 0;
	}
	
	// 检测发送数据包中PB0的状态（从最新发送数据中获取）
	
	// 解析发送数据包中的IO状态（如果有效）
	if(g_LatestTxData[0] == 0xAA) {  // 有效的数据包
		tx_result = g_LatestTxData[1] + (g_LatestTxData[2]<<8) + (g_LatestTxData[3]<<16);
		pb0_in_tx_data = (tx_result & (0x01<<13)) ? 1 : 0;  // 检测bit13(PB0)
	}
	
	// 只处理PA0,PA3,PA4,PA5 (索引0-3)
	for(i = 0; i < 4; i++)
	{
		if((result&(0x01<<i))==(0x01<<i))  // 接收数据包中bit0-3被置位
		{
			if(pb0_in_tx_data)  // 发送数据包中PB0被置位
			{
				g_PA_BlinkEnable[i] = 1;   // 启用闪烁模式
			}
			else  // 发送数据包中PB0未置位
			{
				g_PA_NormalEnable[i] = 1;  // 启用正常置位模式
			}
		}
		// 如果接收数据包中bit0-3未置位，则保持关闭状态（已在上面清空）
	}
	
	// 立即应用新的状态（调用闪烁处理函数）
	PA_Blink_Process();
	
	// 处理PA6,PA7,PA8,PA12等其他IO（索引4及以上）
	// 这些IO保持原有的简单逻辑：直接根据接收数据包对应bit控制
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

    // === 新增：保存原始接收数据 ===
    g_LastReceivedData = result;

	// 接收模式下：PC3-PC6需要与PB0进行与逻辑判断
	for(i=0;i<sizeof(IO_App_Out_Struct)/sizeof(IO_APP_STRUCT);i++)
	{
		if((result&(0x01<<i))==(0x01<<i))
		{
			// PC3-PC6 (索引0-3) 需要与PB0 (索引13) 进行与逻辑判断
			if(i <= 3)  // PC3(0), PC4(1), PC5(2), PC6(3)
			{
				// 只有当PB0对应的bit13也被置位时，PC3-PC6才能输出高电平
				if((result&(0x01<<13))==(0x01<<13))  // PB0被置位
				{
					GPIO_SetBits(IO_App_Out_Struct[i].GPIOx,IO_App_Out_Struct[i].GPIO_Pin);
				}
				else
				{
					GPIO_ResetBits(IO_App_Out_Struct[i].GPIOx,IO_App_Out_Struct[i].GPIO_Pin);
				}
			}
			else  // 其他IO引脚保持原逻辑
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
/*函数名：USART3调试初始化函数                     */
/*参  数：波特率                                   */
/*返回值：无                                       */
/*说  明：初始化USART3用于调试输出，PB10(TX),PB11(RX)*/
/*-------------------------------------------------*/
void USART3_Debug_Init(uint32_t baudrate)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	// 使能USART3和GPIOB时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	// 配置PB10为USART3_TX (推挽复用输出)
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	// 配置PB11为USART3_RX (浮空输入)
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	// 配置USART3参数
	USART_InitStructure.USART_BaudRate = baudrate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_Init(USART3, &USART_InitStructure);
	
	// 使能USART3
	USART_Cmd(USART3, ENABLE);
}

/*-------------------------------------------------*/
/*函数名：USART3发送字符函数                       */
/*参  数：字符                                     */
/*返回值：无                                       */
/*说  明：通过USART3发送单个字符                   */
/*-------------------------------------------------*/
void USART3_SendChar(char ch)
{
	// 等待发送寄存器空
	while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
	
	// 发送字符
	USART_SendData(USART3, ch);
	
	// 等待发送完成
	while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);
}

/*-------------------------------------------------*/
/*函数名：USART3发送字符串函数                     */
/*参  数：字符串指针                               */
/*返回值：无                                       */
/*说  明：通过USART3发送字符串                     */
/*-------------------------------------------------*/
void Debug_Print(char* str)
{
	while(*str) {
		USART3_SendChar(*str);
		str++;
	}
}

/*-------------------------------------------------*/
/*函数名：USART3发送十六进制数函数                 */
/*参  数：32位数值                                */
/*返回值：无                                       */
/*说  明：以十六进制格式发送数值 (0x12345678)      */
/*-------------------------------------------------*/
void Debug_PrintHex(uint32_t value)
{
	char hex_chars[] = "0123456789ABCDEF";
	char buffer[11] = "0x";
	int i;
	
	// 转换为十六进制字符串
	for(i = 7; i >= 0; i--) {
		buffer[2 + (7-i)] = hex_chars[(value >> (i*4)) & 0x0F];
	}
	buffer[10] = '\0';
	
	Debug_Print(buffer);
}

#if DEVICE_MODE==DEVICE_MODE_SEND
/*-------------------------------------------------*/
/*函数名：调试数据输出函数                         */
/*参  数：无                                       */
/*返回值：无                                       */
/*说  明：每500ms输出一次调试信息，避免刷屏        */
/*-------------------------------------------------*/
void Debug_OutputData(void)
{
	int i;
	
	// 输出发送数据（如果有新的发送数据）
	if(g_HasNewTxData) {
		Debug_Print("TX Data: [");
		for(i = 0; i < 14; i++) {
			Debug_PrintHex(g_LatestTxData[i]);
			if(i < 13) Debug_Print(" ");
		}
		Debug_Print("]\r\n");
		
		// 输出发送的IO状态字节详细信息
		Debug_Print("TX IO Status: B0=");
		Debug_PrintHex(g_LatestTxData[1]);
		Debug_Print(" B1=");
		Debug_PrintHex(g_LatestTxData[2]);
		Debug_Print(" B2=");
		Debug_PrintHex(g_LatestTxData[3]);
		Debug_Print("\r\n");
		
		// 输出发送的具体IO位状态
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
		g_HasNewTxData = 0;  // 清除发送数据标记
	}
	
	// 输出接收数据（如果有新的接收数据）
	if(g_HasNewDebugData) {
		if(g_IsCrcError) {
			// CRC校验失败时的诊断信息
			Debug_Print("RX CRC Error! Calc=");
			Debug_PrintHex(g_LatestCheckSum);
			Debug_Print(" Recv=");
			Debug_PrintHex(g_LatestRxData[12]+(g_LatestRxData[13]<<8));
			Debug_Print("\r\n");
		} else {
			// 输出完整接收数据包
			Debug_Print("RX Data: [");
			for(i = 0; i < 14; i++) {
				Debug_PrintHex(g_LatestRxData[i]);
				if(i < 13) Debug_Print(" ");
			}
			Debug_Print("]\r\n");
			
			// 输出接收的IO状态字节的详细信息
			Debug_Print("RX IO Status: B0=");
			Debug_PrintHex(g_LatestRxData[1]);
			Debug_Print(" B1=");
			Debug_PrintHex(g_LatestRxData[2]);
			Debug_Print(" B2=");
			Debug_PrintHex(g_LatestRxData[3]);
			Debug_Print("\r\n");
			
			// 输出接收的具体IO位状态
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
		g_HasNewDebugData = 0;  // 清除接收数据标记
	}
	
	Debug_Print("\r\n");  // 添加分隔行
}

/*-------------------------------------------------*/
/*函数名：PA闪烁处理函数                           */
/*参  数：无                                       */
/*返回值：无                                       */
/*说  明：处理PA0,PA3,PA4,PA5的闪烁和正常状态     */
/*-------------------------------------------------*/
void PA_Blink_Process(void)
{
	int i;
	
	for(i = 0; i < 4; i++) {
		if(g_PA_BlinkEnable[i]) {
			// 闪烁模式：根据g_BlinkState设置GPIO状态
			if(g_BlinkState) {
				GPIO_SetBits(IO_App_Out_Struct[i].GPIOx, IO_App_Out_Struct[i].GPIO_Pin);
			} else {
				GPIO_ResetBits(IO_App_Out_Struct[i].GPIOx, IO_App_Out_Struct[i].GPIO_Pin);
			}
		} else if(g_PA_NormalEnable[i]) {
			// 正常模式：稳定置位
			GPIO_SetBits(IO_App_Out_Struct[i].GPIOx, IO_App_Out_Struct[i].GPIO_Pin);
		} else {
			// 关闭状态
			GPIO_ResetBits(IO_App_Out_Struct[i].GPIOx, IO_App_Out_Struct[i].GPIO_Pin);
		}
	}
}
#endif
#endif
