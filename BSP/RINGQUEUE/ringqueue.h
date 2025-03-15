#ifndef RINGBUFFER_H
#define RINGBUFFER_H
#include "main.h"
#include <stdint.h>
#include <stdbool.h>
#include <string.h> 


/************************* 可移植的串口配置 *************************/
// #define USART_INSTANCE    USART2           // 默认串口为 USART2，修改为其他串口时，只需要修改这一行
// #define USART_HANDLE      huart2           // 串口句柄
// #define DMA_INSTANCE      hdma_usart2_rx   // DMA实例
// #define RX_DMA_BUFFER     uart_rx_dma_buffer  // 接收DMA缓冲区
// #define USART_IRQn        USART2_IRQn      // 串口中断号


/***************************  环形缓冲区 ***************************/
#define RINGBUFFER_SIZE 256  // 根据需求设置缓冲区大小
typedef struct {
    uint32_t w;                        // 写指针
    uint32_t r;                        // 读指针
    uint8_t buffer[RINGBUFFER_SIZE];   // 数据存储缓冲区
    uint32_t itemCount;                // 当前缓冲区数据量
} ringqueue_t;

/***************************  环形缓冲区全局变量 **********/

extern ringqueue_t husart_rx_rb;  // 接收环形缓冲区
extern ringqueue_t husart_tx_rb;  // 发送环形缓冲区

/***************************  环形缓冲区API ***************************/
void ringqueue_init(ringqueue_t *rb);
bool ringqueue_is_empty(ringqueue_t *rb);
bool ringqueue_is_full(ringqueue_t *rb);
uint32_t ringqueue_write(ringqueue_t *rb, uint8_t data);
uint32_t ringqueue_read(ringqueue_t *rb, uint8_t *data);
/*************************** 串口DMA初始化 ***************************/
// void UART_DMA_Init(UART_HandleTypeDef *huart) ;
// void u_SendArray(uint8_t *array, uint16_t len);



#endif  // RINGBUFFER_H

