/*
 * distance_measurements.h
 *
 *  Created on: Sep 24, 2024
 *      Author: lucas
 */

#ifndef INC_MYSERVICESINC_DISTANCE_MEASUREMENTS_H_
#define INC_MYSERVICESINC_DISTANCE_MEASUREMENTS_H_

#include <stdint.h>
#include <stdbool.h>

typedef struct LSD_CLIMATE_DATA_TAG{
	float temperature;
	float humidity;
	uint8_t temperature_category;
	uint8_t humidity_category;
} LSD_CLIMATE_DATA_T;

typedef struct LSD_DISTANCE_DATA_TAG{
	float distance;
	float speed_of_sound;
	uint8_t distance_category;
	uint8_t speed_of_sound_category;
} LSD_DISTANCE_DATA_T;

typedef struct LSD_LOG_DATA_TAG{
	LSD_CLIMATE_DATA_T clim;
	LSD_DISTANCE_DATA_T dist;
	uint8_t errors;
	bool transmitted;
} LSD_LOG_DATA_T;

void lsd_measure_distance( LSD_LOG_DATA_T *p_log );


#endif /* INC_MYSERVICESINC_DISTANCE_MEASUREMENTS_H_ */
