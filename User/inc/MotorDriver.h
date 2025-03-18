#include "stm32f10x.h"

#define MotorMaxNum	6
#define MotorPositionMaxNum	30

#define Dir_Up		0x00
#define Dir_Down	0x01
#define SpeedStep 20

typedef struct _MOTOR_RUN_PARAM
{
	GPIO_TypeDef* MotorDirPort[MotorMaxNum];
	uint16_t MotorDirPin[MotorMaxNum];
	GPIO_TypeDef* MotorShutDownPort[MotorMaxNum];
	uint16_t MotorShutDownPin[MotorMaxNum];
	GPIO_TypeDef* MotorONOFFPort[MotorMaxNum];
	uint16_t MotorONOFFPin[MotorMaxNum];
	u8 Dir[MotorMaxNum];
	u8 Speed[MotorMaxNum];
	s32 Position[MotorMaxNum];
	s32 PositionArray[MotorMaxNum][MotorPositionMaxNum+1];
}MOTOR_RUN_PARAM;

extern MOTOR_RUN_PARAM Motor_Run_Param;

void MotorCalPositionValue(void);
void MotorParamInit(void);
void MotorDriver(void);
void MotorTest(void);

