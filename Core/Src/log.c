/*******************************************************************************
  * File Name			: log.c
  * Description			: This module implements functions & wrapper related to
  * 					  the log storage.
  *
  * Author				: Charlie Moreno, Robson Viera de Souza
  * Date				: September 27, 2021
  ******************************************************************************
  */

#include "log.h"

logElementTypedef logBuffer [HS_BUFFER_SIZE];

static bool initLog(void){
	return true;
}
