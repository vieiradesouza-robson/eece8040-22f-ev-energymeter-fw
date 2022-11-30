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
#include "eeprom.h"
#include "sd.h"
#include "adc.h"


#define HS_BUFFER_SIZE			250
#define LS_LOG_SAMPLE_INTERVAL	25
#define HS_LOG_SAMPLE_QTY		HS_BUFFER_SIZE/2

#define LOG_HV_VOLTAGE_THRSH	30.0

#define MAX_POWER		85000
#define MAX_VOLTAGE		600

typedef enum
{
    PENDING,
    LOG,
	DONOTLOG,
	LOGGED
} logDirective ;

typedef struct log{
	double 			current;
	double 			voltage;
	uint32_t 		timestamp;
	bool			ruleVoided;
	logDirective	directive;
} logElementTypedef;

void dataLogRoutine(uint32_t timestamp, double *ADCConvertedData, uint8_t *ADCnewData);

#endif /* INC_LOG_H_ */
