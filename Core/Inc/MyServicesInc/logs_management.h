/*
 * logs_management.h
 *
 *  Created on: Nov 1, 2024
 *      Author: lucas
 */

#ifndef INC_MYSERVICESINC_LOGS_MANAGEMENT_H_
#define INC_MYSERVICESINC_LOGS_MANAGEMENT_H_

#include <stdint.h>
#include "MyServicesInc/distance_measurements.h"

void lsd_log_write( LSD_LOG_DATA_T *p_log_data );

void lsd_log_read_index( int16_t idx, LSD_LOG_DATA_T *p_log_data );  // negative represent how many logs back, positive represent absolute position in buffer
void lsd_log_read_batch( int8_t qty, LSD_LOG_DATA_T **p_log_data );

#endif /* INC_MYSERVICESINC_LOGS_MANAGEMENT_H_ */
