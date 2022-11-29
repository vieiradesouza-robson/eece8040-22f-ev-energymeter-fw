/*******************************************************************************
  * File Name			: adc_spi.c
  * Description			: This module implements functions & wrapper related to
  * 					  the SPI protocol used to operate the external ADC
  * 					  peripheral. Shall be in charge of implementing the
  * 					  commands that must be sent to the external ADC module.
  *
  * Author				: Charlie Moreno, Robson Viera de Souza
  * Date				: September 27, 2021
  ******************************************************************************
  */

#include "adc_spi.h"


SPI_HandleTypeDef * ADC_SPI = NULL;
uint8_t ADCrawData[ADC_WORD_SIZE/8 * 5];
uint8_t ADCdummy[ADC_WORD_SIZE/8 * 5];

void ADC_CS_ENABLE(void)
{
	HAL_GPIO_WritePin(ADC_CS_GPIO_Port, ADC_CS_Pin, GPIO_PIN_RESET);
}

void ADC_CS_DISABLE(void)
{
	HAL_GPIO_WritePin(ADC_CS_GPIO_Port, ADC_CS_Pin, GPIO_PIN_SET);
}

uint8_t *ADCrawChannels(void)
{

	while (ADC_SPI->State != HAL_SPI_STATE_READY) {
		HAL_Delay(1);
	}

	ADC_CS_ENABLE();

	while (HAL_SPI_TransmitReceive(ADC_SPI, (uint8_t *)&ADCdummy, (uint8_t *)&ADCrawData, (ADC_WORD_SIZE/8) * 5, ADC_TIMEOUT) == HAL_BUSY) {
		HAL_Delay(1);
	}

	ADC_CS_DISABLE();

	return (uint8_t *)&ADCrawData;
}

uint8_t ADCwriteReg(uint8_t reg, uint32_t data)
{

	// Make command word using syntax found in data sheet
	uint8_t commandWord[ADC_WORD_SIZE/8 * 5];
	uint8_t responseArr[ADC_WORD_SIZE/8 * 5];

	commandWord[0] = (WREG >> 8) + (reg >> 1);
	commandWord[1] = reg << 7;
	commandWord[2] = 0;
	commandWord[3] = data >> 16;
	commandWord[4] = data >> 8;
	commandWord[5] = data;

	//filling the remaining spots in the command being sent with DUMMY so the ADC Tx buffer is cleared when the command is sent
	for (uint8_t i = 6; i < ADC_WORD_SIZE/8 * 5; i++) {
		commandWord[i] = DUMMY;
	}

//  responseWord[0] = (WREG_RES >> 8) + (reg >> 1);
//  responseWord[1] = reg << 7;
//  responseWord[2] = 0;

	printf("Command = %x %x %x\n\r", commandWord[0], commandWord[1], commandWord[2]);
	printf("Reg data = %x %x %x\n\r", commandWord[3], commandWord[4], commandWord[5]);

	//clear the ADC SPI buffer
	ADC_CS_ENABLE();

	while (ADC_SPI->State != HAL_SPI_STATE_READY) {
		HAL_Delay(1);
	}

	HAL_SPI_Transmit(ADC_SPI, ADCdummy, ADC_WORD_SIZE/8 * 5, ADC_TIMEOUT);

	ADC_CS_DISABLE();

	if (HAL_GPIO_ReadPin(ADC_DRDY_GPIO_Port, ADC_DRDY_Pin) == GPIO_PIN_RESET) {
		ADC_CS_ENABLE();

		while (ADC_SPI->State != HAL_SPI_STATE_READY) {
			HAL_Delay(1);
		}

		HAL_SPI_Transmit(ADC_SPI, ADCdummy, ADC_WORD_SIZE/8 * 5, ADC_TIMEOUT);

		ADC_CS_DISABLE();
	}

	while(HAL_GPIO_ReadPin(ADC_DRDY_GPIO_Port, ADC_DRDY_Pin) == GPIO_PIN_SET) {}

	ADC_CS_ENABLE();

	while (ADC_SPI->State != HAL_SPI_STATE_READY) {
		HAL_Delay(1);
	}

	HAL_SPI_Transmit(ADC_SPI, commandWord, ADC_WORD_SIZE/8 * 5, ADC_TIMEOUT);

	ADC_CS_DISABLE();

	// Get response

	while(HAL_GPIO_ReadPin(ADC_DRDY_GPIO_Port, ADC_DRDY_Pin) == GPIO_PIN_SET) {}

	ADC_CS_ENABLE();

	while (ADC_SPI->State != HAL_SPI_STATE_READY) {
		HAL_Delay(1);
	}

	while (HAL_SPI_TransmitReceive(ADC_SPI, ADCdummy, responseArr, ADC_WORD_SIZE/8 * 5, ADC_TIMEOUT) == HAL_BUSY) {
	}

	ADC_CS_DISABLE();

	printf("Response array = %x %x %x\n\r", responseArr[0], responseArr[1], responseArr[2]);

//  if ((responseWord[0] == responseArr[0]) && (responseWord[1] == responseArr[1]) && (responseWord[2] == responseArr[2])) {
//    return 1;
//  } else {
//
//    return 0;
//  }
	return 1;
}

uint8_t ADCsetGain(uint8_t log2GainCH0, uint8_t log2GainCH1, uint8_t log2GainCH2)
{

	uint32_t gainData = 0x00000000 + ((0x07 & log2GainCH0)<<16) + ((0x07 & log2GainCH1) << 20) + ((0x07 & log2GainCH2) << 24);
	return ADCwriteReg(GAIN, gainData);

}

uint8_t ADCinit(SPI_HandleTypeDef * hspi)
{
	ADC_SPI = hspi;
	uint32_t regConfig;
	uint8_t res;

	//fills dummy communication array with dummy data to be used when getting a response or capturing ADC data
	for (uint8_t i = 0; i<(ADC_WORD_SIZE/8 * 5); i++) {
		ADCdummy[i] = DUMMY;
	}

	//disable all channels to enable short frames during configuration
//  regConfig = (CLOCK_CH0_DIS | CLOCK_CH1_DIS | CLOCK_CH2_DIS | CLOCK_TBM | CLOCK_OSR | CLOCK_PWR);
//  res = ADCwriteReg(CLOCK, regConfig);
//  if (res == 0){
//	  printf("Error disabling channels.\n\r");
//	  //return res;
//  }

	//write mode register to clear reset flag and make DRDY active low pulse
	regConfig = (MODE_REG_CRC_EN | MODE_RX_CRC_EN | MODE_CRC_TYPE | MODE_RESET_CLEAR |
				 MODE_WLENGTH | MODE_TIMEOUT | MODE_DRDY_SEL | MODE_DRDY_HIZ | MODE_DRDY_FMT_PULSE);
	res = ADCwriteReg(MODE, regConfig);
	if (res == 0) {
		printf("Error setting MODE register.\n\r");
		//return res;
	}

	//set CH0 gain to 32, CH1 to 128 and CH2 to 128
	regConfig = (GAIN_PGA_GAIN0_32 | GAIN_PGA_GAIN1_128 | GAIN_PGA_GAIN2_128);
	res = ADCwriteReg(GAIN, regConfig);
	if (res == 0) {
		printf("Error setting GAIN register.\n\r");
		//return res;
	}

	//enable all channels again and set OSR to 16256
	regConfig = (CLOCK_CH0_EN | CLOCK_CH1_EN | CLOCK_CH2_EN | CLOCK_TBM | CLOCK_OSR_16256 | CLOCK_PWR);
	res = ADCwriteReg(CLOCK, regConfig);

	return res;
}

