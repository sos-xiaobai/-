/**
 * @file crt_chassis.h
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

#ifndef CRT_CHASSIS_H
#define CRT_CHASSIS_H

/* Includes ------------------------------------------------------------------*/

#include "alg_slope.h"
#include "dvc_motor.h"
#include "dvc_imu.h"
#include "tim.h"
/* Exported macros -----------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/**
 * @brief 底盘冲刺状态枚举
 *
 */
enum Enum_Sprint_Status : uint8_t
{
    Sprint_Status_DISABLE = 0, 
    Sprint_Status_ENABLE,
};


/**
 * @brief 底盘控制类型
 *
 */
enum Enum_Chassis_Control_Type :uint8_t
{
    Chassis_Control_Type_DISABLE = 0,
    Chassis_Control_Type_FLLOW,
    Chassis_Control_Type_SPIN,
};

/**
 * @brief Specialized, 三轮舵轮底盘类
 *
 */
//omnidirectional 全向轮
class Class_Chassis
{
public:

    //斜坡函数加减速速度X
    Class_Slope Slope_Velocity_X;
    //斜坡函数加减速速度Y
    Class_Slope Slope_Velocity_Y;
    //斜坡函数加减速角速度
    Class_Slope Slope_Omega;

    //四个电机
    Class_Motor Motor[4];


    //IMU
    Class_IMU IMU;

    //坐标和角度的PID
    Class_PID Position_X_PID;
    Class_PID Position_Y_PID;
    Class_PID Position_Yaw_PID;

    void Init();

    inline void Set_Target_Omega(float __Target_Omega);
    inline void Set_Target_Angle(float __Target_Angle);
    inline void Set_Target_Position_X(float __Target_Position_X);
    inline void Set_Target_Position_Y(float __Target_Position_Y);
    inline void Set_Target_Velocity_X(float __Target_Velocity_X);
    inline void Set_Target_Velocity_Y(float __Target_Velocity_Y);

    inline void Set_Now_Omega(float __Now_Omega);
    // inline void Set_Now_Angle(float __Now_Angle);
    // inline void Set_Now_Position_X(float __Now_Position_X);
    // inline void Set_Now_Position_Y(float __Now_Position_Y);
    inline void Set_Now_Velocity_X(float __Now_Velocity_X);
    inline void Set_Now_Velocity_Y(float __Now_Velocity_Y);

    inline void Set_Chassis_Control_Type(Enum_Chassis_Control_Type __Chassis_Control_Type);    

    inline float Get_Now_Omega();
    inline float Get_Now_Angle();
    inline float Get_Now_Position_X();
    inline float Get_Now_Position_Y();
    inline float Get_Target_Position_X();
    inline float Get_Target_Position_Y();
    inline float Get_Target_Velocity_X();
    inline float Get_Target_Velocity_Y();
    inline float Get_Target_Angle();
    inline float Get_Now_Velocity_X();
    inline float Get_Now_Velocity_Y();
    inline Enum_Chassis_Control_Type Get_Chassis_Control_Type();

    void TIM_Calculate_PeriodElapsedCallback();
    void TIM1ms_Chassis_Posture_PeriodElapsedCallback();

protected:
    //初始化相关常量

    //速度X限制
    float Velocity_X_Max = 10.0f;
    //速度Y限制
    float Velocity_Y_Max = 10.0f;
    //角速度限制
    float Omega_Max = 4.0f;

    //常量

    //电机理论上最大输出
    float Wheel_Max_Output = 16384.0f;

    //内部变量

    //转动电机目标值
    float Target_Wheel_Omega[4];

    //读变量

    //写变量

    //读写变量

    //底盘控制方法
    Enum_Chassis_Control_Type Chassis_Control_Type = Chassis_Control_Type_FLLOW;

