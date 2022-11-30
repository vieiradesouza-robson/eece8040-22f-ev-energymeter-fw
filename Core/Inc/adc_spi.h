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

#include "main.h"
#include <stdio.h>

#define ADC_WORD_SIZE	24
#define ADC_TIMEOUT		100

//commands mapping
#define DUMMY		0x00
#define RESET		0x0011
#define STANDBY		0x0022
#define WAKEUP		0x0033
#define LOCK		0x0555
#define UNLOCK		0x0655
#define RREG		0xA000
#define WREG		0x6000

//response mapping
#define RREG_RES	0xE000
#define WREG_RES	0x4000

//registers mapping
#define ID				0x00
#define STATUS			0x01
#define MODE			0x02
#define CLOCK			0x03
#define GAIN			0x04
#define CFG				0x06
#define THRSHLD_MSB		0x07
#define THRSHLD_LSB		0x08
#define CH0_CFG			0x09
#define CH0_OCAL_MSB	0x0A
#define CH0_OCAL_LSB	0x0B
#define CH0_GCAL_MSB	0x0C
#define CH0_GCAL_LSB	0x0D
#define CH1_CFG			0x0E
#define CH1_OCAL_MSB	0x0F
#define CH1_OCAL_LSB	0x10
#define CH1_GCAL_MSB	0x11
#define CH1_GCAL_LSB	0x12
#define CH2_CFG			0x13
#define CH2_OCAL_MSB	0x14
#define CH2_OCAL_LSB	0x15
#define CH2_GCAL_MSB	0x16
#define CH2_GCAL_LSB	0x17
#define REGMAP_CRC		0x3E

//MODE register default values
#define MODE_REG_CRC_EN		0x000000
#define MODE_RX_CRC_EN		0x000000
#define MODE_CRC_TYPE		0x000000
#define MODE_RESET			0x040000
#define MODE_WLENGTH		0x010000
#define MODE_TIMEOUT		0x001000
#define MODE_DRDY_SEL		0x000000
#define MODE_DRDY_HIZ		0x000000
#define MODE_DRDY_FMT		0x000000

//MODE register used values
#define MODE_RESET_CLEAR	0x000000
#define MODE_WLENGTH_16		0x000000
#define MODE_WLENGTH_24		0x010000
#define MODE_WLENGTH_32_L	0x020000
#define MODE_WLENGTH_32_M	0x030000
#define MODE_DRDY_FMT_PULSE	0x000100

//CLOCK register default values
#define CLOCK_CH2_EN		0x040000
#define CLOCK_CH1_EN		0x020000
#define CLOCK_CH0_EN		0x010000
#define CLOCK_TBM			0x000000
#define CLOCK_OSR			0x000C00
#define CLOCK_PWR			0x000200

//CLOCK register used values
#define CLOCK_CH2_DIS		0x000000
#define CLOCK_CH1_DIS		0x000000
#define CLOCK_CH0_DIS		0x000000
#define CLOCK_OSR_16256		0x001C00

//GAIN register default values
#define GAIN_PGA_GAIN2_1	0X000000
#define GAIN_PGA_GAIN1_1	0X000000
#define GAIN_PGA_GAIN0_1	0X000000

//GAIN register used values
#define GAIN_PGA_GAIN_2		1
#define GAIN_PGA_GAIN_4		2
#define GAIN_PGA_GAIN_8		3
#define GAIN_PGA_GAIN_16	4
#define GAIN_PGA_GAIN_32	5
#define GAIN_PGA_GAIN_64	6
#define GAIN_PGA_GAIN_128	7

#define ADC_CPOL			(SPI_POLARITY_LOW)
#define ADC_CPHA			(SPI_PHASE_2EDGE)

uint8_t *ADCrawChannels(void);
uint8_t ADCwriteReg(uint8_t reg, uint32_t data);
uint8_t ADCinit(SPI_HandleTypeDef * hspi);

#endif /* INC_ADC_SPI_H_ */

#include "common.h"
