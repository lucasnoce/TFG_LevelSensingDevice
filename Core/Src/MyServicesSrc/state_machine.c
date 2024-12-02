/*
 * state_machine.c
 *
 *  Created on: Aug 31, 2024
 *      Author: lucas
 */

/* ==========================================================================================================
 * Includes
 */

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "main.h"

#include "MyDriversInc/lsd_errno.h"
#include "MyDriversInc/lsd_config.h"
#include "MyDriversInc/lsd_common.h"
#include "MyDriversInc/lsd_callbacks.h"

#include "MyDriversInc/leds.h"
#include "MyDriversInc/aht20.h"
#include "MyDriversInc/ajsr04m.h"
#include "MyDriversInc/le910r1br.h"

#include "MyServicesInc/state_machine.h"
#include "MyServicesInc/distance_measurements.h"
#include "MyServicesInc/logs_management.h"
#include "MyServicesInc/data_analysis.h"
#include "MyServicesInc/device_sleep.h"
#include "MyServicesInc/lte_transmission.h"
#include "MyServicesInc/usb_transmission.h"

/* ==========================================================================================================
 * Definitions
 */

#define LSD_SENSORS_ON	GPIO_PIN_RESET
#define LSD_SENSORS_OFF GPIO_PIN_SET

/* ==========================================================================================================
 * Static Variables
 */

extern volatile uint32_t lsd_cycle_count;

static STATES_E previous_state = S0_WAKEUP;  // Store the current state of the state machine
static STATES_E current_state = S0_WAKEUP;  // Store the current state of the state machine

static LSD_MCU_PERIPH_HANDLES_T *hmcu_periph;
static LSD_LOG_DATA_T lsd_log_data;
static lsd_sys_clk_cfg_t lsd_clk_cfg;

/* ==========================================================================================================
 * Local Function Prototypes
 */

static void state_machine_run_s0_wakeup( void );
static void state_machine_run_s1_measure( void );
static void state_machine_run_s2_analyze( void );
static void state_machine_run_s3_transmit( void );
static void state_machine_run_s4_sleep( void );
static void state_machine_run_s5_fail_safe( void );

/* ==========================================================================================================
 * Global Functions Implementation
 */

void state_machine_init( LSD_MCU_PERIPH_HANDLES_T *hmcu, lsd_sys_clk_cfg_t clk_cfg ){
	hmcu_periph = hmcu;
	lsd_clk_cfg = clk_cfg;

	HAL_GPIO_WritePin( SENSOR_PWR_GPIO_Port, SENSOR_PWR_Pin, LSD_SENSORS_OFF );  // turn sensors ON

	int8_t ret = 0;

	ret += lsd_common_init( hmcu_periph->mcu_htim[LSD_TIM_COMMON], hmcu_periph->mcu_hrtc );
	ret += lsd_usb_init( ( uint32_t * ) hmcu_periph->mcu_huart[LSD_UART_FTDI] );
	ret += leds_init( hmcu_periph->mcu_htim[LSD_TIM_GENERAL] );
	ret += aht20_init( hmcu_periph->mcu_hi2c );
	ret += ajsr04m_init( hmcu_periph->mcu_htim[LSD_TIM_SENSORS] );
	ret += le910r1br_init( hmcu_periph->mcu_huart[LSD_UART_MODEM], hmcu_periph->mcu_htim[LSD_TIM_GENERAL] );

	char *msg = "System Initialized\r\n";
	lsd_print_msg_usb( msg, strlen( msg ) );

	HAL_Delay( 1000 );

	return;
}

void state_machine_run( void ){
	switch( current_state ){
		case S0_WAKEUP:
			state_machine_run_s0_wakeup();
			break;

		case S1_MEASURE:
			state_machine_run_s1_measure();
			break;

		case S2_ANALYZE:
			state_machine_run_s2_analyze();
			break;

		case S3_TRANSMIT:
			state_machine_run_s3_transmit();
			break;

		case S4_SLEEP:
			state_machine_run_s4_sleep();
			break;

		case S5_FAIL_SAFE:
			state_machine_run_s5_fail_safe();
			break;

		default:
			current_state = S5_FAIL_SAFE;
			break;
	}
	return;
}

