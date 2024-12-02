/*
 * usb_transmission.h
 *
 *  Created on: Nov 30, 2024
 *      Author: lucas
 */

#ifndef INC_MYSERVICESINC_USB_TRANSMISSION_H_
#define INC_MYSERVICESINC_USB_TRANSMISSION_H_

/* ==========================================================================================================
 * Includes
 */

#include "main.h"
#include "MyServicesInc/distance_measurements.h"

/* ==========================================================================================================
 * Global Function Prototypes
 */

int8_t lsd_usb_init( uint32_t *periph_handle );
int8_t lsd_usb_reinit( void );
int8_t lsd_usb_deinit( void );
int8_t lsd_print_msg_usb( char *msg, uint16_t msg_len );
int8_t lsd_print_log_data_usb( LSD_LOG_DATA_T *p_log );

#endif /* INC_MYSERVICESINC_USB_TRANSMISSION_H_ */
