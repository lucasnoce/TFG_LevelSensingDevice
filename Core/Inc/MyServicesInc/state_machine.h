/*
 * state_machine.h
 *
 *  Created on: Aug 31, 2024
 *      Author: Lucas Noce
 */

#ifndef INC_MYSERVICESINC_STATE_MACHINE_H_
#define INC_MYSERVICESINC_STATE_MACHINE_H_

#include <stdint.h>
#include "stm32f1xx_hal_tim.h"
#include "stm32f1xx_hal_i2c.h"
#include "stm32f1xx_hal_uart.h"

/* ==========================================================================================================
 * Enums
 */

/*
 * @brief		This enum lists all the possible states for the application.
 */
typedef enum{
	S0_WAKEUP = 0,
	S1_MEASURE,
	S2_ANALYZE,
	S3_TRANSMIT,
	S4_SLEEP,
	S5_FAIL_SAFE,
} STATES_E;

/* ==========================================================================================================
 * Structs
 */

/* ==========================================================================================================
 * Static Variables
 */

/* ==========================================================================================================
 * Global Function Prototypes
 */

/*
 * @brief		This function initializes the state machine.
 * @param		None
 * @retval		None
 */
void state_machine_init( LSD_MCU_PERIPH_HANDLES_T *hmcu_periph, lsd_sys_clk_cfg_t clk_cfg );

/*
 * @brief		This function runs the state machine, calling the appropriate functions for each state.
 * @param		None
 * @retval		None
 */
void state_machine_run( void );

#endif /* INC_MYSERVICESINC_STATE_MACHINE_H_ */
