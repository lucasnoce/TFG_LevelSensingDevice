/*
 * ajsr04m.h
 *
 *  Created on: Aug 31, 2024
 *      Author: lucas
 */

#ifndef INC_MYDRIVERSINC_AJSR04M_H_
#define INC_MYDRIVERSINC_AJSR04M_H_

#include <stdint.h>
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_def.h"
#include "stm32f1xx_hal_tim.h"
#include "MyDriversInc/lsd_config.h"

#define AJSR04M_TRIG_MIN_DELAY_US	1100	// 1,1 ms

int8_t ajsr04m_init( TIM_HandleTypeDef *htim );
int8_t ajsr04m_read_tof_blocking( uint16_t timeout_us );
int8_t ajsr04m_read_tof_exti( void );
float ajsr04m_get_tof_ms( void );
void ajsr04m_callback_handler_echo_start( void );
void ajsr04m_callback_handler_echo_finish( void );

#ifdef TEST_MODE_DRIVER_AJSR04M
int8_t ajsr04m_self_test( TIM_HandleTypeDef *htim );
#endif /* TEST_MODE_DRIVER_AJSR04M */

#endif /* INC_MYDRIVERSINC_AJSR04M_H_ */
