// ring_buffer.h

#ifndef RING_BUFFER_H
#define RING_BUFFER_H
#include "main.h"
#include <stdbool.h>
#include <stdint.h>

#define BUFFER_SIZE 128 // 定义环形数组的大小

typedef struct {
    float buffer[BUFFER_SIZE];
    int head;
    int tail;
    int size;
} RingBuffer;

void rb_init(RingBuffer* rb);
bool rb_is_full(RingBuffer* rb);
bool rb_is_empty(RingBuffer* rb);
void rb_enqueue(RingBuffer* rb, float data);
bool rb_dequeue(RingBuffer* rb, float* data);
void rb_to_array(RingBuffer* rb, float* array);
void rb_to_FFTarray(RingBuffer* rb, float* array,int size);



extern RingBuffer Rb_CalcData[AD7616_CHANNEL_GROUP_MAX * AD7616_CHANNEL_GROUP_NUM];  



#endif // RING_BUFFER_H
