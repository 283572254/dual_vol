#ifndef __AD7616_H__
#define __AD7616_H__

#include "main.h"

/* 串行模式：   0   并行模式：  1 */
#define USE_PARALLEL_MODE 1
/* 硬件模式：   0   软件模式：  1 */
#define USE_SOFTWARE_MODE 0
/* 输入通道组 */
#define AD7616_CHANNEL_GROUP_MAX 8

#define AD7616_CHANNEL_GROUP_NUM 2

/* AD7616工作模式 */
enum AD7616_Working_Mode {
  Hardware_Mode = 0,
  Software_Mode,
};

enum AD7616_Channel_Group {
  Channel_Group_0 = 0x00,
  Channel_Group_1,
  Channel_Group_2,
  Channel_Group_3,
  Channel_Group_4,
  Channel_Group_5,
  Channel_Group_6,
  Channel_Group_7,
};
/* 电压量程 */
enum AD7616_Range {
  Range_10_V = 0x00,
  Range_2_5_V,
  Range_5_V,
};
/* DoutX输出格式选择 */
enum AD7606B_Serial_Output_Format {
  Serial_Line_1_Output = 0x00,
  Serial_Line_2_Output,
};



void AD7616_Init(uint8_t mode);
void AD7616_Reset(void);
void AD7616_Conversion(void);
void AD7616_Set_Range(uint8_t range);
void AD7616_Read_Data(uint16_t *data);
void AD7616_Channel_Group_Select(uint8_t channel);
float AD7616_Digital2Voltage(uint16_t data);
void AD7616_Set_Serial_Output_Format(uint8_t format);
void ad7616_date_buffer(void);


#endif


