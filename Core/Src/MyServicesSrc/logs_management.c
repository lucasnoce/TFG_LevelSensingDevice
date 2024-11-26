/*
 * logs_management.c
 *
 *  Created on: Nov 1, 2024
 *      Author: lucas
 */

/* ==========================================================================================================
 * Includes
 */

#include "main.h"

#include "MyDriversInc/lsd_errno.h"
#include "MyDriversInc/lsd_config.h"
#include "MyDriversInc/lsd_common.h"

#include "MyServicesInc/logs_management.h"

/* ==========================================================================================================
 * Static Variables
 */

extern LSD_LOG_BUFFER_T lsd_log_buffer[LSD_LOG_BUFFER_SIZE];
extern uint8_t lsd_log_buffer_next_pos;
extern uint8_t lsd_log_buffer_first_pos;

static LSD_TIMESTAMP_T log_timestamp;
static LSD_LOG_BUFFER_T log_mngr;

/* ==========================================================================================================
 * Local Function Prototypes
 */

static void log_copy_local_to_global( uint8_t pos );
static void log_copy_global_to_local( uint8_t pos );
static void write_log_values( LSD_LOG_DATA_T *p_log_data );
static int8_t log_values_check( void );
static void update_pos_indexes( void );

/* ==========================================================================================================
 * Global Functions Implementation
 */

void lsd_log_write( LSD_LOG_DATA_T *p_log_data ){
	lsd_get_rtc_timestamp( &log_timestamp );

	uint8_t try_count = 10;
	do{
		write_log_values( p_log_data );
		try_count--;
		HAL_Delay( 10 );
	} while( log_values_check() != LSD_OK && try_count >= 0 );

	update_pos_indexes();
	return;
}

void lsd_log_read_index( int16_t idx, LSD_LOG_DATA_T *p_log_data ){
	if( idx >= 0 ){
		log_copy_global_to_local( idx );
	}
	else{
		log_copy_global_to_local( ( lsd_log_buffer_next_pos + idx ) % LSD_LOG_BUFFER_SIZE );
	}

	p_log_data->clim.temperature	= (float) ( log_mngr.temperature_centi_C / 100.0 );
	p_log_data->clim.humidity		= (float) ( log_mngr.humidity_centi_RH / 100.0 );
	p_log_data->dist.distance		= (float) ( log_mngr.distance_mm / 10.0 );
	p_log_data->errors				= log_mngr.found_errors;
	p_log_data->transmitted			= log_mngr.was_transmitted;

	return;
}

void lsd_log_read_batch( int8_t qty, LSD_LOG_DATA_T **p_log_data ){
	uint8_t pos;

	if( qty >= 0 ){
		pos = lsd_log_buffer_first_pos;	// from oldest to newest
	}
	else{
		pos = lsd_log_buffer_next_pos;	// from newest to oldest
	}

	for( uint8_t i=0; i<qty; i++ ){
		log_copy_global_to_local( ( pos + i ) % LSD_LOG_BUFFER_SIZE );

		p_log_data[i]->clim.temperature	= (float) ( log_mngr.temperature_centi_C / 100.0 );
		p_log_data[i]->clim.humidity	= (float) ( log_mngr.humidity_centi_RH / 100.0 );
		p_log_data[i]->dist.distance	= (float) ( log_mngr.distance_mm / 10.0 );
		p_log_data[i]->errors			= log_mngr.found_errors;
		p_log_data[i]->transmitted		= log_mngr.was_transmitted;
	}

	return;
}

/* ==========================================================================================================
 * Local Functions Implementation
 */

static void log_copy_local_to_global( uint8_t pos ){
	lsd_log_buffer[pos].timestamp.year 		= log_mngr.timestamp.year;
	lsd_log_buffer[pos].timestamp.month 	= log_mngr.timestamp.month;
	lsd_log_buffer[pos].timestamp.day 		= log_mngr.timestamp.day;
	lsd_log_buffer[pos].timestamp.hour 		= log_mngr.timestamp.hour;
	lsd_log_buffer[pos].timestamp.minute 	= log_mngr.timestamp.minute;
	lsd_log_buffer[pos].timestamp.second 	= log_mngr.timestamp.second;
	lsd_log_buffer[pos].timestamp.reserved	= log_mngr.timestamp.reserved;

	lsd_log_buffer[pos].temperature_centi_C	= log_mngr.temperature_centi_C;
	lsd_log_buffer[pos].humidity_centi_RH	= log_mngr.humidity_centi_RH;
	lsd_log_buffer[pos].speed_of_sound_mps 	= log_mngr.speed_of_sound_mps;
	lsd_log_buffer[pos].distance_mm 		= log_mngr.distance_mm;		// mm to cm
	lsd_log_buffer[pos].found_errors 		= log_mngr.found_errors;
	lsd_log_buffer[pos].was_transmitted 	= log_mngr.was_transmitted;

	return;
}

