/*******************************************************************************
  * File Name			: integrity.c
  * Description			: This module implements functions & wrapper related to
  * 					  error and integrity handling.
  *
  * Author				: Charlie Moreno, Robson Viera de Souza
  * Date				: September 27, 2021
  ******************************************************************************
  */

#include "integrity.h"
#include "ui.h"
#include "power.h"

static ERROR_CODES currentStatus;

/* Function      : setCurrentStatus
 *
 * Description   : Sets the current status value.
 *
 * Parameters    :     status the ERROR_CODES value.
 *
 * Returns         : None
 */
void setCurrentStatus (ERROR_CODES status)
{
	currentStatus = status;
}

/* Function      : initCurrentStatus
 *
 * Description   : Initializes the current status value.
 *
 * Parameters    :  None.
 *
 * Returns         : None
 */
void initCurrentStatus ( void )
{
	setCurrentStatus(NO_ERROR);
}


/* Function      : getCurrentStatus
 *
 * Description   : Read the current status value.
 *
 * Parameters    :  None.
 *
 * Returns         : The current status value
 */
ERROR_CODES getCurrentStatus ( void )
{
	return currentStatus;
}


/* Function      : goToErrorMode
 *
 * Description   :  This function is executed in case of error occurrence.
 *
 * Parameters    :  error code.
 *
 * Returns         : The current status value
 */
void goToErrorMode(ERROR_CODES error)
{
//	__disable_irq();		//If IRQ are enabled, interrupts must be disabled
	if (getCurrentStatus() == NO_ERROR) {
		setCurrentStatus(error);
		printErrorMessage(error);		//TODO: Define this routine in the UI module
	}

}


/* Function      : houseKeep
 *
 * Description   : Calls periodic routines that must be executed to
 * 					keep the integrity of the system or to change
 * 					states.
 *
 * Parameters    : None.
 *
 * Returns		 : None.
 */
void houseKeep(void)
{
	checkPowerEnState();
}
