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

#define UART_DMA_BUFFER_SIZE 256    // DMA接收缓冲区大小
#define SENDBUFFER_SIZE 256            // 发送缓冲区大小
#define DMA_TX_BUFFER_SIZE 256  // DMA 发送缓冲区大小

extern OPREATION husart1__operation;
extern uint8_t uart_rx_dma_buffer[UART_DMA_BUFFER_SIZE];  // DMA接收缓冲区
void UART_SendData(uint8_t *data, uint16_t size, UART_HandleTypeDef *huart);
void UART_Driver_Init(UART_HandleTypeDef *huart) ;
#endif



