/*
 * le910r1br.c
 *
 *  Created on: Oct 5, 2024
 *      Author: lucas
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_def.h"
#include "stm32f1xx_hal_tim.h"
#include "stm32f1xx_hal_uart.h"
#include "stm32f1xx_hal_gpio.h"
#include "main.h"
#include "MyDriversInc/lsd_errno.h"
#include "MyDriversInc/lsd_common.h"
#include "MyDriversInc/lsd_config.h"
#include "MyDriversInc/le910r1br_at_cmds.h"
#include "MyDriversInc/le910r1br.h"

/* ==========================================================================================================
 * Definitions
 */

#define LE910R1BR_TURN_ON_PULSE_MS	(uint32_t) 4000
#define LE910R1BR_TURN_ON_DELAY_MS	(uint32_t) 30000
#define LE910R1BR_TURN_OFF_PULSE_MS	(uint32_t) 2000

/* ==========================================================================================================
 * Enums
 */

typedef enum{
	AT_ONLY = 0,
	AT_CMD_ONLY,
	AT_CMD_ASK,
	AT_CMD_TEST,
	AT_CMD_PRM,
} AT_CMD_TYPE_E;

typedef enum{
	MODEM_TAKE_RX_BUF = 0,
	MODEM_TAKE_TX_BUF,
} MODEM_TAKE_BUF_E;

/* ==========================================================================================================
 * Typedefs
 */

typedef struct MODEM_DATA_TAG{
	uint8_t tim_channel;
	GPIO_TypeDef *port_pwr;		// GPIO Port
	uint16_t pin_pwr;			// GPIO Pin
	bool ready;
} MODEM_DATA_T;

typedef struct MODEM_AT_CMD_TAG{
	const char *cmd;
	char **prm;
} MODEM_AT_CMD_T;

/* ==========================================================================================================
 * Static Variables
 */

static UART_HandleTypeDef *p_modem_huart;
static TIM_HandleTypeDef *p_modem_htim;

static MODEM_DATA_T modem_data;
static MODEM_AT_CMD_T modem_cmd;

extern LSD_BUFFER_T modem_rx_buffer;
extern LSD_BUFFER_T modem_tx_buffer;

/* ==========================================================================================================
 * Local Function Prototypes
 */

static int8_t modem_at_cmd_concat( AT_CMD_TYPE_E type, uint8_t prm_count, MODEM_AT_CMD_T *cmd_list, char *cmd_concat/*, uint8_t cmd_concat_size */);
static int8_t modem_send_command_receive_response( AT_CMD_TYPE_E type, uint8_t prm_count, MODEM_AT_CMD_T *cmd_list, char *cmd_concat );
static int8_t modem_try_take_buffer( uint8_t buf_rx_tx, uint8_t max_tries );
static int8_t modem_send_at_cmd( AT_CMD_TYPE_E type, const char *cmd, char **prm, uint8_t prm_count );

/* ==========================================================================================================
 * Global Functions Implementation
 */

int8_t le910r1br_init( UART_HandleTypeDef *huart, TIM_HandleTypeDef *htim ){
	if( huart != NULL && htim != NULL ){
		p_modem_huart  = huart;
		p_modem_htim   = htim;

		modem_data.tim_channel  = TIM_CHANNEL_3;	// todo: select correct channel
		modem_data.port_pwr		= uLTE_PWR_GPIO_Port;
		modem_data.pin_pwr 		= uLTE_PWR_Pin;
		modem_data.ready 		= false;

		HAL_GPIO_WritePin( modem_data.port_pwr, modem_data.pin_pwr, GPIO_PIN_RESET );

		return LSD_OK;
	}
	return LSD_ERR_NULL_HANDLE;
}

void le910r1br_power_on( void ){
	HAL_GPIO_WritePin( modem_data.port_pwr, modem_data.pin_pwr, GPIO_PIN_SET );
	HAL_Delay( LE910R1BR_TURN_ON_PULSE_MS );
	HAL_GPIO_WritePin( modem_data.port_pwr, modem_data.pin_pwr, GPIO_PIN_RESET );
	HAL_Delay( LE910R1BR_TURN_ON_DELAY_MS );

	/*
	HAL_TIM_OC_Start_IT( p_modem_htim, modem_data.tim_channel );

	__HAL_TIM_SET_COMPARE( p_modem_htim, modem_data.tim_channel, \
		( __HAL_TIM_GET_COMPARE( p_modem_htim, modem_data.tim_channel ) + LE910R1BR_TURN_ON_PULSE_MS ) );
*/

	return;
}

