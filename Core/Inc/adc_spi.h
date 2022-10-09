/*******************************************************************************
  * File Name			: adc_spi.h
  * Description			: This module contains the definitions of constants and
  * 					  functions related to the ADC SPI protocol. High-level
  * 					  routines that will be called by other modules.
  *
  * Author				: Charlie Moreno, Robson Viera de Souza
  * Date				: September 27, 2021
  ******************************************************************************
  */
#ifndef INC_ADC_SPI_H_
#define INC_ADC_SPI_H_

#include "common.h"
#include "stm32l4xx_hal.h"
#include <stdbool.h>

/* ADS131M03 delta-sigma ADC */

#define ADC_CPOL		(SPI_POLARITY_LOW_)
#define ADC_CPHA		(SPI_PHASE_2EDGE)
#define CLK_OUT_CHANNEL	(TIM_CHANNEL_1)

/************************** Ports Definition **************************/
#define CLK_OUT_GPIO_Port			(GPIOA)
#define CLK_OUT_Pin					(GPIO_PIN_6)
#define ADC_NOT_CS_GPIO_Port		(GPIOB)
#define ADC_NOT_CS_Pin				(GPIO_PIN_1)
#define NOT_DRDY_GPIO_Port			(GPIOA)
#define NOT_DRDY_Pin				(GPIO_PIN_3)
#define NOT_SYNC_RESET_GPIO_Port	(GPIOB)
#define NOT_SYNC_RESET_Pin			(GPIO_PIN_6)

/************************** Commands Definition **************************/
#define NULL_CMD					(0x00000000)
#define RESET_CMD					(0x00000011)
#define STANDBY_CMD					(0x00000022)
#define WAKEUP_CMD					(0x00000033)
#define LOCK_CMD					(0x00000555)
#define UNLOCK_CMD					(0x00000655)


/************************** ADC SPI Routines **************************/
void initAdSpi(TIM_HandleTypeDef *handler);
bool isDataReady(void);


#endif /* INC_ADC_SPI_H_ */


