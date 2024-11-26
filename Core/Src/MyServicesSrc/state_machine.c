/*
 * state_machine.c
 *
 *  Created on: Aug 31, 2024
 *      Author: lucas
 */

/* ==========================================================================================================
 * Includes
 */

#include "main.h"
//#include "usb_device.h"
//#include "usbd_cdc_if.h"

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
#include "MyServicesInc/lte_transmissions.h"

/* ==========================================================================================================
 * Definitions
 */

#define LSD_SENSORS_ON	GPIO_PIN_RESET
#define LSD_SENSORS_OFF GPIO_PIN_SET

/* ==========================================================================================================
 * Static Variables
 */

extern volatile uint32_t lsd_cycle_count;

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

	ret = lsd_common_init( hmcu_periph->mcu_htim[LSD_TIM_COMMON], hmcu_periph->mcu_hrtc );
	ret += leds_init( hmcu_periph->mcu_htim[LSD_TIM_GENERAL] );
	ret += aht20_init( hmcu_periph->mcu_hi2c );
	ret += ajsr04m_init( hmcu_periph->mcu_htim[LSD_TIM_SENSORS] );
	ret += le910r1br_init( hmcu_periph->mcu_huart, hmcu_periph->mcu_htim[LSD_TIM_GENERAL] );

//	uint8_t *msg = "System Initialized\r\n";
//	CDC_Transmit_FS( msg, strlen( (char *) msg ) );

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
	HAL_Delay( 1000 );
	lsd_exit_stop_mode( lsd_clk_cfg );

	lsd_cycle_count++;

	leds_turn_on( LSD_LED_BLUE );

//	le910r1br_power_on();
	HAL_GPIO_WritePin( SENSOR_PWR_GPIO_Port, SENSOR_PWR_Pin, LSD_SENSORS_ON );

	leds_turn_off( LSD_LED_BLUE );

	HAL_Delay( 1000 );
	current_state = S1_MEASURE;
	return;
}

static void state_machine_run_s1_measure( void ){
	leds_turn_on( LSD_ALL_LEDS);
	lsd_measure_distance( &lsd_log_data );
	leds_turn_off( LSD_ALL_LEDS );

	current_state = S1_MEASURE;
	HAL_Delay( 1000 );
	return;
}

static void state_machine_run_s2_analyze( void ){
	lsd_analyze_data( &lsd_log_data );
	lsd_log_write( &lsd_log_data );

	/*
	 * TODO: change next state depending on data, maybe skip a transmission or shorten sleep time
	 */
//	lsd_change_sleep_time( hmcu_periph->mcu_hrtc, 0, 0, 5 );
	current_state = S3_TRANSMIT;
	return;
}

static void state_machine_run_s3_transmit( void ){
//	lsd_transmit_lte();
	int8_t ret = le910r1br_check_at();
	HAL_Delay(500);

	ret = le910r1br_check_cpin();
	HAL_Delay(500);

//	uint8_t msg_size = 64;
//	char msg[msg_size];
//
//	CDC_Transmit_FS( "\n==========\n\n", 13 );
//
//	snprintf( msg, msg_size, "temperature = %.2f [*C]\r\n", lsd_log_data.clim.temperature );
//	CDC_Transmit_FS( (uint8_t *) msg, strlen( msg ) );
//	snprintf( msg, msg_size, "category : %d\r\n\n", lsd_log_data.clim.temperature_category );
//	CDC_Transmit_FS( (uint8_t *) msg, strlen( msg ) );
//
//	snprintf( msg, msg_size, "humidity = %.2f [RH]\r\n", lsd_log_data.clim.humidity );
//	CDC_Transmit_FS( (uint8_t *) msg, strlen( msg ) );
//	snprintf( msg, msg_size, "category : %d\r\n\n", lsd_log_data.clim.humidity_category );
//	CDC_Transmit_FS( (uint8_t *) msg, strlen( msg ) );
//
//	snprintf( msg, msg_size, "distance = %.2f [m]\r\n", lsd_log_data.dist.distance );
//	CDC_Transmit_FS( (uint8_t *) msg, strlen( msg ) );
//	snprintf( msg, msg_size, "category : %d\r\n\n", lsd_log_data.dist.distance_category );
//	CDC_Transmit_FS( (uint8_t *) msg, strlen( msg ) );
//
//	snprintf( msg, msg_size, "speed_of_sound = %.2f [m/s]\r\n", lsd_log_data.dist.speed_of_sound );
//	CDC_Transmit_FS( (uint8_t *) msg, strlen( msg ) );
//	snprintf( msg, msg_size, "category : %d\r\n\n", lsd_log_data.dist.speed_of_sound_category );
//	CDC_Transmit_FS( (uint8_t *) msg, strlen( msg ) );
//
//	snprintf( msg, msg_size, "errors : %c%c%c%c%c%c%c%c\r\n", BYTE_TO_BINARY( lsd_log_data.errors ) );
//	CDC_Transmit_FS( (uint8_t *) msg, strlen( msg ) );
//	snprintf( msg, msg_size, "transmitted : %s\r\n\n", ( lsd_log_data.transmitted ? "true" : "false" ) );
//	CDC_Transmit_FS( (uint8_t *) msg, strlen( msg ) );

	current_state = S3_TRANSMIT;
	return;
}

static void state_machine_run_s4_sleep( void ){
	current_state = S0_WAKEUP;

	leds_turn_on( LSD_LED_BLUE );
	le910r1br_power_off();
	HAL_GPIO_WritePin( SENSOR_PWR_GPIO_Port, SENSOR_PWR_Pin, LSD_SENSORS_OFF );
	leds_turn_off( LSD_LED_BLUE );

//	lsd_enter_stop_mode();
	return;
}

static void state_machine_run_s5_fail_safe( void ){
	current_state = S5_FAIL_SAFE;
	return;
}



//static void state_machine_run_s0_wakeup( void ){
//	// depends on where the program will wakeup (from the beginning or from loop)
//	return;
//}
//
//static void state_machine_run_s1_measure( void ){
////	HAL_StatusTypeDef HAL_I2C_Master_Transmit(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t Timeout)
////	HAL_I2C_Master_Transmit();
//	read_climate_variables();
//	calculate_speed_of_sound();
//	read_time_of_flight();
//	calculate_distance();
//	return;
//}
//
//void state_machine_run_s2_analyze( void ){
//	categoryze_variables();
//	generate_warnings_events();
//	calculate_trend();
//	log_results();
//	return;
//}
//
//static void state_machine_run_s3_transmit( void ){
//	check_connection();
//	encapsulate_data_json();
//	send_data();
//	return;
//}
//
//static void state_machine_run_s4_sleep( void ){
//	turn_off_modem();
//	turn_off_peripherals();
//	set_wakeup_timer_interrupt();
//	enter_sleep_mode();
//	return;
//}
//
//static void state_machine_run_s5_fail_safe( void ){
//	try_to_identify_issue();
//	log_issue_info();
//	try_to_solve_issue();
//	if ( solved ){
//		go_back_to_previous_state();
//		send_issue_event();
//	}
//	else{
//		reset_everything();
//	}
//	return;
//}

