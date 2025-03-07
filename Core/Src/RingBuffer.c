#include "RingBuffer.h"

/**
 * @brief 初始化环形缓冲区
 *
 * 将给定的环形缓冲区（RingBuffer）对象进行初始化，设置其头指针、尾指针和大小为0。
 *
 * @param rb 环形缓冲区指针
 */
void rb_init(RingBuffer* rb) {
    rb->head = 0;
    rb->tail = 0;
    rb->size = 0;
}

/**
 * @brief 判断环形缓冲区是否已满
 *
 * 判断给定的环形缓冲区是否已满。
 *
 * @param rb 环形缓冲区指针
 *
 * @return 如果环形缓冲区已满，返回 true；否则返回 false
 */
bool rb_is_full(RingBuffer* rb) {
    return rb->size == BUFFER_SIZE;
}

/**
 * @brief 判断环形缓冲区是否为空
 *
 * 检查给定的环形缓冲区是否为空。
 *
 * @param rb 环形缓冲区指针
 *
 * @return 如果环形缓冲区为空，则返回true；否则返回false
 */
bool rb_is_empty(RingBuffer* rb) {
    return rb->size == 0;
}

/**
 * @brief 向环形缓冲区中入队数据
 *
 * 将给定的浮点数数据入队到指定的环形缓冲区中。如果缓冲区已满，则覆盖最旧的数据，并移动头部指针。
 *
 * @param rb 环形缓冲区指针
 * @param data 要入队的数据
 */
void rb_enqueue(RingBuffer* rb, float data) {
    if (rb_is_full(rb)) {
        // 如果缓冲区已满，覆盖最旧的数据，并移动头部指针
        rb->head = (rb->head + 1) % BUFFER_SIZE;
    } else {
        rb->size++;
    }
    rb->buffer[rb->tail] = data;
    rb->tail = (rb->tail + 1) % BUFFER_SIZE;
}

/**
 * @brief 从环形缓冲区中出队数据
 *
 * 从给定的环形缓冲区中取出一个数据，并将其存储在提供的指针指向的内存位置。
 * 如果缓冲区为空，则返回 false。
 *
 * @param rb 指向环形缓冲区的指针
 * @param data 用于存储从缓冲区中取出的数据的指针
 *
 * @return 如果成功从缓冲区中取出数据，则返回 true；否则返回 false
 */
bool rb_dequeue(RingBuffer* rb, float* data) {
    if (rb_is_empty(rb)) {
        return false; // 缓冲区为空
    }
    *data = rb->buffer[rb->head];
    rb->head = (rb->head + 1) % BUFFER_SIZE;
    rb->size--;
    return true;
}
/**
 * @brief 将环形缓冲区转换为数组
 *
 * 将给定的环形缓冲区（RingBuffer）中的数据转换为数组，并将结果存储在指定的数组中。
 *
 * @param rb 环形缓冲区指针
 * @param array 存储转换结果的数组指针
 */
void rb_to_array(RingBuffer* rb, float* array) {
    for (int i = 0; i < rb->size; i++) {
        int pos = (rb->head + i) % BUFFER_SIZE;
        array[i] = rb->buffer[pos];
    }
}


/**
 * @brief 将环形缓冲区数据转换为 FFT 数组
 *
 * 将给定的环形缓冲区（RingBuffer）中的数据复制到 FFT 数组中，并将 FFT 数组的虚部设置为 0。
 *
 * @param rb 环形缓冲区指针
 * @param array FFT 数组指针
 * @param size FFT 数组的大小（实际存储的实数样本数的一半）
 */
void rb_to_FFTarray(RingBuffer* rb, float* array,int size) {
    for (int i = 0; i < size; i++) {
        array[i*2] = rb->buffer[i];
        array[i*2+1] = 0;
    }
}


