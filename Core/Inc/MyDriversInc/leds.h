/*
 * leds.h
 *
 *  Created on: Oct 12, 2024
 *      Author: lucas-noce
 */

#ifndef INC_MYDRIVERSINC_LEDS_H_
#define INC_MYDRIVERSINC_LEDS_H_

#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_def.h"
#include "stm32f1xx_hal_tim.h"
#include "MyDriversInc/lsd_config.h"

typedef enum{
	LSD_LED_RED = 0,
	LSD_LED_BLUE,
	LSD_ALL_LEDS,
} LSD_LEDS_E;

int8_t leds_init( TIM_HandleTypeDef *htim );
int8_t leds_turn_on( uint8_t led );
int8_t leds_turn_off( uint8_t led );
int8_t leds_toggle( uint8_t led );
int8_t leds_blink_slow( uint8_t led );
int8_t leds_blink_fast( uint8_t led );
int8_t leds_blink_single( uint8_t led );
void leds_callback_handler( uint8_t led );

#ifdef TEST_MODE_DRIVER_LEDS
int8_t leds_self_test( TIM_HandleTypeDef *htim );
#endif /* TEST_MODE_DRIVER_LEDS */

#endif /* INC_MYDRIVERSINC_LEDS_H_ */
