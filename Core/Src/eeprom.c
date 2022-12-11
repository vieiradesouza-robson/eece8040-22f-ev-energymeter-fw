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
#include "string.h"

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

	logQty = 0;

	for (uint16_t i = 0; i < EEPROM_MAX_LOG * 3; i += 3) {
		if (i == 0) {
			logList[i/3].startAddress = 0;
		} else {
			logList[i/3].startAddress = logList[(i/3)-1].endAddress == 0 ? 0 : logList[(i/3)-1].endAddress + 1;
		}

		logList[i/3].endAddress = idBuffer[i] + (idBuffer[i+1] << 8) + (idBuffer[i+2] << 16);

		if (logList[i/3].endAddress <= logList[i/3].startAddress) {
			logList[i/3].size = 0;
		} else {
			logList[i/3].size = logList[i/3].endAddress - logList[i/3].startAddress + 1;
		}

		logQty = logList[i/3].size == 0 ? logQty : logQty + 1;
	}

	for (uint16_t i = (3 * EEPROM_MAX_LOG); i < EEPROM_PAGESIZE; i ++) {
		extraInfo[i-(3 * EEPROM_MAX_LOG)] = idBuffer[i];
	}

	return res;
}

void getEEPROMstatistics(eepromStatisticsTypeDef *eepromStat){

	EEPROMgetLogMetaData();

	if(logQty != 0){
		eepromStat->logQty = logQty;
		eepromStat->memoryOccupied = (((float)logList[logQty-1].endAddress)/128.0);
		eepromStat->memoryRemaining = 4.0-(eepromStat->memoryOccupied / 1000.0);
	} else {
		eepromStat->logQty = 0;
		eepromStat->memoryOccupied = 0.0;
		eepromStat->memoryRemaining = 4.0-(eepromStat->memoryOccupied / 1000.0);
	}
}

void clearLogs(void){
	memset(idBuffer, 0x00, (EEPROM_PAGESIZE - (EEPROM_PARAMETERS_SIZE)));
	EEPROM_SPI_WriteID(idBuffer, 0x00000000, (EEPROM_PAGESIZE - (EEPROM_PARAMETERS_SIZE)));
}

void downloadLogsUART(void){
	uint8_t logData[EEPROM_PAGESIZE];
	uint32_t readAdd;
	uint16_t byteQty;
	float voltage, current;
	uint32_t timestamp;
	char fileName[9];
	uint32_t totalSamples, samplesSent;

	EEPROMgetLogMetaData();

	printf("$simB4LmL/SS\r\n"); //starting data stream

	for (uint8_t i = 0; i<logQty; i++){

		totalSamples = logList[i].size/8;
		samplesSent = 0;

		sprintf(fileName, "log%02d\r\n", i);
		printf("$simB4LmL/BOF/%s", fileName); //starting new file
		printf("$simB4LmL/LD/%lu\r\n", totalSamples + 2); //file header: putting log size in first line
		printf("$simB4LmL/LD/timestamp,voltage,current\r\n"); //file header: fields label

		readAdd = logList[i].startAddress;

		while (readAdd < logList[i].endAddress){
			byteQty = (readAdd + EEPROM_PAGESIZE) <= logList[i].endAddress ? EEPROM_PAGESIZE : (logList[i].endAddress - readAdd + 1);
			EEPROM_SPI_ReadBuffer(logData, readAdd, byteQty);
			for (uint16_t j = 0; j<EEPROM_PAGESIZE && samplesSent<totalSamples; j+=8){
				timestamp = (logData[j] << 24) + (logData[j+1] << 16) + (logData[j+2] << 8) + logData[j+3];
				voltage = ((float)((logData[j+4] << 8) + logData[j+5]))/100.0;
				current = ((float)((logData[j+6] << 8) + logData[j+7]))/100.0;
				printf("$simB4LmL/LD/%lu,%.2f,%.2f\r\n", timestamp, voltage, current);
				samplesSent++;
			}
			readAdd = readAdd + byteQty;
		}

		printf("$simB4LmL/EOF/%s", fileName); //ending file
	}

	printf("$simB4LmL/ES\r\n"); //ending data stream
}

void initIdPage(void)
{
	memset(idBuffer, 0x00, sizeof(idBuffer));
	EEPROM_SPI_WriteID(idBuffer, 0x00000000, sizeof(idBuffer)/sizeof(uint8_t));
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

EepromOperations EEPROMlogData(uint32_t timestamp, double voltage, double current)
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

	if (((writeAddr % EEPROM_PAGESIZE) + dataIndex)/EEPROM_PAGESIZE >= 1) {

		printf("[eeprom.c]Writing new page to eeprom address: %lu\n\r", writeAddr);

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
	if(logList[logQty].startAddress < logList[logQty].endAddress) {
		logList[logQty].size = logList[logQty].endAddress - logList[logQty].startAddress + 1;
	} else {
		//To account for the case when writeAddr is greater than EEPROM_MAX_ADDRESS and it goes back to 0
		logList[logQty].size = logList[logQty].endAddress - (logList[logQty].startAddress + EEPROM_MAX_ADDRESS)%EEPROM_MAX_ADDRESS +1;
	}

	res = EEPROM_SPI_WriteID((uint8_t*) &logList[logQty].endAddress, logQty * 3, 3);

	logQty++;

	printf("[eeprom.c]Log ended.\n\r");

	return res;
}

uint8_t *EEPROMextraInfo(void)
{
	return &extraInfo[0];
}

EepromOperations EEPROMreadData(uint8_t* dataBuffer, uint32_t address, uint32_t size)
{
	EepromOperations res = EEPROM_STATUS_COMPLETE;
	// The EndLog routine shall be called before the reading function to make
	// sure that the information has been stored in the EEPROM

	//Read from memory
	EEPROM_SPI_ReadBuffer(dataBuffer, address, size);

	return res;
}

void getLogInfo(uint32_t logId, uint32_t* startAddress, uint32_t* endAddress, uint32_t* size)
{
	// The EndLog routine shall be called before the getLogInfo function to make
	// sure that the proper information has been updated
	*startAddress = logList[logId].startAddress;
	*endAddress = logList[logId].endAddress;
	*size = logList[logId].size;
}
