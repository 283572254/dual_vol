#include "ProRecData.h"



ProessData RxTx_buffer; //接受发送结构体


void RxTx_buffer_init(void)
{

  for(int i=0;i<BUFFER_CHANNLE;i++)
  {
    RxTx_buffer.data_length[i] = 1;
      for(int j=0;j<BUFFER_LENGTH;j++)
      {
        RxTx_buffer.rx_buffer[i][j] = 0;
        RxTx_buffer.tx_buffer[i][j] = 0;
      }
  }
    
}

uint32_t data_length = 0;
uint32_t data_rec = 0;
/**
 * @brief 开始UART DMA接收
 *
 * 该函数用于配置DMA接收，指定缓冲区地址和大小，并启动UART的DMA接收。
 * 同时，使能UART的空闲中断。
 *
 * @note 目前仅对huart2和huart3进行了配置
 */
void Start_UART_DMA_Receive(void)
{
  // 配置DMA接收，指定缓冲区地址和大小
  HAL_UART_Receive_DMA(&huart2, RxTx_buffer.rx_buffer[0], sizeof(RxTx_buffer.rx_buffer[0]));
  HAL_UART_Receive_DMA(&huart3, RxTx_buffer.rx_buffer[1], sizeof(RxTx_buffer.rx_buffer[1]));
 

  // 使能空闲中断
  __HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);
  __HAL_UART_ENABLE_IT(&huart3, UART_IT_IDLE);

}

void ProcessReceivedData(uint8_t* rx_data, uint8_t* tx_data,uint32_t size)
{
    memcpy(tx_data,rx_data,size*sizeof(uint8_t));
    
}


// 数据交换格式
// 帧头+命令域+地址域+数据域+校验域+帧尾