#include "Control.h"

DEV_EVENT   Dev_Event =     {0,0,0,0,0};
ZERO_STATUS Zero_Status =   {0,0,0,0,0,0};


volatile uint8_t g_u8M_to_Sflag = 0;





extern float32_t FFT_Voltage_Value(RingBuffer *FVoltage_Value);
extern uint8_t Impelement_Start;
/** 
*转态1：运行在主路继电器
*状态2：运行在备路SCR
*状态3：运行在备路继电器
*状态4：运行在主路SCR
*次态5：同时运行在备路SCR和备路继电器
*次态6：同时运行在主路SCR和主路继电器
*/
uint8_t DEV_CurrentStaute = 0;
enum dev_CurrentStaute_enum
{
    MainRelay = 1,
    SubSCR ,
    SubRelay,
    MainSCR ,
    SubRelay_SubSCR,
    MianSCR_MainRelay 
};




void  EventStatue_Init(void)
{
    Dev_Event.Line_AB_resume   = 0;
    Dev_Event.Line_Abnormal    = 0;
    Dev_Event.Line_MAL_resume  = 0;
    Dev_Event.Line_Malfunction = 0;
    Dev_Event.Line_Normal      = 1;

    Zero_Status.Broken_MainPower = 0;
    Zero_Status.Broken_SubPower  = 0;
    Zero_Status.Zero_MainCurrent = 0;
    Zero_Status.Zero_MainVoltage = 0;
    Zero_Status.Zero_SubCurrent  = 0;
    Zero_Status.Zero_SubVoltage  = 0;

    DEV_CurrentStaute = MainRelay;
    // DEV_CurrentStaute.Dev_MainRelay = 1;
    // DEV_CurrentStaute.Dev_SubSCR   = 0;
    // DEV_CurrentStaute.Dev_SubRelay = 0;
    // DEV_CurrentStaute.Dev_MainSCR  = 0;
}

/**
 * @brief 初始化控制函数
 *
 * 该函数用于初始化控制相关的GPIO引脚，将所有IGBT和KM通道的引脚设置为低电平（RESET状态）。
 *
 * @note 无返回值
 */
void Control_Init(void)
{
    //主路控制初始化
    MSCR_OFF;
    MRELAY_ON;
    //备路控制初始化
    SSCR_OFF;
    SRELAY_OFF;
    //设备当前运行在主路继电器
    // MRELAY_ON;
    // HAL_Delay(1);
    // SRELAY_ON;
    // HAL_Delay(1);
    // SSCR_ON;
    // HAL_Delay(1);
    // MSCR_ON;
    // HAL_Delay(1);


    EventStatue_Init();
}

void FFT_Cheack_volatge(void)
{
    Cheack_Flag.Main_Voltage = FFT_Voltage_Value(&Rb_CalcData[UM_C]);
    if(Cheack_Flag.Main_Voltage<Threshold.Main_Voltage)
    {
        Dev_Event.Line_Malfunction = 1;
    }
    else if(Cheack_Flag.Main_Voltage > Threshold.Main_Voltage)
    {
        Dev_Event.Line_Malfunction = 0;
        Dev_Event.Line_Abnormal = 0;
        Dev_Event.Line_AB_resume = 1;
    }

}

void System_CleackResume(void)
{
    Cheack_Flag.Main_Signal = FFT_Voltage_Value(&Rb_CalcData[SM_C]);
    if(Cheack_Flag.Main_Signal < Threshold.Main_Signal)
    {
        Dev_Event.Line_Malfunction = 1;
    }
    else if(Cheack_Flag.Main_Signal > Threshold.Main_Signal)
    {
        Dev_Event.Line_Malfunction = 0;
        Dev_Event.Line_Abnormal = 0;
        Dev_Event.Line_MAL_resume = 1;
    }

}

void Dual_Task1(void)
{
    
    MRELAY_OFF;
    if(Zero_Status.Broken_MainPower == 1)
    {

        SSCR_ON;
        DEV_CurrentStaute = SubSCR;
        
        HAL_GPIO_WritePin(Test_Pin_GPIO_Port,Test_Pin_Pin,GPIO_PIN_SET);
    }   
}

void Dual_Task2(void)
{
    SRELAY_ON;
    if(Zero_Status.Zero_SubCurrent == 1)
    {
        
        DEV_CurrentStaute = SubRelay;
        g_u8M_to_Sflag = 1;
        HAL_Delay(30);
				SSCR_OFF;
    }
}

