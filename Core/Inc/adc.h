/*******************************************************************************
  * File Name			: adc.h
  * Description			: This module contains the definitions of constants and
  * 					  functions related to the ADC high-level
  * 					  routines that will be called by other modules.
  *
  * Author				: Charlie Moreno, Robson Viera de Souza
  * Date				: November 27, 2021
  ******************************************************************************
  */


#define ADC_DEFAULT_RESOLUTION		24
#define ADC_DEFAULT_MAX_RAW			0xFFFFFF
#define SUPPLY_I_SHUNT_RESISTANCE	0.1
#define SHUNT_RESISTANCE			0.0001
#define VOLTAGE_DIVIDER				0.001153
#define SUPPLY_CURRENT_CH			0
#define HV_CURRENT_CH				1
#define HV_VOLTAGE_CH				2
#define ADC_MAX_RANGE				2.4

double *getADCConvertedData();
double getADCSingleChannel(uint8_t channel);
