/*******************************************************************************
  * File Name			: memory_spi.c
  * Description			: This module implements functions & wrapper related to
  * 					  the SPI protocol used to operate the external eeprom
  * 					  peripheral. Shall be in charge of implementing the
  * 					  commands that must be sent to the external memory.
  *
  * Author				: Charlie Moreno, Robson Viera de Souza
  * Creation Date		: September 27, 2021
  * Last Change Date	: October 18, 2021
  ******************************************************************************
  */

#include <eeprom_spi.h>
#include "spi.h"

extern SPI_HandleTypeDef	hspi1;
SPI_HandleTypeDef * EEPROM_SPI;

/**
 * @brief Init EEPROM SPI
 *
 * @param hspi Pointer to SPI struct handler
 */
void EEPROM_SPI_INIT(void)
{
	EEPROM_SPI = &hspi1;
}

/**
  * @brief  Writes more than one byte to the EEPROM with a single WRITE cycle
  *         (Page WRITE sequence).
  *
  * @note   The number of bytes can't exceed the EEPROM page size.
  * @param  pBuffer: pointer to the buffer  containing the data to be written
  *         to the EEPROM.
  * @param  WriteAddr: EEPROM's internal address to write to.
  * @param  NumByteToWrite: number of bytes to write to the EEPROM, must be equal
  *         or less than "EEPROM_PAGESIZE" value.
  * @retval EepromOperations value: EEPROM_STATUS_COMPLETE or EEPROM_STATUS_ERROR
  */
EepromOperations EEPROM_SPI_WritePage(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
	checkAndConfigureSpiMode(EEPROM_SPI, EEPROM_CPOL, EEPROM_CPHA);		//Check the SPI configuration before doing any Tx or Rx

	while (EEPROM_SPI->State != HAL_SPI_STATE_READY) {
		HAL_Delay(1);
	}

	HAL_StatusTypeDef spiTransmitStatus;

	sEE_WriteEnable();

	//the instruction is sent in a package of 4 bytes, one is the write instruction itself and the other 3 the address
	uint8_t header[4];

	header[0] = EEPROM_WRITE;    // Send "Write to Memory" instruction
	header[1] = WriteAddr >> 16; // Send 24-bit address
	header[2] = WriteAddr >> 8;
	header[3] = WriteAddr;

	//Select the EEPROM: Chip Select low
	EEPROM_CS_LOW();

	EEPROM_SPI_SendInstruction((uint8_t*)header, 4);

	//Makes 5 attempts to write the data
	for (uint8_t i = 0; i < 5; i++) {
		spiTransmitStatus = HAL_SPI_Transmit(EEPROM_SPI, pBuffer, NumByteToWrite, 100);

		if (spiTransmitStatus == HAL_BUSY) {
			HAL_Delay(5);
		} else {
			break;
		}
	}

	//Deselect the EEPROM: Chip Select high
	EEPROM_CS_HIGH();

	//Wait the end of EEPROM writing
	EEPROM_SPI_WaitStandbyState();

	//Disable the write access to the EEPROM
	sEE_WriteDisable();

	if (spiTransmitStatus == HAL_ERROR) {
		return EEPROM_STATUS_ERROR;
	} else {
		return EEPROM_STATUS_COMPLETE;
	}
}

/**
  * @brief  Writes block of data to the EEPROM. In this function, the number of
  *         WRITE cycles are reduced, using Page WRITE sequence.
  *
  * @param  pBuffer: pointer to the buffer  containing the data to be written
  *         to the EEPROM.
  * @param  WriteAddr: EEPROM's internal address to write to.
  * @param  NumByteToWrite: number of bytes to write to the EEPROM.
  * @retval EepromOperations value: EEPROM_STATUS_COMPLETE or EEPROM_STATUS_ERROR
  */
