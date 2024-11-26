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
#include <stdbool.h>
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
#define LSD_BUF_SIZE	64

typedef void ( *lsd_sys_clk_cfg_t ) ( void );

typedef struct LSD_BUFFER_TAG{
	uint8_t msg[LSD_BUF_SIZE];
	volatile bool busy;
} LSD_BUFFER_T;

typedef struct LSD_MCU_PERIPH_HANDLES_TAG{
	I2C_HandleTypeDef *mcu_hi2c;
	TIM_HandleTypeDef *mcu_htim[3];
	UART_HandleTypeDef *mcu_huart;
	RTC_HandleTypeDef *mcu_hrtc;
} LSD_MCU_PERIPH_HANDLES_T;


typedef struct LSD_TIMESTAMP_TAG{
	uint8_t year		: 8;	// 2xxx
	uint8_t month		: 4;	// up to 12
	uint8_t day			: 5;	// up to 31
	uint8_t hour		: 5;	// up to 23
	uint8_t minute		: 6;	// up to 59
	uint8_t second		: 6;	// up to 59
	uint8_t reserved	: 4;	// reserved for 8-bit alignment
} LSD_TIMESTAMP_T;

typedef struct LSD_LOG_BUFFER_TAG{
	LSD_TIMESTAMP_T timestamp;
	uint16_t temperature_centi_C;
	uint16_t humidity_centi_RH;
	uint16_t speed_of_sound_mps;
	uint16_t distance_mm;
	uint8_t found_errors;
	bool was_transmitted;
} LSD_LOG_BUFFER_T;

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
typedef enum{
	LSD_LOG_ERR_ = 0,
	LSD_LOG_ERR_TIMESTAMP,
} LSD_LOG_ERROR_E;
//MODEM_BUFFER_T modem_rx_buf[MODEM_BUF_FIFO_LENGTH] = {0};
//MODEM_BUFFER_T modem_tx_buf[MODEM_BUF_FIFO_LENGTH] = {0};
//uint8_t modem_rx_fifo_remove = 0;
//uint8_t modem_rx_fifo_insert = 0;
//uint8_t modem_tx_fifo_remove = 0;
//uint8_t modem_tx_fifo_insert = 0;
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
#define US_ECHO_EXTI_IRQn EXTI0_IRQn
#define US_TRIG_Pin GPIO_PIN_1
#define US_TRIG_GPIO_Port GPIOB
#define uLTE_PWR_Pin GPIO_PIN_10
#define uLTE_PWR_GPIO_Port GPIOB
#define uLTE_TX_Pin GPIO_PIN_9
#define uLTE_TX_GPIO_Port GPIOA
#define uLTE_RX_Pin GPIO_PIN_10
#define uLTE_RX_GPIO_Port GPIOA
#define LED_INFO1_Pin GPIO_PIN_11
#define LED_INFO1_GPIO_Port GPIOC
#define LED_INFO2_Pin GPIO_PIN_12
#define LED_INFO2_GPIO_Port GPIOC
#define SENSOR_PWR_Pin GPIO_PIN_4
#define SENSOR_PWR_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