void Dual_Task3(void)
{
    if(Zero_Status.Zero_SubCurrent == 1 )
    {
        SSCR_OFF;
        MSCR_ON;
        DEV_CurrentStaute = MainSCR;
        MRELAY_ON;
        //判断主路继电器是否有电流
        HAL_Delay(30);
        MSCR_OFF;
        DEV_CurrentStaute = MainSCR;
        Dev_Event.Line_Malfunction = 0;
        Dev_Event.Line_AB_resume = 0;
        Dev_Event.Line_MAL_resume = 0;
        Impelement_Start = 0;
        g_u8M_to_Sflag = 0;
        Dev_Event.Line_Normal = 1;
         
    }
}
void Dual_Task4(void)
{
    SSCR_ON;
    SRELAY_OFF;
    DEV_CurrentStaute = SubRelay_SubSCR;
}

void System_Maltfunction(void)
{
    //主路切换到备路
    if(g_u8M_to_Sflag == 0)
    {
        //确定线路异常
        if(Dev_Event.Line_Abnormal == 1&&Impelement_Start==1&&Dev_Event.Line_Malfunction == 0)
        {
            //执行动作1：断开主路继电器，接通备路SCR
            Dual_Task1();
            //判断故障是否长期存在
            FFT_Cheack_volatge();
        }
        
        if(Dev_Event.Line_Malfunction == 1)
        {
            //确认故障长期存在，执行动作2：断开备路SCR,接通备路继电器
            Dual_Task2();
        }
        if(Dev_Event.Line_AB_resume == 1)
        {
            //非故障，电路异常恢复，执行动作3：断开备路SCR，接通主路SCR，接通主路继电器
            Dual_Task3();
        }   
    }
    //备路切换到主路
    else if(g_u8M_to_Sflag == 1)
    {
        System_CleackResume();
        if(Dev_Event.Line_MAL_resume == 1)
        {
            //执行动作4：断开备路继电器，接通备路SCR
            Dual_Task4();
            //判断故障备路继电器是否还有电流
            HAL_Delay(30);
            //执行动作3：断开备路SCR，接通主路SCR，接通主路继电器
            Dual_Task3();
          
        }
    }
}


void System_Maltfunction2(void)
{
    if(Impelement_Start==1)
    {
        MRELAY_OFF;
        
        HAL_Delay(5);
        while(!Zero_Status.Broken_MainPower);       //阻塞,判断主路是否完全断开,检测主路电流
        SSCR_ON;
        DEV_CurrentStaute = SubSCR;
        System_CleackResume();                       //判断故障是否长期存在               
        if(Dev_Event.Line_Malfunction == 1)
        {
            SRELAY_ON;
            DEV_CurrentStaute = SubRelay_SubSCR;
        }
        else if(Dev_Event.Line_AB_resume == 1)
        {
            SSCR_OFF;
            HAL_Delay(1);
            MSCR_ON;
            MRELAY_ON;
            DEV_CurrentStaute = MianSCR_MainRelay;
        }
        HAL_Delay(30);
        if(Dev_Event.Line_Malfunction == 1)
        {
            SSCR_OFF;
            DEV_CurrentStaute = SubRelay;
        
            Dev_Event.Line_Abnormal = 0;
        }
        else if(Dev_Event.Line_AB_resume == 1)
        {
            MSCR_OFF;
            DEV_CurrentStaute = MainRelay;
            EventStatue_Init();
            Dev_Event.Line_Abnormal = 0;
        }
        
    }
}
void System_Resume2(void)
{
    if(Dev_Event.Line_MAL_resume == 1)
    {
        SSCR_ON;
        HAL_Delay(1);
        SRELAY_OFF;
        HAL_Delay(5);
        DEV_CurrentStaute = SubSCR;
        while(!Zero_Status.Broken_MainPower);
        SSCR_OFF;
        while (!Zero_Status.Zero_SubCurrent);
        delay_us(100);
        MSCR_ON;
        MRELAY_ON;
        DEV_CurrentStaute = MianSCR_MainRelay;
        HAL_Delay(30);
        MSCR_OFF;
        DEV_CurrentStaute = MainRelay;
        EventStatue_Init();
    }
}



/**
 * @brief 系统实现函数
 *
 * 根据当前设备状态和事件，实现系统相应的逻辑控制。
 *
 * @note 具体的控制逻辑根据实际需求进行编写。
 */
void System_impelment(void)
{

    if(Dev_Event.Line_Abnormal == 1)
    {
        System_Maltfunction2();
    }
    else if((Dev_Event.Line_Malfunction == 1)&&(Dev_Event.Line_Abnormal==0))
    {
        
        System_CleackResume();
        System_Resume2();
    }
   
}