EepromOperations EEPROM_SPI_WriteBuffer(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
	uint32_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;
	uint32_t sEE_DataNum = 0;

	checkAndConfigureSpiMode(EEPROM_SPI, EEPROM_CPOL, EEPROM_CPHA);		//Check the SPI configuration before doing any Tx or Rx

	EepromOperations pageWriteStatus = EEPROM_STATUS_PENDING;

	Addr = WriteAddr % EEPROM_PAGESIZE;
	count = EEPROM_PAGESIZE - Addr;
	NumOfPage =  NumByteToWrite / EEPROM_PAGESIZE;
	NumOfSingle = NumByteToWrite % EEPROM_PAGESIZE;

	if (Addr == 0) { /* WriteAddr is EEPROM_PAGESIZE aligned  */
		if (NumOfPage == 0) { /* NumByteToWrite < EEPROM_PAGESIZE */
			sEE_DataNum = NumByteToWrite;
			pageWriteStatus = EEPROM_SPI_WritePage(pBuffer, WriteAddr, sEE_DataNum);

			if (pageWriteStatus != EEPROM_STATUS_COMPLETE) {
				return pageWriteStatus;
			}

		} else { /* NumByteToWrite >= EEPROM_PAGESIZE */
			while (NumOfPage--) {
				sEE_DataNum = EEPROM_PAGESIZE;
				pageWriteStatus = EEPROM_SPI_WritePage(pBuffer, WriteAddr, sEE_DataNum);

				if (pageWriteStatus != EEPROM_STATUS_COMPLETE) {
					return pageWriteStatus;
				}

				WriteAddr +=  EEPROM_PAGESIZE;
				pBuffer += EEPROM_PAGESIZE;
			}

			sEE_DataNum = NumOfSingle;
			pageWriteStatus = EEPROM_SPI_WritePage(pBuffer, WriteAddr, sEE_DataNum);

			if (pageWriteStatus != EEPROM_STATUS_COMPLETE) {
				return pageWriteStatus;
			}
		}
	} else { /* WriteAddr is not EEPROM_PAGESIZE aligned  */
		if (NumOfPage == 0) { /* NumByteToWrite < EEPROM_PAGESIZE */
			if (NumOfSingle > count) { /* (NumByteToWrite + WriteAddr) > EEPROM_PAGESIZE */
				temp = NumOfSingle - count;
				sEE_DataNum = count;
				pageWriteStatus = EEPROM_SPI_WritePage(pBuffer, WriteAddr, sEE_DataNum);

				if (pageWriteStatus != EEPROM_STATUS_COMPLETE) {
					return pageWriteStatus;
				}

				WriteAddr +=  count;
				pBuffer += count;

				sEE_DataNum = temp;
				pageWriteStatus = EEPROM_SPI_WritePage(pBuffer, WriteAddr, sEE_DataNum);
			} else {
				sEE_DataNum = NumByteToWrite;
				pageWriteStatus = EEPROM_SPI_WritePage(pBuffer, WriteAddr, sEE_DataNum);
			}

			if (pageWriteStatus != EEPROM_STATUS_COMPLETE) {
				return pageWriteStatus;
			}
		} else { /* NumByteToWrite > EEPROM_PAGESIZE */
			NumByteToWrite -= count;
			NumOfPage =  NumByteToWrite / EEPROM_PAGESIZE;
			NumOfSingle = NumByteToWrite % EEPROM_PAGESIZE;

			sEE_DataNum = count;

			pageWriteStatus = EEPROM_SPI_WritePage(pBuffer, WriteAddr, sEE_DataNum);

			if (pageWriteStatus != EEPROM_STATUS_COMPLETE) {
				return pageWriteStatus;
			}

			WriteAddr +=  count;
			pBuffer += count;

			while (NumOfPage--) {
				sEE_DataNum = EEPROM_PAGESIZE;

				pageWriteStatus = EEPROM_SPI_WritePage(pBuffer, WriteAddr, sEE_DataNum);

				if (pageWriteStatus != EEPROM_STATUS_COMPLETE) {
					return pageWriteStatus;
				}

				WriteAddr +=  EEPROM_PAGESIZE;
				pBuffer += EEPROM_PAGESIZE;
			}

			if (NumOfSingle != 0) {
				sEE_DataNum = NumOfSingle;

				pageWriteStatus = EEPROM_SPI_WritePage(pBuffer, WriteAddr, sEE_DataNum);

				if (pageWriteStatus != EEPROM_STATUS_COMPLETE) {
					return pageWriteStatus;
				}
			}
		}
	}

	return EEPROM_STATUS_COMPLETE;
}

/**
  * @brief  Reads a block of data from the EEPROM.
  *
  * @param  pBuffer: pointer to the buffer that receives the data read from the EEPROM.
  * @param  ReadAddr: EEPROM's internal address to read from.
  * @param  NumByteToRead: number of bytes to read from the EEPROM.
  * @retval EepromOperations value: EEPROM_STATUS_COMPLETE or EEPROM_STATUS_ERROR
  */
EepromOperations EEPROM_SPI_ReadBuffer(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead)
{
	checkAndConfigureSpiMode(EEPROM_SPI, EEPROM_CPOL, EEPROM_CPHA);		//Check the SPI configuration before doing any Tx or Rx

	while (EEPROM_SPI->State != HAL_SPI_STATE_READY) {
		HAL_Delay(1);
	}

	/*
	    We gonna send all commands in one packet of 3 bytes
	 */

	uint8_t header[4];

	header[0] = EEPROM_READ;    // Send "Read from Memory" instruction
	header[1] = ReadAddr >> 16;  // Send 24-bit address
	header[2] = ReadAddr >> 8;
	header[3] = ReadAddr;

	// Select the EEPROM: Chip Select low
	EEPROM_CS_LOW();

	/* Send WriteAddr address byte to read from */
	EEPROM_SPI_SendInstruction(header, 4);

	while (HAL_SPI_Receive(EEPROM_SPI, (uint8_t*)pBuffer, NumByteToRead, 200) == HAL_BUSY) {
		HAL_Delay(1);
	};

	// Deselect the EEPROM: Chip Select high
	EEPROM_CS_HIGH();

	return EEPROM_STATUS_COMPLETE;
}

