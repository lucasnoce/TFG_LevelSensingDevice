/*
 * leds.c
 *
 *  Created on: Oct 12, 2024
 *      Author: lucas-noce
 */

/* ==========================================================================================================
 * Includes
 */

#include <stdbool.h>
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_def.h"
#include "stm32f1xx_hal_tim.h"
#include "stm32f1xx_hal_gpio.h"
#include "main.h"
#include "MyDriversInc/lsd_errno.h"
#include "MyDriversInc/lsd_common.h"
#include "MyDriversInc/leds.h"

/* ==========================================================================================================
 * Definitions
 */

#define LEDS_SLOW_HALF_PERIOD_MS	(uint16_t) 500
#define LEDS_FAST_HALF_PERIOD_MS	(uint16_t) 100

/* ==========================================================================================================
 * Typedefs
 */

typedef struct LEDS_DATA_TAG{
	uint8_t channel;				// Timer channel for blink interrupts
	GPIO_TypeDef *port;				// GPIO Port
	uint16_t pin;					// GPIO Pin
	uint8_t state;					// Current state (on or off)
	uint16_t blink_half_period;		// Period of generating interruption to toggle the LED
	bool single_blink;				// Flag to indicate if single blink or if blinks indefinitely
} LEDS_DATA_T;

/* ==========================================================================================================
 * Static Variables
 */

static TIM_HandleTypeDef *p_leds_htim;
static LEDS_DATA_T leds_data[LSD_ALL_LEDS];

/* ==========================================================================================================
 * Local Function Prototypes
 */

static inline void leds_write_value( uint8_t led, uint8_t value );
static inline void leds_timer_set_compare_value( uint8_t led );

/* ==========================================================================================================
 * Global Functions Implementation
 */

int8_t leds_init( TIM_HandleTypeDef *htim ){
	if( htim != NULL ){
		p_leds_htim = htim;

		leds_data[LSD_LED_BLUE].channel = TIM_CHANNEL_1;
		leds_data[LSD_LED_RED].channel = TIM_CHANNEL_2;

		leds_data[LSD_LED_BLUE].pin = LED_INFO1_Pin;
		leds_data[LSD_LED_RED].pin = LED_INFO2_Pin;

		leds_data[LSD_LED_BLUE].port = LED_INFO1_GPIO_Port;
		leds_data[LSD_LED_RED].port = LED_INFO2_GPIO_Port;

		leds_data[LSD_LED_BLUE].state = GPIO_PIN_SET;
		leds_data[LSD_LED_RED].state = GPIO_PIN_SET;

		leds_data[LSD_LED_BLUE].blink_half_period = 0;
		leds_data[LSD_LED_RED].blink_half_period = 0;

		leds_data[LSD_LED_BLUE].single_blink = false;
		leds_data[LSD_LED_RED].single_blink = false;

//		HAL_GPIO_WritePin( LED_INFO1_GPIO_Port, LED_INFO1_Pin, GPIO_PIN_RESET );
//		HAL_GPIO_WritePin( LED_INFO2_GPIO_Port, LED_INFO2_Pin, GPIO_PIN_RESET );

		int8_t ret = HAL_OK;
		ret = HAL_TIM_Base_Start_IT( htim );
		for( int i=0; i<LSD_ALL_LEDS; i++ ){
			ret += HAL_TIM_OC_Stop_IT( p_leds_htim, leds_data[i].channel );
		}

		return ret;
	}
	return LSD_ERR_NULL_HANDLE;
}

int8_t leds_turn_on( uint8_t led ){
	int8_t ret = LSD_OK;
	if( led < 0 || led > LSD_ALL_LEDS ){
		return LSD_ERR_WRONG_PARAM;
	}
	else if( led < LSD_ALL_LEDS ){
		HAL_TIM_OC_Stop_IT( p_leds_htim, leds_data[led].channel );
		leds_data[led].state = GPIO_PIN_SET;
		leds_write_value( led, GPIO_PIN_SET );
	}
	else{
		for( int i=0; i<LSD_ALL_LEDS; i++ ){
			ret += HAL_TIM_OC_Stop_IT( p_leds_htim, leds_data[i].channel );
			leds_data[i].state = GPIO_PIN_SET;
			leds_write_value( i, GPIO_PIN_SET );
		}
	}
	return ret;
}

int8_t leds_turn_off( uint8_t led ){
	if( led < 0 || led > LSD_ALL_LEDS ){
		return LSD_ERR_WRONG_PARAM;
	}
	else if( led < LSD_ALL_LEDS ){
		HAL_TIM_OC_Stop_IT( p_leds_htim, leds_data[led].channel );
		leds_data[led].state = GPIO_PIN_RESET;
		leds_write_value( led, GPIO_PIN_RESET );
	}
	else{
		for( int i=0; i<LSD_ALL_LEDS; i++ ){
			HAL_TIM_OC_Stop_IT( p_leds_htim, leds_data[i].channel );
			leds_data[i].state = GPIO_PIN_RESET;
			leds_write_value( i, GPIO_PIN_RESET );
		}
	}
	return LSD_OK;
}

int8_t leds_toggle( uint8_t led ){
	if( led < 0 || led > LSD_ALL_LEDS ){
		return LSD_ERR_WRONG_PARAM;
	}
	else if( led < LSD_ALL_LEDS ){
		leds_data[led].state = !leds_data[led].state;
		leds_write_value( led, leds_data[led].state );
	}
	else{
		for( int i=0; i<LSD_ALL_LEDS; i++ ){
			leds_data[i].state = !leds_data[i].state;
			leds_write_value( i, leds_data[i].state );
		}
	}
	return LSD_OK;
}

