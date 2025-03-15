#include "balance_task.h"
int monitored_var = 0;
//≤‚ ‘»ŒŒÒ
sys_task_t task_Encoder_get;
sys_task_t task_printf;
sys_task_t task_process_uart_data;   
sys_task_t task_MotorOutput;
sys_task_t task_Angle_Get;
sys_task_t task_MotorSpeedControl;
sys_task_t task_Deal_Control;
sys_task_t task_MotorSpeedControlOutput;
void Encoder_get(void)
{
    left_motor.MotorPulse = GetTimEncoder(&htim2);
    right_motor.MotorPulse = GetTimEncoder(&htim3);
    left_motor.MotorPulseSigma += left_motor.MotorPulse;
    right_motor.MotorPulseSigma += right_motor.MotorPulse;
    // printf("Encoder_get_task %d\n",Get_Sys_Tick());
    //monitored_var += left_motor.MotorPulse;
}

void printf_task(void)
{

    Send_Value(left_motor.MotorPulse,left_motor.MotorTargetPulse);
    Send_Value(right_motor.MotorPulse,right_motor.MotorTargetPulse);
    Send_Value(left_motor.PWMout,right_motor.PWMout);
    Send_Value(Mpu6050_calc_data.CarAngle,Mpu6050_calc_data.CarAngleTarget);
    Send_Value(Mpu6050_calc_data.x_fGyro,Mpu6050_calc_data.y_fGyro);
    u_SendArray(g_tail,4);
    // printf("printf_task %d\n",Get_Sys_Tick());
    // monitored_var = 0;
}


void task_start(void)
{    
    sys_task_create(&task_Angle_Get, Angle_Get, 3);
    // sys_task_create(&task_MotorSpeedControlOutput, MotorSpeedControlOutput, 1);
    sys_task_create(&task_MotorOutput, MotorOutput, 3);
    sys_task_create(&task_Encoder_get, Encoder_get,5);
    sys_task_create(&task_MotorSpeedControl, MotorSpeedControl, 5); 
    sys_task_create(&task_printf, printf_task, 5);
    sys_task_create(&task_process_uart_data, process_uart_data, 5);
//    sys_task_create(&task_Deal_Control, Deal_Control, 5);

    sys_task_start(&task_Angle_Get);
    sys_task_start(&task_MotorSpeedControlOutput);
    sys_task_start(&task_MotorOutput);
    sys_task_start(&task_Encoder_get);
    sys_task_start(&task_MotorSpeedControl);
    sys_task_start(&task_printf);
    sys_task_start(&task_process_uart_data);
    // sys_task_start(&task_Deal_Control);
}



