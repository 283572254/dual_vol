#ifndef CONTROL_H
#define CONTROL_H

#include "main.h"
 
void Control_Init(void);
void Control_Logical(void);
void System_impelment(void);


#define MSCR_ON    HAL_GPIO_WritePin(IGBT_Channel1_GPIO_Port, IGBT_Channel1_Pin, GPIO_PIN_RESET)
#define MSCR_OFF   HAL_GPIO_WritePin(IGBT_Channel1_GPIO_Port, IGBT_Channel1_Pin, GPIO_PIN_SET)
#define SSCR_ON    HAL_GPIO_WritePin(sIGBT_Channel1_GPIO_Port, sIGBT_Channel1_Pin, GPIO_PIN_RESET)
#define SSCR_OFF   HAL_GPIO_WritePin(sIGBT_Channel1_GPIO_Port, sIGBT_Channel1_Pin, GPIO_PIN_SET)
#define MRELAY_ON  HAL_GPIO_WritePin(KM_Channel1_GPIO_Port,KM_Channel1_Pin , GPIO_PIN_SET)
#define MRELAY_OFF HAL_GPIO_WritePin(KM_Channel1_GPIO_Port, KM_Channel1_Pin , GPIO_PIN_RESET)
#define SRELAY_ON  HAL_GPIO_WritePin(sKM_Channel1_GPIO_Port, sKM_Channel1_Pin , GPIO_PIN_SET)
#define SRELAY_OFF HAL_GPIO_WritePin(sKM_Channel1_GPIO_Port, sKM_Channel1_Pin , GPIO_PIN_RESET)
#define TEST_ON    HAL_GPIO_WritePin(Test_Pin_GPIO_Port, Test_Pin_Pin , GPIO_PIN_SET)
#define TEST_OFF   HAL_GPIO_WritePin(Test_Pin_GPIO_Port, Test_Pin_Pin , GPIO_PIN_RESET)

typedef struct  Control_Struct
{
    uint16_t Mscr   : 1;
    uint16_t Mrelay : 1;
    uint16_t Sscr   : 1;
    uint16_t Srelay : 1;
}OUTCONTROL;

typedef struct  
{
    uint16_t Line_Normal        : 1;
    uint16_t Line_Abnormal      : 1;
    uint16_t Line_Malfunction   : 1;
    uint16_t Line_AB_resume     : 1;
    uint16_t Line_MAL_resume    : 1;
    
}DEV_EVENT;

typedef struct  
{
    uint16_t  Zero_MainCurrent       : 1;
    uint16_t  Zero_SubCurrent        : 1;
    uint16_t  Zero_MainVoltage       : 1;
    uint16_t  Zero_SubVoltage        : 1;
    uint16_t  Broken_MainPower       : 1;
    uint16_t  Broken_SubPower        : 1;
}ZERO_STATUS;


typedef struct  
{
    uint16_t Dev_MainSCR : 1;
    uint16_t Dev_MainRelay : 1;
    uint16_t Dev_SubSCR : 1;
    uint16_t Dev_SubRelay : 1;
}DEV_STATUE;

// extern DEV_STATUE DEV_CurrentStaute;
extern DEV_EVENT Dev_Event;
extern ZERO_STATUS Zero_Status;


void System_Maltfunction(void);
#endif // CONTROL_H