void le910r1br_power_off( void ){
	HAL_GPIO_WritePin( modem_data.port_pwr, modem_data.pin_pwr, GPIO_PIN_SET );
	HAL_Delay( LE910R1BR_TURN_OFF_PULSE_MS );
	HAL_GPIO_WritePin( modem_data.port_pwr, modem_data.pin_pwr, GPIO_PIN_RESET );

	/*
	HAL_TIM_OC_Start_IT( p_modem_htim, modem_data.tim_channel );

	__HAL_TIM_SET_COMPARE( p_modem_htim, modem_data.tim_channel, \
		( __HAL_TIM_GET_COMPARE( p_modem_htim, modem_data.tim_channel ) + LE910R1BR_TURN_OFF_PULSE_MS ) );
*/
	return;
}

int8_t le910r1br_send_message( uint8_t *msg, uint8_t msg_size ){
	if( !modem_data.ready ){
		return LSD_ERR_DEVICE_BUSY;
	}

	if( msg == NULL ){
		return LSD_ERR_NULL_HANDLE;
	}

	int ret = 0;
	ret = HAL_UART_Transmit( p_modem_huart, msg, msg_size, 10000);

	return ret;
}

int8_t le910r1br_sw_shutdown( void ){
	return modem_send_at_cmd( AT_CMD_ONLY, AT_CMD_SHDN, NULL, 0 );
}

int8_t le910r1br_sw_reboot( void ){
	return modem_send_at_cmd( AT_CMD_ONLY, AT_CMD_REBOOT, NULL, 0 );
}

int8_t le910r1br_check_at( void ){
	return modem_send_at_cmd( AT_ONLY, AT_CMD_AT, NULL, 0 );
}

int8_t le910r1br_check_cpin( void ){
	return modem_send_at_cmd( AT_CMD_ASK, AT_CMD_CPIN, NULL, 0 );
}

int8_t le910r1br_check_csq( void ){
	return modem_send_at_cmd( AT_CMD_ONLY, AT_CMD_CSQ, NULL, 0 );
}

int8_t le910r1br_set_cops( uint8_t mode, uint8_t format, char *oper, uint8_t act ){
	uint8_t prm_count = 1;
	char **cops = NULL;

	char cops_mode[]	= { ( mode + 48 ),   '\0' };
	char cops_format[]	= { ( format + 48 ), '\0' };
	char *cops_oper		= oper;
	char cops_act[]		= { ( act + 48 ),    '\0' };

	char *cops_prm[4] = {
		cops_mode,
		cops_format,
		cops_oper,
		cops_act
	};

	if( mode != 0 ){
		prm_count++;
		if( format != 0 ){
			prm_count++;
			if( oper != NULL ){
				prm_count++;
			}
		}
	}

	cops = malloc( prm_count * sizeof(char *) );
	if( cops == NULL ){
	    return LSD_ERR_NULL_HANDLE;
	}

	for( int i=0; i<prm_count; i++ ){
		cops[i] = cops_prm[i];
	}

	return modem_send_at_cmd( AT_CMD_PRM, AT_CMD_COPS, cops, prm_count );
}

void le910r1br_power_callback_handler( void ){
	modem_data.ready = !modem_data.ready;

	HAL_GPIO_WritePin( modem_data.port_pwr, modem_data.pin_pwr, GPIO_PIN_SET );
	HAL_TIM_OC_Stop_IT( p_modem_htim, modem_data.tim_channel );

	return;
}

//void le910r1br_rx_handler( void ){
//	modem_rx_buffer.busy = false;
//	return;
//}
//
//void le910r1br_tx_handler( void ){
//	modem_tx_buffer.busy = false;
//	return;
//}

/* ==========================================================================================================
 * Local Functions Implementation
 */

