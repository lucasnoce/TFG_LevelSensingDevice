/*
 * device_sleep.h
 *
 *  Created on: Nov 2, 2024
 *      Author: lucas
 */

#ifndef INC_MYSERVICESINC_DEVICE_SLEEP_H_
#define INC_MYSERVICESINC_DEVICE_SLEEP_H_


uint8_t lsd_exit_stop_mode( lsd_sys_clk_cfg_t clk_cfg );
void lsd_enter_stop_mode( void );
void lsd_change_sleep_time( RTC_HandleTypeDef *mcu_hrtc, uint8_t hours, uint8_t minutes, uint8_t seconds );

#endif /* INC_MYSERVICESINC_DEVICE_SLEEP_H_ */
