#ifndef DVC_MOTOR_H
#define DVC_MOTOR_H

#include "alg_pid.h"
#include "drv_tim.h"
#define RECORD_NUM 50

class Class_Motor
{
    public:

        //电机 电机PID
        Class_PID Speed_PID;

        void Init(TIM_HandleTypeDef *__TIM_PWMHandle, TIM_HandleTypeDef *__TIM_EncoderHandle, uint8_t __PWM_channel_1, uint8_t __PWM_channel_2, uint16_t __max_Compare, uint16_t __encoder_resolution, uint32_t __max_encoder_sum = 65535); 

        inline void Set_Target_Speed(float __Target_Speed);
        inline void Set_Target_Compare(uint16_t __Target_Compare);
        inline void Set_Compare(int16_t __Compare);
        inline void Set_Max_Compare(uint16_t __Max_Compare);

        inline float Get_Now_Speed();
        float Filter(float new_data, float *data_record);
        void TIM1ms_Motor_Data_PeriodElapsedCallback();
        void TIM5ms_Motor_Calculate_PeriodElapsedCallback();

    protected:

        //电机 电机TIM Encoder句柄
        TIM_HandleTypeDef *TIM_EncoderHandle;
        //电机 电机TIM PWM句柄
        TIM_HandleTypeDef *TIM_PWMHandle;
        //电机 电机TIM PWM通道
        uint8_t Channel_1;
        uint8_t Channel_2;

        //电机 电机输出PWM占空比
        int32_t Compare;
        uint16_t Abs_Compare;

        //电机 目标角速度 rad/s
        float Target_Speed;
        //电机 当前角速度 rad/s
        float Now_Speed;
        float Record_Speed[RECORD_NUM];
        //电机 当前编码器值
        int32_t Now_Encoder;
        //电机 上次编码器值
        int32_t Last_Encoder;
        //电机 编码器值差
        int32_t Encoder_Diff;

        //电机 单圈编码器值
        uint16_t Now_Angle_Encoder;
        //电机 外圈总圈数
        int16_t Total_Round;

        //电机 pwm输出
        uint16_t Target_Compare;
        //电机 最大pwm输出
        uint16_t Max_Compare;
        //定时器最大脉冲计数周期数 0-65535/0-4294967295
        int32_t Max_Encoder_Sum;

        //电机 编码器输出轴一圈脉冲数
        uint16_t Encoder_Resolution;

    private:

        void Output();
};


void Class_Motor::Set_Target_Speed(float __Target_Speed)
{
    Target_Speed = __Target_Speed;
}

void Class_Motor::Set_Target_Compare(uint16_t __Target_Compare)
{
    Target_Compare = __Target_Compare;
}

void Class_Motor::Set_Max_Compare(uint16_t __Max_Compare)
{
    Max_Compare = __Max_Compare;
}

void Class_Motor::Set_Compare(int16_t __Compare)
{
    Compare = __Compare;
    Abs_Compare = (uint16_t)abs((float)Compare);
}

float Class_Motor::Get_Now_Speed()
{
    return (Now_Speed);
}





















#endif