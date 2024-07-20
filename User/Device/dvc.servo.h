#include "drv_tim.h"
#include "tim.h"
#define Angle_to_Compare(x) 1500+x/180*2000 

class Class_Servo
{
public:
    void Init(TIM_HandleTypeDef *__TIM_PWMHandle,uint8_t __PWM_channel_);

    inline void Set_Angle(float __Angle);
    inline float Get_Angle();

    void TIM1ms_Motor_Data_PeriodElapsedCallback();
    void TIM5ms_Motor_Calculate_PeriodElapsedCallback();

protected:

    // TIM PWM句柄
    TIM_HandleTypeDef *TIM_PWMHandle;
    // TIM PWM通道
    uint8_t Channel;

    float Angle;//从-90到90度

    // 输出PWM占空比
    int32_t Compare;
    uint16_t Abs_Compare;

private:
    void Output_Angle();
};

void Class_Servo::Set_Angle(float __Angle)
{
    Angle = __Angle;
    __HAL_TIM_SET_COMPARE(TIM_PWMHandle, Channel, Angle_to_Compare(Angle));
}

float Class_Servo::Get_Angle()
{
   return Angle;
}