/* ==========================================================================================================
 * Local Functions Implementation
 */

static void state_machine_run_s0_wakeup( void ){
	if( lsd_cycle_count > 0 ){
		lsd_exit_stop_mode( lsd_clk_cfg );
	}

	HAL_Delay( 1000 );
	lsd_cycle_count++;

	char msg_cyle[40] = { 0 };
	snprintf( msg_cyle, 40, "\n********** Cycle %ld **********\n", lsd_cycle_count );
	lsd_print_msg_usb( msg_cyle, strlen( msg_cyle ) );

	char *msg = "S0 - Wakeup\n";
	lsd_print_msg_usb( msg, strlen( msg ) );

	leds_turn_on( LSD_LED_RED );
//	le910r1br_power_on();
	HAL_GPIO_WritePin( SENSOR_PWR_GPIO_Port, SENSOR_PWR_Pin, LSD_SENSORS_ON );
	HAL_Delay( 100 );
	leds_turn_off( LSD_LED_RED );

	previous_state = current_state;
	current_state = S1_MEASURE;
	return;
}

static void state_machine_run_s1_measure( void ){
	char *msg = "S1 - Measuring\n";
	lsd_print_msg_usb( msg, strlen( msg ) );

	leds_turn_on( LSD_LED_BLUE );
	lsd_measure_distance( &lsd_log_data );
	HAL_Delay( 100 );
	leds_turn_off( LSD_LED_BLUE );

	previous_state = current_state;
	current_state = S2_ANALYZE;
	return;
}

static void state_machine_run_s2_analyze( void ){
	char *msg = "S2 - Analyzing data\n";
	lsd_print_msg_usb( msg, strlen( msg ) );

	lsd_analyze_data( &lsd_log_data );
	lsd_log_write( &lsd_log_data );

	/*
	 *  TODO: change next state depending on data, maybe skip a transmission or shorten sleep time.
	 *
	 *  lsd_change_sleep_time( hmcu_periph->mcu_hrtc, 0, 0, 5 );
	 */

	previous_state = current_state;
	current_state = S3_TRANSMIT;
	return;
}

static void state_machine_run_s3_transmit( void ){
	char *msg = "S3 - Transmitting\n";
	lsd_print_msg_usb( msg, strlen( msg ) );

//	lsd_transmit_lte();
//	int8_t ret = le910r1br_check_at();
//	HAL_Delay(500);
//
//	ret = le910r1br_check_cpin();
//	HAL_Delay(500);

	lsd_print_log_data_usb( &lsd_log_data );

	previous_state = current_state;
	current_state = S4_SLEEP;
	return;
}

static void state_machine_run_s4_sleep( void ){
	char *msg = "S4 - Entering sleep\n";
	lsd_print_msg_usb( msg, strlen( msg ) );

	previous_state = current_state;
	current_state = S0_WAKEUP;

	leds_turn_on( LSD_LED_RED );
//	le910r1br_power_off();
	HAL_GPIO_WritePin( SENSOR_PWR_GPIO_Port, SENSOR_PWR_Pin, LSD_SENSORS_OFF );
	HAL_Delay( 100 );
	leds_turn_off( LSD_LED_RED );

	lsd_enter_stop_mode();
	HAL_Delay( 1000 );
	return;
}

static void state_machine_run_s5_fail_safe( void ){
	char *msg = "S5 - Fail safe\n";
	lsd_print_msg_usb( msg, strlen( msg ) );

	if( previous_state != current_state ){  // first time entering S5 state
		leds_blink_fast( LSD_LED_RED );
	}

	/*
	 *	TODO: identify issue, print and try solving it
	 */

	previous_state = current_state;
	current_state = S5_FAIL_SAFE;
	return;
}

