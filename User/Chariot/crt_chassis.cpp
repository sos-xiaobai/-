/**
 * @file crt_chassis.cpp
 * @author yssickjgd (yssickjgd@mail.ustc.edu.cn)
 * @brief 底盘电控
 * @version 0.1
 * @date 2023-08-29 0.1 23赛季定稿
 *
 * @copyright USTC-RoboWalker (c) 2022
 *
 */

/**
 * @brief 轮组编号
 * 3 2
 *  1
 */

/* Includes ------------------------------------------------------------------*/

#include "crt_chassis.h"

/* Private macros ------------------------------------------------------------*/
#define ENCODER_NAVIGATE
#define IMU_NAVIGATE

/* Private types -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function declarations ---------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/

// 1号 tim14 ch1 
// 2号 tim13 ch1 
// 3号 tim5  ch3 
// 4号 tim5  ch4 
/**
 * @brief 底盘初始化
 *
 * @param __Chassis_Control_Type 底盘控制方式, 默认舵轮方式
 * @param __Speed 底盘速度限制最大值
 */
void Class_Chassis::Init()
{
    //斜坡函数加减速速度X  控制周期1ms
    Slope_Velocity_X.Init(0.004f,0.008f);
    //斜坡函数加减速速度Y  控制周期1ms
    Slope_Velocity_Y.Init(0.004f,0.008f);
    //斜坡函数加减速角速度
    Slope_Omega.Init(0.05f, 0.05f);

    //各个PID初始化
    Position_X_PID.Init(10, 0, 0, 0, 0, 0 , 0, 0, 0, 0.005, 0);
    Position_Y_PID.Init(10, 0, 0, 0, 0, 0 , 0, 0, 0, 0.005, 0);
    Position_Yaw_PID.Init(0.1, 0, 0, 0, 0, 0 , 0, 0, 0, 0.005, 0);



    //电机初始化
    Motor[0].Init(&htim12, &htim2, TIM_CHANNEL_1, TIM_CHANNEL_2, 5000, 2484);
    Motor[1].Init(&htim8, &htim1, TIM_CHANNEL_1, TIM_CHANNEL_2, 5000, 2471);
    Motor[2].Init(&htim8,&htim4,TIM_CHANNEL_3,TIM_CHANNEL_4,5000,2494);
    Motor[3].Init(&htim5,&htim3,TIM_CHANNEL_1,TIM_CHANNEL_2,20000,2468);

    //电机PID初始化
    Motor[0].Speed_PID.Init(1300, 1200, 0, 0, 1500, 4999 , 0, 0, 0, 0.005, 0);
    Motor[1].Speed_PID.Init(1300, 1200, 0, 0, 1500, 4999 , 0, 0, 0, 0.005, 0);
    Motor[2].Speed_PID.Init(1300, 1200, 0, 0, 1500, 4999 , 0, 0, 0, 0.005, 0);
    Motor[3].Speed_PID.Init(4500, 2200, 0, 0, 5000, 20000 , 0, 0, 0, 0.005, 0);
}


/**
 * @brief 速度解算
 *
 */
