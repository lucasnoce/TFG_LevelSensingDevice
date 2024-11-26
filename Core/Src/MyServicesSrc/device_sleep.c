/*
 * device_sleep.c
 *
 *  Created on: Nov 2, 2024
 *      Author: lucas
 */

#include <stdint.h>

#include "stm32f1xx_hal.h"

#include "main.h"
//#include "usb_device.h"
#include "MyDriversInc/lsd_errno.h"
#include "MyServicesInc/device_sleep.h"


uint8_t lsd_exit_stop_mode( lsd_sys_clk_cfg_t clk_cfg ){
	if( clk_cfg != NULL ){
		clk_cfg();
//		MX_USB_DEVICE_Init();
		return LSD_OK;
	}

	return LSD_ERR_NULL_HANDLE;
}

void lsd_enter_stop_mode( void ){
    __HAL_RCC_PWR_CLK_ENABLE();

    // Prepare wake-up sources as needed
    // (e.g., configure an EXTI interrupt or RTC alarm here)

    HAL_PWR_EnterSTOPMode( PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI );
}

void lsd_change_sleep_time( RTC_HandleTypeDef *mcu_hrtc, uint8_t hours, uint8_t minutes, uint8_t seconds ){
	if( mcu_hrtc != NULL ){
		RTC_AlarmTypeDef sAlarm = {0};

		sAlarm.AlarmTime.Hours	 = hours;
		sAlarm.AlarmTime.Minutes = minutes;
		sAlarm.AlarmTime.Seconds = seconds;
		sAlarm.Alarm = RTC_ALARM_A;

		if( HAL_RTC_SetAlarm_IT( mcu_hrtc, &sAlarm, RTC_FORMAT_BIN ) != HAL_OK ){
			Error_Handler();
		}
	}

	return;
}