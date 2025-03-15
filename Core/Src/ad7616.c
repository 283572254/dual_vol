#include "main.h"
#include "stdlib.h"


static uint16_t Phase_Count = 0;                                                           //以“BUFFER_SIZE”一周期
uint8_t PhaseFlag = 0;                                                                     //周期标志位
                                                                          //上一周波与当前周波点差值、

uint8_t ad7616_range;                                                                      //AD7616采集范围
float fAd7616_data[AD7616_CHANNEL_GROUP_MAX * AD7616_CHANNEL_GROUP_NUM] = {0};             //电压采样值

float Phase_Date[SAMPLING_CHANNEL][BUFFER_SIZE];                                           //周波数据    
uint16_t ad7616_data[AD7616_CHANNEL_GROUP_MAX * AD7616_CHANNEL_GROUP_NUM];

RingBuffer Rb_CalcData[AD7616_CHANNEL_GROUP_MAX * AD7616_CHANNEL_GROUP_NUM]; 
uint8_t Phase_CountUM = 0;
/**
 * @brief AD7616工作模式设置
 *
 * @param mode uint8_t 工作模式
 */
static void AD7616_Working_Mode(uint8_t mode) {
  if (mode == Hardware_Mode) {
    HAL_GPIO_WritePin(AD7616_RNG0_GPIO_Port, AD7616_RNG0_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(AD7616_RNG1_GPIO_Port, AD7616_RNG1_Pin, GPIO_PIN_SET);
  } else if (mode == Software_Mode) {
    HAL_GPIO_WritePin(AD7616_RNG0_GPIO_Port, AD7616_RNG0_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(AD7616_RNG1_GPIO_Port, AD7616_RNG1_Pin, GPIO_PIN_RESET);
  }
}


/* Dx引脚端口 */
GPIO_TypeDef *port_list[] = {
    AD7616_D0_GPIO_Port,  AD7616_D1_GPIO_Port,  AD7616_D2_GPIO_Port,
    AD7616_D3_GPIO_Port,  AD7616_D4_GPIO_Port,  AD7616_D5_GPIO_Port,
    AD7616_D6_GPIO_Port,  AD7616_D7_GPIO_Port,  AD7616_D8_GPIO_Port,
    AD7616_D9_GPIO_Port,  AD7616_D10_GPIO_Port, AD7616_D11_GPIO_Port,
    AD7616_D12_GPIO_Port, AD7616_D13_GPIO_Port, AD7616_D14_GPIO_Port,
    AD7616_D15_GPIO_Port,
};

/* Dx引脚 */
uint16_t pin_list[] = {
    AD7616_D0_Pin,  AD7616_D1_Pin,  AD7616_D2_Pin,  AD7616_D3_Pin,
    AD7616_D4_Pin,  AD7616_D5_Pin,  AD7616_D6_Pin,  AD7616_D7_Pin,
    AD7616_D8_Pin,  AD7616_D9_Pin,  AD7616_D10_Pin, AD7616_D11_Pin,
    AD7616_D12_Pin, AD7616_D13_Pin, AD7616_D14_Pin, AD7616_D15_Pin,
};
/**
 * @brief 获取AD7616通道数据
 *
 * @return uint16_t 通道数据
 */
static uint16_t AD7616_Get_Parallel_Data(void) {
  uint16_t shift = 0x0001;
  uint16_t input_level = 0;
  for (uint8_t i = 0; i < sizeof(pin_list) / sizeof(uint16_t); i++) {
    GPIO_PinState state = HAL_GPIO_ReadPin(port_list[i], pin_list[i]);
    if (state == GPIO_PIN_SET) {
      input_level |= shift;
    } else if (state == GPIO_PIN_RESET) {
      input_level &= (~shift);
    }
    shift <<= 1;
  }
  return input_level;
}


/**
 * @brief AD7616初始化
 *
 * @param mode uint8_t 模式选择
 */
void AD7616_Init(uint8_t mode) {
#if USE_PARALLEL_MODE
//   AD7616_Parallel_GPIO_Init();
//   AD7616_Set_Dx_Pin_Input_Mode();
  HAL_GPIO_WritePin(AD7616_SER_GPIO_Port, AD7616_SER_Pin, GPIO_PIN_RESET);
#else
  AD7616_Serial_GPIO_Init();
  HAL_GPIO_WritePin(AD7616_SER_GPIO_Port, AD7616_SER_Pin, GPIO_PIN_SET);
#endif

  HAL_GPIO_WritePin(AD7616_CS_GPIO_Port, AD7616_CS_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(AD7616_RD_GPIO_Port, AD7616_RD_Pin, GPIO_PIN_SET);

  AD7616_Working_Mode(mode);
}

/**
 * @brief AD7616复位
 *
 */
void AD7616_Reset(void) {
  HAL_GPIO_WritePin(AD7616_RST_GPIO_Port, AD7616_RST_Pin, GPIO_PIN_RESET);
  delay_us(1);
  HAL_GPIO_WritePin(AD7616_RST_GPIO_Port, AD7616_RST_Pin, GPIO_PIN_SET);
  delay_us(20);
}

/**
 * @brief 启动AD7616转换
 *
 */
void AD7616_Conversion(void) {
  HAL_GPIO_WritePin(AD7616_CONV_GPIO_Port, AD7616_CONV_Pin, GPIO_PIN_RESET);
  delay_us(1);
  HAL_GPIO_WritePin(AD7616_CONV_GPIO_Port, AD7616_CONV_Pin, GPIO_PIN_SET);
}

/**
 * @brief 配置AD7616量程
 *
 * @param range uint8_t 量程
 */
void AD7616_Set_Range(uint8_t range) {
  ad7616_range = range;
#if USE_SOFTWARE_MODE
  uint8_t range_data = (range << 6) | (range << 4) | (range << 2) | range;
  AD7616_Write_Register(Input_Range_Register_A1, range_data);
  AD7616_Write_Register(Input_Range_Register_A2, range_data);
  AD7616_Write_Register(Input_Range_Register_B1, range_data);
  AD7616_Write_Register(Input_Range_Register_B2, range_data);
#else
  if (ad7616_range == Range_2_5_V) {
    HAL_GPIO_WritePin(AD7616_RNG0_GPIO_Port, AD7616_RNG0_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(AD7616_RNG1_GPIO_Port, AD7616_RNG1_Pin, GPIO_PIN_RESET);
  } else if (ad7616_range == Range_5_V) {
    HAL_GPIO_WritePin(AD7616_RNG0_GPIO_Port, AD7616_RNG0_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(AD7616_RNG1_GPIO_Port, AD7616_RNG1_Pin, GPIO_PIN_SET);
  } else if (ad7616_range == Range_10_V) {
    HAL_GPIO_WritePin(AD7616_RNG0_GPIO_Port, AD7616_RNG0_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(AD7616_RNG1_GPIO_Port, AD7616_RNG1_Pin, GPIO_PIN_SET);
  }
#endif
}

/**
 * @brief 获取AD7616单次采集数据
 *
 * @param data uint16_t 数组指针
 */
void AD7616_Read_Data(uint16_t *data) {
  GPIO_PinState state = GPIO_PIN_SET;
  do {
    state = HAL_GPIO_ReadPin(AD7616_BUSY_GPIO_Port, AD7616_BUSY_Pin);
  } while (state);
  HAL_GPIO_WritePin(AD7616_CS_GPIO_Port, AD7616_CS_Pin, GPIO_PIN_RESET);
  delay_us(1);
#if USE_PARALLEL_MODE
  for (uint8_t i = 0; i < AD7616_CHANNEL_GROUP_NUM; i++) {
    HAL_GPIO_WritePin(AD7616_RD_GPIO_Port, AD7616_RD_Pin, GPIO_PIN_RESET);
    data[i] = AD7616_Get_Parallel_Data();
    HAL_GPIO_WritePin(AD7616_RD_GPIO_Port, AD7616_RD_Pin, GPIO_PIN_SET);
  }
#else
  AD7616_Get_Serial_Data(data);
#endif
  HAL_GPIO_WritePin(AD7616_CS_GPIO_Port, AD7616_CS_Pin, GPIO_PIN_SET);
}

/**
 * @brief AD7616通道组选择  共8组
 *
 * @param channel uint8_t 通道组
 */
void AD7616_Channel_Group_Select(uint8_t channel) {
#if USE_SOFTWARE_MODE
  uint8_t channel_data = (channel << 4) | channel;
#endif
  switch (channel) {
  case Channel_Group_0:
#if USE_SOFTWARE_MODE
    AD7616_Write_Register(Channel_Register, channel_data);
#else
    HAL_GPIO_WritePin(AD7616_CHS0_GPIO_Port, AD7616_CHS0_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(AD7616_CHS1_GPIO_Port, AD7616_CHS1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(AD7616_CHS2_GPIO_Port, AD7616_CHS2_Pin, GPIO_PIN_RESET);
#endif
    break;
  case Channel_Group_1:
#if USE_SOFTWARE_MODE
    AD7616_Write_Register(Channel_Register, channel_data);
#else
    HAL_GPIO_WritePin(AD7616_CHS0_GPIO_Port, AD7616_CHS0_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(AD7616_CHS1_GPIO_Port, AD7616_CHS1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(AD7616_CHS2_GPIO_Port, AD7616_CHS2_Pin, GPIO_PIN_RESET);
#endif
    break;
  case Channel_Group_2:
#if USE_SOFTWARE_MODE
    AD7616_Write_Register(Channel_Register, channel_data);
#else
    HAL_GPIO_WritePin(AD7616_CHS0_GPIO_Port, AD7616_CHS0_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(AD7616_CHS1_GPIO_Port, AD7616_CHS1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(AD7616_CHS2_GPIO_Port, AD7616_CHS2_Pin, GPIO_PIN_RESET);
#endif
    break;
  case Channel_Group_3:
#if USE_SOFTWARE_MODE
    AD7616_Write_Register(Channel_Register, channel_data);
#else
    HAL_GPIO_WritePin(AD7616_CHS0_GPIO_Port, AD7616_CHS0_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(AD7616_CHS1_GPIO_Port, AD7616_CHS1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(AD7616_CHS2_GPIO_Port, AD7616_CHS2_Pin, GPIO_PIN_RESET);
#endif
    break;
  case Channel_Group_4:
#if USE_SOFTWARE_MODE
    AD7616_Write_Register(Channel_Register, channel_data);
#else
    HAL_GPIO_WritePin(AD7616_CHS0_GPIO_Port, AD7616_CHS0_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(AD7616_CHS1_GPIO_Port, AD7616_CHS1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(AD7616_CHS2_GPIO_Port, AD7616_CHS2_Pin, GPIO_PIN_SET);
#endif
    break;
  case Channel_Group_5:
#if USE_SOFTWARE_MODE
    AD7616_Write_Register(Channel_Register, channel_data);
#else
    HAL_GPIO_WritePin(AD7616_CHS0_GPIO_Port, AD7616_CHS0_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(AD7616_CHS1_GPIO_Port, AD7616_CHS1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(AD7616_CHS2_GPIO_Port, AD7616_CHS2_Pin, GPIO_PIN_SET);
#endif
    break;
  case Channel_Group_6:
#if USE_SOFTWARE_MODE
    AD7616_Write_Register(Channel_Register, channel_data);
#else
    HAL_GPIO_WritePin(AD7616_CHS0_GPIO_Port, AD7616_CHS0_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(AD7616_CHS1_GPIO_Port, AD7616_CHS1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(AD7616_CHS2_GPIO_Port, AD7616_CHS2_Pin, GPIO_PIN_SET);
#endif
    break;
  case Channel_Group_7:
#if USE_SOFTWARE_MODE
    AD7616_Write_Register(Channel_Register, channel_data);
#else
    HAL_GPIO_WritePin(AD7616_CHS0_GPIO_Port, AD7616_CHS0_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(AD7616_CHS1_GPIO_Port, AD7616_CHS1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(AD7616_CHS2_GPIO_Port, AD7616_CHS2_Pin, GPIO_PIN_SET);
#endif
    break;
  }
}

/**
 * @brief 将数字量转换为电压量
 *
 * @param data uint16_t 数字量
 * @return float 电压量 单位毫伏(mv)
 */
float AD7616_Digital2Voltage(uint16_t data) {
    int16_t signed_data = (int16_t)data; // 将无符号的 uint16_t 转换为有符号的 int16_t
    
    switch (ad7616_range) {
    case Range_2_5_V:
        return signed_data * 2.5 / 32768.0; // -32768 到 32767 对应 -2.5V 到 2.5V
    case Range_5_V:
        return signed_data * 5.0 / 32768.0; // -32768 到 32767 对应 -5V 到 5V
    case Range_10_V:
        return signed_data * 10.0 / 32768.0; // -32768 到 32767 对应 -10V 到 10V
    }
    return 0;
}

/**
 * @brief 设置AD7616串行输出模式
 *
 * @param format uint8_t 格式
 */
void AD7616_Set_Serial_Output_Format(uint8_t format) {
  if (format == Serial_Line_1_Output) {
    HAL_GPIO_WritePin(AD7616_D4_GPIO_Port, AD7616_D4_Pin, GPIO_PIN_RESET);
  } else if (format == Serial_Line_2_Output) {
    HAL_GPIO_WritePin(AD7616_D4_GPIO_Port, AD7616_D4_Pin, GPIO_PIN_SET);
  }
}






/**
 * @brief 波形全点比较
 *
 * 将环形缓冲区中的数据与给定数据数组进行比较，根据波形全点差异设置相关标志。
 *
 * @param rb 环形缓冲区指针
 * @param data 数据数组指针
 */

void Phase_Compare(RingBuffer* rb, float32_t *data,uint8_t *Phase_Count)
{
	static float32_t diff = 0;
	// 周期标志位
	static uint8_t PhaseFlag = 0;                //周期标志位
	// 波形全点标志
    static uint8_t ChangeCount = 0;                //波形全点标志
	// 如果波形全点计数大于缓冲区大小，重置波形全点计数并设置周期标志位
    if(*Phase_Count>BUFFER_SIZE)
    {
      *Phase_Count = 0;
      PhaseFlag = 1;
    }
	// 如果周期标志位为1
    if(PhaseFlag==1)
    {
		// 将环形缓冲区的数据复制到data数组中
        memcpy(data,rb->buffer,BUFFER_SIZE*sizeof(float32_t));

		// 重置周期标志位
        PhaseFlag = 0;
    }
	// 计算diff值
    if(rb->head==0)
      diff = fabs(data[BUFFER_SIZE-1] - rb->buffer[BUFFER_SIZE-1]);
    else
      diff = fabs(data[rb->head-1] - rb->buffer[rb->head-1]);
	// 如果diff值大于120，增加波形全点标志,不同的电流定值下，diff需要调整
    if(diff>0.02)
      ChangeCount++;
	// 如果波形全点标志大于10，设置开始标志并重置波形全点标志
    if(ChangeCount>2)
    {
	    StartFlag = 1;
      ChangeCount = 0;
	   
    }
	// 增加波形全点计数
    (*Phase_Count)++;
	
}


/**
 * @brief 检测零交叉点
 *
 * 根据给定的当前采样值，检测是否存在零交叉点。
 *
 * @param current_sample 当前采样值
 *
 * @return 如果存在零交叉点，返回 1；否则返回 0
 */
uint8_t Zero_Crossing_Detect(const float current_sample)
{
  static float last_sample = 0;
  uint8_t zero_crosssed = 0;
  if((last_sample>0 && current_sample<0)||(last_sample<0 && current_sample>0))
  {
    zero_crosssed = 1;
  }
  last_sample = current_sample;
  return zero_crosssed;
}

uint8_t Broken_Detect(const float current_sample)
{
  static uint8_t count = 0;
  uint8_t Broken = 0;
  if(fabs(current_sample)<0.01)
    count++;
  else
    count = 0;
  if(count>10)
  {
    Broken= 1;
  }
  return Broken;
}

/**
 * @brief AD7616 数据缓冲区处理
 *
 * 该函数用于处理 AD7616 的数据缓冲区，依次选择各个通道组进行转换并读取数据，
 * 并将读取到的数据转换为电压和电流值，最后将电压值存入环形缓冲区，并比较相位。
 */
void ad7616_date_buffer(void)
{
      for (uint8_t channel_group = 0; channel_group < AD7616_CHANNEL_GROUP_MAX;channel_group++) // 依次选择各个通道组进行转换并读取数据
      {
        AD7616_Channel_Group_Select(channel_group);
        AD7616_Conversion();
        if (!channel_group) {
          AD7616_Read_Data(&ad7616_data[Channel_Group_7 * 2]);
        } else {
          AD7616_Read_Data(&ad7616_data[(channel_group - 1) * 2]);
        }
      }
      
      for (int i = 0; i < AD7616_CHANNEL_GROUP_MAX*AD7616_CHANNEL_GROUP_NUM; i++)
      {
        fAd7616_data[i] = AD7616_Digital2Voltage(ad7616_data[i]);                                //A组获取电压
        rb_enqueue(&Rb_CalcData[i],fAd7616_data[i]);                                             //存入环形缓冲区
      }
      //相位比较 
      Phase_Compare(&Rb_CalcData[UM_C],Phase_Date[0],&Phase_CountUM); 
       //检测零交叉点
        Zero_Status.Zero_SubCurrent = Zero_Crossing_Detect(fAd7616_data[CS_C]);
        Zero_Status.Zero_MainCurrent = Zero_Crossing_Detect(fAd7616_data[CM_C]);
       //检测断路点
        Zero_Status.Broken_MainPower =  Broken_Detect(fAd7616_data[UM_C]);
        Zero_Status.Broken_SubPower = Broken_Detect(fAd7616_data[CS_C]);


}



