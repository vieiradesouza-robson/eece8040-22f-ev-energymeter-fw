/*******************************************************************************
  * File Name			: sd.h
  * Description			: This module contains the definitions of constants and
  * 					  functions related to high level SD card operation
  *
  * Author				: Charlie Moreno, Robson Viera de Souza
  * Creation Date		: October 18, 2021
  * Last Change Date	: October 18, 2021
  ******************************************************************************
  */
#ifndef INC_SD_H_
#define INC_SD_H_

#include "common.h"
#include "fatfs.h"
#include <stdio.h>
#include "ff.h"
#include "sd_spi.h"

#define TRUE  1
#define FALSE 0
#define bool BYTE
#define MAXFILES 100

bool SDisFileOpen(void);
uint32_t SDfreeSpace(void);
FRESULT SDstartLog(void);
int16_t SDlogData(uint32_t timestamp, float voltage, float current);
FRESULT SDendLog(void);

#endif /* INC_SD_H_ */
