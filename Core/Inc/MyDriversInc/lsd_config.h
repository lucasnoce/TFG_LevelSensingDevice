/*
 * lsd_config.h
 *
 *  Created on: Sep 1, 2024
 *      Author: lucas-noce
 */

#ifndef INC_MYDRIVERSINC_LSD_CONFIG_H_
#define INC_MYDRIVERSINC_LSD_CONFIG_H_

#include <stdint.h>

#define TRUE	1
#define FALSE	0

/*
 * Configure Peripherals Used
 */

#define PERIPH_USED_AHT20		TRUE
#define PERIPH_USED_AJSR04M		TRUE
#define PERIPH_USED_LEDS		TRUE
#define PERIPH_USED_LE910R1BR	TRUE
#define PERIPH_USED_VL53L0CX	TRUE

/*
 * Timers and channels
 */

#define LSD_TIM_SENSORS				( 4 - 2 )
#define LSD_TIM_COMMON				( 3 - 2 )
#define LSD_TIM_GENERAL				( 2 - 2 )
#define LSD_TIM_GENERAL_CH_LED_BLUE	1
#define LSD_TIM_GENERAL_CH_LED_RED	2
#define LSD_TIM_GENERAL_CH_MODEM	3

#define LSD_CONFIG_DMA			TRUE
#define LSD_CONFIG_IT			TRUE

#define LSD_LOG_BUF_SIZE		sizeof(uint8_t)

#define LSD_STD_TIMEOUT_MS		1
#define LSD_STD_TIMEOUT_US		(1000 * LSD_STD_TIMEOUT_MS)

#define MEAN_LOCAL_PRESSURE 	101.77  // [kPa]

#define LSD_SENSOR_HEIGHT		2	// [m]

#define LSD_LOG_BUFFER_SIZE		256

/*
 * Configure Data to be Transmitted
 */
#define DATA_TRANSMIT_DISTANCE		TRUE
#define DATA_TRANSMIT_TEMPERATURE	TRUE
#define DATA_TRANSMIT_HUMIDITY		TRUE

#define MQTT_DOMAIN     "mqtt.tago.io"
#define MQTT_PORT       1883
#define MQTT_ID         "MQTT_1"
#define MQTT_USER       "levelsensingdevice"
#define MQTT_PASSWORD   "9213ad58-886a-4bae-87d0-4182c4600dd2"
#define MQTT_TOPIC_SYS  "syscheck"
#define MQTT_TOPIC_READ "readings"
#define MQTT_TOPIC_TEST "test"



/*
 * Test Modes for individual modules
 */
#define TEST_MODE_ENABLE

#ifdef TEST_MODE_ENABLE
//#define TEST_MODE_STATE_MACHINE
//#define TEST_MODE_DRIVER_AHT20
//#define TEST_MODE_DRIVER_AJSR04M
#define TEST_MODE_DRIVER_LEDS
//#define TEST_MODE_DRIVER_LE910R1BR
//#define TEST_MODE_DRIVER_VL53L0CX
//#define TEST_MODE_SERVICE_DATA_ANALYSIS
//#define TEST_MODE_SERVICE_DATA_TRANSMISSION
//#define TEST_MODE_SERVICE_DISTANCE_MEASUREMENTS
//#define TEST_MODE_SERVICE_SYSTEM_SLEEP
#endif /* TEST_MODE_ENABLE */


//#if ( LSD_CONFIG_DMA && LSD_CONFIG_IT )
//
//#error "Both DMA and IT are configured"
//
//#elif LSD_CONFIG_DMA
//
//#define LSD_CONFIG_I2C_DMA
//#define LSD_CONFIG_UART_DMA
//#define LSD_CONFIG_SPI_DMA
//
//#elif LSD_CONFIG_IT
//
//#define LSD_CONFIG_I2C_IT
//#define LSD_CONFIG_UART_IT
//#define LSD_CONFIG_SPI_IT
//
//#endif /* LSD_CONFIG_DMA && LSD_CONFIG_IT */


#endif /* INC_MYDRIVERSINC_LSD_CONFIG_H_ */
