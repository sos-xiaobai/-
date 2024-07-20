/**
 * @file tsk_config_and_callback.cpp
 * @author yssickjgd (yssickjgd@mail.ustc.edu.cn)
 * @brief 临时任务调度测试用函数, 后续用来存放个人定义的回调函数以及若干任务
 * @version 0.1
 * @date 2023-08-29 0.1 23赛季定稿
 *
 * @copyright USTC-RoboWalker (c) 2022
 *
 */

/**
 * @brief TIM开头的默认任务均1ms, 特殊任务需额外标记时间
 *
 */

/* Includes ------------------------------------------------------------------*/

#include "tsk_config_and_callback.h"

/* Private macros ------------------------------------------------------------*/

/* Private types -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

uint16_t Motor_Now_Encoder[4];
uint16_t Motor_Direction[4];
uint16_t Motor_Pre_Encoder[4];

//uint8_t buffer1[19];
uint8_t buffer2[4];

Class_Chariot Chariot;

//机器人控制对象
Class_FSM_Chariot_Control FSM_Chariot;

/* Private function declarations ---------------------------------------------*/
void TIM6_Task1ms_PeriodElapsedCallback();
void TIM7_Task5ms_PeriodElapsedCallback();
void SPI1_IMU_Task_Callback(uint8_t* Tx_Buffer,uint8_t* Rx_Buffer,uint16_t Now_Tx_Length);

/* Function prototypes -------------------------------------------------------*/
void ER08_UART1_Callback(uint8_t *Buffer, uint16_t Length)
{
    FSM_Chariot.Chariot->ER08.UART_RxCpltCallback(Buffer);
}

void Tjc011_UART2_Callback(uint8_t *Buffer, uint16_t Length)
{
    FSM_Chariot.Chariot->Tjc011.UART_RxCpltCallback(Buffer);
}

void SIM_UART5_Callback(uint8_t *Buffer, uint16_t Length)
{
    //FSM_Chariot.Chariot->SIM900A.UART_RxCpltCallback(Buffer);
}

/**
 * @brief 初始化任务
 *
 */
 
uint16_t test_compare1,test_compare2,test_compare3,test_compare4;
uint8_t start_flag=0;
void Task_Init()
{
    
    //NVIC_SystemReset(); // 软件复位
    // DWT初始化
    DWT_Init(168);

    //定时器循环任务
    TIM_Init(&htim6, TIM6_Task1ms_PeriodElapsedCallback);
    TIM_Init(&htim7, TIM7_Task5ms_PeriodElapsedCallback);

    //陀螺仪SPI分配
    SPI_Init(&hspi1,SPI1_IMU_Task_Callback);

    UART_Init(&huart1, ER08_UART1_Callback, 19);
    UART_Init(&huart2, Tjc011_UART2_Callback, 20);
    UART_Init(&huart5, SIM_UART5_Callback, 20);
	HAL_UART_Receive_IT(&huart1, UART1_Manage_Object.Rx_Buffer, 19);

    /********************************* 设备层初始化 *********************************/

     //设备层集成在交互层初始化中，没有显视地初始化

    /********************************* 交互层初始化 *********************************/

   FSM_Chariot.Chariot = &Chariot;
   FSM_Chariot.Init(8,0);
   FSM_Chariot.Chariot->Init();

    /********************************* 使能调度时钟 *********************************/

    // //启动定时器PWM输出
    HAL_TIM_PWM_Start(&htim14,TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim13,TIM_CHANNEL_1);

    HAL_TIM_PWM_Start(&htim5,TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim5,TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim5,TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim5,TIM_CHANNEL_4);

    HAL_TIM_PWM_Start(&htim12,TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim12,TIM_CHANNEL_2);

    HAL_TIM_PWM_Start(&htim8,TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim8,TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim8,TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim8,TIM_CHANNEL_4);

    //编码器初始化
    HAL_TIM_Encoder_Start(&htim1,TIM_CHANNEL_ALL);
    HAL_TIM_Encoder_Start(&htim2,TIM_CHANNEL_ALL);
    HAL_TIM_Encoder_Start(&htim3,TIM_CHANNEL_ALL);
    HAL_TIM_Encoder_Start(&htim4,TIM_CHANNEL_ALL);

   //启动定时器任务中断
   HAL_TIM_Base_Start_IT(&htim6);
   HAL_TIM_Base_Start_IT(&htim7);
}


/**
 * @brief TIM6任务回调函数
 *
 */
void TIM6_Task1ms_PeriodElapsedCallback()
{	
   //任务状态机
   FSM_Chariot.Reload_TIM_Status_PeriodElapsedCallback();

   // IMU任务
   FSM_Chariot.Chariot->Chassis.IMU.TIM_Calculate_PeriodElapsedCallback();    

   // 加速度积分计算位移
   Chariot.Chassis.TIM1ms_Chassis_Posture_PeriodElapsedCallback();

   // 编码器微分计算转速
    for (auto i = 0; i < 4; i++)
        Chariot.Chassis.Motor[i].TIM1ms_Motor_Data_PeriodElapsedCallback();
}



/**
 * @brief TIM7任务回调函数
 *
 */
void TIM7_Task5ms_PeriodElapsedCallback()
{
    /****************************** 交互层回调函数 1ms *****************************************/
    
   //底盘速度解算
   Chariot.Chassis.TIM_Calculate_PeriodElapsedCallback();

   //四电机PID
  for(auto i = 0; i < 4; i++)
     Chariot.Chassis.Motor[i].TIM5ms_Motor_Calculate_PeriodElapsedCallback();
    
    /****************************** 驱动层回调函数 1ms *****************************************/ 
}

/**
 * @brief 前台循环任务
 *
 */
void Task_Loop()
{
    uint8_t test_data[24] = {0};
    test_data[0] = Chariot.Now_Cargo.Position_X;
    test_data[1] = Chariot.Now_Cargo.Position_Y;
    test_data[2] = ' ';
    memcpy(&test_data[3], Chariot.Now_Cargo.Code, 4);
    test_data[7] = ' ';
    memcpy(&test_data[8], Chariot.Now_Cargo.Phone_Number, 11);
	test_data[19] = ' ';
	memcpy(&test_data[20],Chariot.Tjc011.Get_Input_Number(),4);
    HAL_UART_Transmit(&huart6, test_data, 24, 10);

    //喂狗
    HAL_IWDG_Refresh(&hiwdg);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1)
    {
			// 处理USART1接收到的数据
	        ER08_UART1_Callback(UART1_Manage_Object.Rx_Buffer, 19);
          HAL_UART_Receive_IT(&huart1, UART1_Manage_Object.Rx_Buffer, 19);
            
    }
}

void SPI1_IMU_Task_Callback(uint8_t* Tx_Buffer,uint8_t* Rx_Buffer,uint16_t Now_Tx_Length)
{
    /*此SPI接收中断回调函数没用到，为空*/
}


/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/
