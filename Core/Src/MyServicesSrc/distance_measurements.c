/*
 * distance_measurements.c
 *
 *  Created on: Sep 19, 2024
 *      Author: lucas
 */

#include <math.h>
#include "MyDriversInc/lsd_config.h"
#include "MyDriversInc/lsd_errno.h"

#include "MyServicesInc/distance_measurements.h"

#ifdef PERIPH_USED_AJSR04M
#include "MyDriversInc/ajsr04m.h"
#endif /* PERIPH_USED_AJSR04M */

//#ifdef PERIPH_USED_AHT20
#include "MyDriversInc/aht20.h"
//#endif /* PERIPH_USED_AHT20 */

//#ifdef PERIPH_USED_VL53L0CX
//#include "MyDriversInc/vl53l0cx.h"
//#endif /* PERIPH_USED_VL53L0CX */

static LSD_LOG_DATA_T *p_log_data;

/* ==========================================================================================================
 * Local Function Prototypes
 */

static void calculate_speed_of_sound_Cramer_1993( void );

#ifndef PERIPH_USED_AHT20
static void interpolateSpeedOfSound( LSD_CLIMATE_DATA_T *clim_data, LSD_DISTANCE_DATA_T *dist_data );
#endif /* PERIPH_USED_AHT20 */

static void read_climate_variables( void );
static void calculate_speed_of_sound( void );
static void calculate_distance( void );

/* ==========================================================================================================
 * Global Functions Implementation
 */

void lsd_measure_distance( LSD_LOG_DATA_T *p_log ){
	p_log_data = p_log;

	read_climate_variables();
//	calculate_speed_of_sound();
//	calculate_distance();
}

/* ==========================================================================================================
 * Local Functions Implementation
 */

static void read_climate_variables( void ){
	if( aht20_update() != LSD_OK){
		// todo
	}

//	clim_data->pos_buf = ( clim_data->pos_buf+ 1 ) % LSD_CLIMATE_BUF_SIZE;
	p_log_data->clim.temperature = aht20_get_temperature_C();
	p_log_data->clim.humidity = aht20_get_humidity_RH();

	return;
}

static void calculate_speed_of_sound( void ){
#ifdef PERIPH_USED_AHT20
	calculate_speed_of_sound_Cramer_1993();
#else
	interpolateSpeedOfSound( );
#endif /* PERIPH_USED_AHT20 */
	return;
}

static void calculate_distance( void ){
	int8_t ret;
//	ret = ajsr04m_read_tof_blocking( LSD_STD_TIMEOUT_US );
	ret = ajsr04m_read_tof_exti();

	uint8_t try_count = 10;
	float tof = 0.0;

	do{
		HAL_Delay( 25 );
		tof = ajsr04m_get_tof_ms();
		try_count--;
	}while( tof == LSD_ERR_DEVICE_BUSY && try_count >= 0 );

	p_log_data->dist.distance = (float) p_log_data->dist.speed_of_sound * tof / 2.0;

	return;
}

static void calculate_speed_of_sound_Cramer_1993( void ){
	/*
	 * Calculation based on CRAMER, Owen (1993): "The variation of the specific heat ratio and the
	 * speed of sound in air with temperature, pressure, humidity, and CO2 concentration".
	 */
	float mTemp = p_log_data->clim.temperature;
	float mHumi = p_log_data->clim.humidity;
	float mPres = MEAN_LOCAL_PRESSURE;

	float mTemp_K = mTemp + 273.15;  // Convert to Kelvin
	float mTemp_2 = mTemp * mTemp;
	mPres *= 1000.0;                     // Convert to Pa

	float ENH = ( 3.14 * mPres * pow(10,-8) ) + 1.00062 + ( mTemp_2 * 5.600 * pow(10,-7) );

	float e = 2.71828182845904523536;
	float PSV1 = ( mTemp_K * mTemp_K * 1.2378847 * pow(10,-5) ) - ( 1.9121316 * mTemp_K / 100.0 );
	float PSV2 = 33.93711047 - ( 6343.1645 / mTemp_K );
	float PSV = pow(e,PSV1) * pow(e,PSV2);

	float Xw = ( mHumi * ENH * PSV / mPres ) / 100.0; 	// Mole fraction of water vapor
	float Xc = 0.000400;                           			// Mole fraction of carbon dioxide

	float C1 = ( 0.603055 * mTemp ) + 331.5024 - ( mTemp_2 * 5.28 * pow(10,-4) ) + ( 0.1495874 * mTemp + 51.471935 - ( mTemp_2 * 7.82 * pow(10,-4) ) ) * Xw;
	float C2 = ( (-1.82 * pow(10,-7) ) + ( 3.73 * mTemp * pow(10,-8) ) - ( mTemp_2 * 2.93 * pow(10,-10) ) ) * mPres + ( -85.20931 - 0.228525 * mTemp + ( mTemp_2 * 5.91 * pow(10,-5) ) ) * Xc;
	float C3 = Xw * Xw * 2.835149 - mPres * mPres * 2.15 * pow(10,-13) + Xc * Xc * 29.179762 + 4.86 * pow(10,-4) * Xw * mPres * Xc;

	p_log_data->dist.speed_of_sound = C1 + C2 - C3;

	return;
}


