/*
 * lsd_common.h
 *
 *  Created on: Sep 11, 2024
 *      Author: lucas-noce
 */

#ifndef INC_MYDRIVERSINC_LSD_COMMON_H_
#define INC_MYDRIVERSINC_LSD_COMMON_H_

#include <stdint.h>
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_def.h"
#include "stm32f1xx_hal_tim.h"
#include "stm32f1xx_hal_rtc.h"
#include "main.h"

/*
 * Conversion functions
 */

#define LSD_SEC_TO_MSEC(x)	(uint32_t) (1000 * (x))
#define LSD_SEC_TO_USEC(x)	(uint32_t) (1000 * 1000 * (x))
#define LSD_MSEC_TO_USEC(x)	(uint32_t) (1000 * (x))

#define BYTE_TO_BINARY(byte)	( ( byte ) & 0x80 ? '1' : '0' ), \
								( ( byte ) & 0x40 ? '1' : '0' ), \
								( ( byte ) & 0x20 ? '1' : '0' ), \
								( ( byte ) & 0x10 ? '1' : '0' ), \
								( ( byte ) & 0x08 ? '1' : '0' ), \
								( ( byte ) & 0x04 ? '1' : '0' ), \
								( ( byte ) & 0x02 ? '1' : '0' ), \
								( ( byte ) & 0x01 ? '1' : '0' )

int8_t lsd_common_init( TIM_HandleTypeDef *htim, RTC_HandleTypeDef *hrtc );
void lsd_delay_us( uint32_t time_us );
void lsd_delay_ms( uint32_t time_ms );
void lsd_update_rtc_timestamp( uint8_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second );
void lsd_get_rtc_timestamp( LSD_TIMESTAMP_T *timestamp );

#endif /* INC_MYDRIVERSINC_LSD_COMMON_H_ */
