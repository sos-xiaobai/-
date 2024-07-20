#include "dvc.servo.h"

void Class_Servo::Init(TIM_HandleTypeDef *__TIM_PWMHandle,uint8_t __PWM_channel_)
{
   TIM_PWMHandle = __TIM_PWMHandle;
   Channel = __PWM_channel_;
   Angle = 0;


   
}
