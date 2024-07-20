/**
 * @file ita_chariot.cpp
 * @author yssickjgd (yssickjgd@mail.ustc.edu.cn)
 * @brief 人机交互控制逻辑
 * @version 0.1
 * @date 2023-08-29 0.1 23赛季定稿
 *
 * @copyright USTC-RoboWalker (c) 2022
 *
 */

/* Includes ------------------------------------------------------------------*/

#include "ita_chariot.h"
#include "drv_math.h"
#include <stdlib.h>

/* Private macros ------------------------------------------------------------*/

/* Private types -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function declarations ---------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/

// 1号 tim14 ch1  1000   	800
// 2号 tim13 ch1  1250 		400
// 3号 tim5  ch3  1550 		1200
// 4号 tim5  ch4  1250 		450

// 夹取 1 3 4 2
// 放回 2 4 3 1
// 舵机输出轴方向看过去，逆时针旋转
float test_angle_1 = -60;
float test_angle_2 = 0;
float test_angle_3 = 60;
float test_angle_4 = 30;
void Class_Chariot::Init_Position()
{
    // 默认先松开夹爪

    // Servo[0].Set_Angle(20);
    Servo[1].Set_Angle(10);
    // Servo[2].Set_Angle(60);
    // Servo[3].Set_Angle(30);
    //__HAL_TIM_SET_COMPARE(&htim14, TIM_CHANNEL_1, 1000);

    // if (__time_cnt >= 450) // 抬起大臂 500ms
    // {
    //     Servo[1].Set_Angle(0);
    // }
    // if (__time_cnt >= 720) // 旋转yaw 500ms
    // {
    //     Servo[2].Set_Angle(0);
    // }
    // if (__time_cnt >= 920) // 折叠小臂 50ms
    // {
    //     Servo[3].Set_Angle(0);
    // }
}

float hi_angle = 10;
float low_angle = -30;
float delta = 0.01;
float angle_3=70.0f;
float angle_4=45;
void Class_Chariot::Scan_Burry(uint16_t __time_cnt)
{
    static float delta_angle = hi_angle;

    delta_angle -= delta;
    if (delta_angle > hi_angle || delta_angle < low_angle)
        delta *= -1.0f;

    Servo[0].Set_Angle(test_angle_1);
    Servo[1].Set_Angle(delta_angle);
    Servo[2].Set_Angle(angle_3);
    Servo[3].Set_Angle(angle_4);
}
// 有货夹取放车上
// 1 3 4 2
//  -5 -45 90 90

// 有货放置到货架上
// 2 4 3 1
//-45 30 -75 -30

// 无货去货架夹取 记得最后夹取
// 2 4 3 1
//-45 30 -75 -5

// 无货 取货回来放置 夹爪保持放置
// 1 2 3 4
//-5 初始 初始 初始
float test_angle_4_=90;//改小往货舱移动
float test_angle_2_=80;//改小往上移动
void Class_Chariot::Burry_Input_Cargo_1(uint16_t __time_cnt)
{
    if (__time_cnt >= 50)
    {
        Servo[1].Set_Angle(0);
    }
    if (__time_cnt >= 250)
    {
        Servo[0].Set_Angle(-5);
    }
    if (__time_cnt >= 560)
    {
        Servo[2].Set_Angle(-45);
    }
    if (__time_cnt >= 1060)
    {
        Servo[3].Set_Angle(test_angle_4_);
    }
    if (__time_cnt >= 1460)
    {
        Servo[1].Set_Angle(test_angle_2_);
    }
}

void Class_Chariot::Burry_Input_Cargo_2(uint16_t __time_cnt)
{
    if (__time_cnt >= 50)
    {
        Servo[1].Set_Angle(-20);
    }
    if (__time_cnt >= 260)
    {
        Servo[3].Set_Angle(45);
    }
    if (__time_cnt >= 660)
    {
        Servo[2].Set_Angle(-90);
    }
    if (__time_cnt >= 1560)
    {
        Servo[0].Set_Angle(-55);
    }
}

void Class_Chariot::Burry_Output_Cargo_1(uint16_t __time_cnt)
{
	if (__time_cnt >= 50)
    Servo[0].Set_Angle(-55);
	 if (__time_cnt >= 360)
    Servo[1].Set_Angle(-45);
	 if (__time_cnt >= 660)
    Servo[3].Set_Angle(45);
	  if (__time_cnt >= 1200)
    Servo[2].Set_Angle(-90);
    if (__time_cnt >= 2000)
    {
        Servo[0].Set_Angle(-5);
    }
}

void Class_Chariot::Burry_Output_Cargo_2(uint16_t __time_cnt)
{
	    if (__time_cnt >= 50)
    Servo[1].Set_Angle(-25);
	        if (__time_cnt >= 360)
    Servo[2].Set_Angle(60);
		    if (__time_cnt >= 660)
    Servo[3].Set_Angle(45);
    if (__time_cnt >= 2000)
    {
        Servo[0].Set_Angle(-55);
    }
}

void Class_Chariot::Midlle_Position(uint16_t __time_cnt)
{
    if (__time_cnt >= 50)
    {
        Servo[2].Set_Angle(-45);
    }
    if (__time_cnt >= 360)
    {
        Servo[3].Set_Angle(105);
    }
    if (__time_cnt >= 660)
    {
        Servo[1].Set_Angle(90);
    }
}

void Put_Burry(uint16_t __time_cnt)
{
    // 默认先加紧夹爪
    __HAL_TIM_SET_COMPARE(&htim14, TIM_CHANNEL_1, 800);
    if (__time_cnt >= 50) // 抬起小臂 500ms
    {
        __HAL_TIM_SET_COMPARE(&htim13, TIM_CHANNEL_1, 1250);
    }
    if (__time_cnt >= 260) // 旋转yaw 500ms
    {
        __HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_4, 1250);
    }
    if (__time_cnt >= 660) // 伸长大臂 500ms
    {
        __HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_3, 1550);
    }
    if (__time_cnt >= 1260) // 松开夹爪 50ms
    {
        __HAL_TIM_SET_COMPARE(&htim14, TIM_CHANNEL_1, 1000);
    }
}

/**
 * @brief 控制交互端初始化
 *
 */
