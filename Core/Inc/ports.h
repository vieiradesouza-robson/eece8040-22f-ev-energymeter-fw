/*******************************************************************************
  * File Name			: ports.h
  * Description			: This module contains the definitions of constants and
  * 					  functions related to the GPIO pins.
  *
  * Author				: Charlie Moreno, Robson Viera de Souza
  * Date				: September 27, 2021
  ******************************************************************************
  */

#ifndef INC_PORTS_H_
#define INC_PORTS_H_



#endif /* INC_PORTS_H_ */

#include "common.h"
#include "stm32l4xx_hal.h"

/************************** Ports Definition **************************/

/* RGB Led */
#define LED_R_PORT		(GPIOA)
#define LED_R_PIN		(GPIO_PIN_9)
#define LED_G_PORT		(GPIOA)
#define LED_G_PIN		(GPIO_PIN_10)
#define LED_B_PORT		(GPIOA)
#define LED_B_PIN		(GPIO_PIN_8)


/************************** Ports Routines **************************/

void setPin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void resetPin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
GPIO_PinState getPinState (GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
