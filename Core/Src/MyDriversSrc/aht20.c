/*
 * aht20.c
 *
 *  Created on: Sep 3, 2024
 *      Author: lucas-noce
 */

#include <stdint.h>
#include <stdbool.h>
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_def.h"
#include "stm32f1xx_hal_i2c.h"
#include "MyDriversInc/aht20_reg.h"
#include "MyDriversInc/lsd_errno.h"
#include "MyDriversInc/lsd_config.h"
#include "MyDriversInc/lsd_callbacks.h"
#include "MyDriversInc/lsd_common.h"
#include "MyDriversInc/aht20.h"
#include "MyDriversInc/aht20_reg.h"

/* ==========================================================================================================
 * Definitions
 */

/* ==========================================================================================================
 * Local Structs
 */

struct AHT20_STATE_T{
	uint8_t busy		: 1;
	uint8_t mode		: 2;
	uint8_t reserved1	: 1;
	uint8_t calibration	: 1;
	uint8_t reserved2	: 3;
};

union AHT20_STATE_U{
	struct AHT20_STATE_T bit;
	uint8_t byte;
};

union AHT20_HT_U{
	uint8_t bytes[4];
	uint32_t word;
};

typedef struct AHT20_DATA_TAG{
	union AHT20_STATE_U state;
	union AHT20_HT_U humidity;
	union AHT20_HT_U temperature;
	uint8_t check_sum;
} AHT20_DATA_T;

/* ==========================================================================================================
 * Static Variables
 */

static I2C_HandleTypeDef *aht20_hi2c;
static AHT20_DATA_T aht20_data;

/* ==========================================================================================================
 * Local Function Prototypes
 */

static int8_t aht20_hal_i2c_transmit( uint16_t dev_addr, uint8_t *p_buf, uint16_t buf_size );
static int8_t aht20_hal_i2c_receive( uint16_t dev_addr, uint8_t *p_buf, uint16_t buf_size );
static uint8_t calculate_crc8( uint8_t *data, uint8_t length );

/* ==========================================================================================================
 * Global Functions Implementation
 */

int8_t aht20_init( I2C_HandleTypeDef *hi2c ){
	if( hi2c != NULL ){
		aht20_hi2c = hi2c;
		uint8_t init_cmd = AHT20_I2C_CMD_INIT;
		return aht20_hal_i2c_transmit( AHT20_I2C_ADDR_WRITE, &init_cmd, 1 );
	}
	return LSD_ERR_NULL_HANDLE;
}

int8_t aht20_soft_reset( void ){
	if( aht20_data.state.bit.busy == AHT20_I2C_STATE_NOT_BUSY )
	{
		uint8_t rst_cmd = AHT20_I2C_CMD_SOFT_RST;
		return aht20_hal_i2c_transmit( AHT20_I2C_ADDR_WRITE, &rst_cmd, 1 );
	}
	return LSD_ERR_DEVICE_BUSY;
}

int8_t aht20_update( void ){
	int ret = 0;

	uint8_t trig_cmd[3] = {
		AHT20_I2C_CMD_TRIG_MEASURE,
		AHT20_I2C_TRIG_MEAS_DATA0,
		AHT20_I2C_TRIG_MEAS_DATA1
	};

	ret = aht20_hal_i2c_transmit( AHT20_I2C_ADDR_WRITE, trig_cmd, 3 );

	// wait at least 75ms
	// todo: add non-blocking function
	lsd_delay_ms( 80 );

	uint8_t retry_count = 0;
	uint8_t rx_buf[7] = {0};

	do{
		ret = aht20_hal_i2c_receive( AHT20_I2C_ADDR_READ, rx_buf, 7 );
		if( ret != HAL_OK ){
			aht20_data.state.byte = 0;
			aht20_data.humidity.word = 0;
			aht20_data.temperature.word = 0;
			aht20_data.check_sum = 0;
	//		*data = aht20_data;
			return ret;
		}

		if( ++retry_count > 10 ){
			return LSD_ERR_TIMEOUT;
		}

		lsd_delay_ms( 2 );
	} while( ( rx_buf[0] & 0x80 ) != AHT20_I2C_STATE_NOT_BUSY );

	aht20_data.state.byte = rx_buf[0] & 0xFF;

	aht20_data.humidity.bytes[0] = rx_buf[3] & 0xF0;
	aht20_data.humidity.bytes[1] = rx_buf[2];
	aht20_data.humidity.bytes[2] = rx_buf[1];
	aht20_data.humidity.word = ( aht20_data.humidity.word >> 4 ) & 0x0FFFFFFF;

	aht20_data.temperature.bytes[0] = rx_buf[5];
	aht20_data.temperature.bytes[1] = rx_buf[4];
	aht20_data.temperature.bytes[2] = rx_buf[3] & 0x0F;

	aht20_data.check_sum = rx_buf[6];

	if( aht20_data.check_sum != calculate_crc8( &rx_buf[0], 6 ) ){
		return LSD_ERR_CRC_CHECK;
	}

//	*data = aht20_data; // copy the contents of aht20_data to the variable pointed by data

	return LSD_OK;
}

