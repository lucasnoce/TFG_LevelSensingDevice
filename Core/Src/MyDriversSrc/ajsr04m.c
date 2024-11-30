/*
 * ajsr04m.c
 *
 *  Created on: Sep 5, 2024
 *      Author: lucas-noce
 */

#include <MyDriversInc/lsd_common.h>
#include <MyDriversInc/lsd_errno.h>
#include <stdint.h>
#include <stdbool.h>
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_def.h"
#include "stm32f1xx_hal_tim.h"
#include "main.h"
#include "MyDriversInc/ajsr04m.h"

TIM_HandleTypeDef *p_ajsr04m_htim;

typedef struct AJSR04M_DATA_TAG{
	GPIO_TypeDef *port;			// GPIO Port
	uint16_t pin_echo;			// GPIO Pin
	uint16_t pin_trig;			// GPIO Pin
	volatile uint_fast32_t tof_ticks;			// Timer count from trigger until echo
	volatile bool measuring;				// Flag to indicate ongoing measurement
	float tof;					// Time Of Flight (in ns)
} AJSR04M_DATA_T;

static AJSR04M_DATA_T ajsr04m_data;

/* ==========================================================================================================
 * Local Function Prototypes
 */

static void ajsr04m_trigger_measurement( void );

/* ==========================================================================================================
 * Global Functions Implementation
 */

int8_t ajsr04m_init( TIM_HandleTypeDef *htim ){
	if ( htim != NULL ){
		p_ajsr04m_htim = htim;

		ajsr04m_data.port = US_ECHO_GPIO_Port;
		ajsr04m_data.pin_echo = US_ECHO_Pin;
		ajsr04m_data.pin_trig = US_TRIG_Pin;
		ajsr04m_data.tof_ticks = 0;
		ajsr04m_data.measuring = false;
		ajsr04m_data.tof = 0.0;

		return HAL_TIM_Base_Start( p_ajsr04m_htim );
	}
	return LSD_ERR_NULL_HANDLE;
}

int8_t ajsr04m_read_tof_blocking( uint16_t timeout_us ){
	ajsr04m_trigger_measurement();

	ajsr04m_data.tof_ticks = 0;
	ajsr04m_data.measuring = true;
	while( HAL_GPIO_ReadPin( ajsr04m_data.port, ajsr04m_data.pin_echo ) != GPIO_PIN_SET ){
		ajsr04m_data.tof_ticks = __HAL_TIM_GET_COUNTER( p_ajsr04m_htim );
		if( ajsr04m_data.tof_ticks > timeout_us )
		{
			HAL_TIM_Base_Stop( p_ajsr04m_htim );
			ajsr04m_data.tof = 0.0;
			ajsr04m_data.measuring = false;
			return LSD_ERR_TIMEOUT;
		}
	}

//	HAL_TIM_Base_Stop( p_ajsr04m_htim );
	ajsr04m_data.measuring = false;

	return LSD_OK;
}

int8_t ajsr04m_read_tof_exti( void ){
	ajsr04m_trigger_measurement();
	return LSD_OK;
}
//static volatile int tim_flag;
float ajsr04m_get_tof_ms( void ){
//	HAL_TIM_Base_Stop( p_ajsr04m_htim );
//	__HAL_TIM_SET_COUNTER( p_ajsr04m_htim, 0 );

	if( ajsr04m_data.measuring == false ){
//		int ret = tim_flag ;
		ajsr04m_data.tof = (float) ajsr04m_data.tof_ticks / 10000.0;		// translates to miliseconds
		return ajsr04m_data.tof;
	}
	return LSD_ERR_DEVICE_BUSY;
}

void ajsr04m_callback_handler_echo_start( void ){
//	HAL_TIM_Base_Start( p_ajsr04m_htim );
	__HAL_TIM_SET_COUNTER( p_ajsr04m_htim, 0 );
//	__HAL_TIM_CLEAR_FLAG(p_ajsr04m_htim, TIM_FLAG_UPDATE);
	ajsr04m_data.tof_ticks = 0;
	ajsr04m_data.measuring = true;
	return;
}

void ajsr04m_callback_handler_echo_finish( void ){
	ajsr04m_data.tof_ticks = __HAL_TIM_GET_COUNTER( p_ajsr04m_htim );
	ajsr04m_data.measuring = false;
//	tim_flag = __HAL_TIM_GET_FLAG(p_ajsr04m_htim, TIM_FLAG_UPDATE);
//	HAL_TIM_Base_Stop( p_ajsr04m_htim );
	return;
}

#ifdef TEST_MODE_DRIVER_AJSR04M
int8_t ajsr04m_self_test( TIM_HandleTypeDef *htim ){
	int ret = 0;

	ret = ajsr04m_init( htim );
	if( ret != LSD_OK ){
		// todo: print error
		return ret;
	}

	uint32_t tof = 0;
	ret = ajsr04m_read_tof_blocking( &tof, LSD_SEC_TO_USEC(1) );
	if( ret != LSD_OK ){
		// todo: print error
		return ret;
	}
	// todo: print results

	return LSD_OK;
}
#endif /* TEST_MODE_DRIVER_AJSR04M */

/* ==========================================================================================================
 * Local Functions Implementation
 */

static void ajsr04m_trigger_measurement( void ){
	HAL_GPIO_WritePin( ajsr04m_data.port, ajsr04m_data.pin_trig, GPIO_PIN_SET );

//	HAL_TIM_Base_Start( p_ajsr04m_htim );
//	__HAL_TIM_SET_COUNTER( p_ajsr04m_htim, 0 );
	lsd_delay_us( AJSR04M_TRIG_MIN_DELAY_US );

	HAL_GPIO_WritePin( ajsr04m_data.port, ajsr04m_data.pin_trig, GPIO_PIN_RESET );
	return;
}
