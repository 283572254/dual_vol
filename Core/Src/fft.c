#include "fft.h"
#include "arm_math.h"  // ȷ��������CMSIS���ͷ�ļ�
#include "arm_const_structs.h"
//extern RingBuffer Rb_CalcData[3];  

float32_t FFT_Voltage_Value(RingBuffer *FVoltage_Value);

// �ֶ�����FFT����
arm_cfft_radix4_instance_f32 fft_inst;

float32_t fft_sum = 0;
float32_t FFT_50Hz_Value(void)
{

    float FFT_input[FFT_SIZE * 2] = {0};  // ���뻺������ʼ��Ϊ0������ʵ�����鲿
    float32_t fft_output[FFT_SIZE];       // �������
    float32_t freq_resolution;            //fft�����õ���Ƶ��
    uint32_t index_50hz = 1;              //50Hz��־λ
    float32_t amplitude_50hz;             //50Hzģֵ
    // �ӻ��λ�������ȡFFT��������
    rb_to_FFTarray(&Rb_CalcData[0], FFT_input, FFT_SIZE);

    // ʹ��Ԥ�����128��FFTʵ��
    const arm_cfft_instance_f32 *fft_inst = &arm_cfft_sR_f32_len128;

    // ִ��FFT
    arm_cfft_f32(fft_inst, FFT_input, 0, 1);

    // ���㸴������ķ���
    arm_cmplx_mag_f32(FFT_input, fft_output, FFT_SIZE);

    // ����Ƶ�ʷֱ���
    freq_resolution = SAMPLING_FREQ / (float)FFT_SIZE;

    // ����50Hz������
    index_50hz = (uint32_t)(50 / freq_resolution);


    amplitude_50hz = fft_output[index_50hz] / (FFT_SIZE / 2);
    // ��һ��ֱ������
    fft_output[0] /= FFT_SIZE;

    // ��һ������Ƶ�ʳɷ�
    for (int i = 1; i < FFT_SIZE; i++)
    {
        fft_output[i] /= (FFT_SIZE / 2);
    }

    // ��ȡ50HzƵ�ʵķ�ֵ

//     for (int i = 0; i <= 32; i++)
//     {
//         fft_sum += fft_output[i]*fft_output[i];
        
//     }
//     for (int i = 95; i <= 127; i++)
//     {
//         fft_sum += fft_output[i]*fft_output[i];
        
//     }
//     amplitude_50hz = sqrt(fft_sum);
//     fft_sum = 0;


         return amplitude_50hz ;  // ����50Hz�ķ�ֵ
}


float32_t FFT_Voltage_Value(RingBuffer *FVoltage_Value)
{
    float FFT_input[FFT_SIZE * 2] = {0};  // ���뻺������ʼ��Ϊ0������ʵ�����鲿
    float32_t fft_output[FFT_SIZE];       // �������
    float32_t freq_resolution;            //fft�����õ���Ƶ��
    uint32_t index_50hz = 1;              //50Hz��־λ
    float32_t amplitude_50hz;             //50Hzģֵ
        // �ӻ��λ�������ȡFFT��������
    rb_to_FFTarray(FVoltage_Value, FFT_input, FFT_SIZE);

    // ʹ��Ԥ�����128��FFTʵ��
    const arm_cfft_instance_f32 *fft_inst = &arm_cfft_sR_f32_len128;

    // ִ��FFT
    arm_cfft_f32(fft_inst, FFT_input, 0, 1);

    // ���㸴������ķ���
    arm_cmplx_mag_f32(FFT_input, fft_output, FFT_SIZE);

    // ����Ƶ�ʷֱ���
    freq_resolution = SAMPLING_FREQ / (float)FFT_SIZE;

    // ����50Hz������
    index_50hz = (uint32_t)(50 / freq_resolution);


    amplitude_50hz = fft_output[index_50hz] / (FFT_SIZE / 2);
    // ��һ��ֱ������
    fft_output[0] /= FFT_SIZE;

    // ��һ������Ƶ�ʳɷ�
    for (int i = 1; i < FFT_SIZE; i++)
    {
        fft_output[i] /= (FFT_SIZE / 2);
    }

    // ��ȡ50HzƵ�ʵķ�ֵ
    return amplitude_50hz ;  // ����50Hz�ķ�ֵ
}




