/*******************************************************************************
  * File Name			: spi.c
  * Description			: This module implements functions & wrapper related to
  * 					  processing of the SPI protocol primitives. Including
  * 					  the initialization and the routines to send and receive
  * 					  data.
  *
  * Author				: Charlie Moreno, Robson Viera de Souza
  * Date				: September 27, 2021
  ******************************************************************************
  */

#include "spi.h"
#include "integrity.h"


/* Function      : spiWrite
 *
 * Description   :  Primitive to transmit an amount of data in blocking mode..
 *
 * Parameters    :  handler pointer to a SPI_HandleTypeDef structure that contains
 *               the configuration information for SPI module.error code.
 *               pData pointer to data buffer.
 *               size amount of data to be sent.
 *               tout Timeout duration in ticks
 *
 * Returns         : None
 */
void spiWrite(SPI_HandleTypeDef* handler, uint8_t* pData, uint16_t size, uint32_t tout)
{
	//TODO: Later on the project we should implement our own library instead of using the HAL_LAYER

	HAL_StatusTypeDef errorCode = HAL_OK;

	errorCode = HAL_SPI_Transmit(handler, pData, size, tout);

	if ( errorCode != HAL_OK ) {
		goToErrorMode(SPI_WRITE_ERROR);
	}

}

/* Function      : spiRead
 *
 * Description   :  Primitive to transmit an amount of data in blocking mode..
 *
 * Parameters    :  handler pointer to a SPI_HandleTypeDef structure that contains
 *               the configuration information for SPI module.error code.
 *               size amount of data to be received.
 *               pData pointer to data buffer.
 *               tout Timeout duration in ticks
 *
 * Returns         : None
 */
void spiRead(SPI_HandleTypeDef* handler, uint8_t* pData, uint16_t size, uint32_t tout)
{
	//TODO: Later on the project we should implement our own library instead of using the HAL_LAYER

	HAL_StatusTypeDef errorCode = HAL_OK;

	errorCode = HAL_SPI_Receive(handler, pData, size, tout);

	if ( errorCode != HAL_OK ) {
		goToErrorMode(SPI_READ_ERROR);
	}

}

/* Function      : modifySpiMode
 *
 * Description   : Function to modify the SPI mode.
 *
 * Parameters    :  handler pointer to a SPI_HandleTypeDef structure that contains
 *               the configuration information for SPI module.error code.
 *               cpol clock polarity.
 *               cpha clock phase.
 *
 * Returns         : None
 */
void modifySpiMode(SPI_HandleTypeDef* handler, uint8_t cpol, uint8_t cpha)
{
	/* SPI parameter configuration*/

	handler->Init.CLKPolarity = (cpol == 0) ? SPI_POLARITY_LOW : SPI_POLARITY_HIGH;
	handler->Init.CLKPhase = (cpha == 0) ? SPI_PHASE_1EDGE : SPI_PHASE_2EDGE;

	if (HAL_SPI_Init(handler) != HAL_OK) {
		goToErrorMode(SPI_MODE_CONFG_ERROR);
	}

}

/* Function      : isSpiModeConfigured
 *
 * Description   : Function to check if the SPI mode is the desired.
 *
 * Parameters    :  handler pointer to a SPI_HandleTypeDef structure that contains
 *               the configuration information for SPI module.error code.
 *               cpol clock polarity.
 *               cpha clock phase.
 *
 * Returns         : True if the cpol and cpha matches, false otherwise.
 */
bool isSpiModeConfigured(SPI_HandleTypeDef* handler, uint8_t cpol, uint8_t cpha)
{
	/* SPI parameter configuration*/
	if(handler->Init.CLKPolarity != cpol || handler->Init.CLKPhase != cpha) {
		return false;
	}

	return true;

}
