#include "Control.h"

DEV_EVENT   Dev_Event =     {0,0,0,0,0};
ZERO_STATUS Zero_Status =   {0,0,0,0,0,0};


volatile uint8_t g_u8M_to_Sflag = 0;





extern float32_t FFT_Voltage_Value(RingBuffer *FVoltage_Value);
extern uint8_t Impelement_Start;
/** 
*ת̬1����������·�̵���
*״̬2�������ڱ�·SCR
*״̬3�������ڱ�·�̵���
*״̬4����������·SCR
*��̬5��ͬʱ�����ڱ�·SCR�ͱ�·�̵���
*��̬6��ͬʱ��������·SCR����·�̵���
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
 * @brief ��ʼ�����ƺ���
 *
 * �ú������ڳ�ʼ��������ص�GPIO���ţ�������IGBT��KMͨ������������Ϊ�͵�ƽ��RESET״̬����
 *
 * @note �޷���ֵ
 */
void Control_Init(void)
{
    //��·���Ƴ�ʼ��
    MSCR_OFF;
    MRELAY_ON;
    //��·���Ƴ�ʼ��
    SSCR_OFF;
    SRELAY_OFF;
    //�豸��ǰ��������·�̵���
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
        //�ж���·�̵����Ƿ��е���
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
    //��·�л�����·
    if(g_u8M_to_Sflag == 0)
    {
        //ȷ����·�쳣
        if(Dev_Event.Line_Abnormal == 1&&Impelement_Start==1&&Dev_Event.Line_Malfunction == 0)
        {
            //ִ�ж���1���Ͽ���·�̵�������ͨ��·SCR
            Dual_Task1();
            //�жϹ����Ƿ��ڴ���
            FFT_Cheack_volatge();
        }
        
        if(Dev_Event.Line_Malfunction == 1)
        {
            //ȷ�Ϲ��ϳ��ڴ��ڣ�ִ�ж���2���Ͽ���·SCR,��ͨ��·�̵���
            Dual_Task2();
        }
        if(Dev_Event.Line_AB_resume == 1)
        {
            //�ǹ��ϣ���·�쳣�ָ���ִ�ж���3���Ͽ���·SCR����ͨ��·SCR����ͨ��·�̵���
            Dual_Task3();
        }   
    }
    //��·�л�����·
    else if(g_u8M_to_Sflag == 1)
    {
        System_CleackResume();
        if(Dev_Event.Line_MAL_resume == 1)
        {
            //ִ�ж���4���Ͽ���·�̵�������ͨ��·SCR
            Dual_Task4();
            //�жϹ��ϱ�·�̵����Ƿ��е���
            HAL_Delay(30);
            //ִ�ж���3���Ͽ���·SCR����ͨ��·SCR����ͨ��·�̵���
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
        while(!Zero_Status.Broken_MainPower);       //����,�ж���·�Ƿ���ȫ�Ͽ�,�����·����
        SSCR_ON;
        DEV_CurrentStaute = SubSCR;
        System_CleackResume();                       //�жϹ����Ƿ��ڴ���               
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
 * @brief ϵͳʵ�ֺ���
 *
 * ���ݵ�ǰ�豸״̬���¼���ʵ��ϵͳ��Ӧ���߼����ơ�
 *
 * @note ����Ŀ����߼�����ʵ��������б�д��
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