    //目标位移X
    float Target_Position_X = 0.0f;
    //目标位移Y
    float Target_Position_Y = 0.0f;
    //目标速度X
    float Target_Velocity_X = 0.0f;
    //目标速度Y
    float Target_Velocity_Y = 0.0f;
    //目标角速度
    float Target_Omega = 0.0f;
    //目标角度
    float Target_Angle = 0.0f;

    //当前位移X
    float Now_Position_X = 0.0f;
    //当前位移Y
    float Now_Position_Y = 0.0f;

    float imu_now_position_x, imu_now_position_y;
    //当前角度
    float Now_Angle = 0.0f;    
    //当前速度X
    float Now_Velocity_X = 0.0f;
    //当前速度Y
    float Now_Velocity_Y = 0.0f;
    //当前角速度
    float Now_Omega = 0.0f;
    //前一个线速度 x
    float Pre_Velocity_X = 0.0f;
    //前一个线速度 y
    float Pre_Velocity_Y = 0.0f;
    //校准执行循环次数
    int32_t Calibrate_Time = 10000;

    //积分时间间隔 单位s
    float Dt = 0.001f;

    //校准完成标志位
    uint8_t IsCalibrated = 0;

    //用于积分计算的加速度变量
    float IMU_Now_Acceleration_X = 0.0f;
    float IMU_Pre_Acceleration_X = 0.0f;
    float IMU_Now_Acceleration_Y = 0.0f;
    float IMU_Pre_Acceleration_Y = 0.0f;

    //用于积分计算的速度变量
    float Integral_Now_Velocity_X = 0.0f;
    float Integral_Pre_Velocity_X = 0.0f;
    float Integral_Now_Velocity_Y = 0.0f;
    float Integral_Pre_Velocity_Y = 0.0f;

    //校准初始加速度偏移量 max min
    float Calibrate_Acceleration_X_Offset = 0.0f;
    float Calibrate_Acceleration_X_Max = 0.0f;
    float Calibrate_Acceleration_X_Min = 0.0f;
    float Calibrate_Acceleration_Y_Offset = 0.0f;
    float Calibrate_Acceleration_Y_Max = 0.0f;
    float Calibrate_Acceleration_Y_Min = 0.0f;

    //内部函数
    void Speed_Resolution();
    void Navigation_Calibrate();
};

/* Exported variables --------------------------------------------------------*/

//三轮车底盘参数

//轮组半径
const float WHEEL_RADIUS = 0.0520f;

//轮距中心长度
const float WHEEL_TO_CORE_DISTANCE[3] = {0.23724f, 0.21224f, 0.21224f};

//前心距中心长度
const float FRONT_CENTER_TO_CORE_DISTANCE = 0.11862f;

//前后轮距
const float FRONT_TO_REAR_DISTANCE = WHEEL_TO_CORE_DISTANCE[0] + FRONT_CENTER_TO_CORE_DISTANCE;

//前轮距前心
const float FRONT_TO_FRONT_CENTER_DISTANCE = 0.176f;

//轮组方位角
const float WHEEL_AZIMUTH[3] = {0.0f, atan2f(-FRONT_TO_FRONT_CENTER_DISTANCE, -FRONT_CENTER_TO_CORE_DISTANCE), atan2f(FRONT_TO_FRONT_CENTER_DISTANCE, -FRONT_CENTER_TO_CORE_DISTANCE)};

//轮子直径 单位m
const float WHELL_DIAMETER = 0.06f;	

//底盘半宽 单位m
const float HALF_WIDTH = 0.165f;		

//底盘半长 单位m
const float HALF_LENGTH = 0.246f;	

//转速转角速度	1 rpm = 2pi/60 rad/s 
const float RPM2RAD = 0.104720f;				

//转速转线速度	vel = rpn*pi*D/60  cm/s
const float RPM2VEL = 0.806342f;			

//线速度转转度  //1.240168							
const float VEL2RPM = 1.240168f;				

//线速度转角速度 rad/s
const float VEL2RAD = 1.0f/(WHELL_DIAMETER/2.0f);