void Class_Chariot::Init()
{
    ER08.Init(&huart1);
    Tjc011.Init(&huart2);

    // 舵机初始化
    Servo[0].Init(&htim14, TIM_CHANNEL_1);
    Servo[1].Init(&htim13, TIM_CHANNEL_1);
    Servo[2].Init(&htim5, TIM_CHANNEL_3);
    Servo[3].Init(&htim5, TIM_CHANNEL_4);

    Servo[0].Set_Angle(20);
    Servo[1].Set_Angle(0);
    Servo[2].Set_Angle(60);
    Servo[3].Set_Angle(30);

    Chassis.IMU.Init();
    Chassis.Init();

    Cargo_List.Init();
    Now_Cargo_List.Init();

    Now_Cargo_Number = 0;
    memset(Now_Cargo_List_Code, 0, 40 * sizeof(uint8_t));
}

uint8_t Class_Chariot::Get_Cargo_Data()
{
    if (Cargo_List.Exist_Cargo((uint8_t *)ER08.Get_code()) != NULL)
    {
        return 0;
    }
    Cargo_List.Add_Cargo(ER08.Get_x(), ER08.Get_y(), (uint8_t *)ER08.Get_tel(), (uint8_t *)ER08.Get_code());
    Now_Cargo.Position_X = ER08.Get_x();
    Now_Cargo.Position_Y = ER08.Get_y();
    memcpy(Now_Cargo.Code, ER08.Get_code(), 4 * sizeof(uint8_t));
    memcpy(Now_Cargo.Phone_Number, ER08.Get_tel(), 11 * sizeof(uint8_t));
    Now_Cargo_Number++;
    return 1;
}

uint8_t Class_Chariot::Jundge_Cargo()
{
    Struct_Cargo *tmp = Cargo_List.Exist_Cargo((uint8_t *)Tjc011.Get_Input_Number());
    if (tmp != NULL)
    {
        memcpy(&Now_Cargo, tmp, sizeof(Struct_Cargo));
        return 1;
    }
    return 0;
}

void Class_Chariot::Output_Cargo()
{
    Cargo_List.Delete_Cargo(Now_Cargo.Code);
    Now_Cargo_Number--;
    if (Now_Cargo_Number < 0)
    {
        Now_Cargo_Number = 0;
    }
}

void Class_Chariot::Set_Control_Status(Enum_Chariot_Control_Status status)
{
    Control_Status = status;
}

Enum_Chariot_Control_Status Class_Chariot::Get_Control_Status()
{
    return Control_Status;
}

/**
 * @brief 计算回调函数
 *
 */
void Class_Chariot::TIM_Calculate_PeriodElapsedCallback()
{
}

/**
 * @brief 在线判断回调函数
 *
 */
