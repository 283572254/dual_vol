#include "fft.h"
#include "arm_math.h"  // 确保包含了CMSIS库的头文件
#include "arm_const_structs.h"
//extern RingBuffer Rb_CalcData[3];  

float32_t FFT_Voltage_Value(RingBuffer *FVoltage_Value);

// 手动定义FFT配置
arm_cfft_radix4_instance_f32 fft_inst;

float32_t fft_sum = 0;
float32_t FFT_50Hz_Value(void)
{

    float FFT_input[FFT_SIZE * 2] = {0};  // 输入缓冲区初始化为0，包括实部和虚部
    float32_t fft_output[FFT_SIZE];       // 输出数据
    float32_t freq_resolution;            //fft分析得到的频率
    uint32_t index_50hz = 1;              //50Hz标志位
    float32_t amplitude_50hz;             //50Hz模值
    // 从环形缓冲区获取FFT输入数据
    rb_to_FFTarray(&Rb_CalcData[0], FFT_input, FFT_SIZE);

    // 使用预定义的128点FFT实例
    const arm_cfft_instance_f32 *fft_inst = &arm_cfft_sR_f32_len128;

    // 执行FFT
    arm_cfft_f32(fft_inst, FFT_input, 0, 1);

    // 计算复数输出的幅度
    arm_cmplx_mag_f32(FFT_input, fft_output, FFT_SIZE);

    // 计算频率分辨率
    freq_resolution = SAMPLING_FREQ / (float)FFT_SIZE;

    // 计算50Hz的索引
    index_50hz = (uint32_t)(50 / freq_resolution);


    amplitude_50hz = fft_output[index_50hz] / (FFT_SIZE / 2);
    // 归一化直流分量
    fft_output[0] /= FFT_SIZE;

    // 归一化其他频率成分
    for (int i = 1; i < FFT_SIZE; i++)
    {
        fft_output[i] /= (FFT_SIZE / 2);
    }

    // 获取50Hz频率的幅值

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


         return amplitude_50hz ;  // 返回50Hz的幅值
}


float32_t FFT_Voltage_Value(RingBuffer *FVoltage_Value)
{
    float FFT_input[FFT_SIZE * 2] = {0};  // 输入缓冲区初始化为0，包括实部和虚部
    float32_t fft_output[FFT_SIZE];       // 输出数据
    float32_t freq_resolution;            //fft分析得到的频率
    uint32_t index_50hz = 1;              //50Hz标志位
    float32_t amplitude_50hz;             //50Hz模值
        // 从环形缓冲区获取FFT输入数据
    rb_to_FFTarray(FVoltage_Value, FFT_input, FFT_SIZE);

    // 使用预定义的128点FFT实例
    const arm_cfft_instance_f32 *fft_inst = &arm_cfft_sR_f32_len128;

    // 执行FFT
    arm_cfft_f32(fft_inst, FFT_input, 0, 1);

    // 计算复数输出的幅度
    arm_cmplx_mag_f32(FFT_input, fft_output, FFT_SIZE);

    // 计算频率分辨率
    freq_resolution = SAMPLING_FREQ / (float)FFT_SIZE;

    // 计算50Hz的索引
    index_50hz = (uint32_t)(50 / freq_resolution);


    amplitude_50hz = fft_output[index_50hz] / (FFT_SIZE / 2);
    // 归一化直流分量
    fft_output[0] /= FFT_SIZE;

    // 归一化其他频率成分
    for (int i = 1; i < FFT_SIZE; i++)
    {
        fft_output[i] /= (FFT_SIZE / 2);
    }

    // 获取50Hz频率的幅值
    return amplitude_50hz ;  // 返回50Hz的幅值
}




