/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define INT1_ACCEL_Pin GPIO_PIN_1
#define INT1_ACCEL_GPIO_Port GPIOE
#define CS1_ACCEL_Pin GPIO_PIN_0
#define CS1_ACCEL_GPIO_Port GPIOE
#define CS1_GYRO_Pin GPIO_PIN_8
#define CS1_GYRO_GPIO_Port GPIOB
#define INT1_GYRO_Pin GPIO_PIN_9
#define INT1_GYRO_GPIO_Port GPIOB
#define INT1_GYRO_EXTI_IRQn EXTI9_5_IRQn
#define BoardC_CAN1_Rx_Pin GPIO_PIN_0
#define BoardC_CAN1_Rx_GPIO_Port GPIOD
#define TRIG_Pin GPIO_PIN_12
#define TRIG_GPIO_Port GPIOA
#define BoardC_CAN1_Tx_Pin GPIO_PIN_1
#define BoardC_CAN1_Tx_GPIO_Port GPIOD
#define WAKE_Pin GPIO_PIN_11
#define WAKE_GPIO_Port GPIOA
#define RSTN_IST8310_Pin GPIO_PIN_6
#define RSTN_IST8310_GPIO_Port GPIOG
#define IMU_PWM_Pin GPIO_PIN_6
#define IMU_PWM_GPIO_Port GPIOF
#define DRDY_IST8310_Pin GPIO_PIN_3
#define DRDY_IST8310_GPIO_Port GPIOG
#define DRDY_IST8310_EXTI_IRQn EXTI3_IRQn

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
