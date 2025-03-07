#include "LSC_Airthmeitc.h"


float Calc_Data[SAMPLING_CHANNEL][BUFFER_SIZE];     //三相AD采样值

uint8_t Impelement_Start = 0;


extern float32_t FFT_Voltage_Value(RingBuffer *FVoltage_Value);

// 计算系数
const float HL1[SAMPLING_NUM] = {

-0.3489891211, -0.2475655829, -0.1560628934, -0.0748356998, -0.0041113149, 
0.0560111345, 0.1055602604, 0.1446906744, 0.1736799764, 0.1929245047, 
0.2029338913, 0.2043244804, 0.1978116747, 0.1842012907, 0.1643800075, 
0.1393050078, 0.1099929131, 0.0775081262, 0.0429506958, 0.0074438265, 
-0.0278788429, -0.0618860639, -0.0934617789, -0.1215177686, -0.1450060219, 
-0.1629307019, -0.1743595910, -0.1784348985, -0.1743833227, -0.1615252668, 
-0.1392831150, -0.1071884851, -0.0648883835, -0.0121502000, 0.0511345103, 
0.1249474981, 0.2091425709, 0.3034456410, 0.4074560717, 0.5206493130, 
};
const float HL2[SAMPLING_NUM] =  {

0.6778442246, 0.5213334381, 0.3781253151, 0.2489038671, 0.1341759383, 
0.0342681013, -0.0506746125, -0.1206881417, -0.1759869909, -0.2169600241, 
-0.2441644997, -0.2583184111, -0.2602912106, -0.2510930136, -0.2318623913, 
-0.2038528759, -0.1684183151, -0.1269972215, -0.0810962762, -0.0322731496, 
0.0178811873, 0.0677604854, 0.1157613915, 0.1603015960, 0.1998377688, 
0.2328831075, 0.2580243206, 0.2739378761, 0.2794053535, 0.2733277435, 
0.2547385531, 0.2228155826, 0.1768912562, 0.1164613995, 0.0411923756, 
-0.0490734963, -0.1543142963, -0.2743266731, -0.4087258508, -0.5569474772, 
};
float real[SAMPLING_CHANNEL] = {0};
float plural[SAMPLING_CHANNEL] = {0};
float effect[SAMPLING_CHANNEL] = {0};

/**
 * @brief LSC_Airthmeitc 函数
 *
 * 从环形缓冲区拷贝数据到 Calc_Data 数组,通过最小二乘法
 * 计算实部和虚部，并根据计算结果执行条件判断。
 *
 */
void LSC_Airthmeitc(void) {

    // 清零数组
  
        real[0] = 0;
        plural[0] = 0;
        
    

    // 将环形缓冲区的数据拷贝到 Calc_Data 数组中

        rb_to_array(&Rb_CalcData[UM_C], Calc_Data[0]);
    
        // 计算实部和虚部
        for (int k = 0; k < SAMPLING_NUM; k++) {
            real[0] += Calc_Data[0][Ab_SAMPLING_NUM+k] * HL1[k];
            plural[0] += Calc_Data[0][Ab_SAMPLING_NUM+k] * HL2[k];
        }
        effect[0] = sqrt(real[0] * real[0] + plural[0] * plural[0]);
        
        if((effect[0]<Threshold.Main_Voltage)&&(Dev_Event.Line_Malfunction==0))
        {       
                Dev_Event.Line_Normal = 0;//线路异常发生标志
                Dev_Event.Line_Abnormal = 1;//线路异常发生标志
                Impelement_Start = 1;//执行主继电器切换
                
                
        }      
}


void Vlotage_Resume(void)
{
    if(FFT_Voltage_Value(&Rb_CalcData[UM_C])>Threshold.Main_Voltage)
    {
        StartFlag = 0;
        LSC_flag = 1;
    }    
    else
    {
        LSC_flag = 0;
    }

}



