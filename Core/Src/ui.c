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

/* Function      : enterMenu
 *
 * Description   : Prints a menu text and executes its function
 *
 * Parameters    :  None.
 *
 * Returns         : None
 */
void enterMenu(void){
	if (currentScreen->f != NULL){
		currentScreen->f();
	}
	printf(currentScreen->menuText);
}

/* Function      : initUI
 *
 * Description   : Initializes UI menu items.
 *
 * Parameters    :  None.
 *
 * Returns         : None
 */
void initUI(void){
	mainMenu.parent = NULL;
	mainMenu.f = NULL;
	mainMenu.child[0] = &logMenu;
	mainMenu.child[1] = &configMenu;
	mainMenu.child[2] = &aboutMenu;
	mainMenu.child[3] = NULL;
	mainMenu.child[4] = NULL;
	strcpy(mainMenu.menuText, "\033[2J\033[HFSAEeBR Energy Meter\r\nVbeta\r\n1 - Logs\r\n2 - Configuration\r\n3 - About\r\n");

	logMenu.parent = &mainMenu;
	logMenu.f = NULL;
	logMenu.child[0] = &extractLogMenu;
	logMenu.child[1] = &clearEepromMenu;
	logMenu.child[2] = NULL;
	logMenu.child[3] = NULL;
	logMenu.child[4] = NULL;
	strcpy(logMenu.menuText, "\033[2J\033[HFSAEeBR Energy Meter\r\nVbeta\r\n1 - Extract log\r\n2 - Clear EEPROM\r\n");

	extractLogMenu.parent = &logMenu;
	extractLogMenu.f = NULL;
	extractLogMenu.child[0] = NULL;
	extractLogMenu.child[1] = NULL;
	extractLogMenu.child[2] = NULL;
	extractLogMenu.child[3] = NULL;
	extractLogMenu.child[4] = NULL;
	strcpy(extractLogMenu.menuText, "\033[2J\033[HFSAEeBR Energy Meter\r\nVbeta\r\nNot yet implemented.\r\n");

	clearEepromMenu.parent = &logMenu;
	clearEepromMenu.f = &initIdPage;
	clearEepromMenu.child[0] = NULL;
	clearEepromMenu.child[1] = NULL;
	clearEepromMenu.child[2] = NULL;
	clearEepromMenu.child[3] = NULL;
	clearEepromMenu.child[4] = NULL;
	strcpy(clearEepromMenu.menuText, "\033[2J\033[HFSAEeBR Energy Meter\r\nVbeta\r\nEEPROM erased.\r\n");

	configMenu.parent = &mainMenu;
	configMenu.f = NULL;
	configMenu.child[0] = &currentGainMenu;
	configMenu.child[1] = &voltageGainMenu;
	configMenu.child[2] = &shuntValueMenu;
	configMenu.child[3] = &voltageDividerMenu;
	configMenu.child[4] = NULL;
	strcpy(configMenu.menuText, "\033[2J\033[HFSAEeBR Energy Meter\r\nVbeta\r\n1 - Current gain\r\n2 - Voltage gain\r\n3 - Shunt value\r\n4 - Voltage divider\r\n");

	currentGainMenu.parent = &configMenu;
	currentGainMenu.f = NULL;
	currentGainMenu.child[0] = NULL;
	currentGainMenu.child[1] = NULL;
	currentGainMenu.child[2] = NULL;
	currentGainMenu.child[3] = NULL;
	currentGainMenu.child[4] = NULL;
	strcpy(currentGainMenu.menuText, "\033[2J\033[HFSAEeBR Energy Meter\r\nVbeta\r\nNot yet implemented.\r\n");

	voltageGainMenu.parent = &configMenu;
	voltageGainMenu.f = NULL;
	voltageGainMenu.child[0] = NULL;
	voltageGainMenu.child[1] = NULL;
	voltageGainMenu.child[2] = NULL;
	voltageGainMenu.child[3] = NULL;
	voltageGainMenu.child[4] = NULL;
	strcpy(voltageGainMenu.menuText, "\033[2J\033[HFSAEeBR Energy Meter\r\nVbeta\r\nNot yet implemented.\r\n");

	shuntValueMenu.parent = &configMenu;
	shuntValueMenu.f = NULL;
	shuntValueMenu.child[0] = NULL;
	shuntValueMenu.child[1] = NULL;
	shuntValueMenu.child[2] = NULL;
	shuntValueMenu.child[3] = NULL;
	shuntValueMenu.child[4] = NULL;
	strcpy(shuntValueMenu.menuText, "\033[2J\033[HFSAEeBR Energy Meter\r\nVbeta\r\nNot yet implemented.\r\n");

	voltageDividerMenu.parent = &configMenu;
	voltageDividerMenu.f = NULL;
	voltageDividerMenu.child[0] = NULL;
	voltageDividerMenu.child[1] = NULL;
	voltageDividerMenu.child[2] = NULL;
	voltageDividerMenu.child[3] = NULL;
	voltageDividerMenu.child[4] = NULL;
	strcpy(voltageDividerMenu.menuText, "\033[2J\033[HFSAEeBR Energy Meter\r\nVbeta\r\nNot yet implemented.\r\n");

	aboutMenu.parent = &mainMenu;
	aboutMenu.f = NULL;
	aboutMenu.child[0] = NULL;
	aboutMenu.child[1] = NULL;
	aboutMenu.child[2] = NULL;
	aboutMenu.child[3] = NULL;
	aboutMenu.child[4] = NULL;
	strcpy(aboutMenu.menuText, "\033[2J\033[HFSAEeBR Energy Meter\r\nVbeta\r\nDeveloped by:\r\nRobson Vieira de Souza\r\nCharlie Moreno\r\n");

	currentScreen = &mainMenu;
	enterMenu();
}

/* Function      : chosenChild
 *
 * Description   : Changes the meno to the child chosen from UART input
 *
 * Parameters    :  None.
 *
 * Returns         : None
 */
void chosenChild(char *choice){
	//checking if it's a valid option
	if ((choice[0] - '0') >= 1 && (choice[0] - '0') <= 5){
		//using '1' instead of '0' because the menu options are from 1 to 5 and the array index from 0 to 4
		if (currentScreen->child[choice[0] - '1'] != NULL){
			currentScreen = currentScreen->child[choice[0] - '1'];
			enterMenu();
			return;
		}
	} else if (choice[0] == 27){ //esc goes back one menu
		if (currentScreen->parent != NULL){
			currentScreen = currentScreen->parent;
		}
		enterMenu();
	}
}

/* Function      : userInput
 *
 * Description   : Gets the character input by the user on UART
 *
 * Parameters    :  None.
 *
 * Returns         : None
 */

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
