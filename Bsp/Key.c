
#include "Key.h"
#include "Timer.h"
KEY_STRUCT Key_Struct;
void ReadKey(void)
{
		static u8 Key_Lock_Flag=0;
    u8 i=0;
    u8 KeyTemp=0;
    static u8 Step[KEYNUM]={0,0};
    for(i=0;i<KEYNUM;i++)
    {
        switch (i)
        {
            case KEY1:
							if(GPIO_ReadInputDataBit(KEY1Port,KEY1Pin)==RESET)
							{
									KeyTemp = KEYRESETVALUE;
							}
							else
							{
									KeyTemp = KEYSETVALUE;
							}
              break;
            case KEY5:
							if(GPIO_ReadInputDataBit(KEY5Port,KEY5Pin)==RESET)
							{
									KeyTemp = KEYRESETVALUE;
							}
							else
							{
									KeyTemp = KEYSETVALUE;
							}
							break;
            
        }
        switch(Step[i])
        {
        case 0:
          if(KeyTemp==KEYRESETVALUE)
          {
							if(Key_Lock_Flag==0)
							{
								Key_Lock_Flag=1;
							}
              Key_Struct.KeyPressTime[i]=0;
              Step[i]=1;
          }
          break;
        case 1:
          Key_Struct.KeyTime[i]=Key_Struct.KeyPressTime[i];
          if(KeyTemp!=KEYRESETVALUE)
          {
              Step[i]=2;
          }
          break;
        case 2:
              Step[i]=3;
              Key_Struct.ReleaseTime[i]=0;;
        case 3:
          if(KeyTemp==KEYRESETVALUE)
          {
              //Param_Struct.ReleaseTime[i]++;
              Key_Struct.ReleaseTime[i]=0;
              Step[i]=1;
          }
          if(Key_Struct.ReleaseTime[i]>ButtonOffFilter)
          {
              Key_Struct.ReleaseTime[i]=0;
              Step[i]=4;
          }
          break;
        case 4:
					Key_Lock_Flag=0;
          Key_Struct.KeyValue[i]=0x01;
					Key_Struct.KeyPressFlag=1;
          //Param_Struct.SaveTime=0;
          Step[i]=0;
          break;
        }
    }
    for(i=0;i<KEYNUM;i++)
    {
        if(Key_Struct.KeyPressTime[i]<60000)
        {
            Key_Struct.KeyPressTime[i]++;
        }
        if(Key_Struct.ReleaseTime[i]<250)
        {
            Key_Struct.ReleaseTime[i]++;
        }
    }
}
u8 GetKeyValue(u8 KeyIndex)
{
		return Key_Struct.KeyValue[KeyIndex];
}
u16 GetKeyCount(u8 KeyIndex)
{
		return Key_Struct.KeyTime[KeyIndex];
}
void ClrKeyValue(u8 KeyIndex)
{
		Key_Struct.KeyValue[KeyIndex]=0;
}
void ClrKeyCount(u8 KeyIndex)
{
		Key_Struct.KeyTime[KeyIndex]=0;
		Key_Struct.KeyPressTime[KeyIndex]=0;
}
