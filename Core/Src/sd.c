/*******************************************************************************
  * File Name			: sd.c
  * Description			: This module implements high level functions related to
  * 					  the SD card operation
  *
  * Author				: Charlie Moreno, Robson Viera de Souza
  * Creation Date		: October 18, 2021
  * Last Change Date	: October 18, 2021
  ******************************************************************************
  */

#include "sd.h"

FATFS fs;
FATFS *pfs;
FIL fil;
bool flagFileOpen = FALSE;

bool SDisFileOpen(void)
{
	return flagFileOpen;
}

uint32_t SDfreeSpace(void)
{
	DWORD fre_clust;
	uint32_t free;

	/* Check free space */
	if(f_getfree("", &fre_clust, &pfs) != FR_OK) {
		Error_Handler();
	}

	free = (uint32_t)(fre_clust * pfs->csize * 0.5);

	return free;
}

FRESULT SDstartLog(void)
{
	/* Mount SD Card */
	FRESULT res;
	uint8_t fileIndex = 0;
	char fileName[10] = "";

	res = f_mount(&fs, "", 0);

	if(res != FR_OK) {
		return res;
	}

	//creates a new file with the next index available, limited to index 99
	do {
		sprintf(fileName, "log%2d.csv", fileIndex);
		res = f_open(&fil, fileName, FA_CREATE_NEW | FA_READ | FA_WRITE);
		fileIndex++;
	} while(fileIndex < MAXFILES && res != FR_OK);

	if (res!=FR_OK) {
		return FR_INVALID_OBJECT;
	}

	//writes the first line with the headers
	f_puts("timestamp;voltage;current\n", &fil);

	flagFileOpen = TRUE;

	return res;
}

int16_t SDlogData(uint32_t timestamp, double voltage, double current)
{
	char str[27];
	uint32_t hour, minute, second, milisecond;

	//if the file is not open, exits
	if (!SDisFileOpen()) {
		return -1;
	}

	hour = timestamp/3600000;
	timestamp = timestamp%3600000;
	minute = timestamp/60000;
	timestamp = timestamp%60000;
	second = timestamp/1000;
	milisecond = timestamp%1000;

	sprintf(str, "%2lu:%2lu:%2lu.%3lu;%3.2f;%3.2f\n", hour, minute, second, milisecond, voltage, current);

	return (int16_t)f_puts(str, &fil);
}

FRESULT SDendLog(void)
{
	/* Mount SD Card */
	FRESULT res;

	/* Close file */
	if (SDisFileOpen()) {
		res = f_close(&fil);

		if(res != FR_OK) {
			return res;
		}
	}

	/* Unmount SDCARD */
	res = f_mount(NULL, "", 1);

	if(res != FR_OK) {
		return res;
	}

	flagFileOpen = FALSE;

	return res;
}

uint8_t SDgetLogList(uint32_t *files[MAXFILES])
{
	FRESULT res;
	uint8_t fileQty = 0;
	char fileName[11] = "";
	FILINFO fileInfo;

	do {
		sprintf(fileName, "/log%2d.csv", fileQty);
		res = f_stat(fileName,&fileInfo);
		*files[fileQty] = (uint32_t)fileInfo.fsize;
		fileQty++;
	} while(fileQty < MAXFILES && res != FR_OK);

	return fileQty;
}
