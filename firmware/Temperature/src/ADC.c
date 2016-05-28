/////////////////////////////////////////////////////////////////////////
///	\file ADC.c
///	\brief STM32 ADC MCU Hardware
///
///	Author: vagrant
/////////////////////////////////////////////////////////////////////////

#include "MCU/ADC.h"

///////////////////////////////////////////////////////////////////////////////
/// \brief Enable the ADC for the internal temperature sensor
///////////////////////////////////////////////////////////////////////////////
void ADC_On(void) {
	//RCC->APB2RSTR |= RCC_APB2RSTR_ADCRST; // reset the ADC
	RCC->APB2ENR |= RCC_APB2ENR_ADCEN; // enable the ADC clock

	ADC->CCR |= ADC_CCR_TSEN; // enable the temperature sensor

	ADC1->CR |= ADC_CR_ADEN; //enable the ADC
	while ( !(ADC1->ISR & ADC_ISR_ADRDY) );  // wait until the ADC is ready to start conversation
	//while ((ADC1->ISR & ADC_ISR_ADRDY) == 0 );  // wait until the ADC is ready to start conversation
}

///////////////////////////////////////////////////////////////////////////////
/// \brief Read a value from the ADC for the internal temperature sensor
///
/// \param ADC channel 0-17
/// \param destination pointer to return the value read
///
/// \return True on success else adc is not open
///////////////////////////////////////////////////////////////////////////////
uint_fast8_t ADC_Read(uint_fast32_t channel, uint_fast16_t *destination) {
	if ((!ADC1->CR & ADC_CR_ADEN) || channel > 17 || !destination) {
		return FALSE; // adc is not turned on, bad channel, or null destination
	}

	if (ADC1->CR & ADC_CR_ADSTART) {
		return FALSE;  // a conversion has already been started
	}

	ADC1->CHSELR = ((uint32_t)(1 << channel)); // select the channel

	ADC1->CR |= ADC_CR_ADSTART; // start the conversion

	while (!(ADC1->ISR & ADC_ISR_EOC)); //wait until ADC is done with conversion

	*destination = (uint32_t)(0x0000FFFF & ADC1->DR); // only want to keep the 1/2 of the bits


	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief Normalize the value returned from an ADC_Read
///
/// \param ADC channel
/// \param destination pointer to return the value read
///
/// \return True on success else adc is not open
///////////////////////////////////////////////////////////////////////////////
uint_fast8_t ADC_ReadNorm(uint_fast32_t channel, float *destination) {

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief Disable the ADC for the temperature sensor
///////////////////////////////////////////////////////////////////////////////
void ADC_Off(void) {
	ADC1->CR |= ADC_CR_ADDIS; //disable the ADC
	while ( !(ADC1->CR & ADC_CR_ADDIS) );  // wait until the ADC is shut down

}

///////////////////////////////////////////////////////////////////////////////
/// \brief Calculated the calibrated value from an ADC_Read result
///
/// \param rawData from the destination parameter in ADC_Read
///
/// \return Calibrated reading value
/// \sa ADC_Read()
///////////////////////////////////////////////////////////////////////////////
float ADC_CalibratedTemperature(uint_fast16_t rawData) {
	float result = 0.0;

	return result;
}
