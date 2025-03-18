
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#define KEYNUM 2
#define KEY1  0
#define KEY5  1

#define KEY1Port        GPIOC
#define KEY1Pin         GPIO_Pin_3
#define KEY5Port        GPIOC
#define KEY5Pin         GPIO_Pin_7


#define KEYRESETVALUE		0
#define KEYSETVALUE			1

//50ms为基准
#define ShortPress              5    
#define Long1SPress             100 
#define Long2SPress             200 
#define Long3SPress             300  
#define Long5SPress             500      

#define ButtonOnFilter 5
#define ButtonOffFilter 3

typedef struct _KEY_STRUCT
{
    u16 KeyTime[KEYNUM];//三个按键按下时间，供抬起判断用
    u16 KeyPressTime[KEYNUM];//三个按钮按下的时间，供程序使用。
    u8  KeyValue[KEYNUM];//三个按键的状态。
    u8  ReleaseTime[KEYNUM];
		u16 KeyBuzzerCount;
		u8 KeyPressFlag;
}KEY_STRUCT;
extern KEY_STRUCT Key_Struct;
void ReadKey(void);
u8 GetKeyValue(u8 KeyIndex);
u16 GetKeyCount(u8 KeyIndex);
void ClrKeyValue(u8 KeyIndex);
void ClrKeyCount(u8 KeyIndex);
