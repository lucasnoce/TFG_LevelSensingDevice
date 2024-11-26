/*
 * uls_callbacks.h
 *
 *  Created on: Sep 5, 2024
 *      Author: lucas-noce
 */

#ifndef ULS_SYSTEM_ULS_CALLBACKS_H_
#define ULS_SYSTEM_ULS_CALLBACKS_H_

#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_def.h"
#include "stm32f1xx_hal_tim.h"
//#include "stm32f1xx_hal_i2c.h"
#include "MyDriversInc/lsd_config.h"

////typedef enum ULS_I2C_IRQ_DEV_ID_E{
////	ULS_I2C_IRQ_DEV_ID_AHT20,
////	ULS_I2C_IRQ_DEV_ID_AJSR04M,
////};
////
////extern enum ULS_I2C_IRQ_DEV_ID_E ULS_I2C_IRQ_DEV_ID_IT_TX;
////extern enum ULS_I2C_IRQ_DEV_ID_E ULS_I2C_IRQ_DEV_ID_IT_RX;
////extern enum ULS_I2C_IRQ_DEV_ID_E ULS_I2C_IRQ_DEV_ID_DMA_TX;
////extern enum ULS_I2C_IRQ_DEV_ID_E ULS_I2C_IRQ_DEV_ID_DMA_RX;
////
//typedef enum ULS_IRQ_DEV_ID_E{
//	ULS_IRQ_DEV_ID_AHT20,
//	ULS_IRQ_DEV_ID_AJSR04M,
//	ULS_IRQ_DEV_ID_LE910R1,
//	ULS_IRQ_DEV_ID_VL53L0CX,
//	ULS_IRQ_DEV_ID_ALL_DEVICES,
//};
////
////extern ULS_DMA_DEV_ID_E ULS_DMA_DEV_ID_TX;
////extern ULS_DMA_DEV_ID_E ULS_DMA_DEV_ID_RX;
//
//struct ULS_ISR_MODE_T{
//	uint8_t it_rx	: 1;
//	uint8_t it_tx	: 1;
//	uint8_t it_err	: 1;
//	uint8_t dma_rx	: 1;
//	uint8_t dma_tx	: 1;
//	uint8_t dma_err	: 1;
//};
//
//union ULS_ISR_MODE_U{
//	struct ULS_ISR_MODE_T bit;
//	uint8_t byte;
//};
//
//union ULS_ISR_MODE_U uls_irq_dev[ULS_IRQ_DEV_ID_ALL_DEVICES];

void HAL_TIM_OC_DelayElapsedCallback( TIM_HandleTypeDef *htim );

//void HAL_I2C_MasterTxCpltCallback ( I2C_HandleTypeDef *hi2c );
//void HAL_I2C_MasterRxCpltCallback ( I2C_HandleTypeDef *hi2c );
//void HAL_I2C_ErrorCallback ( I2C_HandleTypeDef *hi2c );


#endif /* ULS_SYSTEM_ULS_CALLBACKS_H_ */
