#ifndef PRORECDATA_H
#define PRORECDATA_H

#include "main.h"
#include "usart.h"
#include "global.h"

#define BUFFER_CHANNLE 3
#define BUFFER_LENGTH 200

void Start_UART_DMA_Receive(void);
void ProcessReceivedData(uint8_t* rx_data, uint8_t* tx_data,uint32_t size);
void RxTx_buffer_init(void);
typedef struct 
{
  uint8_t rx_buffer[BUFFER_CHANNLE][BUFFER_LENGTH];
  uint8_t tx_buffer[BUFFER_CHANNLE][BUFFER_LENGTH];
  uint32_t data_length[BUFFER_CHANNLE];

} ProessData;

extern ProessData RxTx_buffer; 
#endif // !PRORECDATA_H
 







 









