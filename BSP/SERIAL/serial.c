
#include "../BSP/SERIAL/serial.h"
#include "../BSP/RINGQUEUE/ringqueue.h"


uint8_t uart_rx_dma_buffer[UART_DMA_BUFFER_SIZE];  // DMA���ջ�����
uint8_t send_buffer[SENDBUFFER_SIZE];  // ���ͻ�����
OPREATION husart1__operation = {0, 0};




// ��ʼ����������

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
    // ������д�뻷�λ�����
    static uint8_t buffer[DMA_TX_BUFFER_SIZE];
    uint16_t size_to_send = 0;
    for (uint16_t i = 0; i < size; i++) {
        ringqueue_write(&husart_tx_rb, data[i]);
    }

    // ���û�����ڷ��͵����ݣ����� DMA ����
    if (!husart1__operation.send_flag) 
    {
        husart1__operation.send_flag = 1;


        // �ӻ��λ�������ȡ���ݵ� DMA ������
        while (!ringqueue_is_empty(&husart_tx_rb) && size_to_send < DMA_TX_BUFFER_SIZE)
        {
            ringqueue_read(&husart_tx_rb, &buffer[size_to_send]);
            size_to_send++;
        }

        // ���� DMA ����
        if (size_to_send > 0)
        {
            HAL_UART_Transmit_DMA(huart, buffer, size_to_send);
        } 
        else
        {
            husart1__operation.send_flag = 0;  // û�����ݿɷ���
            size_to_send = 0;
        }
    }
}
uint8_t buffer[DMA_TX_BUFFER_SIZE];
// DMA ������ɻص�����
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) 
{
    if (huart->Instance == USART2) 
    {
        // ������������ʣ������

        uint16_t size_to_send = 0;

        // �ӻ��λ�������ȡ���ݵ� DMA ������
        while (!ringqueue_is_empty(&husart_tx_rb) && size_to_send < DMA_TX_BUFFER_SIZE) 
        {
            ringqueue_read(&husart_tx_rb, &buffer[size_to_send]);
            size_to_send++;
        }

        if (size_to_send > 0) {
            // ����ʹ�� DMA ����
            HAL_UART_Transmit_DMA(huart, buffer, size_to_send);
        } else {
            // ���������ȫ�����ͣ�����״̬
            husart1__operation.send_flag = 0;
        }
    }
}



void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size) {
    if (huart->Instance == USART2)  // ʹ�ú궨�壬֧�ָ�������
    {
        // ȷ�����ջ��λ�����û����
        if (!ringqueue_is_full(&husart_rx_rb))
         {
            // ��DMA���յ�������д����ջ��λ�����
            for (uint16_t i = 0; i < Size; i++)
             {
                if (!ringqueue_is_full(&husart_rx_rb))
                 {
                    ringqueue_write(&husart_rx_rb, uart_rx_dma_buffer[i]);
                } else 
                {
                    break;  // ��������ʱ�˳�
                }
            }
        }

        // ��� DMA ������
        memset(uart_rx_dma_buffer, 0, sizeof(uart_rx_dma_buffer));
        
        // �������� DMA ����
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



