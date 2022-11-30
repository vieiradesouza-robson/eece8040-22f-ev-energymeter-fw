/*******************************************************************************
  * File Name			: log.h
  * Description			: This module contains the definitions of constants and
  * 					  functions related to the log storage.
  *
  * Author				: Charlie Moreno, Robson Viera de Souza
  * Date				: September 27, 2021
  ******************************************************************************
  */

#ifndef INC_LOG_H_
#define INC_LOG_H_

#include "common.h"
#include "main.h"
#include "stdbool.h"


#define HS_BUFFER_SIZE		240

typedef struct log{
	double current;
	double voltage;
	uint32_t timestamp;
	bool	ruleVoided;
	bool	log;
} logElementTypedef;

#endif /* INC_LOG_H_ */
