/*******************************************************************************
  * File Name			: adc_spi.c
  * Description			: This module implements functions & wrapper related to
  * 					  the SPI protocol used to operate the external ADC
  * 					  peripheral. Shall be in charge of implementing the
  * 					  commands that must be sent to the external ADC module.
  *
  * Author				: Charlie Moreno, Robson Viera de Souza
  * Date				: September 27, 2021
  ******************************************************************************
  */

#include "adc_spi.h"
#include "spi.h"
#include "ports.h"


/* Function      : setAdCS
 *
 * Description   : Set CS signal.
 *
 * Parameters    :  None.
 *
 * Returns         : None
 */
static void setAdCS(void)
{
	setPin(ADC_NOT_CS_GPIO_Port, ADC_NOT_CS_Pin);
}

/* Function      : clearAdCS
 *
 * Description   : Reset CS signal.
 *
 * Parameters    :  None.
 *
 * Returns         : None
 */
static void clearAdCS(void)
{
	resetPin(ADC_NOT_CS_GPIO_Port, ADC_NOT_CS_Pin);
}

/* Function      : clearAdCS
 *
 * Description   : Reset CS signal.
 *
 * Parameters    :  None.
 *
 * Returns         : None
 */
bool isDataReady(void)
{
	return !getPinState(NOT_DRDY_GPIO_Port, NOT_DRDY_Pin);
}



/* Function      : initAdSpi
 *
 * Description   : Function to initialize ADC spi pins and registers
 *
 * Parameters    :  handler: Timer handler for HAL layer.
 *
 * Returns         : None
 */
void initAdSpi(TIM_HandleTypeDef *handler)
{
	setAdCS();	// CS is active Low
	HAL_TIM_PWM_Stop(handler, CLK_OUT_CHANNEL);	//Stop Clock signal

}

/* Function      : writeAdSpi
 *
 * Description   : Function to write through the SPI peripheral
 *
 * Parameters    :  timHandler: Timer handler for HAL layer.
 * 					spiHandler: SPI handler for HAL layer.
 * 					pData: Pointer to the data to be sent.
 * 					size: Size of data to be sent.
 * 					eot: End of transmission flag.
 *
 * Returns         : None
 */
void writeAdSpi(TIM_HandleTypeDef *timHandler, SPI_HandleTypeDef* spiHandler, uint8_t* pData, uint16_t size, bool eot)
{
	HAL_TIM_PWM_Start(timHandler, CLK_OUT_CHANNEL);	//Start Clock signal
	clearAdCS();	// CS is active Low
	spiWrite(spiHandler, pData, size, 100);
	if(eot) {
		setAdCS();
		HAL_TIM_PWM_Stop(timHandler, CLK_OUT_CHANNEL);
	}
}

/* Function      : readAdSpi
 *
 * Description   : Function to write through the SPI peripheral
 *
 * Parameters    :  timHandler: Timer handler for HAL layer.
 * 					spiHandler: SPI handler for HAL layer.
 * 					pData: Pointer to the buffer where the data will be stored.
 * 					size: Size of data to be received.
 * 					eot: End of transmission flag.
 *
 * Returns         : None
 */
void readAdSpi(TIM_HandleTypeDef *timHandler, SPI_HandleTypeDef* spiHandler, uint8_t* pData, uint16_t size, bool eot)
{
	HAL_TIM_PWM_Start(timHandler, CLK_OUT_CHANNEL);	//Start Clock signal
	clearAdCS();	// CS is active Low
	spiRead(spiHandler, pData, size, 100);
	if(eot) {
		setAdCS();
		HAL_TIM_PWM_Stop(timHandler, CLK_OUT_CHANNEL);
	}
}
