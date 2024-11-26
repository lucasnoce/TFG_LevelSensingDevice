/*
 * lsd_callbacks.c
 *
 *  Created on: Sep 5, 2024
 *      Author: lucas-noce
 */

#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_def.h"
//#include "stm32f1xx_hal_i2c.h"
#include "stm32f1xx_hal_tim.h"
#include "MyDriversInc/leds.h"
#include "MyDriversInc/ajsr04m.h"
#include "MyDriversInc/le910r1br.h"
#include "MyDriversInc/lsd_callbacks.h"
#include "main.h"

extern LSD_BUFFER_T modem_rx_buffer;
extern LSD_BUFFER_T modem_tx_buffer;

//void HAL_I2C_MasterTxCpltCallback ( I2C_HandleTypeDef *hi2c );
//void HAL_I2C_MasterRxCpltCallback ( I2C_HandleTypeDef *hi2c );
//void HAL_I2C_ErrorCallback ( I2C_HandleTypeDef *hi2c );


void HAL_TIM_OC_DelayElapsedCallback( TIM_HandleTypeDef *htim ){
    if( htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1 ){
    	leds_callback_handler( LSD_LED_BLUE );
    }
    else if( htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2 ){
    	leds_callback_handler( LSD_LED_RED );
    }
//    else if( htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3 ){
//    	le910r1br_turn_on_callback_handle();
//    }
    return;
}

void HAL_GPIO_EXTI_Callback( uint16_t GPIO_Pin ){
	if( GPIO_Pin == US_ECHO_Pin ){
		if( HAL_GPIO_ReadPin( US_ECHO_GPIO_Port, US_ECHO_Pin ) == GPIO_PIN_SET ){
			ajsr04m_callback_handler_echo_start();
		}
		else{
			ajsr04m_callback_handler_echo_finish();
		}
	}
	return;
}

void HAL_UART_RxCpltCallback( UART_HandleTypeDef *huart ) {
	if( huart->Instance == USART1 ){
//		le910r1br_rx_handler();
//		modem_rx_buffer.busy = false;
	}
}

void HAL_UART_TxCpltCallback( UART_HandleTypeDef *huart ) {
	if( huart->Instance == USART1 ){
//		le910r1br_rx_handler();
		modem_tx_buffer.busy = false;
	}
}
