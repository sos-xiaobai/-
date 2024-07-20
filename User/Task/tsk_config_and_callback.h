/**
 * @file tsk_config_and_callback.h
 * @author yssickjgd (yssickjgd@mail.ustc.edu.cn)
 * @brief
 * @version 0.1
 * @date 2023-08-29 0.1 23赛季定稿
 *
 * @copyright USTC-RoboWalker (c) 2022
 *
 */


#ifndef TSK_CONFIG_AND_CALLBACK_H
#define TSK_CONFIG_AND_CALLBACK_H

/* Includes ------------------------------------------------------------------*/

#include "ita_chariot.h"

/* Exported macros -----------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/* Exported variables --------------------------------------------------------*/

extern Class_Chariot Chariot;

// 机器人控制对象
extern Class_FSM_Chariot_Control FSM_Chariot;
/* Exported function declarations --------------------------------------------*/

void Task_Init();
void Task_Loop();

#endif

/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/
