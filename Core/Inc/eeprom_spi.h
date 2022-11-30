/*******************************************************************************
  * File Name			: eeprom_spi.h
  * Description			: This module contains the definitions of constants and
  * 					  functions related to the eeprom SPI protocol. High-level
  * 					  routines that will be called by other modules.
  *
  * Author				: Charlie Moreno, Robson Viera de Souza
  * Creation Date		: September 27, 2021
  * Last Change Date	: October 18, 2021
  ******************************************************************************
  */
#ifndef INC_EEPROM_SPI_H_
#define INC_EEPROM_SPI_H_

#include "common.h"
#include "main.h"

/* M95M04-DR SPI EEPROM defines */
#define EEPROM_WREN		0x06  //Write Enable instruction
#define EEPROM_WRDI		0x04  //Write Disable instruction
#define EEPROM_RDSR		0x05  //Read Status Register instruction
#define EEPROM_WRSR		0x01  //Write Status Register instruction
#define EEPROM_READ		0x03  //Read from Memory Array instruction
#define EEPROM_WRITE	0x02  //Write to Memory Array instruction
#define EEPROM_READ_ID	0x83  //Write to identification page
#define EEPROM_WRITE_ID	0x82  //Write to Memory Array instruction

#define EEPROM_WIP_FLAG        0x01  //Write In Progress (WIP) flag

#define EEPROM_PAGESIZE        512   //Page size in bytes according to datasheet
#define EEPROM_BUFFER_SIZE     32    //EEPROM Buffer size in bytes.

#define EEPROM_MAX_ADDRESS	   0x7FFFF //Address of the last EEPROM memory byte

#define EEPROM_CS_GPIO_Port	   GPIOB		//Port to which EEPROM CS pin is connected
#define EEPROM_CS_Pin          GPIO_PIN_5	//Pin to which EEPROM CS pin is connected

#define EEPROM_CS_HIGH()    HAL_GPIO_WritePin(EEPROM_CS_GPIO_Port, EEPROM_CS_Pin, GPIO_PIN_SET)
#define EEPROM_CS_LOW()     HAL_GPIO_WritePin(EEPROM_CS_GPIO_Port, EEPROM_CS_Pin, GPIO_PIN_RESET)

#define EEPROM_CPOL			(SPI_POLARITY_LOW)
#define EEPROM_CPHA			(SPI_PHASE_1EDGE)

/**
 * EEPROM Operations statuses
 */
typedef enum {
	EEPROM_STATUS_PENDING,
	EEPROM_STATUS_COMPLETE,
	EEPROM_STATUS_ERROR
} EepromOperations;

/* High level functions */
void EEPROM_SPI_INIT(void);
EepromOperations EEPROM_SPI_WriteBuffer(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
EepromOperations EEPROM_SPI_WritePage(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
EepromOperations EEPROM_SPI_ReadBuffer(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead);
EepromOperations EEPROM_SPI_ReadID(uint8_t* pBuffer);
EepromOperations EEPROM_SPI_WriteID(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
uint8_t EEPROM_SPI_WaitStandbyState(void);

/* Low level functions */
uint8_t EEPROM_SendByte(uint8_t byte);
void sEE_WriteEnable(void);
void sEE_WriteDisable(void);
void sEE_WriteStatusRegister(uint8_t regval);
uint8_t sEE_ReadStatusRegister(void);

void  EEPROM_SPI_SendInstruction(uint8_t *instruction, uint8_t size);
void  EEPROM_SPI_ReadStatusByte(SPI_HandleTypeDef SPIe, uint8_t *statusByte );

#endif /* INC_EEPROM_SPI_H_ */
