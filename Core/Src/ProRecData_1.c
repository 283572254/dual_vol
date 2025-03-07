#include "ProRecData.h"



ProessData RxTx_buffer; //���ܷ��ͽṹ��


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
 * @brief ��ʼUART DMA����
 *
 * �ú�����������DMA���գ�ָ����������ַ�ʹ�С��������UART��DMA���ա�
 * ͬʱ��ʹ��UART�Ŀ����жϡ�
 *
 * @note Ŀǰ����huart2��huart3����������
 */
void Start_UART_DMA_Receive(void)
{
  // ����DMA���գ�ָ����������ַ�ʹ�С
  HAL_UART_Receive_DMA(&huart2, RxTx_buffer.rx_buffer[0], sizeof(RxTx_buffer.rx_buffer[0]));
  HAL_UART_Receive_DMA(&huart3, RxTx_buffer.rx_buffer[1], sizeof(RxTx_buffer.rx_buffer[1]));
 

  // ʹ�ܿ����ж�
  __HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);
  __HAL_UART_ENABLE_IT(&huart3, UART_IT_IDLE);

}

void ProcessReceivedData(uint8_t* rx_data, uint8_t* tx_data,uint32_t size)
{
    memcpy(tx_data,rx_data,size*sizeof(uint8_t));
    
}


// ���ݽ�����ʽ
// ֡ͷ+������+��ַ��+������+У����+֡β