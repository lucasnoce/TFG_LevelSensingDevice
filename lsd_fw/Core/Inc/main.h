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
#include "stm32f1xx_hal.h"

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
#define US_ECHO_Pin GPIO_PIN_0
#define US_ECHO_GPIO_Port GPIOB
#define US_TRIG_Pin GPIO_PIN_1
#define US_TRIG_GPIO_Port GPIOB
#define uLTE_PWR_Pin GPIO_PIN_10
#define uLTE_PWR_GPIO_Port GPIOB
#define LED_INFO1_Pin GPIO_PIN_11
#define LED_INFO1_GPIO_Port GPIOC
#define LED_INFO2_Pin GPIO_PIN_12
#define LED_INFO2_GPIO_Port GPIOC
#define SENSOR_PWR_Pin GPIO_PIN_4
#define SENSOR_PWR_GPIO_Port GPIOB
#define TOF_GPIO1_Pin GPIO_PIN_9
#define TOF_GPIO1_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
