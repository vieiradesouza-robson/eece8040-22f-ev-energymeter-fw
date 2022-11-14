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
#define NULL_CMD					(0x0000)
#define RESET_CMD					(0x0011)
#define STANDBY_CMD					(0x0022)
#define WAKEUP_CMD					(0x0033)
#define LOCK_CMD					(0x0555)
#define UNLOCK_CMD					(0x0655)
#define RREG_CMD					(0xA000)
#define WREG_CMD					(0x6000)

/************************** Register Definition **************************/
#define ID_REG						(0x0000)
#define STATUS_REG					(0x0001)
#define MODE_REG					(0x0002)
#define CLOCK_REG					(0x0003)
#define GAIN_REG					(0x0004)
#define CFG_REG						(0x0006)
#define THRSHLD_MSB_REG				(0x0007)
#define THRSHLD_LSB_REG				(0x0008)
#define CH0_CFG_REG					(0x0009)
#define CH0_OCAL_MSB_REG			(0x000A)
#define CH0_OCAL_LSB_REG			(0x000B)
#define CH0_GCAL_MSB_REG			(0x000C)
#define CH0_GCAL_LSB_REG			(0x000D)
#define CH1_CFG_REG					(0x000E)
#define CH1_OCAL_MSB_REG			(0x000F)
#define CH1_OCAL_LSB_REG			(0x0010)
#define CH1_GCAL_MSB_REG			(0x0011)
#define CH1_GCAL_LSB_REG			(0x0012)
#define CH2_CFG_REG					(0x0013)
#define CH2_OCAL_MSB_REG			(0x0014)
#define CH2_OCAL_LSB_REG			(0x0015)
#define CH2_GCAL_MSB_REG			(0x0016)
#define CH2_GCAL_LSB_REG			(0x0017)
#define REGMAP_CRC_REG				(0x003E)
#define RESERVED_REG				(0x003F)


/************************** ADC SPI Routines **************************/
void initAdSpi(TIM_HandleTypeDef *handler);
bool isDataReady(void);
uint8_t readAds131mRegister (TIM_HandleTypeDef *timHandler, SPI_HandleTypeDef* spiHandler);

#endif /* INC_ADC_SPI_H_ */