/**
  * @brief  Reads identification page from the EEPROM.
  *
  * @param  pBuffer: pointer to the buffer that receives the data read from the EEPROM identification page.
  * @retval EepromOperations value: EEPROM_STATUS_COMPLETE or EEPROM_STATUS_ERROR
  */
EepromOperations EEPROM_SPI_ReadID(uint8_t* pBuffer)
{
	checkAndConfigureSpiMode(EEPROM_SPI, EEPROM_CPOL, EEPROM_CPHA);		//Check the SPI configuration before doing any Tx or Rx

	while (EEPROM_SPI->State != HAL_SPI_STATE_READY) {
		HAL_Delay(1);
	}

	uint8_t header[4];

	header[0] = EEPROM_READ_ID;    // Send "Read identification page" instruction
	header[1] = 0;  // Start on address zero since we are reading the whole page
	header[2] = 0;
	header[3] = 0;

	// Select the EEPROM: Chip Select low
	EEPROM_CS_LOW();

	/* Send WriteAddr address byte to read from */
	EEPROM_SPI_SendInstruction(header, 4);

	while (HAL_SPI_Receive(EEPROM_SPI, (uint8_t*)pBuffer, EEPROM_PAGESIZE, 200) == HAL_BUSY) {
		HAL_Delay(1);
	};

	// Deselect the EEPROM: Chip Select high
	EEPROM_CS_HIGH();

	return EEPROM_STATUS_COMPLETE;
}

/**
  * @brief  Writes data to the EEPROM identification page.
  *
  * @param  pBuffer: pointer to the buffer that has the data to be written to the EEPROM identification page.
  * @param  WriteAddr: EEPROM's identification page address to write to.
  * @param  NumByteToWrite: number of bytes to write to the EEPROM identification page.
  * @retval EepromOperations value: EEPROM_STATUS_COMPLETE or EEPROM_STATUS_ERROR
  */
EepromOperations EEPROM_SPI_WriteID(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
	checkAndConfigureSpiMode(EEPROM_SPI, EEPROM_CPOL, EEPROM_CPHA);		//Check the SPI configuration before doing any Tx or Rx

	while (EEPROM_SPI->State != HAL_SPI_STATE_READY) {

	}

	HAL_StatusTypeDef spiTransmitStatus;

	sEE_WriteEnable();

	//the instruction is sent in a package of 4 bytes, one is the write instruction itself and the other 3 the address
	uint8_t header[4];

	header[0] = EEPROM_WRITE_ID;    // Send "Write to Memory" instruction
	header[1] = WriteAddr >> 16; // Send 24-bit address
	header[2] = WriteAddr >> 8;
	header[3] = WriteAddr;

	//Select the EEPROM: Chip Select low
	EEPROM_CS_LOW();

	EEPROM_SPI_SendInstruction((uint8_t*)header, 4);

	//Makes 5 attempts to write the data
	for (uint8_t i = 0; i < 5; i++) {
		spiTransmitStatus = HAL_SPI_Transmit(EEPROM_SPI, pBuffer, NumByteToWrite, 100);

		if (spiTransmitStatus == HAL_BUSY) {
			HAL_Delay(5);
		} else {
			break;
		}
	}

	//Deselect the EEPROM: Chip Select high
	EEPROM_CS_HIGH();

	//Wait the end of EEPROM writing
	EEPROM_SPI_WaitStandbyState();

	//Disable the write access to the EEPROM
	sEE_WriteDisable();

	if (spiTransmitStatus == HAL_ERROR) {
		return EEPROM_STATUS_ERROR;
	} else {
		return EEPROM_STATUS_COMPLETE;
	}
}

/**
  * @brief  Sends a byte through the SPI interface and return the byte received
  *         from the SPI bus.
  *
  * @param  byte: byte to send.
  * @retval The value of the received byte.
  */
