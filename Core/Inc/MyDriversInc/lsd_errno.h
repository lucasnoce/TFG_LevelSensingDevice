/*
 * uls_errno.h
 *
 *  Created on: Sep 4, 2024
 *      Author: lucas-noce
 */

#ifndef INC_MYDRIVERSINC_LSD_ERRNO_H_
#define INC_MYDRIVERSINC_LSD_ERRNO_H_

#include <stdint.h>

#define LSD_OK					( ( int8_t )  0 )
#define LSD_ERR_INIT_FAIL		( ( int8_t ) -1 )
#define LSD_ERR_TIMEOUT			( ( int8_t ) -2 )
#define LSD_ERR_NULL_HANDLE		( ( int8_t ) -3 )
#define LSD_ERR_WRONG_PARAM		( ( int8_t ) -4 )
#define LSD_ERR_DEVICE_BUSY		( ( int8_t ) -5 )
#define LSD_ERR_CRC_CHECK		( ( int8_t ) -6 )

#endif /* INC_MYDRIVERSINC_LSD_ERRNO_H_ */
