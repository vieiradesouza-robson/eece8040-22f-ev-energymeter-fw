/*******************************************************************************
  * File Name			: ports.c
  * Description			: This module implements functions & wrapper related to
  * 					  the I/O pins.
  *
  * Author				: Charlie Moreno, Robson Viera de Souza
  * Date				: September 27, 2021
  ******************************************************************************
  */

#include "ports.h"



/* Function      : writePin
 *
 * Description   : Set or clear the selected data port bit.
 *
 * Parameters    : 	GPIOx where x can be (A..H) to select the GPIO peripheral for STM32L4 family
 * 					GPIO_Pin specifies the port bit to be written. This parameter can be any combination of GPIO_Pin_x where x can be (0..15).
 * 					PinState specifies the value to be written to the selected bit.
 *         			This parameter can be one of the GPIO_PinState enum values:
 *           			GPIO_PIN_RESET: to clear the port pin
 * 			        	GPIO_PIN_SET: to set the port pin
 *
 * Returns		 : None
 */
static void writePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState)
{
	//It assumes that the parameters are correct
	if(PinState != GPIO_PIN_RESET) {
		GPIOx->BSRR = (uint32_t)GPIO_Pin;	// Sets the corresponding ODx bit
	} else {
		GPIOx->BRR = (uint32_t)GPIO_Pin;	// Reset the corresponding ODx bit
	}
}

/* Function      : setPin
 *
 * Description   : Sets the selected data port bit.
 *
 * Parameters    : 	GPIOx where x can be (A..H) to select the GPIO peripheral for STM32L4 family
 * 					GPIO_Pin specifies the port bit to be written. This parameter can be any combination of GPIO_Pin_x where x can be (0..15).
 *
 * Returns		 : None
 */
void setPin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
	writePin(GPIOx, GPIO_Pin, GPIO_PIN_SET);
}


/* Function      : resetPin
 *
 * Description   : Clear the selected data port bit.
 *
 * Parameters    : 	GPIOx where x can be (A..H) to select the GPIO peripheral for STM32L4 family
 * 					GPIO_Pin specifies the port bit to be written. This parameter can be any combination of GPIO_Pin_x where x can be (0..15).
 *
 * Returns		 : None
 */
void resetPin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
	writePin(GPIOx, GPIO_Pin, GPIO_PIN_RESET);
}


/* Function      : getPinState
 *
 * Description   : Read the specified input port pin.
 *
 * Parameters    : 	GPIOx where x can be (A..H) to select the GPIO peripheral for STM32L4 family
 * 					GPIO_Pin specifies the port bit to read. This parameter can be any combination of GPIO_Pin_x where x can be (0..15).
 *
 * Returns		 : The input port pin value.
 */
GPIO_PinState getPinState (GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{

	GPIO_PinState bitState;

	bitState = ((GPIOx->IDR & GPIO_Pin) != 0x00u) ? GPIO_PIN_SET : GPIO_PIN_RESET;

	return bitState;
}