float motor1_temp_linear_vel;
float motor3_temp_linear_vel;
float motor2_temp_linear_vel;
float motor4_temp_linear_vel;
void Class_Chassis::Speed_Resolution(){
    //获取当前速度值，用于速度解算初始值获取
    switch (Chassis_Control_Type)
    {
        case (Chassis_Control_Type_DISABLE):
        {
            //底盘失能 四轮子自锁
            for (int i = 0; i < 4; i++)
            {
                Motor[i].Set_Target_Speed(0.0f);
            }            
        }
        break;
        default:
            //底盘限速
            if (Velocity_X_Max != 0)
            {
                Math_Constrain(&Target_Velocity_X, -Velocity_X_Max, Velocity_X_Max);
            }
            if (Velocity_Y_Max != 0)
            {
                Math_Constrain(&Target_Velocity_Y, -Velocity_Y_Max, Velocity_Y_Max);
            }
            if (Omega_Max != 0)
            {
                Math_Constrain(&Target_Omega, -Omega_Max, Omega_Max);
            }

            #ifdef SPEED_SLOPE
            //速度换算，正运动学分解
            float motor1_temp_linear_vel = Slope_Velocity_Y.Get_Out() - Slope_Velocity_X.Get_Out() + Slope_Omega.Get_Out()*(HALF_WIDTH+HALF_LENGTH);
            float motor2_temp_linear_vel = Slope_Velocity_Y.Get_Out() + Slope_Velocity_X.Get_Out() - Slope_Omega.Get_Out()*(HALF_WIDTH+HALF_LENGTH);
            float motor3_temp_linear_vel = Slope_Velocity_Y.Get_Out() + Slope_Velocity_X.Get_Out() + Slope_Omega.Get_Out()*(HALF_WIDTH+HALF_LENGTH);
            float motor4_temp_linear_vel = Slope_Velocity_Y.Get_Out() - Slope_Velocity_X.Get_Out() - Slope_Omega.Get_Out()*(HALF_WIDTH+HALF_LENGTH);
            #else
            //速度换算，正运动学分解
             motor1_temp_linear_vel = Target_Velocity_Y - Target_Velocity_X + Target_Omega*(HALF_WIDTH+HALF_LENGTH);
             motor2_temp_linear_vel = Target_Velocity_Y + Target_Velocity_X - Target_Omega*(HALF_WIDTH+HALF_LENGTH);
             motor3_temp_linear_vel = Target_Velocity_Y - Target_Velocity_X - Target_Omega*(HALF_WIDTH+HALF_LENGTH);
             motor4_temp_linear_vel = Target_Velocity_Y + Target_Velocity_X + Target_Omega*(HALF_WIDTH+HALF_LENGTH);
            #endif            
            //线速度 cm/s  转角速度  RAD 
            float motor1_temp_rad = motor1_temp_linear_vel * VEL2RAD;
            float motor2_temp_rad = motor2_temp_linear_vel * VEL2RAD;
            float motor3_temp_rad = motor3_temp_linear_vel * VEL2RAD;
            float motor4_temp_rad = motor4_temp_linear_vel * VEL2RAD;
            //角速度*减速比  设定目标 直接给到电机输出轴

            Motor[0].Set_Target_Speed(- motor2_temp_rad);
            Motor[1].Set_Target_Speed(- motor1_temp_rad);
            Motor[2].Set_Target_Speed(- motor4_temp_rad);
            Motor[3].Set_Target_Speed(- motor3_temp_rad);
    }   
}


void Class_Chassis::Navigation_Calibrate()
{
    static uint16_t calibrate_time = 0;
    float temp_accl_x, temp_accl_y;
    float temp_derta_accl_x, temp_derta_accl_y;
    float temp_angle_roll,temp_angle_pitch;

    if(IsCalibrated != 1)
    {
        calibrate_time++;
        if(calibrate_time < Calibrate_Time)
        {
            //获取加速度值
            temp_angle_roll = IMU.Get_Rad_Roll();
            temp_angle_pitch = IMU.Get_Rad_Pitch();
            temp_accl_x = IMU.Get_Accel_X() * cos(temp_angle_roll) - IMU.Get_Accel_Z() * sin(temp_angle_roll);
            temp_accl_y = IMU.Get_Accel_Y() * cos(temp_angle_pitch) + IMU.Get_Accel_Z() * sin(temp_angle_pitch);

            // temp_accl_x = IMU.Get_Accel_X();
            // temp_accl_y = IMU.Get_Accel_Y();

            //加速度sum
            Calibrate_Acceleration_X_Offset += temp_accl_x;
            Calibrate_Acceleration_Y_Offset += temp_accl_y;

            //得到采样过程中最大最小值
            if (temp_accl_x > Calibrate_Acceleration_X_Max)
                Calibrate_Acceleration_X_Max = temp_accl_x;
            if (temp_accl_x < Calibrate_Acceleration_X_Min)
                Calibrate_Acceleration_X_Min = temp_accl_x;
            if (temp_accl_y > Calibrate_Acceleration_Y_Max)
                Calibrate_Acceleration_Y_Max = temp_accl_y;
            if (temp_accl_y < Calibrate_Acceleration_Y_Min)
                Calibrate_Acceleration_Y_Min = temp_accl_y;

            //得到采样过程中最大最小值差值
            temp_derta_accl_x = Calibrate_Acceleration_X_Max - Calibrate_Acceleration_X_Min;
            temp_derta_accl_y = Calibrate_Acceleration_Y_Max - Calibrate_Acceleration_Y_Min;

//            //如果加速度变化大于0.01fm/s/s 视为受到外力冲击 清零重新校准
//            if(fabs(temp_derta_accl_x) >0.01f ||
//            fabs(temp_derta_accl_y) >0.01f)
//            {
//                Calibrate_Acceleration_X_Offset = 0.0f;
//                Calibrate_Acceleration_Y_Offset = 0.0f;
//                Calibrate_Acceleration_X_Max = 0.0f;
//                Calibrate_Acceleration_X_Min = 0.0f;
//                Calibrate_Acceleration_Y_Max = 0.0f;
//                Calibrate_Acceleration_Y_Min = 0.0f;
//                calibrate_time = 0;
//            }
        }
        else if(calibrate_time >= Calibrate_Time)
        {
            //校准完成 计算平均offset值
            Calibrate_Acceleration_X_Offset /= (float)Calibrate_Time;
            Calibrate_Acceleration_Y_Offset /= (float)Calibrate_Time;
            IsCalibrated = 1;
        }
    }
}

