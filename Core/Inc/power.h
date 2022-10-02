/*******************************************************************************
  * File Name			: power.h
  * Description			: This module contains the definitions of constants and
  * 					  functions related to the power hardware module.
  *
  * Author				: Charlie Moreno, Robson Viera de Souza
  * Date				: September 27, 2021
  ******************************************************************************
  */
#ifndef INC_POWER_H_
#define INC_POWER_H_

#include "common.h"
#include "stm32l4xx_hal.h"
#include "stdbool.h"

/************************** Ports Definition **************************/

#define MCU_PWR_EN_GPIO_Port	(GPIOB)
#define MCU_PWR_EN_Pin			(GPIO_PIN_3)
#define PWR_EN_GPIO_Port		(GPIOA)
#define PWR_EN_Pin				(GPIO_PIN_5)


/************************** Power Routines **************************/
void initPowerModule (void);
void checkPowerEnState (void);
bool isWaitingToTurnOff (void);


#endif /* INC_POWER_H_ */



