/*******************************************************************************
  * File Name			: eeprom.c
  * Description			: This module implements high level functions related to
  * 					  the eeprom operation
  *
  * Author				: Charlie Moreno, Robson Viera de Souza
  * Creation Date		: October 18, 2021
  * Last Change Date	: October 18, 2021
  ******************************************************************************
  */

#include "eeprom.h"

uint8_t logBuffer[EEPROM_PAGESIZE] = {0x00};
uint16_t dataIndex = 0;
uint32_t writeAddr = 0;

uint8_t idBuffer[EEPROM_PAGESIZE] = {0x00};
uint32_t idAddr = 0;

logMetaData logList[EEPROM_MAX_LOG];
uint32_t logQty = 0;

uint8_t extraInfo[EEPROM_PARAMETERS_SIZE];

EepromOperations EEPROMgetLogMetaData(void)
{
	EepromOperations res = EEPROM_STATUS_COMPLETE;

	res = EEPROM_SPI_ReadID(idBuffer);

	if (res != EEPROM_STATUS_COMPLETE) {
		return res;
	}

	for (uint8_t i = 0; i < EEPROM_MAX_LOG * 3; i += 3) {
		if (i == 0) {
			logList[i/3].startAddress = 0;
		} else {
			logList[i/3].startAddress = logList[(i/3)-1].endAddress + 1;
		}

		logList[i/3].endAddress = idBuffer[i] + (idBuffer[i+1] << 8) + (idBuffer[i+2] << 16);

		if (logList[i/3].endAddress < logList[i/3].startAddress) {
			logList[i/3].size = 0;
		} else {
			logList[i/3].size = logList[i/3].endAddress - logList[i/3].startAddress + 1;
		}

		logQty = logList[i/3].size = 0 ? logQty : logQty + 1;
	}

	for (uint8_t i = EEPROM_MAX_LOG; i < EEPROM_PAGESIZE; i ++) {
		extraInfo[i-EEPROM_MAX_LOG] = idBuffer[i];
	}

	return res;
}

EepromOperations EEPROMstartLog(void)
{
	EepromOperations res = EEPROM_STATUS_COMPLETE;

	res = EEPROMgetLogMetaData();

	if (res != EEPROM_STATUS_COMPLETE) {
		return res;
	}

	//sets the start address for the next log to the next memory byte
	writeAddr = logQty == 0 ? 0 : logList[logQty-1].endAddress + 1;
	logList[logQty].startAddress = writeAddr;

	res = EEPROM_SPI_WriteID((uint8_t*) &logList[logQty].startAddress, (uint32_t)(logQty * 6), 3);

	return res;
}

EepromOperations EEPROMlogData(uint32_t timestamp, float voltage, float current)
{
	uint16_t voltage_int, current_int;
	EepromOperations res = EEPROM_STATUS_COMPLETE;

	voltage_int = (uint16_t)(voltage*100);
	current_int = (uint16_t)(current*100);

	logBuffer[dataIndex] = timestamp >> 24;
	dataIndex ++;
	logBuffer[dataIndex] = timestamp >> 16;
	dataIndex ++;
	logBuffer[dataIndex] = timestamp >> 8;
	dataIndex ++;
	logBuffer[dataIndex] = timestamp;
	dataIndex ++;
	logBuffer[dataIndex] = voltage_int >> 8;
	dataIndex ++;
	logBuffer[dataIndex] = voltage_int;
	dataIndex ++;
	logBuffer[dataIndex] = current_int >> 8;
	dataIndex ++;
	logBuffer[dataIndex] = current_int;
	dataIndex ++;

	if ((writeAddr + dataIndex)/EEPROM_PAGESIZE >= 1) {
		res = EEPROM_SPI_WriteBuffer(logBuffer, writeAddr, dataIndex);
		writeAddr = writeAddr + dataIndex;
		writeAddr = writeAddr <= EEPROM_MAX_ADDRESS ? writeAddr : 0;
		dataIndex = 0;
	}

	return res;
}

EepromOperations EEPROMendLog(void)
{
	EepromOperations res = EEPROM_STATUS_COMPLETE;

	res = EEPROM_SPI_WriteBuffer(logBuffer, writeAddr, dataIndex);

	if(res != EEPROM_STATUS_COMPLETE) {
		return res;
	}

	writeAddr = writeAddr + dataIndex;
	writeAddr = writeAddr <= EEPROM_MAX_ADDRESS ? writeAddr : 0;
	dataIndex = 0;

	logList[logQty].endAddress = writeAddr - 1;
	logList[logQty].size = logList[logQty].endAddress - logList[logQty].startAddress + 1;

	res = EEPROM_SPI_WriteID((uint8_t*) &logList[logQty].endAddress, logQty * 3, 3);

	logQty++;

	return res;
}

uint8_t *EEPROMextraInfo(void)
{
	return (uint8_t *)&extraInfo;
}
