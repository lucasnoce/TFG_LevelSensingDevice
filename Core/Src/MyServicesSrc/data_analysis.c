/*
 * data_analysis.c
 *
 *  Created on: Nov 2, 2024
 *      Author: lucas
 */

/* ==========================================================================================================
 * Includes
 */

#include "MyServicesInc/logs_management.h"
#include "MyServicesInc/data_analysis.h"

/* ==========================================================================================================
 * Definitions
 */

//#define LSD_RECENT_LOG_SIZE	10

/* ==========================================================================================================
 * Static Variables
 */

static LSD_TRHES_T lsd_thres[LSD_ALL_THRES];
static LSD_LOG_DATA_T *p_recent_log_data;

/* ==========================================================================================================
 * Local Function Prototypes
 */

static void update_thresold_values( void );
static void categoryze_recent_data( void );
static uint8_t compare_threshold( float value, LSD_TRHES_T *thres );

/* ==========================================================================================================
 * Global Functions Implementation
 */

void lsd_analyze_data( LSD_LOG_DATA_T *p_log_data ){
	p_recent_log_data = p_log_data;
	update_thresold_values();
//	lsd_log_read_batch( -LSD_RECENT_LOG_SIZE, p_recent_log_data );
	categoryze_recent_data();
	return;
}

/* ==========================================================================================================
 * Local Functions Implementation
 */

static void update_thresold_values( void ){
	lsd_thres[LSD_THRES_CLIM_TEMP].very_low		= LSD_THRES_CLIM_TEMP_VL;
	lsd_thres[LSD_THRES_CLIM_TEMP].low			= LSD_THRES_CLIM_TEMP_L;
	lsd_thres[LSD_THRES_CLIM_TEMP].high			= LSD_THRES_CLIM_TEMP_H;
	lsd_thres[LSD_THRES_CLIM_TEMP].very_high	= LSD_THRES_CLIM_TEMP_VH;

	lsd_thres[LSD_THRES_CLIM_HUMI].very_low		= LSD_THRES_CLIM_HUMI_VL;
	lsd_thres[LSD_THRES_CLIM_HUMI].low			= LSD_THRES_CLIM_HUMI_L;
	lsd_thres[LSD_THRES_CLIM_HUMI].high			= LSD_THRES_CLIM_HUMI_H;
	lsd_thres[LSD_THRES_CLIM_HUMI].very_high	= LSD_THRES_CLIM_HUMI_VH;

	lsd_thres[LSD_THRES_DIST_DIST].very_low		= LSD_THRES_DIST_DIST_VL;
	lsd_thres[LSD_THRES_DIST_DIST].low			= LSD_THRES_DIST_DIST_L;
	lsd_thres[LSD_THRES_DIST_DIST].high			= LSD_THRES_DIST_DIST_H;
	lsd_thres[LSD_THRES_DIST_DIST].very_high	= LSD_THRES_DIST_DIST_VH;

	lsd_thres[LSD_THRES_DIST_SPEED].very_low	= LSD_THRES_DIST_SPEED_VL;
	lsd_thres[LSD_THRES_DIST_SPEED].low			= LSD_THRES_DIST_SPEED_L;
	lsd_thres[LSD_THRES_DIST_SPEED].high		= LSD_THRES_DIST_SPEED_H;
	lsd_thres[LSD_THRES_DIST_SPEED].very_high	= LSD_THRES_DIST_SPEED_VH;
	return;
}

static void categoryze_recent_data( void ){
	p_recent_log_data->clim.temperature_category = \
		compare_threshold( p_recent_log_data->clim.temperature, &lsd_thres[LSD_THRES_CLIM_TEMP] );

	p_recent_log_data->clim.humidity_category = \
		compare_threshold( p_recent_log_data->clim.humidity, &lsd_thres[LSD_THRES_CLIM_HUMI] );

	p_recent_log_data->dist.distance_category = \
		compare_threshold( p_recent_log_data->dist.distance, &lsd_thres[LSD_THRES_DIST_DIST] );

	p_recent_log_data->dist.speed_of_sound_category = \
		compare_threshold( p_recent_log_data->dist.speed_of_sound, &lsd_thres[LSD_THRES_DIST_SPEED] );

	return;
}

static uint8_t compare_threshold( float value, LSD_TRHES_T *thres ){
	if( value <= thres->very_low ){
		return LSD_THRES_BELOW_VERY_LOW;
	}
	else if( value <= thres->low ){
		return LSD_THRES_BELOW_LOW;
	}
	else if( value >= thres->very_high ){
		return LSD_THRES_ABOVE_VERY_HIGH;
	}
	else if( value >= thres->high ){
		return LSD_THRES_ABOVE_HIGH;
	}
	else{
		return LSD_THRES_CENTER;
	}
}
