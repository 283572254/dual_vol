#include "RingBuffer.h"

/**
 * @brief ��ʼ�����λ�����
 *
 * �������Ļ��λ�������RingBuffer��������г�ʼ����������ͷָ�롢βָ��ʹ�СΪ0��
 *
 * @param rb ���λ�����ָ��
 */
void rb_init(RingBuffer* rb) {
    rb->head = 0;
    rb->tail = 0;
    rb->size = 0;
}

/**
 * @brief �жϻ��λ������Ƿ�����
 *
 * �жϸ����Ļ��λ������Ƿ�������
 *
 * @param rb ���λ�����ָ��
 *
 * @return ������λ��������������� true�����򷵻� false
 */
bool rb_is_full(RingBuffer* rb) {
    return rb->size == BUFFER_SIZE;
}

/**
 * @brief �жϻ��λ������Ƿ�Ϊ��
 *
 * �������Ļ��λ������Ƿ�Ϊ�ա�
 *
 * @param rb ���λ�����ָ��
 *
 * @return ������λ�����Ϊ�գ��򷵻�true�����򷵻�false
 */
bool rb_is_empty(RingBuffer* rb) {
    return rb->size == 0;
}

/**
 * @brief ���λ��������������
 *
 * �������ĸ�����������ӵ�ָ���Ļ��λ������С�����������������򸲸���ɵ����ݣ����ƶ�ͷ��ָ�롣
 *
 * @param rb ���λ�����ָ��
 * @param data Ҫ��ӵ�����
 */
void rb_enqueue(RingBuffer* rb, float data) {
    if (rb_is_full(rb)) {
        // ���������������������ɵ����ݣ����ƶ�ͷ��ָ��
        rb->head = (rb->head + 1) % BUFFER_SIZE;
    } else {
        rb->size++;
    }
    rb->buffer[rb->tail] = data;
    rb->tail = (rb->tail + 1) % BUFFER_SIZE;
}

/**
 * @brief �ӻ��λ������г�������
 *
 * �Ӹ����Ļ��λ�������ȡ��һ�����ݣ�������洢���ṩ��ָ��ָ����ڴ�λ�á�
 * ���������Ϊ�գ��򷵻� false��
 *
 * @param rb ָ���λ�������ָ��
 * @param data ���ڴ洢�ӻ�������ȡ�������ݵ�ָ��
 *
 * @return ����ɹ��ӻ�������ȡ�����ݣ��򷵻� true�����򷵻� false
 */
bool rb_dequeue(RingBuffer* rb, float* data) {
    if (rb_is_empty(rb)) {
        return false; // ������Ϊ��
    }
    *data = rb->buffer[rb->head];
    rb->head = (rb->head + 1) % BUFFER_SIZE;
    rb->size--;
    return true;
}
/**
 * @brief �����λ�����ת��Ϊ����
 *
 * �������Ļ��λ�������RingBuffer���е�����ת��Ϊ���飬��������洢��ָ���������С�
 *
 * @param rb ���λ�����ָ��
 * @param array �洢ת�����������ָ��
 */
void rb_to_array(RingBuffer* rb, float* array) {
    for (int i = 0; i < rb->size; i++) {
        int pos = (rb->head + i) % BUFFER_SIZE;
        array[i] = rb->buffer[pos];
    }
}


/**
 * @brief �����λ���������ת��Ϊ FFT ����
 *
 * �������Ļ��λ�������RingBuffer���е����ݸ��Ƶ� FFT �����У����� FFT ������鲿����Ϊ 0��
 *
 * @param rb ���λ�����ָ��
 * @param array FFT ����ָ��
 * @param size FFT ����Ĵ�С��ʵ�ʴ洢��ʵ����������һ�룩
 */
void rb_to_FFTarray(RingBuffer* rb, float* array,int size) {
    for (int i = 0; i < size; i++) {
        array[i*2] = rb->buffer[i];
        array[i*2+1] = 0;
    }
}


