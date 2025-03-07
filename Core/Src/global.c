#include "global.h"

volatile uint8_t StartFlag = 0; //启动判据
volatile uint8_t LSC_flag = 0; //LSC启动判据

uint8_t u8FFt_Sequence[6] = {UM_C,US_C,CM_C,CS_C,SM_C,SS_C};

THRESHOLD Threshold;

OBSERVER Observer;

CHEACK Cheack_Flag;


void Threshold_Init(void)
{   
    Threshold.Main_Voltage  = 0.5;
    Threshold.Main_Current  = 0.5;
    Threshold.Main_Signal   = 0.6;

    Threshold.Sub_Current   = 0.5;
    Threshold.Sub_Voltage   = 0.5;
    Threshold.Sub_Signal    = 0.6;
}

void Observer_Init(void)
{   
    Observer.Main_Voltage  = 0;
    Observer.Main_Current  = 0;
    Observer.Main_Signal   = 0;

    Observer.Sub_Current   = 0;
    Observer.Sub_Voltage   = 0;
    Observer.Sub_Signal    = 0;
}
void Cheack_Flag_Init(void)
{
    Cheack_Flag.Main_Voltage  = 0;
    Cheack_Flag.Main_Current  = 0;
    Cheack_Flag.Main_Signal   = 0;

    Cheack_Flag.Sub_Current   = 0;
    Cheack_Flag.Sub_Voltage   = 0;  
    Cheack_Flag.Sub_Signal    = 0;
}

void Global_Init(void)
{
    Threshold_Init();
    Observer_Init();
    Cheack_Flag_Init();
}


