#ifndef PERIPH_USED_AHT20

/*
 * Speed of sound Look Up Table ranging from 10 to 35 °C (), with UR at 10% and 90%:
 */

#define SPEED_SOUND_COLS_TEMP 51
#define SPEED_SOUND_LINS_UR   2

static const float speed_of_sound_LUT[SPEED_SOUND_LINS_UR][SPEED_SOUND_COLS_TEMP] = {
  { 337.52, 337.82, 338.12, 338.42, 338.72, 339.02, 339.32, 339.62, 339.92, 340.22,
	340.51, 340.81, 341.11, 341.41, 341.70, 342.00, 342.30, 342.60, 342.89, 343.19,
	343.48, 343.78, 344.08, 344.37, 344.67, 344.96, 345.26, 345.55, 345.85, 346.14,
	346.44, 346.73, 347.03, 347.32, 347.62, 347.91, 348.21, 348.50, 348.79, 349.09,
	349.38, 349.67, 349.97, 350.26, 350.55, 350.85, 351.14, 351.43, 351.73, 352.02,
	352.31 },
  { 338.04, 338.36, 338.67, 338.99, 339.31, 339.63, 339.95, 340.27, 340.59, 340.91,
	341.24, 341.56, 341.88, 342.20, 342.53, 342.85, 343.18, 343.50, 343.83, 344.16,
	344.49, 344.82, 345.14, 345.47, 345.81, 346.14, 346.47, 346.80, 347.14, 347.47,
	347.81, 348.15, 348.49, 348.83, 349.17, 349.51, 349.85, 350.20, 350.54, 350.89,
	351.23, 351.58, 351.93, 352.29, 352.64, 352.99, 353.35, 353.71, 354.07, 354.43,
	354.79 }
};


static void interpolateSpeedOfSound( void ){
	/*
	 * If the values of temperature and humidity are out of the LUT boundaries, get the closest value
	 * and if the values are inside the boundaries, get the mean value between the two closest values.
	 */
	float speed = 0.0;
	float mTemp = p_log_data->clim.temperature;
	float mHumi = p_log_data->clim.humidity;

	if( mTemp >= 35.0 ){
		if ( mHumi <= 10.0 ){
			speed = speed_of_sound_LUT[0][SPEED_SOUND_COLS_TEMP-1];
		}
		else if( mHumi >= 90.0 ){
			speed = speed_of_sound_LUT[1][SPEED_SOUND_COLS_TEMP-1];
		}
		else{
			speed = speed_of_sound_LUT[0][SPEED_SOUND_COLS_TEMP-1] +
					( ( speed_of_sound_LUT[1][SPEED_SOUND_COLS_TEMP-1] - speed_of_sound_LUT[0][SPEED_SOUND_COLS_TEMP-1] ) * ( mHumi / 100.0 ) );
		}
	}
	else if( mTemp < 10.0 ){
		if( mHumi <= 10.0 ){
			speed = speed_of_sound_LUT[0][0];
		}
		else if( mHumi >= 90.0 ){
			speed = speed_of_sound_LUT[1][0];
		}
		else{
			speed = speed_of_sound_LUT[0][0] +
					( ( speed_of_sound_LUT[1][0] - speed_of_sound_LUT[0][0] ) * ( mHumi / 100.0 ) );
		}
	}
	else{
		for (int i=0; i<SPEED_SOUND_COLS_TEMP; i++){
			if( mTemp >= (float)( 10.0 + ( (float) i / 2.0 ) ) &&
				mTemp < (float) ( 10.5 + ( (float) i / 2.0 ) ) ){
				if (mHumi <= 10.0){
					speed = speed_of_sound_LUT[0][i];
				}
				else if (mHumi >= 90.0){
					speed = speed_of_sound_LUT[1][i];
				}
				else{
					speed = speed_of_sound_LUT[0][i] +
							( ( speed_of_sound_LUT[1][i] - speed_of_sound_LUT[0][i] ) * ( mHumi / 100.0 ) );
				}
				break;
			}
		}
	}

	p_log_data->dist.speed_of_sound = speed;

	return;
}

#endif /* PERIPH_USED_AHT20 */

