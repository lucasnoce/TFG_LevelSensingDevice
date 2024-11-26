/*
 * data_analysis.h
 *
 *  Created on: Nov 2, 2024
 *      Author: lucas
 */

#ifndef INC_MYSERVICESINC_DATA_ANALYSIS_H_
#define INC_MYSERVICESINC_DATA_ANALYSIS_H_

/* ==========================================================================================================
 * Includes
 */

#include "MyServicesInc/distance_measurements.h"

/* ==========================================================================================================
 * Definitions
 */

#define LSD_THRES_CLIM_TEMP_VL	(float) 10.0	// [*C]
#define LSD_THRES_CLIM_TEMP_L	(float) 15.0	// [*C]
#define LSD_THRES_CLIM_TEMP_H	(float) 30.0	// [*C]
#define LSD_THRES_CLIM_TEMP_VH	(float) 35.0	// [*C]

#define LSD_THRES_CLIM_HUMI_VL	(float) 20.0	// [RH]
#define LSD_THRES_CLIM_HUMI_L	(float) 30.0	// [RH]
#define LSD_THRES_CLIM_HUMI_H	(float) 80.0	// [RH]
#define LSD_THRES_CLIM_HUMI_VH	(float) 90.0	// [RH]

#define LSD_THRES_DIST_DIST_VL	(float) 10.0	// [cm]
#define LSD_THRES_DIST_DIST_L	(float) 30.0	// [cm]
#define LSD_THRES_DIST_DIST_H	(float) 200.0	// [cm]
#define LSD_THRES_DIST_DIST_VH	(float) 210.0	// [cm]

#define LSD_THRES_DIST_SPEED_VL	(float) 335.0	// [m/s]
#define LSD_THRES_DIST_SPEED_L	(float) 338.0	// [m/s]
#define LSD_THRES_DIST_SPEED_H	(float) 350.0	// [m/s]
#define LSD_THRES_DIST_SPEED_VH	(float) 354.0	// [m/s]

/* ==========================================================================================================
 * Enums
 */

typedef enum{
	LSD_THRES_CLIM_TEMP = 0,
	LSD_THRES_CLIM_HUMI,
	LSD_THRES_DIST_DIST,
	LSD_THRES_DIST_SPEED,
	LSD_ALL_THRES,
} LSD_THRES_VARIABLES_E;

typedef enum{
	LSD_THRES_BELOW_VERY_LOW = 0,
	LSD_THRES_BELOW_LOW,
	LSD_THRES_CENTER,
	LSD_THRES_ABOVE_HIGH,
	LSD_THRES_ABOVE_VERY_HIGH,
} LSD_THRES_CATEGORIES_E;

/* ==========================================================================================================
 * Structs
 */

typedef struct LSD_TRHES_TAG{
	float very_low;
	float low;
	float high;
	float very_high;
} LSD_TRHES_T;

/* ==========================================================================================================
 * Global Functions
 */

void lsd_analyze_data( LSD_LOG_DATA_T *p_log_data );

#endif /* INC_MYSERVICESINC_DATA_ANALYSIS_H_ */