uint8_t EEPROM_SendByte(uint8_t byte)
{
	uint8_t answerByte;

	checkAndConfigureSpiMode(EEPROM_SPI, EEPROM_CPOL, EEPROM_CPHA);		//Check the SPI configuration before doing any Tx or Rx

	/* Loop while DR register in not empty */
	while (EEPROM_SPI->State == HAL_SPI_STATE_RESET) {
		HAL_Delay(1);
	}

	/* Send byte through the SPI peripheral */
	if (HAL_SPI_Transmit(EEPROM_SPI, &byte, 1, 200) != HAL_OK) {
		Error_Handler();
	}

	/* Wait to receive a byte */
	while (EEPROM_SPI->State == HAL_SPI_STATE_RESET) {
		HAL_Delay(1);
	}

	/* Return the byte read from the SPI bus */
	if (HAL_SPI_Receive(EEPROM_SPI, &answerByte, 1, 200) != HAL_OK) {
		Error_Handler();
	}

	return (uint8_t)answerByte;
}
/**
  * @brief  Enables the write access to the EEPROM.
  *
  * @param  None
  * @retval None
  */
void sEE_WriteEnable(void)
{
	checkAndConfigureSpiMode(EEPROM_SPI, EEPROM_CPOL, EEPROM_CPHA);		//Check the SPI configuration before doing any Tx or Rx

	// Select the EEPROM: Chip Select low
	EEPROM_CS_LOW();

	uint8_t command[1] = { EEPROM_WREN };
	/* Send "Write Enable" instruction */
	EEPROM_SPI_SendInstruction((uint8_t*)command, 1);

	// Deselect the EEPROM: Chip Select high
	EEPROM_CS_HIGH();
}

/**
  * @brief  Disables the write access to the EEPROM.
  *
  * @param  None
  * @retval None
  */
void sEE_WriteDisable(void)
{
	checkAndConfigureSpiMode(EEPROM_SPI, EEPROM_CPOL, EEPROM_CPHA);		//Check the SPI configuration before doing any Tx or Rx

	// Select the EEPROM: Chip Select low
	EEPROM_CS_LOW();

	uint8_t command[1] = { EEPROM_WRDI };

	/* Send "Write Disable" instruction */
	EEPROM_SPI_SendInstruction((uint8_t*)command, 1);

	// Deselect the EEPROM: Chip Select high
	EEPROM_CS_HIGH();
}

/**
  * @brief  Write new value in EEPROM Status Register.
  *
  * @param  regval : new value of register
  * @retval None
  */
void sEE_WriteStatusRegister(uint8_t regval)
{
	uint8_t command[2];

	checkAndConfigureSpiMode(EEPROM_SPI, EEPROM_CPOL, EEPROM_CPHA);		//Check the SPI configuration before doing any Tx or Rx

	command[0] = EEPROM_WRSR;
	command[1] = regval;

	// Enable the write access to the EEPROM
	sEE_WriteEnable();

	// Select the EEPROM: Chip Select low
	EEPROM_CS_LOW();

	// Send "Write Status Register" instruction
	// and Regval in one packet
	EEPROM_SPI_SendInstruction((uint8_t*)command, 2);

	// Deselect the EEPROM: Chip Select high
	EEPROM_CS_HIGH();

	sEE_WriteDisable();
}


/**
  * @brief  Polls the status of the Write In Progress (WIP) flag in the EEPROM's
  *         status register and loop until write operation has completed.
  *
  * @param  None
  * @retval None
  */
uint8_t EEPROM_SPI_WaitStandbyState(void)
{
	checkAndConfigureSpiMode(EEPROM_SPI, EEPROM_CPOL, EEPROM_CPHA);		//Check the SPI configuration before doing any Tx or Rx

	uint8_t sEEstatus[1] = { 0x00 };
	uint8_t command[1] = { EEPROM_RDSR };

	// Select the EEPROM: Chip Select low
	EEPROM_CS_LOW();

	// Send "Read Status Register" instruction
	EEPROM_SPI_SendInstruction((uint8_t*)command, 1);

	// Loop as long as the memory is busy with a write cycle
	do {

		while (HAL_SPI_Receive(EEPROM_SPI, (uint8_t*)sEEstatus, 1, 200) == HAL_BUSY) {

		};

	} while ((sEEstatus[0] & EEPROM_WIP_FLAG) == SET); // Write in progress

	// Deselect the EEPROM: Chip Select high
	EEPROM_CS_HIGH();

	return 0;
}

/**
 * @brief Low level function to send header data to EEPROM
 *
 * @param instruction array of bytes to send
 * @param size        data size in bytes
 */
void EEPROM_SPI_SendInstruction(uint8_t *instruction, uint8_t size)
{
	checkAndConfigureSpiMode(EEPROM_SPI, EEPROM_CPOL, EEPROM_CPHA);		//Check the SPI configuration before doing any Tx or Rx

	while (EEPROM_SPI->State == HAL_SPI_STATE_RESET) {
		HAL_Delay(1);
	}

	if (HAL_SPI_Transmit(EEPROM_SPI, (uint8_t*)instruction, (uint16_t)size, 200) != HAL_OK) {
		Error_Handler();
	}
}