int8_t leds_blink_slow( uint8_t led ){
	if( led < 0 || led > LSD_ALL_LEDS ){
		return LSD_ERR_WRONG_PARAM;
	}
	else if( led < LSD_ALL_LEDS ){
		leds_data[led].blink_half_period = LEDS_SLOW_HALF_PERIOD_MS;
		leds_data[led].single_blink = false;
		leds_data[led].state = GPIO_PIN_RESET;
		leds_write_value( led, GPIO_PIN_RESET );
		HAL_TIM_OC_Start_IT( p_leds_htim, leds_data[led].channel );
		leds_timer_set_compare_value( led );
	}
	else{
		for( int i=0; i<LSD_ALL_LEDS; i++ ){
			leds_data[i].blink_half_period = LEDS_SLOW_HALF_PERIOD_MS;
			leds_data[i].single_blink = false;
			leds_data[i].state = GPIO_PIN_RESET;
			leds_write_value( i, GPIO_PIN_RESET );
			HAL_TIM_OC_Start_IT( p_leds_htim, leds_data[i].channel );
			leds_timer_set_compare_value( i );
		}
	}
	return LSD_OK;
}

int8_t leds_blink_fast( uint8_t led ){
	if( led < 0 || led > LSD_ALL_LEDS ){
		return LSD_ERR_WRONG_PARAM;
	}
	else if( led < LSD_ALL_LEDS ){
		leds_data[led].blink_half_period = LEDS_FAST_HALF_PERIOD_MS;
		leds_data[led].single_blink = false;
		leds_data[led].state = GPIO_PIN_RESET;
		leds_write_value( led, GPIO_PIN_RESET );
		HAL_TIM_OC_Start_IT( p_leds_htim, leds_data[led].channel );
		leds_timer_set_compare_value( led );
	}
	else{
		for( int i=0; i<LSD_ALL_LEDS; i++ ){
			leds_data[i].blink_half_period = LEDS_FAST_HALF_PERIOD_MS;
			leds_data[i].single_blink = false;
			leds_data[i].state = GPIO_PIN_RESET;
			leds_write_value( i, GPIO_PIN_RESET );
			HAL_TIM_OC_Start_IT( p_leds_htim, leds_data[i].channel );
			leds_timer_set_compare_value( i );
		}
	}
	return LSD_OK;
}

int8_t leds_blink_single( uint8_t led ){
	if( led < 0 || led > LSD_ALL_LEDS ){
		return LSD_ERR_WRONG_PARAM;
	}
	else if( led < LSD_ALL_LEDS ){
		leds_data[led].blink_half_period = LEDS_FAST_HALF_PERIOD_MS;
		leds_data[led].single_blink = true;
		leds_data[led].state = !leds_data[led].state;
		leds_write_value( led, leds_data[led].state );
		HAL_TIM_OC_Start_IT( p_leds_htim, leds_data[led].channel );
		leds_timer_set_compare_value( led );
	}
	else{
		for( int i=0; i<LSD_ALL_LEDS; i++ ){
			leds_data[i].blink_half_period = LEDS_FAST_HALF_PERIOD_MS;
			leds_data[i].single_blink = true;
			leds_data[i].state = !leds_data[i].state;
			leds_write_value( i, leds_data[i].state );
			HAL_TIM_OC_Start_IT( p_leds_htim, leds_data[i].channel );
			leds_timer_set_compare_value( i );
		}
	}
	return LSD_OK;
}

void leds_callback_handler( uint8_t led ){
	leds_data[led].state = ! leds_data[led].state;
	leds_write_value( led, leds_data[led].state );

	if( leds_data[led].single_blink == true ){
		HAL_TIM_OC_Stop_IT( p_leds_htim, leds_data[led].channel );
	}
	else{
		leds_timer_set_compare_value( led );
	}

	return;
}

#ifdef TEST_MODE_DRIVER_LEDS
int8_t leds_self_test( TIM_HandleTypeDef *htim ){
	int ret = 0;

	ret = leds_init( htim );
	if( ret != LSD_OK ){
		// todo: print error
		return ret;
	}

	ret = leds_turn_on( LSD_ALL_LEDS );
	if( ret != LSD_OK ){
		// todo: print error
		return ret;
	}

	lsd_delay_us( LSD_SEC_TO_USEC(2) );

	ret = leds_turn_off( LSD_ALL_LEDS );
	if( ret != LSD_OK ){
		// todo: print error
		return ret;
	}

	lsd_delay_us( LSD_SEC_TO_USEC(2) );

	ret = leds_blink_slow( LSD_LED_BLUE );
	if( ret != LSD_OK ){
		// todo: print error
		return ret;
	}

	ret = leds_blink_fast( LSD_LED_RED );
	if( ret != LSD_OK ){
		// todo: print error
		return ret;
	}

	lsd_delay_us( LSD_SEC_TO_USEC(2) );

	ret = leds_toggle( LSD_LED_BLUE );
	if( ret != LSD_OK ){
		// todo: print error
		return ret;
	}

	return LSD_OK;
}
#endif /* TEST_MODE_DRIVER_LEDS */

/* ==========================================================================================================
 * Local Functions Implementation
 */

static inline void leds_write_value( uint8_t led, uint8_t value ){
	HAL_GPIO_WritePin( leds_data[led].port, leds_data[led].pin, value );
	return;
}

static inline void leds_timer_set_compare_value( uint8_t led ){
	uint32_t current_count = __HAL_TIM_GET_COUNTER( p_leds_htim );
	uint32_t new_count = current_count + (uint32_t) leds_data[led].blink_half_period;
	__HAL_TIM_SET_COMPARE( p_leds_htim, leds_data[led].channel, new_count );
	return;
}

