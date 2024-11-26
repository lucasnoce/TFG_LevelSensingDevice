/*
 * aht20.h
 *
 *  Created on: Aug 31, 2024
 *      Author: lucas
 */

#ifndef INC_MYDRIVERSINC_AHT20_H_
#define INC_MYDRIVERSINC_AHT20_H_

#include <stdint.h>
#include <stdbool.h>
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_def.h"
#include "stm32f1xx_hal_i2c.h"
#include "MyDriversInc/lsd_config.h"

int8_t aht20_init( I2C_HandleTypeDef *hi2c );
int8_t aht20_soft_reset( void );
int8_t aht20_update( void );
uint8_t aht20_get_status( void );
bool aht20_is_busy( void );
uint8_t aht20_get_mode( void );
bool aht20_is_calibrated( void );
float aht20_get_temperature_C( void );
float aht20_get_humidity_RH( void );

#ifdef TEST_MODE_DRIVER_AHT20
int8_t aht20_self_test( I2C_HandleTypeDef *hi2c );
#endif /* TEST_MODE_DRIVER_AHT20 */

#endif /* INC_MYDRIVERSINC_AHT20_H_ */
