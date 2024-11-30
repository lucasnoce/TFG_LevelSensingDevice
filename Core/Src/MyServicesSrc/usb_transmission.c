/*
 * usb_transmission.c
 *
 *  Created on: Nov 30, 2024
 *      Author: lucas
 */

#include <stdint.h>
#include "stm32f1xx_hal.h"
#include "usb_device.h"
#include "usbd_cdc_if.h"
#include "main.h"

#include "MyDriversInc/lsd_common.h"

#include "MyServicesInc/distance_measurements.h"
#include "MyServicesInc/usb_transmission.h"

void lsd_transmit_usb( LSD_LOG_DATA_T *p_log ){
	char msg[128];

	snprintf( msg, 128, "temperature = %.2f [*C]\tcategory : %d\n",
			p_log->clim.temperature, p_log->clim.temperature_category );
	CDC_Transmit_FS( (uint8_t *) msg, strlen( msg ) );

	snprintf( msg, 128, "humidity = %.2f [RH]\tcategory : %d\n",
			p_log->clim.humidity, p_log->clim.humidity_category );
	CDC_Transmit_FS( (uint8_t *) msg, strlen( msg ) );

	snprintf( msg, 128, "distance = %.2f [m]\tcategory : %d\n",
			p_log->dist.distance, p_log->dist.distance_category );
	CDC_Transmit_FS( (uint8_t *) msg, strlen( msg ) );

	snprintf( msg, 128, "speed_of_sound = %.2f [m/s]\tcategory : %d\n",
			p_log->dist.speed_of_sound, p_log->dist.speed_of_sound_category );
	CDC_Transmit_FS( (uint8_t *) msg, strlen( msg ) );

	snprintf( msg, 128, "errors : %c%c%c%c%c%c%c%c\ttransmitted : %s\n",
			BYTE_TO_BINARY( p_log->errors ), ( p_log->transmitted ? "true" : "false" ) );
	CDC_Transmit_FS( (uint8_t *) msg, strlen( msg ) );

	return;
}