//齿轮箱减速比;	
const float M3508_REDUCTION_RATIO = 13.733f;	
/* Exported function declarations --------------------------------------------*/

/**
 * @brief 获取底盘控制方法
 *
 * @return Enum_Chassis_Control_Type 底盘控制方法
 */
Enum_Chassis_Control_Type Class_Chassis::Get_Chassis_Control_Type()
{
    return (Chassis_Control_Type);
}

/**
 * @brief 设定底盘控制方法
 *
 * @param __Chassis_Control_Type 底盘控制方法
 */
void Class_Chassis::Set_Chassis_Control_Type(Enum_Chassis_Control_Type __Chassis_Control_Type)
{
    Chassis_Control_Type = __Chassis_Control_Type;
}

/**
 * @brief 设定目标速度X
 *
 * @param __Target_Velocity_X 目标速度X
 */
void Class_Chassis::Set_Target_Velocity_X(float __Target_Velocity_X)
{
    Target_Velocity_X = __Target_Velocity_X;
}

/**
 * @brief 设定目标速度Y
 *
 * @param __Target_Velocity_Y 目标速度Y
 */
void Class_Chassis::Set_Target_Velocity_Y(float __Target_Velocity_Y)
{
    Target_Velocity_Y = __Target_Velocity_Y;
}

/**
 * @brief 设定目标角速度
 *
 * @param __Target_Omega 目标角速度
 */
void Class_Chassis::Set_Target_Omega(float __Target_Omega)
{
    Target_Omega = __Target_Omega;
}

void Class_Chassis::Set_Target_Angle(float __Target_Angle)
{
    Target_Angle = __Target_Angle;
}
void Class_Chassis::Set_Target_Position_X(float __Target_Position_X)
{
    Target_Position_X = __Target_Position_X;
}
void Class_Chassis::Set_Target_Position_Y(float __Target_Position_Y)
{
    Target_Position_Y = __Target_Position_Y; 
}

/**
 * @brief 设定当前速度X
 *
 * @param __Now_Velocity_X 当前速度X
 */
void Class_Chassis::Set_Now_Velocity_X(float __Now_Velocity_X)
{
    Now_Velocity_X = __Now_Velocity_X;
}

/**
 * @brief 设定当前速度Y
 *
 * @param __Now_Velocity_Y 当前速度Y
 */
void Class_Chassis::Set_Now_Velocity_Y(float __Now_Velocity_Y)
{
    Now_Velocity_Y = __Now_Velocity_Y;
}

/**
 * @brief 设定当前角速度
 *
 * @param __Now_Omega 当前角速度
 */
void Class_Chassis::Set_Now_Omega(float __Velocity_Y_Max)
{
    Now_Omega = __Velocity_Y_Max;
}

float Class_Chassis::Get_Target_Velocity_X()
{
    return (Target_Velocity_X);
}

float Class_Chassis::Get_Target_Velocity_Y()
{
    return (Target_Velocity_Y);

}

float Class_Chassis::Get_Target_Angle()
{
    return (Target_Angle);
}

float Class_Chassis::Get_Now_Velocity_X()
{
    return (Now_Velocity_X);
}


float Class_Chassis::Get_Now_Velocity_Y()
{
    return (Now_Velocity_Y);
}


float Class_Chassis::Get_Now_Position_X()
{
    return (Now_Position_X);
}
float Class_Chassis::Get_Now_Position_Y()
{
    return (Now_Position_Y);

}
float Class_Chassis::Get_Target_Position_X()
{
    return (Target_Position_X);
}
float Class_Chassis::Get_Target_Position_Y()
{
    return (Target_Position_Y);
}

float Class_Chassis::Get_Now_Omega()
{
    return (Now_Omega);
}

float Class_Chassis::Get_Now_Angle()
{
    return (Now_Angle);
}

#endif

/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/
