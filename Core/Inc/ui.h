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
#include "integrity.h"


void printWelcomeMessage( void );
void printErrorMessage (ERROR_CODES error);

#endif /* INC_UI_H_ */


