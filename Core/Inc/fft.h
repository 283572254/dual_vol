#ifndef FFT_H
#define FFT_H

#include "main.h"
#include "RingBuffer.h"
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define FFT_SIZE 128  // FFT�Ĵ�С��ӦΪ2���ݴη�
#define SAMPLING_FREQ 6400  // ����Ƶ�ʣ���λHz

float32_t  FFT_50Hz_Value(void);
#endif // !FFT_H

