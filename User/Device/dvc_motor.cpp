#include "dvc_motor.h"


void Class_Motor::Init(TIM_HandleTypeDef *__TIM_PWMHandle, TIM_HandleTypeDef *__TIM_EncoderHandle, uint8_t __PWM_channel_1, uint8_t __PWM_channel_2, uint16_t __max_Compare, uint16_t __encoder_resolution, uint32_t __max_encoder_sum)
{
    TIM_PWMHandle = __TIM_PWMHandle;
    TIM_EncoderHandle = __TIM_EncoderHandle;
    Channel_1 = __PWM_channel_1;
    Channel_2 = __PWM_channel_2;
    Max_Compare = __max_Compare;
    Max_Encoder_Sum = __max_encoder_sum;
    Encoder_Resolution = __encoder_resolution;
    Speed_PID.Set_Out_Max(__max_Compare);
    Compare = 0;
    Target_Speed = 0;
    Now_Speed = 0;
    Now_Encoder = 0;
    Last_Encoder = 0;
    Encoder_Diff = 0;
    Target_Compare = 0;
}


void Class_Motor::Output()
{
    if (Abs_Compare > TIM_PWMHandle->Instance->ARR)
        Abs_Compare = TIM_PWMHandle->Instance->ARR;

    if(Compare > 0)
    {
        HAL_TIM_PWM_Start(TIM_PWMHandle, Channel_1);
        HAL_TIM_PWM_Stop(TIM_PWMHandle, Channel_2);
        __HAL_TIM_SET_COMPARE(TIM_PWMHandle, Channel_1, Abs_Compare);
    }
    else if(Compare < 0)
    {
        HAL_TIM_PWM_Start(TIM_PWMHandle, Channel_2);
        HAL_TIM_PWM_Stop(TIM_PWMHandle, Channel_1);
        __HAL_TIM_SET_COMPARE(TIM_PWMHandle, Channel_2, Abs_Compare);
    }
    else
    {
        HAL_TIM_PWM_Stop(TIM_PWMHandle, Channel_1);
        HAL_TIM_PWM_Stop(TIM_PWMHandle, Channel_2);
    }
}

float Class_Motor::Filter(float new_data, float *data_record)
{
    float sum = 0.0;

    for (uint8_t i = RECORD_NUM - 1; i > 0; i--) // 将现有数据后移一位
    {
        data_record[i] = data_record[i - 1];
        sum += data_record[i - 1];
    }
    data_record[0] = new_data; // 第一位是新的数据
    sum += new_data;

    return sum / (RECORD_NUM * 1.0); // 返回均值
}

//电机编码器数据处理 电机速度计算
void Class_Motor::TIM1ms_Motor_Data_PeriodElapsedCallback()
{
    static int32_t Encoder_Diff_Sum = 0;
    //获取当前脉冲值
    Now_Encoder = TIM_EncoderHandle->Instance->CNT;
    //计算脉冲差值
    Encoder_Diff = (int32_t)(Now_Encoder - Last_Encoder);
    //更新上次编码器值
    Last_Encoder = Now_Encoder;

    if(Encoder_Diff > Max_Encoder_Sum/2)
    {
        Encoder_Diff = Encoder_Diff - Max_Encoder_Sum;
    }
    else if(Encoder_Diff < -(int32_t)Max_Encoder_Sum/2)
    {
        Encoder_Diff = Encoder_Diff + Max_Encoder_Sum;
    }
    //计算当前速度 脉冲差值除以编码器分辨率乘以2PI乘以1000（1ms的定时器任务）
    Now_Speed = Filter((float)Encoder_Diff / Encoder_Resolution * 2. * PI * 1000., Record_Speed);
    //Now_Speed = (float)Encoder_Diff/Encoder_Resolution*2.*PI*1000.;

    //模拟单圈编码器 0-Encoder_Resolution 计算圈数
    Encoder_Diff_Sum += Encoder_Diff;
    if(Encoder_Diff_Sum > Encoder_Resolution)
    {
        Encoder_Diff_Sum = Encoder_Diff_Sum - Encoder_Resolution;
        Total_Round++;
    }
    else if(Encoder_Diff_Sum < 0)
    {
        Encoder_Diff_Sum = Encoder_Diff_Sum + Encoder_Resolution;
        Total_Round--;
    }
    Now_Angle_Encoder = Encoder_Diff_Sum;
}


void Class_Motor::TIM5ms_Motor_Calculate_PeriodElapsedCallback()
{
    //计算电机速度PID
    Speed_PID.Set_Target(Target_Speed);
    Speed_PID.Set_Now(Now_Speed);
    Speed_PID.TIM_Adjust_PeriodElapsedCallback(); 
    //设置电机PWM
    if(TIM_EncoderHandle == &htim4||TIM_EncoderHandle == &htim3)
    {
        Set_Compare(-1.0*(int16_t)Speed_PID.Get_Out());
    }
    else
    {
        Set_Compare((int16_t)Speed_PID.Get_Out());
    }
    //输出电机PWM
    Output();
}



