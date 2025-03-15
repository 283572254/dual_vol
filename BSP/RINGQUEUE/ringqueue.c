#include "../BSP/RINGQUEUE/ringqueue.h"
#include "usart.h"

ringqueue_t husart_rx_rb;  // ���ջ��λ�����
ringqueue_t husart_tx_rb;  // ���ͻ��λ�����




void ringqueue_init(ringqueue_t *rb) {
    rb->w = 0;
    rb->r = 0;
    rb->itemCount = 0;
}

bool ringqueue_is_empty(ringqueue_t *rb) {
    return rb->itemCount == 0;
}

bool ringqueue_is_full(ringqueue_t *rb) {
    return rb->itemCount == RINGBUFFER_SIZE;
}

uint32_t ringqueue_write(ringqueue_t *rb, uint8_t data) {
    if (ringqueue_is_full(rb)) {
        return 0;  // ������������д��ʧ��
    }
    
    if (rb->w >= RINGBUFFER_SIZE || rb->r >= RINGBUFFER_SIZE) {
        return 0;  // ָ��Խ�磬д��ʧ��
    }
    
    rb->buffer[rb->w] = data;
    rb->w = (rb->w + 1) % RINGBUFFER_SIZE;
    rb->itemCount++;
    return 1;
}

uint32_t ringqueue_read(ringqueue_t *rb, uint8_t *data) {
    if (!rb || !data) {
        return 0;  // ��ָ����
    }
    
    if (ringqueue_is_empty(rb)) {
        return 0;  // ������Ϊ�գ���ȡʧ��
    }
    
    *data = rb->buffer[rb->r];
    rb->r = (rb->r + 1) % RINGBUFFER_SIZE;
    rb->itemCount--;
    return 1;
}

void ringqueue_clear(ringqueue_t *rb) {
    if (!rb) {
        return;
    }
    
    rb->w = 0;
    rb->r = 0;
    rb->itemCount = 0;
}

uint32_t ringqueue_available(ringqueue_t *rb) {
    if (!rb) {
        return 0;
    }
    
    return RINGBUFFER_SIZE - rb->itemCount;
}
