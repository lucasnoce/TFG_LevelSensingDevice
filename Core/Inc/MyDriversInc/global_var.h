/*
 * global_var.h
 *
 *  Created on: Oct 28, 2024
 *      Author: lucas
 */

#ifndef INC_MYDRIVERSINC_GLOBAL_VAR_H_
#define INC_MYDRIVERSINC_GLOBAL_VAR_H_

#include "main.h"

volatile uint32_t lsd_cycle_count;

LSD_LOG_BUFFER_T lsd_log_buffer[LSD_LOG_BUFFER_SIZE];
uint8_t lsd_log_buffer_next_pos;
uint8_t lsd_log_buffer_first_pos;

LSD_BUFFER_T modem_rx_buffer;
LSD_BUFFER_T modem_tx_buffer;

#endif /* INC_MYDRIVERSINC_GLOBAL_VAR_H_ */
