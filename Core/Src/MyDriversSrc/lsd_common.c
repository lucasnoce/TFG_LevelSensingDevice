/*
 * lsd_common.c
 *
 *  Created on: Sep 11, 2024
 *      Author: lucas-noce
 */
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_def.h"
#include "stm32f1xx_hal_tim.h"
#include "main.h"
#include "MyDriversInc/lsd_errno.h"
#include "MyDriversInc/lsd_common.h"

typedef struct LSD_COMMON_DATA_TAG{
	TIM_HandleTypeDef *tim_handle;
//	uint8_t channel;
} LSD_COMMON_DATA_T;

static TIM_HandleTypeDef *p_lsd_htim;
static RTC_HandleTypeDef *p_lsd_hrtc;

/* ==========================================================================================================
 * Global Functions Implementation
 */

int8_t lsd_common_init( TIM_HandleTypeDef *htim, RTC_HandleTypeDef *hrtc ){
	if ( htim != NULL && hrtc != NULL ){
		p_lsd_htim = htim;
		p_lsd_hrtc = hrtc;
//		lsd_common_data.channel = TIM_CHANNEL_3;
//		return HAL_TIM_Base_Start_IT( htim );
		return LSD_OK;
	}
	return LSD_ERR_NULL_HANDLE;
}

void lsd_delay_us( uint32_t time_us ){
	__HAL_TIM_SET_COUNTER( p_lsd_htim, 0 );
	int ret = HAL_TIM_Base_Start( p_lsd_htim );
	if( ret != HAL_OK ){
		return;
	}
	while( __HAL_TIM_GET_COUNTER( p_lsd_htim ) < time_us );
	HAL_TIM_Base_Stop( p_lsd_htim );
	return;
}

void lsd_delay_ms( uint32_t time_ms ){
	for( int i=0; i<time_ms; i++ ){
		lsd_delay_us( 1000 );
	}
	return;
}

void lsd_update_rtc_timestamp( uint8_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second ){
    RTC_DateTypeDef sDate = {0};
    RTC_TimeTypeDef sTime = {0};

    sDate.Year = year;     // e.g., 23 for 2023
    sDate.Month = month;   // 1-12
    sDate.Date = day;      // 1-31

    if(HAL_RTC_SetDate( p_lsd_hrtc, &sDate, RTC_FORMAT_BIN ) != HAL_OK ){
        // LSD_LOG_ERR_TIMESTAMP
    	return;
    }

    sTime.Hours = hour;
    sTime.Minutes = minute;
    sTime.Seconds = second;

    if(HAL_RTC_SetTime( p_lsd_hrtc, &sTime, RTC_FORMAT_BIN ) != HAL_OK ){
    	// LSD_LOG_ERR_TIMESTAMP
    	return;
	}

    return;
}

void lsd_get_rtc_timestamp( LSD_TIMESTAMP_T *timestamp ){
	RTC_TimeTypeDef sTime;
	RTC_DateTypeDef sDate;

	HAL_RTC_GetTime( p_lsd_hrtc, &sTime, RTC_FORMAT_BIN );
	HAL_RTC_GetDate( p_lsd_hrtc, &sDate, RTC_FORMAT_BIN );

	timestamp->year		= sDate.Year;
	timestamp->month	= sDate.Month;
	timestamp->day		= sDate.Date;
	timestamp->hour		= sTime.Hours;
	timestamp->minute	= sTime.Minutes;
	timestamp->second	= sTime.Seconds;

	return;
}
