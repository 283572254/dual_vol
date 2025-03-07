#ifndef FFT_H
#define FFT_H

#include "main.h"
#include "RingBuffer.h"
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define FFT_SIZE 128  // FFT的大小，应为2的幂次方
#define SAMPLING_FREQ 6400  // 采样频率，单位Hz

float32_t  FFT_50Hz_Value(void);
#endif // !FFT_H

