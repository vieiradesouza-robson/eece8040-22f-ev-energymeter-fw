/*******************************************************************************
  * File Name			: power.c
  * Description			: This module implements functions & wrapper related to
  * 					  the power hardware module.
  * 					  It shall be in charge of driving the enabling pins for
  * 					  the system.
  *
  *
  * Author				: Charlie Moreno, Robson Viera de Souza
  * Date				: September 27, 2021
  ******************************************************************************
  */

#include "power.h"
#include "ports.h"
#include "stdbool.h"

static bool waitingToTurnOff;

/************************** Primitives **************************/

/* Function      : getPowerEnState
 *
 * Description   : returns the power enable pin state.
 *
 * Parameters    : None
 *
 * Returns		 : returns the PWR_EN pin state
 */
static GPIO_PinState getPowerEnState(void)
{
	return getPinState (PWR_EN_GPIO_Port, PWR_EN_Pin);
}


/* Function      : setMcuPwrEn
 *
 * Description   : Sets MCU_PWR_EN pin to high state.
 *
 * Parameters    : None
 *
 * Returns		 : None
 */
static void setMcuPwrEn(void)
{
	setPin(MCU_PWR_EN_GPIO_Port, MCU_PWR_EN_Pin);
}

/* Function      : resetMcuPwrEn
 *
 * Description   : Sets MCU_PWR_EN pin to low state.
 *
 * Parameters    : None
 *
 * Returns		 : None
 */
static void resetMcuPwrEn(void)
{
	resetPin(MCU_PWR_EN_GPIO_Port, MCU_PWR_EN_Pin);
}

/************************** Power Module functions **************************/

/* Function      : initPowerModule
 *
 * Description   : Initializes the Power module variables and pins.
 *
 * Parameters    : None
 *
 * Returns		 : None
 */
void initPowerModule (void)
{
	setMcuPwrEn();
	waitingToTurnOff = false;
}

/* Function      : initPowerModule
 *
 * Description   : Initializes the Power module variables and pins.
 *
 * Parameters    : None
 *
 * Returns		 : None
 */
void checkPowerEnState (void)
{
	waitingToTurnOff = (bool)getPowerEnState();

	if(waitingToTurnOff) {
		//Turn off module if no pending task, do nothing otherwise.
//		if(!pendingTask()){
		resetMcuPwrEn();
//		} else {
		//An option might be to finish any pending task in
		//this section and shut down after that.
//		}
	} else {
		//Refresh Power Pins State and Variables
		setMcuPwrEn();
	}

}


/* Function      : isWaitingToTurnOff
 *
 * Description   : Gets the waitingToTurnOff value.
 *
 * Parameters    : None
 *
 * Returns		 : if the module is waiting to turn off or not.
 */
bool isWaitingToTurnOff (void)
{
	return waitingToTurnOff;
}

