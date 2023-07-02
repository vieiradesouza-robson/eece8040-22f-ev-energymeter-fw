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

userInterfaceMenuTypedef *currentScreen;

userInterfaceMenuTypedef mainMenu;
userInterfaceMenuTypedef logMenu;
userInterfaceMenuTypedef extractLogMenu;
userInterfaceMenuTypedef clearEepromMenu;
userInterfaceMenuTypedef configMenu;
userInterfaceMenuTypedef currentGainMenu;
userInterfaceMenuTypedef voltageGainMenu;
userInterfaceMenuTypedef shuntValueMenu;
userInterfaceMenuTypedef voltageDividerMenu;
userInterfaceMenuTypedef aboutMenu;

void sendMemoryStat(){
	eepromStatisticsTypeDef eepromStat;

	getEEPROMstatistics(&eepromStat);
	printf("$239C5zAI/%lu/%.1f/%.2f\r\n", eepromStat.logQty, eepromStat.memoryOccupied, eepromStat.memoryRemaining);
}

/* Function      : uiCommand
 *
 * Description   : executes the command required by GUI
 *
 * Parameters    :  pointer to array with the UART data
 *
 * Returns       : None
 */

void uiCommand(uint8_t *rxData){

	if (!memcmp(rxData, "$239C5zAI", 9)){
		sendMemoryStat();

	} else if (!memcmp(rxData, "$simB4LmL", 9)){
		downloadLogsUART();

	} else if (!memcmp(rxData, "$ep8uBRMI", 9)){
		clearLogs();
		printf("$ep8uBRMI\r\n");
		sendMemoryStat();

	} else if (!memcmp(rxData, "$AIQvPX5u", 9)){
		printf("WIP: Read Parameters\r\n");

	} else if (!memcmp(rxData, "$S5lKne26", 9)){
		printf("WIP: Write Parameters\r\n");

	} else if (!memcmp(rxData, "$F6hMHnV1", 9)){
		printf("WIP: Reset Parameters\r\n");

	}
}

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
