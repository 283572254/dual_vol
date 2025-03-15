
#include "../BSP/SERIAL/serial.h"
#include "../BSP/RINGQUEUE/ringqueue.h"


uint8_t uart_rx_dma_buffer[UART_DMA_BUFFER_SIZE];  // DMA接收缓冲区
uint8_t send_buffer[SENDBUFFER_SIZE];  // 发送缓冲区
OPREATION husart1__operation = {0, 0};




// 初始化串口驱动

void UART_DMA_Init(UART_HandleTypeDef *huart) 
{
    HAL_UARTEx_ReceiveToIdle_DMA(huart, uart_rx_dma_buffer, UART_DMA_BUFFER_SIZE);
}




void UART_Driver_Init(UART_HandleTypeDef *huart) 
{
    ringqueue_init(&husart_rx_rb);
    ringqueue_init(&husart_tx_rb);
    UART_DMA_Init(huart);
}




void UART_SendData(uint8_t *data, uint16_t size, UART_HandleTypeDef *huart) 
{
    // 将数据写入环形缓冲区
    static uint8_t buffer[DMA_TX_BUFFER_SIZE];
    uint16_t size_to_send = 0;
    for (uint16_t i = 0; i < size; i++) {
        ringqueue_write(&husart_tx_rb, data[i]);
    }

    // 如果没有正在发送的数据，启动 DMA 发送
    if (!husart1__operation.send_flag) 
    {
        husart1__operation.send_flag = 1;


        // 从环形缓冲区读取数据到 DMA 缓冲区
        while (!ringqueue_is_empty(&husart_tx_rb) && size_to_send < DMA_TX_BUFFER_SIZE)
        {
            ringqueue_read(&husart_tx_rb, &buffer[size_to_send]);
            size_to_send++;
        }

        // 启动 DMA 发送
        if (size_to_send > 0)
        {
            HAL_UART_Transmit_DMA(huart, buffer, size_to_send);
        } 
        else
        {
            husart1__operation.send_flag = 0;  // 没有数据可发送
            size_to_send = 0;
        }
    }
}
uint8_t buffer[DMA_TX_BUFFER_SIZE];
// DMA 发送完成回调函数
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) 
{
    if (huart->Instance == USART2) 
    {
        // 启动继续发送剩余数据

        uint16_t size_to_send = 0;

        // 从环形缓冲区读取数据到 DMA 缓冲区
        while (!ringqueue_is_empty(&husart_tx_rb) && size_to_send < DMA_TX_BUFFER_SIZE) 
        {
            ringqueue_read(&husart_tx_rb, &buffer[size_to_send]);
            size_to_send++;
        }

        if (size_to_send > 0) {
            // 继续使用 DMA 发送
            HAL_UART_Transmit_DMA(huart, buffer, size_to_send);
        } else {
            // 如果数据已全部发送，更新状态
            husart1__operation.send_flag = 0;
        }
    }
}



void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size) {
    if (huart->Instance == USART2)  // 使用宏定义，支持更换串口
    {
        // 确保接收环形缓冲区没有满
        if (!ringqueue_is_full(&husart_rx_rb))
         {
            // 将DMA接收到的数据写入接收环形缓冲区
            for (uint16_t i = 0; i < Size; i++)
             {
                if (!ringqueue_is_full(&husart_rx_rb))
                 {
                    ringqueue_write(&husart_rx_rb, uart_rx_dma_buffer[i]);
                } else 
                {
                    break;  // 缓冲区满时退出
                }
            }
        }

        // 清空 DMA 缓冲区
        memset(uart_rx_dma_buffer, 0, sizeof(uart_rx_dma_buffer));
        
        // 重新启动 DMA 接收
        HAL_UARTEx_ReceiveToIdle_DMA(huart, uart_rx_dma_buffer, UART_DMA_BUFFER_SIZE);
        husart1__operation.recv_flag = 1;
    }
}








#ifdef __GNUC__
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

/**
* @brief  Retargets the C library printf function to the USART.
* @param  None
* @retval None
*/
PUTCHAR_PROTOTYPE
{
    /* Place your implementation of fputc here */
    /* e.g. write a character to the EVAL_COM1 and Loop until the end of transmission */
    HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xFFFF);

    return ch;
}