uint8_t aht20_get_status( void ){
	return aht20_data.state.byte;
}

bool aht20_is_busy( void ){
	return aht20_data.state.bit.busy ? true : false;
}

uint8_t aht20_get_mode( void ){
	return aht20_data.state.bit.mode;
}

bool aht20_is_calibrated( void ){
	return aht20_data.state.bit.calibration ? true : false;
}

float aht20_get_temperature_C( void ){
	float temp_raw = (float) aht20_data.temperature.word;
	return (float) ( ( ( temp_raw / 1024.0 ) / 1024.0 ) * 200.0 ) - 50.0;
}

float aht20_get_humidity_RH( void ){
	float rh_raw = (float) aht20_data.humidity.word;
	return (float) ( ( rh_raw / 1024.0 ) / 1024.0 ) * 100.0;
}

#ifdef TEST_MODE_DRIVER_AHT20
int8_t aht20_self_test( I2C_HandleTypeDef *hi2c ){
	int ret = 0;

	ret = aht20_init( hi2c );
	if( ret != LSD_OK ){
		// todo: print error
		return ret;
	}

	ret = aht20_soft_reset();
	if( ret != LSD_OK ){
		// todo: print error
		return ret;
	}

	AHT20_DATA_T data;
	ret = aht20_update( &data );
	if( ret != LSD_OK ){
		// todo: print error
		return ret;
	}

	float tmp = aht20_get_temperature_C();
	float hum = aht20_get_humidity_RH();

	// todo: print results

	return LSD_OK;
}
#endif /* TEST_MODE_DRIVER_AHT20 */


/* ==========================================================================================================
 * Local Functions Implementation
 */
static int8_t aht20_hal_i2c_transmit( uint16_t dev_addr, uint8_t *p_buf, uint16_t buf_size ){
	int8_t ret = 0;

#ifdef LSD_CONFIG_I2C_DMA
	ret = HAL_I2C_Master_Transmit_DMA( aht20_hi2c, dev_addr, p_buf, buf_size);
	LSD_I2C_IRQ_DEV_ID_DMA_TX = LSD_I2C_IRQ_DEV_ID_AHT20;
#elif defined( LSD_CONFIG_I2C_IT )
	ret = HAL_I2C_Master_Transmit_IT( aht20_hi2c, dev_addr, p_buf, buf_size );
	LSD_I2C_IRQ_DEV_ID_IT_TX = LSD_I2C_IRQ_DEV_ID_AHT20;
#else
	ret = HAL_I2C_Master_Transmit( aht20_hi2c, dev_addr, p_buf, buf_size, 1000 );
#endif /* LSD_CONFIG_I2C_DMA || LSD_CONFIG_I2C_IT */

	return ret;
}

static int8_t aht20_hal_i2c_receive( uint16_t dev_addr, uint8_t *p_buf, uint16_t buf_size ){
	int8_t ret = 0;

#ifdef LSD_CONFIG_I2C_DMA
	ret = HAL_I2C_Master_Receive_DMA( aht20_hi2c, dev_addr, p_buf, buf_size);
	LSD_I2C_IRQ_DEV_ID_DMA_RX = LSD_I2C_IRQ_DEV_ID_AHT20;
#elif defined( LSD_CONFIG_I2C_IT )
	ret = HAL_I2C_Master_Receive_IT( aht20_hi2c, dev_addr, p_buf, buf_size );
	LSD_I2C_IRQ_DEV_ID_IT_RX = LSD_I2C_IRQ_DEV_ID_AHT20;
#else
	ret = HAL_I2C_Master_Receive( aht20_hi2c, dev_addr, p_buf, buf_size, 1000 );
#endif /* LSD_CONFIG_I2C_DMA || LSD_CONFIG_I2C_IT */

	return ret;
}

static uint8_t calculate_crc8( uint8_t *data, uint8_t length ){
    uint8_t crc = 0xFF;
    uint8_t polynomial = 0x31;

	for (uint8_t byte_idx=0; byte_idx<length; byte_idx++){
		crc ^= data[byte_idx];
		for (uint8_t bit_idx=8; bit_idx>0; --bit_idx){
			if (crc & 0x80){
				crc = ( crc << 1 ) ^ polynomial;
			}
			else{
				crc <<= 1;
			}
		}
	}

    return crc;
}
