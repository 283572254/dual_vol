#ifndef __SERIAL_H__
#define __SERIAL_H__

#include "main.h"
#include "stdbool.h"
#include "stdio.h"
#include "usart.h"
typedef struct 
{
    bool send_flag;
    bool recv_flag;
}OPREATION;

#define UART_DMA_BUFFER_SIZE 256    // DMA���ջ�������С
#define SENDBUFFER_SIZE 256            // ���ͻ�������С
#define DMA_TX_BUFFER_SIZE 256  // DMA ���ͻ�������С

extern OPREATION husart1__operation;
extern uint8_t uart_rx_dma_buffer[UART_DMA_BUFFER_SIZE];  // DMA���ջ�����
void UART_SendData(uint8_t *data, uint16_t size, UART_HandleTypeDef *huart);
void UART_Driver_Init(UART_HandleTypeDef *huart) ;
#endif