/**
 * @brief TIM定时器中断计算回调函数
 *
 */
void Class_Chassis::TIM_Calculate_PeriodElapsedCallback()
{
    //补充导航部分pid
  Position_Y_PID.Set_Target(Target_Position_Y);
  Position_Y_PID.Set_Now(Now_Position_Y);
  Position_Y_PID.TIM_Adjust_PeriodElapsedCallback();
  Target_Velocity_Y = Position_Y_PID.Get_Out();

  Position_X_PID.Set_Target(Target_Position_X);
  Position_X_PID.Set_Now(Now_Position_X);
  Position_X_PID.TIM_Adjust_PeriodElapsedCallback();
  Target_Velocity_X = -1.0f * Position_X_PID.Get_Out();

   Position_Yaw_PID.Set_Target(Target_Angle);
   Position_Yaw_PID.Set_Now(Now_Angle);
   Position_Yaw_PID.TIM_Adjust_PeriodElapsedCallback();
   Target_Omega = Position_Yaw_PID.Get_Out();

    #ifdef SPEED_SLOPE

    //斜坡函数计算用于速度解算初始值获取
    Slope_Velocity_X.Set_Target(Target_Velocity_X);
    Slope_Velocity_X.TIM_Calculate_PeriodElapsedCallback();
    Slope_Velocity_Y.Set_Target(Target_Velocity_Y);
    Slope_Velocity_Y.TIM_Calculate_PeriodElapsedCallback();
    Slope_Omega.Set_Target(Target_Omega);
    Slope_Omega.TIM_Calculate_PeriodElapsedCallback();

    #endif
    //速度解算
    Speed_Resolution();

}
void Class_Chassis::TIM1ms_Chassis_Posture_PeriodElapsedCallback()
{
    #ifdef IMU_NAVIGATE
    //导航IMU加速度初始化
    Navigation_Calibrate();

    //如果加速度校准成功
    if(IsCalibrated == 1)
    {
        //直接读取IMU yaw姿态
        Now_Angle = -IMU.Get_Angle_Yaw();
        Now_Omega = IMU.Get_Gyro_Yaw();

        //IMU数据获取
        IMU_Now_Acceleration_X = IMU.Get_Accel_X() * cos(IMU.Get_Rad_Roll()) - IMU.Get_Accel_Z() * sin(IMU.Get_Rad_Roll()) - Calibrate_Acceleration_X_Offset;
        IMU_Now_Acceleration_Y = IMU.Get_Accel_Y() * cos(IMU.Get_Rad_Pitch()) + IMU.Get_Accel_Z() * sin(IMU.Get_Rad_Pitch()) - Calibrate_Acceleration_Y_Offset;
        // IMU_Now_Acceleration_X = IMU.Get_Accel_X() - Calibrate_Acceleration_X_Offset;
        // IMU_Now_Acceleration_Y = IMU.Get_Accel_Y() - Calibrate_Acceleration_Y_Offset;

        //加速度积分得到速度
        Integral_Now_Velocity_X += (fabs(IMU_Now_Acceleration_X - IMU_Pre_Acceleration_X) / 2.0f + IMU_Now_Acceleration_X)* Dt;
        Integral_Now_Velocity_Y += (fabs(IMU_Now_Acceleration_Y - IMU_Pre_Acceleration_Y) / 2.0f + IMU_Now_Acceleration_Y)* Dt;

        //如果编码器速度小于0.05f 认为底盘停止
        if (fabs(Motor[0].Get_Now_Speed()) < 0.05f &&
            fabs(Motor[1].Get_Now_Speed()) < 0.05f &&
            fabs(Motor[2].Get_Now_Speed()) < 0.05f &&
            fabs(Motor[3].Get_Now_Speed()) < 0.05f)
        {
            Integral_Now_Velocity_X = 0.0f;
            Integral_Now_Velocity_Y = 0.0f;
        }

        //速度积分得到角度
        imu_now_position_x += (fabs(Integral_Now_Velocity_X - Integral_Pre_Velocity_X) / 2.0f + Integral_Now_Velocity_X) * Dt;
        imu_now_position_y += (fabs(Integral_Now_Velocity_Y - Integral_Pre_Velocity_Y) / 2.0f + Integral_Now_Velocity_Y) * Dt;

        //前一项赋值
        IMU_Pre_Acceleration_X = IMU_Now_Acceleration_X;
        IMU_Pre_Acceleration_Y = IMU_Now_Acceleration_Y;
        Integral_Pre_Velocity_X = Integral_Now_Velocity_X;
        Integral_Pre_Velocity_Y = Integral_Now_Velocity_Y;
    }

    #endif

    #ifdef ENCODER_NAVIGATE

//    //直接读取IMU yaw姿态
    Now_Angle = -IMU.Get_Angle_Yaw();
    Now_Omega = IMU.Get_Gyro_Yaw();

    //  补充导航部分
    float motor1_temp_rad = -Motor[0].Get_Now_Speed();
    float motor2_temp_rad = -Motor[1].Get_Now_Speed();
    float motor3_temp_rad = -Motor[2].Get_Now_Speed();
    float motor4_temp_rad = -Motor[3].Get_Now_Speed();

    // 将角速度转换回线速度
    float motor1_temp_linear_vel = motor1_temp_rad / VEL2RAD;
    float motor2_temp_linear_vel = motor2_temp_rad / VEL2RAD;
    float motor3_temp_linear_vel = motor3_temp_rad / VEL2RAD;
    float motor4_temp_linear_vel = motor4_temp_rad / VEL2RAD;

    // 逆运动学解算
    Now_Velocity_X = (motor2_temp_linear_vel - motor1_temp_linear_vel + motor4_temp_linear_vel - motor3_temp_linear_vel) / 4.0f;
    Now_Velocity_Y = (motor1_temp_linear_vel + motor2_temp_linear_vel + motor3_temp_linear_vel + motor4_temp_linear_vel) / 4.0f;
    Now_Omega = (motor1_temp_linear_vel - motor2_temp_linear_vel - motor3_temp_linear_vel + motor4_temp_linear_vel) / (4.0f * (HALF_WIDTH + HALF_LENGTH));    

    //速度积分得到位置
    Now_Position_X += (fabs(Now_Velocity_X - Pre_Velocity_X) / 2.0f + Now_Velocity_X)* Dt;
    Now_Position_Y += (fabs(Now_Velocity_Y - Pre_Velocity_Y) / 2.0f + Now_Velocity_Y)* Dt;

    //前一项赋值
    Pre_Velocity_X = Now_Velocity_X;
    Pre_Velocity_Y = Now_Velocity_Y;

    #endif
}

/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/