static int8_t modem_at_cmd_concat( AT_CMD_TYPE_E type, uint8_t prm_count, MODEM_AT_CMD_T *cmd_list, char *cmd_concat/*, uint8_t cmd_concat_size */){
	if( cmd_list->cmd == NULL ){
		return LSD_ERR_NULL_HANDLE;
	}

	uint8_t cmd_concat_size = 0;
	cmd_concat_size += strlen( cmd_list->cmd );

	for( int i=0; i<prm_count; i++){
		cmd_concat_size += strlen( cmd_list->prm[i] );
	}
	cmd_concat_size++;	// Add only one '\0' at the end

	memset( cmd_concat, 0, cmd_concat_size );

	switch( type ){
		case AT_ONLY:
			strcat( cmd_concat, cmd_list->cmd );
			strcat( cmd_concat, "\r\n" );
			break;

		case AT_CMD_ONLY:
			strcpy( cmd_concat, AT_CMD_AT );
			strcat( cmd_concat, cmd_list->cmd );
			strcat( cmd_concat, "\r\n" );
			break;

		case AT_CMD_ASK:
			strcpy( cmd_concat, AT_CMD_AT );
			strcat( cmd_concat, cmd_list->cmd );
			strcat( cmd_concat, "?\r\n" );
			break;

		case AT_CMD_TEST:
			strcpy( cmd_concat, AT_CMD_AT );
			strcat( cmd_concat, cmd_list->cmd );
			strcat( cmd_concat, "=?\r\n" );
			break;

		case AT_CMD_PRM:
			strcpy( cmd_concat, AT_CMD_AT );
			strcat( cmd_concat, cmd_list->cmd );
			strcat( cmd_concat, "=" );
			for( int i=0; i<prm_count; i++){
				if( i != 0 ){
					strcat( cmd_concat, "," );
				}
				strcat( cmd_concat, cmd_list->prm[i] );
			}
			strcat( cmd_concat, "\r\n" );
			break;

		default:
			break;
	}

	return LSD_OK;
}

static int8_t modem_send_command_receive_response( AT_CMD_TYPE_E type, uint8_t prm_count, MODEM_AT_CMD_T *cmd_list, char *cmd_concat ){
	int8_t ret = LSD_OK;

	ret = modem_at_cmd_concat( type, prm_count, cmd_list, cmd_concat );

	if( ret != LSD_OK )
	{
		return ret;
	}

	ret = modem_try_take_buffer( MODEM_TAKE_RX_BUF, 10 );
	if( ret != LSD_OK ){
		return ret;
	}

	ret = HAL_UART_Transmit( p_modem_huart, (const uint8_t *) cmd_concat, strlen( cmd_concat ), 1000 );

	uint8_t rx_pos = 0;
	uint8_t c = 0;

	HAL_UART_Receive_IT( p_modem_huart, &c, 1 );

	do{
		if( p_modem_huart->RxState != HAL_UART_STATE_BUSY_RX ){
			modem_rx_buffer.msg[rx_pos++] = c;
			HAL_UART_Receive_IT( p_modem_huart, &c, 1 );
		}
	} while( modem_rx_buffer.msg[rx_pos] != '\r' && modem_rx_buffer.msg[rx_pos] != '\n' );

	if( rx_pos < ( LSD_BUF_SIZE - 1 ) ){
		modem_rx_buffer.msg[++rx_pos] = 0;
	}
	else{
		modem_rx_buffer.msg[LSD_BUF_SIZE - 1] = 0;
	}
	modem_rx_buffer.busy = false;

	return LSD_OK;
}

static int8_t modem_try_take_buffer( uint8_t buf_rx_tx, uint8_t max_tries ){
	uint8_t try_count = max_tries;

	switch( buf_rx_tx ){
		case MODEM_TAKE_RX_BUF:
			while( modem_rx_buffer.busy ){
				HAL_Delay( 10 );
				if( --try_count <= 0 ){
					return LSD_ERR_TIMEOUT;
				}
			}
			modem_rx_buffer.busy = true;
			return LSD_OK;

		case MODEM_TAKE_TX_BUF:
			while( modem_tx_buffer.busy ){
				HAL_Delay( 10 );
				if( --try_count <= 0 ){
					return LSD_ERR_TIMEOUT;
				}
			}
			modem_tx_buffer.busy = true;
			return LSD_OK;

		default:
			return LSD_ERR_WRONG_PARAM;
	}
}

static int8_t modem_send_at_cmd( AT_CMD_TYPE_E type, const char *cmd, char **prm, uint8_t prm_count ){
//	int8_t ret = LSD_OK;

//	ret = modem_try_take_buffer( MODEM_TAKE_TX_BUF, 10 );
//	if( ret != LSD_OK ){
//		return ret;
//	}

	modem_cmd.cmd = cmd;
	modem_cmd.prm = prm;

	return modem_send_command_receive_response( type, prm_count, &modem_cmd, (char *) modem_tx_buffer.msg );
}
