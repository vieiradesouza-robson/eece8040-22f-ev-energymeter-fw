/*******************************************************************************
  * File Name			: adc.c
  * Description			: This module implements functions & wrapper related to
  * 					  high level procedures of the ADC peripherals
  *
  * Author				: Charlie Moreno, Robson Viera de Souza
  * Date				: November 27, 2022
  ******************************************************************************
  */

#include "adc_spi.h"
#include "adc.h"
#include <math.h>

extern uint8_t ADCgain[3];
double ADCconvertedChannels[3];
extern uint8_t ADCrawData[ADC_WORD_SIZE/8 * 5];

int32_t convert24bitTo32bit(uint8_t *byteArray){
	int32_t convertedNumber = 0;

	convertedNumber = (
						(((0xFF & byteArray[0])<<16)) +
						(((0xFF & byteArray[1])<<8)) +
						(((0xFF & byteArray[2])))
						);
	if ((convertedNumber & (0b1 << (ADC_DEFAULT_RESOLUTION - 1))) > 0){
		convertedNumber |= 0xFF000000;
	} else {
		convertedNumber &= 0x00FFFFFF;
	}

	return convertedNumber;
}

double *getADCConvertedData(void){
	int32_t rawData32bits[3];

	for (uint8_t i=0; i<3; i++){
		rawData32bits[i] = convert24bitTo32bit(&(ADCrawData[(i*3)+3]));

		ADCconvertedChannels[i] = ((double)rawData32bits[i] * ADC_MAX_RANGE)/(ADCgain[i] * ADC_DEFAULT_MAX_RAW);
	}

	ADCconvertedChannels[SUPPLY_CURRENT_CH] = ADCconvertedChannels[SUPPLY_CURRENT_CH]/SUPPLY_I_SHUNT_RESISTANCE;
	ADCconvertedChannels[HV_CURRENT_CH] = ADCconvertedChannels[HV_CURRENT_CH]/SHUNT_RESISTANCE;
	ADCconvertedChannels[HV_VOLTAGE_CH] = ADCconvertedChannels[HV_VOLTAGE_CH]/VOLTAGE_DIVIDER;

	return (double *)&ADCconvertedChannels;
}

double getADCSingleChannel(uint8_t channel){
	return ADCconvertedChannels[channel];
}
