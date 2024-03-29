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

logElementTypedef dataLogBuffer [HS_BUFFER_SIZE];
uint16_t bufferHead = 0;
uint16_t bufferTail = 0;
bool isLogging = false;
uint32_t samplesAcquired = 0;
uint16_t samplesToLog = 0;
uint32_t logStartTimestamp = 0;
bool logEndRequested = false;

//initializes the log buffer, resets buffer head and tail and sets the flag that indicates if it's logging
void logStart(void){

	bufferHead = 0;
	bufferTail = 0;

	for (uint8_t i = 0; i < HS_BUFFER_SIZE; i++){
		dataLogBuffer[i].current = 0.0;
		dataLogBuffer[i].voltage = 0.0;
		dataLogBuffer[i].directive = PENDING;
		dataLogBuffer[i].ruleVoided = false;
		dataLogBuffer[i].timestamp = 0;
	}

	EEPROMstartLog();

	isLogging = true;

	printf("[log.c]Log started.\n\r");
}

void requestLogEnd(){
	logEndRequested = true;
}

bool isLoggingOn(){
	return isLogging;
}

bool checkRule(double voltage, double current){
	double power;
	power = voltage * current;
	if (power > MAX_POWER){
		return true;
	}

	if (voltage > MAX_VOLTAGE){
		return true;
	}

	return false;
}

uint8_t bufferSize(void){

	if(bufferHead != bufferTail)
	{
		if(bufferHead >= bufferTail)
		{
			return (bufferHead - bufferTail);
		}
		else
		{
			return (HS_BUFFER_SIZE + bufferHead - bufferTail);
		}
	}

	return 0;
}

void addToBuffer(uint32_t timestamp, double voltage, double current){

	uint8_t i;

	dataLogBuffer[bufferHead].current = current;
	dataLogBuffer[bufferHead].voltage = voltage;
	dataLogBuffer[bufferHead].timestamp = timestamp;
	dataLogBuffer[bufferHead].ruleVoided = checkRule(voltage, current);

	if(dataLogBuffer[bufferHead].ruleVoided){ //if any rule is voided, it marks all samples from tail to head as LOG

		dataLogBuffer[bufferHead].directive = LOG;

		i = bufferHead - 1;

		while (i != bufferTail - 1){
			if (dataLogBuffer[i].directive == PENDING){
				dataLogBuffer[i].directive = LOG;
			}
			i = (i == 0) ? (HS_BUFFER_SIZE - 1) : i - 1;
		}

		samplesToLog = HS_LOG_SAMPLE_QTY/2; //sets samplesToLog so the next samples are marked as LOG immediately

		printf("[log.c]Rule voided:\n\r");
		printf("[log.c]Voltage = %.2f | Current = %.2f\n\r", dataLogBuffer[bufferHead].voltage, dataLogBuffer[bufferHead].current);

	} else if (samplesToLog > 0){

		dataLogBuffer[bufferHead].directive = LOG;
		samplesToLog--;

	} else {

		dataLogBuffer[bufferHead].directive = PENDING;

	}

	if (bufferSize() >= HS_LOG_SAMPLE_QTY/2 && dataLogBuffer[bufferTail].directive == PENDING){
		if (bufferTail % LS_LOG_SAMPLE_INTERVAL == 0){
			dataLogBuffer[bufferTail].directive = LOG;
		} else {
			dataLogBuffer[bufferTail].directive = DONOTLOG;
		}
	}

	bufferHead = (++bufferHead == HS_BUFFER_SIZE) ? 0 : bufferHead;

	if (bufferHead == bufferTail){
		bufferTail = (++bufferTail == HS_BUFFER_SIZE) ? 0 : bufferTail;
	}
}

void logToMemory(void){
	while (dataLogBuffer[bufferTail].directive != PENDING && bufferTail != bufferHead){
		if (dataLogBuffer[bufferTail].directive == LOG){

			EEPROMlogData(dataLogBuffer[bufferTail].timestamp, dataLogBuffer[bufferTail].voltage, dataLogBuffer[bufferTail].current);

			dataLogBuffer[bufferTail].directive = LOGGED;

			printf("[log.c]Data logged:\n\r");
			printf("[log.c]Voltage = %.2f | Current = %.2f\n\r", dataLogBuffer[bufferTail].voltage, dataLogBuffer[bufferTail].current);
		}
		bufferTail = (++bufferTail == HS_BUFFER_SIZE) ? 0 : bufferTail;
	}
}

void logEnd(void){

	uint8_t i = bufferHead - 1;

	while (i != bufferTail - 1){
		if (dataLogBuffer[bufferTail].directive == PENDING){
			if (bufferTail % LS_LOG_SAMPLE_INTERVAL == 0){
				dataLogBuffer[bufferTail].directive = LOG;
			} else {
				dataLogBuffer[bufferTail].directive = DONOTLOG;
			}
		}
		i = (i == 0) ? (HS_BUFFER_SIZE - 1) : i - 1;
	}

	logToMemory();

	EEPROMendLog();

	isLogging = false;
	printf("[log.c]Log ended.\n\r");
}

void dataLogRoutine(uint32_t timestamp, uint8_t *ADCnewData){

	double *ADCConvertedData = NULL;

	if (*ADCnewData){

		ADCConvertedData = getADCConvertedData();

		if (!isLogging && ADCConvertedData[HV_VOLTAGE_CH] >= LOG_HV_VOLTAGE_THRSH){
			printf("[log.c]Starting log.\n\r");
			logStart();
			logStartTimestamp = timestamp;
		}

		if (isLogging) {
			addToBuffer((timestamp - logStartTimestamp), ADCConvertedData[HV_VOLTAGE_CH], ADCConvertedData[HV_CURRENT_CH]);
			logToMemory();

			if (ADCConvertedData[HV_VOLTAGE_CH] < LOG_HV_VOLTAGE_THRSH){
				printf("[log.c]Ending log.\n\r");
				logEnd();
			}
		}

		*ADCnewData = 0;
	} else if (logEndRequested && isLogging){
		printf("[log.c]Ending log.\n\r");
		logEnd();
	}
}
