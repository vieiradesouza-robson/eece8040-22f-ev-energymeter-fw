/*******************************************************************************
  * File Name			: eeprom.h
  * Description			: This module contains the definitions of constants and
  * 					  functions related to high level eeprom operations.
  * 					  Routines that will be called by other modules.
  *
  * Author				: Charlie Moreno, Robson Viera de Souza
  * Creation Date		: October 18, 2021
  * Last Change Date	: October 18, 2021
  ******************************************************************************
 */
#ifndef INC_EEPROM_H_
#define INC_EEPROM_H_

#include "common.h"
#include "main.h"
#include "eeprom_spi.h"
#include <stdio.h>

#define EEPROM_MAX_LOG			100 //maximum logs that will be stored in the EEPROM

//size (in Bytes) of the EEPROM identification page reserved for ADC calibration parameters and other stuff
#define EEPROM_PARAMETERS_SIZE	EEPROM_PAGESIZE - (3 * EEPROM_MAX_LOG)

typedef struct {
	uint32_t startAddress;
	uint32_t endAddress;
	uint32_t size;
} logMetaData;

typedef struct {
	uint32_t logQty;
	float memoryOccupied;
	float memoryRemaining;
} eepromStatisticsTypeDef;

EepromOperations EEPROMgetLogMetaData(void);
EepromOperations EEPROMstartLog(void);
EepromOperations EEPROMlogData(uint32_t timestamp, double voltage, double current);
EepromOperations EEPROMendLog(void);
EepromOperations EEPROMreadData(uint8_t* dataBuffer, uint32_t address, uint32_t size);
uint8_t *EEPROMextraInfo(void);
void getLogInfo(uint32_t logId, uint32_t* startAddress, uint32_t* endAddress, uint32_t* size);
void initIdPage(void);
void getEEPROMstatistics(eepromStatisticsTypeDef *eepromStat);
void clearLogs(void);
void downloadLogsUART(void);

#endif /* INC_EEPROM_H_ */
