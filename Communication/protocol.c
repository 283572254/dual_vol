#include "../Communication/protocol.h"
#include "../BSP/RINGQUEUE/ringqueue.h"
#include "../BSP/SERIAL/serial.h"
#include "../Communication/Notify.h"
#define TEMP_BUFFER_SIZE 128  // 临时缓冲区大小
_SaveData DaCai_Save_Data;
void DaCai_protocol_analysis(void);

uint8_t temp_buffer[TEMP_BUFFER_SIZE];  // 临时缓冲区
size_t temp_buffer_index = 0;           // 临时缓冲区索引
void DaCai_protocol(void)
{
    uint8_t data;

    if(husart1__operation.recv_flag == 1)
    {
        HAL_NVIC_DisableIRQ(USART1_IRQn);
        while(!ringqueue_is_empty(&husart_rx_rb))
        {
            ringqueue_read(&husart_rx_rb, &data);
            if(temp_buffer_index < TEMP_BUFFER_SIZE)
            {
                temp_buffer[temp_buffer_index++] = data;  
            }
            else
            {
                temp_buffer_index = 0;
            }
            if (temp_buffer_index >= 1 && temp_buffer[temp_buffer_index - 1] == 0xEE) 
            {
                    DaCai_Save_Data.head = 1;
            }  
            if (temp_buffer_index >= 4 &&
                temp_buffer[temp_buffer_index - 1] == 0xFF &&
                temp_buffer[temp_buffer_index - 2] == 0xFF &&
                temp_buffer[temp_buffer_index - 3] == 0xFC &&
                temp_buffer[temp_buffer_index - 4] == 0xFF)
            {
                
                // 解析数据
                DaCai_Save_Data.screenID = (temp_buffer[3] << 8) | temp_buffer[4];
                DaCai_Save_Data.widgetID = (temp_buffer[5] << 8) | temp_buffer[6];
                DaCai_Save_Data.widgetType = temp_buffer[7];
                DaCai_Save_Data.isGetData = true;
                DaCai_Save_Data.isGetData = true;
                
                if (temp_buffer_index - 8 <= DACAI_BUFFER_SIZE) {
                    memset(DaCai_Save_Data.DACAI_Buffer, 0, DACAI_BUFFER_SIZE);
                    memcpy(DaCai_Save_Data.DACAI_Buffer, temp_buffer + 8, temp_buffer_index - 8);
                }
                else 
                {
                    // 处理错误：数据超出缓冲区大小
                    printf("protocol_analysis: data size error!");
                }
                DaCai_protocol_analysis();
                // 重置临时缓冲区和索引
                temp_buffer_index = 0;
                break;  // 退出循环
                
            }      
        }
        HAL_NVIC_EnableIRQ(USART1_IRQn);
        husart1__operation.recv_flag = 0;
    }
}

void DaCai_protocol_analysis(void)
{
    if(DaCai_Save_Data.isGetData==1)
    {
        //"It checks what widget this data represents."
        switch (DaCai_Save_Data.widgetType)
        {
            case kCtrlButton:
                //"It handles the control button data."
                NotifyButton(DaCai_Save_Data.screenID, DaCai_Save_Data.widgetID, DaCai_Save_Data.DACAI_Buffer[0]);
            break;
        }

    }
}










