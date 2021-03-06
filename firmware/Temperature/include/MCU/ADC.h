/*
 * \file ADC.h
 *
 *  Created on: May 27, 2016
 *      Author: vagrant
 */

#ifndef ADC_H_
#define ADC_H_

#include "common.h"

void ADC_On(void);
uint_fast8_t ADC_Read(uint_fast32_t channel, uint_fast16_t *destination);
uint_fast8_t ADC_Normalize(uint_fast16_t adcReading, float *destination);
uint_fast8_t ADC_ReadNorm(uint_fast32_t channel, float *destination);
void ADC_Off(void);

int32_t ADC_CalibratedTemperature(uint_fast16_t rawData);




#endif /* ADC_H_ */
