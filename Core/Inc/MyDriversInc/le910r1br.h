/*
 * le910r1br.h
 *
 *  Created on: Oct 5, 2024
 *      Author: lucas
 */

#ifndef INC_MYDRIVERSINC_LE910R1BR_H_
#define INC_MYDRIVERSINC_LE910R1BR_H_

#include <stdint.h>
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_def.h"
#include "stm32f1xx_hal_uart.h"
#include "stm32f1xx_hal_tim.h"
#include "MyDriversInc/lsd_config.h"

//#define MODEM_BUF_SIZE			64
//#define MODEM_BUF_FIFO_LENGTH	5

//typedef struct MODEM_BUFFER_TAG{
//	uint8_t msg[MODEM_BUF_SIZE];
//	uint8_t pos;
//} MODEM_BUFFER_T;

int8_t le910r1br_init( UART_HandleTypeDef *huart, TIM_HandleTypeDef *htim );
void le910r1br_power_on( void );
void le910r1br_power_off( void );

int8_t le910r1br_send_message( uint8_t *msg, uint8_t msg_size );

int8_t le910r1br_sw_shutdown( void );
int8_t le910r1br_sw_reboot( void );
int8_t le910r1br_check_at( void );
int8_t le910r1br_check_cpin( void );
int8_t le910r1br_check_csq( void );
int8_t le910r1br_set_cops( uint8_t mode, uint8_t format, char *oper, uint8_t act );

void le910r1br_power_callback_handler( void );
//void le910r1br_rx_handler( void );
//void le910r1br_tx_handler( void );

#endif /* INC_MYDRIVERSINC_LE910R1BR_H_ */
