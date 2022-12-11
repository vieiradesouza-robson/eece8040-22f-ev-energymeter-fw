/*******************************************************************************
  * File Name			: ui.h
  * Description			: This module contains the definitions of constants and
  * 					  functions related to the user interface.
  *
  * Author				: Charlie Moreno, Robson Viera de Souza
  * Date				: September 27, 2021
  ******************************************************************************
  */
#ifndef INC_UI_H_
#define INC_UI_H_

#include "common.h"
#include "stdio.h"
#include "string.h"
#include "integrity.h"
#include "stdbool.h"
#include "eeprom.h"

typedef struct userInterfaceMenu{
	struct userInterfaceMenu *parent;
	char menuText[150];
	struct userInterfaceMenu *child[5];
	void (*f)(void);
} userInterfaceMenuTypedef;

void printWelcomeMessage( void );
void printErrorMessage (ERROR_CODES error);
void uiCommand(uint8_t *rxData);

#endif /* INC_UI_H_ */


