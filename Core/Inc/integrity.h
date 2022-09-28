/*******************************************************************************
  * File Name			: integrity.h
  * Description			: This module contains the definitions of constants and
  * 					  functions related to the error handling.
  *
  * Author				: Charlie Moreno, Robson Viera de Souza
  * Date				: September 27, 2021
  ******************************************************************************
  */

#ifndef INC_INTEGRITY_H_
#define INC_INTEGRITY_H_

#include "common.h"


/************************** Error Handling Definition **************************/

typedef enum {
	NO_ERROR = 0,

} ERROR_CODES;


/************************** Error Handling Functions **************************/
ERROR_CODES getCurrentStatus ( void );
void initCurrentStatus ( void );
void setCurrentStatus (ERROR_CODES status);
void goToErrorMode(ERROR_CODES error);


#endif /* INC_INTEGRITY_H_ */