void Class_Chariot::TIM1msMod50_Alive_PeriodElapsedCallback()
{
}

/**
 * @brief 有限状态机控制回调函数
 *
 */
uint16_t time_cnt = 0;
void Class_FSM_Chariot_Control::Reload_TIM_Status_PeriodElapsedCallback()
{
    Status[Now_Status_Serial].Time++;
    switch (Now_Status_Serial)
    {
    case 0:
    {
        rgb_SetAllColor(WHITE);
        /*等待状态*/
        //已经入库的货物时间计算
        for (Struct_Cargo *tmp = Chariot->Cargo_List.First_Cargo; tmp != NULL; tmp = tmp->Next_Cargo)
        {
            tmp->Cargo_Save_Time_Cnt++;
            if (tmp->Cargo_Save_Time_Cnt >= 1000)
            {
                tmp->Cargo_Save_Time_Cnt = 0;
                tmp->Cargo_Save_Time++;
            }
            if(tmp->Cargo_Save_Time >= tmp->Cargo_Save_Time_Max && !tmp->Cargo_Take_Warning_Flag)
            {
                //发送短信 第一次提醒
                Chariot->SIM900A.Sim900a_Send_Data_Warning_First((char *)tmp->Code, (char *)tmp->Phone_Number);
                tmp->Cargo_Take_Warning_Flag = 1;
            }
            if(tmp->Cargo_Save_Time >= tmp->Cargo_Save_Time_Max*2 && !tmp->Cargo_Take_Change_Flag)
            {
                //发送短信 第二次提醒 更改取件码
                for(int i = 0; i < 4; i++)
                {
                    uint8_t a = rand() % 10 + 48;
                    tmp->Code[i] = a;
                }
                Chariot->SIM900A.Sim900a_Send_Data_Warning_Second((char *)tmp->Code, (char *)tmp->Phone_Number);
                tmp->Cargo_Take_Change_Flag = 1;
            }
        }
        // 夹爪初始位置
        Chariot->Scan_Burry(Status[Now_Status_Serial].Time);

        // 设置目标点 x:0 y:0
        Chariot->Chassis.Set_Target_Position_X(0);
        Chariot->Chassis.Set_Target_Position_Y(0);

        // 目前未收到任务
        Chariot->Set_Control_Status(Chariot_Disable_Status);

        // 获取货物信息 如果更新就获取信息 转到前往状态
        if (Chariot->ER08.Updata_Flag)
        {
            Chariot->ER08.Updata_Flag = 0;
            // 添加当前货物信息入链表  如果货物信息不重复
            if (Chariot->Get_Cargo_Data())
            {
                // 发送短信
                Chariot->SIM900A.Sim900a_Send_Data((char *)Chariot->Now_Cargo.Code, (char *)Chariot->Now_Cargo.Phone_Number);
                Chariot->Set_Control_Status(Chariot_Input_Cargo_Status);
                Chariot->Servo[1].Set_Angle(-0);
                //Chariot->Servo[2].Set_Angle(30);
                Set_Status(1);
            }
        }
        //如果输入手机尾号后四位 更新需要取件的货物信息
        if(Chariot->Tjc011.Change_data_Flag)
        {
            Struct_Cargo* p = Chariot->Cargo_List.First_Cargo;
            uint8_t Code[4] = {0};
            //得到输入的尾号
            memcpy(Code,Chariot->Tjc011.Get_change_number(),4);
            while (p != NULL)
            {
                uint8_t i = 0;  //索引 存放对应手机尾号的货物取件码
                if (p->Phone_Number[7] == Code[0] && p->Phone_Number[8] == Code[1] && p->Phone_Number[9] == Code[2] && p->Phone_Number[10] == Code[3])
                {
                    //记录信息
                    memcpy(&Chariot->Now_Cargo_List_Code[i][0],p->Code,4);
                    //添加到链表
                    Chariot->Cargo_List.Add_Cargo(p->Position_X,p->Position_Y,p->Phone_Number,p->Code);
                    i++;
                }
                p = p->Next_Cargo;
            }

        }

        // 获取取件码信息 如果更新就获取信息 转到取件状态
        if (Chariot->Tjc011.Input_data_Flag)
        {
            if (Chariot->Jundge_Cargo()) // 如果取件码正确 并且更新当前货物信息 转到前往状态
            {
                Chariot->Set_Control_Status(Chariot_Output_Cargo_Status);
                Set_Status(1);
            }
            else
                rgb_SetAllColor(RED);
            Chariot->Tjc011.Input_data_Flag = 0;
        }

        //如果待取件链表不为空 就一直进行取件操作
        if(Chariot->Now_Cargo_List.First_Cargo != NULL)
        {
            for(uint8_t i =0;i<10;i++)
            {
                //如果取件码存在
                Struct_Cargo *tmp = Chariot->Now_Cargo_List.Exist_Cargo(&Chariot->Now_Cargo_List_Code[i][0]);
                if (tmp != NULL)
                {
                    //赋值到当前货物信息 并且删除待取件链表中的货物
                    memcpy(&Chariot->Now_Cargo, tmp, sizeof(Struct_Cargo));
                    Chariot->Now_Cargo_List.Delete_Cargo(Chariot->Now_Cargo.Code);
                }          
                Chariot->Set_Control_Status(Chariot_Output_Cargo_Status);
                Set_Status(1);
            }
        }
    }
    break;
    case 1:
        /*导航前往状态*/
        rgb_SetAllColor(GREEN);
        // 夹取货物
        Chariot->Burry_Input_Cargo_1(Status[Now_Status_Serial].Time);

        // 夹取完成后延时1s
        if (Status[Now_Status_Serial].Time >= 4000)
        {
            // 设置目标点
            Chariot->Chassis.Set_Target_Position_X((-1.0f * (Chariot->Now_Cargo.Position_X - 48))/4.0);
            Chariot->Chassis.Set_Target_Position_Y((-1.0f * (Chariot->Now_Cargo.Position_Y - 48))/4.0);
            Chariot->Chassis.Set_Target_Angle(0);

            // 到达目标点 跳转到下一个状态
            if (fabs(Chariot->Chassis.Get_Now_Position_X() - Chariot->Chassis.Get_Target_Position_X()) < 0.02 &&
                (fabs(Chariot->Chassis.Get_Now_Position_Y() - Chariot->Chassis.Get_Target_Position_Y()) < 0.02))
            {
                Set_Status(2);
            }
            // if(Status[Now_Status_Serial].Time > 10000)
            // {
            //     Set_Status(2);
            // }
        }
        break;
    case 2:
        /*放件/取件状态*/
        // 先延时1s
        rgb_SetAllColor(RED);
        if (Status[Now_Status_Serial].Time >= 4000)
        {
            // 取件
            if (Chariot->Get_Control_Status() == Chariot_Output_Cargo_Status)
            {
                // 夹取货柜的货物
                Chariot->Burry_Output_Cargo_1(Status[Now_Status_Serial].Time - 4000);
            }
            // 放件
            else if (Chariot->Get_Control_Status() == Chariot_Input_Cargo_Status)
            {
                // 放置货物
                Chariot->Burry_Input_Cargo_2(Status[Now_Status_Serial].Time - 4000);
            }
        }
        // 整个操作5s后返回
        if (Status[Now_Status_Serial].Time > 8000)
        {
            Set_Status(3);
        }
        break;
    case 3:
        /*导航返回状态*/
        rgb_SetAllColor(GREEN);
        Chariot->Midlle_Position(Status[Now_Status_Serial].Time);

        if (Status[Now_Status_Serial].Time > 2000)
        {
            // 设置目标点
            Chariot->Chassis.Set_Target_Position_X(0);
            Chariot->Chassis.Set_Target_Position_Y(0);
            Chariot->Chassis.Set_Target_Angle(0);
            // 到达目标点 跳转到下一个状态
            if (fabs(Chariot->Chassis.Get_Now_Position_X() - Chariot->Chassis.Get_Target_Position_X()) < 0.02 &&
                (fabs(Chariot->Chassis.Get_Now_Position_Y() - Chariot->Chassis.Get_Target_Position_Y()) < 0.02))
            {
                Set_Status(4);
            }
        }
        break;

    case 4:
        /*返回类型判断状态*/
        // 先延时1s
        if (Status[Now_Status_Serial].Time >= 2000)
        {
            // 取件返回
            if (Chariot->Get_Control_Status() == Chariot_Output_Cargo_Status)
            {
                // 链表删除当前货物信息
                Chariot->Output_Cargo();
                // 返回初始状态
                Chariot->Burry_Output_Cargo_2(Status[Now_Status_Serial].Time - 2000);
                if (Status[Now_Status_Serial].Time > 5000)
                    Set_Status(0); // 返回初始状态
            }
            // 放件返回
            else if (Chariot->Get_Control_Status() == Chariot_Input_Cargo_Status)
            {
                Set_Status(0);
            }
        }
        break;
    }
}

/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/
