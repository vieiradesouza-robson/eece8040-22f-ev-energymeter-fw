/*******************************************************************************
  * File Name			: spi.h
  * Description			: This module contains the definitions of constants and
  * 					  functions related to the spi protocol communication.
  *
  * Author				: Charlie Moreno, Robson Viera de Souza
  * Date				: September 27, 2021
  ******************************************************************************
  */
#ifndef INC_SPI_H_
#define INC_SPI_H_

#include "common.h"
#include "stm32l4xx_hal.h"
#include "stdbool.h"


/************************** SPI Primitives **************************/

void spiWrite(SPI_HandleTypeDef* handler, uint8_t* pData, uint16_t size, uint32_t tout);
void spiRead(SPI_HandleTypeDef* handler, uint8_t* pData, uint16_t size, uint32_t tout);
void checkAndConfigureSpiMode(SPI_HandleTypeDef* handler, uint8_t cpol, uint8_t cpha);

#endif /* INC_SPI_H_ */
