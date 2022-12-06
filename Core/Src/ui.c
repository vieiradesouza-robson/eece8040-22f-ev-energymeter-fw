/*******************************************************************************
  * File Name			: ui.c
  * Description			: This module implements functions & wrapper related to
  * 					  the user interface.
  *
  * Author				: Charlie Moreno, Robson Viera de Souza
  * Date				: September 27, 2021
  ******************************************************************************
  */

#include "ui.h"


/* Function      : printWelcomeMessage
 *
 * Description   : Prints the welcome message through the standard IO.
 *
 * Parameters    :  None.
 *
 * Returns         : None
 */
void printWelcomeMessage( void )
{
	printf("\n\n\rWelcome to the FSAE Electric Energy Meter Demo.\n\r");
}

/* Function      : printErrorMessage
 *
 * Description   : Prints the error message through the standard IO with the
 * 					corresponding error code.
 *
 * Parameters    :  error - Error code.
 *
 * Returns         : None
 */
void printErrorMessage (ERROR_CODES error)
{
	printf("\n\n\rUnexpected state, Error code: %d", error);
}
