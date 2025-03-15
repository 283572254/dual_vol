#include "../USER/user.h"
#include "../BSP/driver.h"
#include "../Communication/protocol.h"
#include "../Communication/SendMessage.h"
#include "../Task/sys_task.h"
#include "../Task/sys_time.h"
#include "RingBuffer.h"
#include "global.h"
#include "Control.h"
extern float effect[SAMPLING_CHANNEL];
extern uint8_t DEV_CurrentStaute;
uint8_t my_sin[128] = {0};

void Decive_State_display(void);
void Display_GrapDate(void);
void Monitor_Data(void);
float32_t FFT_Voltage_Value(RingBuffer *FVoltage_Value);;
static float f32FFtTest[6] = {0};

static float compensation_value = 0;

sys_task_t task_Decive_State_display;
sys_task_t task_Display_GrapDate;
sys_task_t task_DaCai_protocol;
sys_task_t task_Monitor_Data;


void user_setup(void)
{

    init_driver();
    sys_task_create(&task_Decive_State_display, Decive_State_display, 100);
    sys_task_create(&task_Display_GrapDate, Display_GrapDate, 20);
    sys_task_create(&task_DaCai_protocol, DaCai_protocol, 1);
	sys_task_create(&task_Monitor_Data, Monitor_Data, 20);

    sys_task_start(&task_Decive_State_display);
    sys_task_start(&task_Display_GrapDate);
    sys_task_start(&task_DaCai_protocol);
    sys_task_start(&task_Monitor_Data);
}

void user_loop(void)
{
    
    sys_task_process();

}






void Display_GrapDate(void)
{
    if(compensation_value == 0)
        compensation_value = effect[0]+0.01f; 
    // 显示曲线
    
    SendGrapDate(4,2,0,my_sin,128);                                    //添加数据到曲线  一次两个数据
    for(int i= 0;i<128;i++)
    {
        my_sin[i] = (uint8_t)((Rb_CalcData[UM_C].buffer[i]+compensation_value)*100);
    }                                                   
    
    
}

void Decive_State_display(void)
{
    static uint8_t current_state = 0;
    static uint8_t last_state = 0;
    current_state = DEV_CurrentStaute;
    switch (current_state)
    {
    case 1:
        SendTextString(0x01,0x02,"ON");
        break;
    case 2:
        SendTextString(0x01,0x07,"ON");
        break;
    case 3:
        SendTextString(0x01,0x06,"ON");
        break;
    case 4:
        SendTextString(0x01,0x03,"ON");
        break;
    case 5:
        SendTextString(0x01,0x06,"ON");
        SendTextString(0x01,0x07,"ON");
        break;
    case 6:
        SendTextString(0x01,0x02,"ON");
        SendTextString(0x01,0x03,"ON");
        break;
    default:
        break;
    }
    if(last_state!= current_state)
    {

        SendTextString(0x01,0x02,"OFF");
        SendTextString(0x01,0x03,"OFF");
        SendTextString(0x01,0x06,"OFF");
        SendTextString(0x01,0x07,"OFF");
        
            
    }
    last_state = current_state;

}

void Monitor_Data(void)
{
    for(int i=0;i<6;i++)
    {
      int ci = u8FFt_Sequence[i];
      f32FFtTest[i] = FFT_Voltage_Value(&Rb_CalcData[ci]);
      
    }
    SendTextFloat(0x01,0x04,f32FFtTest[UM_C],2,1);
    SendTextFloat(0x01,0x05,f32FFtTest[CM_C],2,1);
    SendTextFloat(0x01,0x08,f32FFtTest[US_C],2,1);
    SendTextFloat(0x01,0x09,f32FFtTest[CS_C],2,1);
}