static void log_copy_global_to_local( uint8_t pos ){
	log_mngr.timestamp.year 		= lsd_log_buffer[pos].timestamp.year;
	log_mngr.timestamp.month 		= lsd_log_buffer[pos].timestamp.month;
	log_mngr.timestamp.day 			= lsd_log_buffer[pos].timestamp.day;
	log_mngr.timestamp.hour 		= lsd_log_buffer[pos].timestamp.hour;
	log_mngr.timestamp.minute 		= lsd_log_buffer[pos].timestamp.minute;
	log_mngr.timestamp.second 		= lsd_log_buffer[pos].timestamp.second;
	log_mngr.timestamp.reserved		= lsd_log_buffer[pos].timestamp.reserved;

	log_mngr.temperature_centi_C	= lsd_log_buffer[pos].temperature_centi_C;
	log_mngr.humidity_centi_RH		= lsd_log_buffer[pos].humidity_centi_RH;
	log_mngr.speed_of_sound_mps 	= lsd_log_buffer[pos].speed_of_sound_mps;
	log_mngr.distance_mm 			= lsd_log_buffer[pos].distance_mm;		// mm to cm
	log_mngr.found_errors 			= lsd_log_buffer[pos].found_errors;
	log_mngr.was_transmitted 		= lsd_log_buffer[pos].was_transmitted;

	return;
}

static void write_log_values( LSD_LOG_DATA_T *p_log_data ){
	// Update local log buffer:
	log_mngr.timestamp.year 		= log_timestamp.year;
	log_mngr.timestamp.month 		= log_timestamp.month;
	log_mngr.timestamp.day 			= log_timestamp.day;
	log_mngr.timestamp.hour 		= log_timestamp.hour;
	log_mngr.timestamp.minute 		= log_timestamp.minute;
	log_mngr.timestamp.second 		= log_timestamp.second;
	log_mngr.timestamp.reserved		= log_timestamp.reserved;
	log_mngr.temperature_centi_C	= (uint16_t) ( 100 * p_log_data->clim.temperature);
	log_mngr.humidity_centi_RH		= (uint16_t) ( 100 * p_log_data->clim.humidity );
	log_mngr.speed_of_sound_mps 	= (uint16_t) ( 100 * p_log_data->dist.speed_of_sound );
	log_mngr.distance_mm 			= (uint16_t) ( 10  * p_log_data->dist.distance );		// cm to mm
	log_mngr.found_errors 			= p_log_data->errors;
	log_mngr.was_transmitted 		= p_log_data->transmitted;

	// Update global log buffer:
	lsd_log_buffer[lsd_log_buffer_next_pos].timestamp.year 			= log_timestamp.year;
	lsd_log_buffer[lsd_log_buffer_next_pos].timestamp.month 		= log_timestamp.month;
	lsd_log_buffer[lsd_log_buffer_next_pos].timestamp.day 			= log_timestamp.day;
	lsd_log_buffer[lsd_log_buffer_next_pos].timestamp.hour 			= log_timestamp.hour;
	lsd_log_buffer[lsd_log_buffer_next_pos].timestamp.minute 		= log_timestamp.minute;
	lsd_log_buffer[lsd_log_buffer_next_pos].timestamp.second 		= log_timestamp.second;
	lsd_log_buffer[lsd_log_buffer_next_pos].timestamp.reserved		= log_timestamp.reserved;

	lsd_log_buffer[lsd_log_buffer_next_pos].temperature_centi_C = (uint16_t) ( 100 * p_log_data->clim.temperature);
	lsd_log_buffer[lsd_log_buffer_next_pos].humidity_centi_RH 	= (uint16_t) ( 100 * p_log_data->clim.humidity );
	lsd_log_buffer[lsd_log_buffer_next_pos].speed_of_sound_mps 	= (uint16_t) ( 100 * p_log_data->dist.speed_of_sound );
	lsd_log_buffer[lsd_log_buffer_next_pos].distance_mm 		= (uint16_t) ( 10  * p_log_data->dist.distance );		// cm to mm
	lsd_log_buffer[lsd_log_buffer_next_pos].found_errors 		= p_log_data->errors;
	lsd_log_buffer[lsd_log_buffer_next_pos].was_transmitted 	= p_log_data->transmitted;

	/*
	 * TODO: save on flash memory
	 */

	return;
}

static int8_t log_values_check( void ){
	uint8_t *p_global = (uint8_t *) &lsd_log_buffer[lsd_log_buffer_next_pos];
	uint8_t *p_local = (uint8_t *) &log_mngr;

	for( uint8_t i=0; i<sizeof(LSD_LOG_BUFFER_T); i++ ){
		if( *p_global != *p_local ){
			return LSD_ERR_CRC_CHECK;
		}
		p_global++;
		p_local++;
	}

	return LSD_OK;
}

static void update_pos_indexes( void ){
	lsd_log_buffer_next_pos = ( lsd_log_buffer_next_pos + 1 ) % LSD_LOG_BUFFER_SIZE;

	if( lsd_log_buffer_next_pos == lsd_log_buffer_first_pos ){
		lsd_log_buffer_first_pos = ( lsd_log_buffer_first_pos + 1 ) % LSD_LOG_BUFFER_SIZE;
	}

	return;
}
