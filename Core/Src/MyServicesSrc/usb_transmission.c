/*
 * usb_transmission.c
 *
 *  Created on: Nov 30, 2024
 *      Author: lucas
 */

/* ==========================================================================================================
 * Includes
 */

#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_uart.h"
#include "main.h"

#include "MyDriversInc/lsd_common.h"
#include "MyDriversInc/lsd_errno.h"
#include "MyDriversInc/lsd_config.h"

#include "MyServicesInc/distance_measurements.h"
#include "MyServicesInc/usb_transmission.h"

#if LSD_PERIPH_USED_USB == TRUE
#include "usb_device.h"
#include "usbd_cdc_if.h"
#endif /* LSD_PERIPH_USED_USB */

/* ==========================================================================================================
 * Definitions
 */

#define USB_LOG_DATA_MSG_SIZE	128

/* ==========================================================================================================
 * Static Variables
 */

static UART_HandleTypeDef *p_lsd_huart;

/* ==========================================================================================================
 * Global Functions Implementation
 */

int8_t lsd_usb_init( uint32_t *periph_handle ){
#if LSD_PERIPH_USED_USB == TRUE
	__HAL_RCC_USB_CLK_ENABLE();
	MX_USB_DEVICE_Init();
#else
	if( periph_handle == NULL ){
		return LSD_ERR_NULL_HANDLE;
	}

	p_lsd_huart = ( UART_HandleTypeDef * ) periph_handle;

	p_lsd_huart->Instance = USART2;
	p_lsd_huart->Init.BaudRate = 115200;
	p_lsd_huart->Init.WordLength = UART_WORDLENGTH_8B;
	p_lsd_huart->Init.StopBits = UART_STOPBITS_1;
	p_lsd_huart->Init.Parity = UART_PARITY_NONE;
	p_lsd_huart->Init.Mode = UART_MODE_TX_RX;
	p_lsd_huart->Init.HwFlowCtl = UART_HWCONTROL_NONE;
	p_lsd_huart->Init.OverSampling = UART_OVERSAMPLING_16;

	if( HAL_UART_Init( p_lsd_huart ) != HAL_OK )
	{
		return LSD_ERR_INIT_FAIL;
	}
#endif /* LSD_PERIPH_USED_USB */

	return LSD_OK;
}

int8_t lsd_usb_reinit( void ){
#if LSD_PERIPH_USED_USB == TRUE
	return lsd_usb_init( NULL );
#else
	return lsd_usb_init( ( uint32_t * ) p_lsd_huart );
#endif /* LSD_PERIPH_USED_USB */
}

int8_t lsd_usb_deinit( void ){
#if LSD_PERIPH_USED_USB == TRUE
	MX_USB_DEVICE_Deinit();
	__HAL_RCC_USB_CLK_DISABLE();
#else
	HAL_UART_DeInit( p_lsd_huart );
#endif /* LSD_PERIPH_USED_USB */

	return LSD_OK;
}

int8_t lsd_print_msg_usb( char *msg, uint16_t msg_len ){
	if( msg == NULL || msg_len <= 0 ){
		return LSD_ERR_NULL_HANDLE;
	}

#if LSD_PERIPH_USED_USB == TRUE
	CDC_Transmit_FS( (uint8_t *) msg, msg_len );
#else
	HAL_UART_Transmit( p_lsd_huart, (const uint8_t *) msg, msg_len, 1000 );
#endif /* LSD_PERIPH_USED_USB */

	return LSD_OK;
}

int8_t lsd_print_log_data_usb( LSD_LOG_DATA_T *p_log ){
	if( p_log == NULL ){
		return LSD_ERR_NULL_HANDLE;
	}

	char msg[USB_LOG_DATA_MSG_SIZE];

	snprintf( msg, USB_LOG_DATA_MSG_SIZE, "temperature = %.2f [*C]\tcategory : %d\n",
			p_log->clim.temperature, p_log->clim.temperature_category );
	lsd_print_msg_usb( msg, strlen( msg ) );

	snprintf( msg, USB_LOG_DATA_MSG_SIZE, "humidity = %.2f [RH]\tcategory : %d\n",
			p_log->clim.humidity, p_log->clim.humidity_category );
	lsd_print_msg_usb( msg, strlen( msg ) );

	snprintf( msg, USB_LOG_DATA_MSG_SIZE, "distance = %.2f [m]\tcategory : %d\n",
			p_log->dist.distance, p_log->dist.distance_category );
	lsd_print_msg_usb( msg, strlen( msg ) );

	snprintf( msg, USB_LOG_DATA_MSG_SIZE, "speed_of_sound = %.2f [m/s]\tcategory : %d\n",
			p_log->dist.speed_of_sound, p_log->dist.speed_of_sound_category );
	lsd_print_msg_usb( msg, strlen( msg ) );

	snprintf( msg, USB_LOG_DATA_MSG_SIZE, "errors : %c%c%c%c%c%c%c%c\ttransmitted : %s\n",
			BYTE_TO_BINARY( p_log->errors ), ( p_log->transmitted ? "true" : "false" ) );
	lsd_print_msg_usb( msg, strlen( msg ) );

	return LSD_OK;
}
