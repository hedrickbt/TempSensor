// https://my.st.com/public/STe2ecommunities/mcu/Lists/STM32Java/Flat.aspx?RootFolder=https%3a%2f%2fmy.st.com%2fpublic%2fSTe2ecommunities%2fmcu%2fLists%2fSTM32Java%2fSTM32F3%20internal%20temp%20sensor&FolderCTID=0x01200200770978C69A1141439FE559EB459D758000F9A0E3A95BA69146A17C2E80209ADC21&currentviews=2577
// STM32 ADC TEMP STM32F3-Discovery - sourcer32@gmail.com
 
/**************************************************************************************/
 
#include "stm32f3_discovery.h"
#include "stm32f30x.h"
 
#include <stdio.h>
 
/**************************************************************************************/
 
void ADC_Configuration(void)
{
  ADC_InitTypeDef        ADC_InitStructure;
  ADC_CommonInitTypeDef  ADC_CommonInitStructure;
  volatile int i;
  uint16_t CalibrationValue;
 
  /* Configure the ADC clocks */
  RCC_ADCCLKConfig(RCC_ADC12PLLCLK_Div1);
 
  /* Enable ADC1/2 clocks */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_ADC12, ENABLE);
 
  /* ADC Calibration procedure */
  ADC_VoltageRegulatorCmd(ADC1, ENABLE);
 
  /* Insert delay equal to 10 us */
  for(i=0; i<10000; i++);
 
  ADC_SelectCalibrationMode(ADC1, ADC_CalibrationMode_Single);
  ADC_StartCalibration(ADC1);
 
  while(ADC_GetCalibrationStatus(ADC1) != RESET);
 
  CalibrationValue = ADC_GetCalibrationValue(ADC1);
 
  /* ADC configuration */
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_CommonInitStructure.ADC_Clock = ADC_Clock_AsynClkMode;
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; // Not Multi Mode
  ADC_CommonInitStructure.ADC_DMAMode = ADC_DMAMode_Circular;
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_SampleTime_2Cycles5;
 
  ADC_CommonInit(ADC1, &ADC_CommonInitStructure);
 
  /* */
  ADC_InitStructure.ADC_ContinuousConvMode = ADC_ContinuousConvMode_Disable; // Manual Trigger
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  ADC_InitStructure.ADC_ExternalTrigEventEdge = ADC_ExternalTrigEventEdge_None;
  ADC_InitStructure.ADC_ExternalTrigConvEvent = ADC_ExternalTrigConvEvent_3; // Not used here
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_OverrunMode = ADC_OverrunMode_Disable;
  ADC_InitStructure.ADC_AutoInjMode = ADC_AutoInjec_Disable;
  ADC_InitStructure.ADC_NbrOfRegChannel = 1;
 
  ADC_Init(ADC1, &ADC_InitStructure);
 
  /* ADC1 regular configuration */
  ADC_RegularChannelConfig(ADC1, ADC_Channel_TempSensor, 1, ADC_SampleTime_61Cycles5);
 
  /* Enable Temp channel */
  ADC_TempSensorCmd(ADC1, ENABLE);
 
  /* Enable ADC1 */
  ADC_Cmd(ADC1, ENABLE);
 
  /* wait for ADC1 ADRDY */
  while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_RDY));
}
 
/**************************************************************************************/
 
int main(void)
{
  uint16_t ADCConvertedValue;
 
  /* Initialize LEDs available on STM32F3-Discovery board */
  STM_EVAL_LEDInit(LED3);
  STM_EVAL_LEDInit(LED4);
 
  /* Turn on LD3 */
  STM_EVAL_LEDOn(LED3);
  STM_EVAL_LEDOn(LED4);
 
  ADC_Configuration();
 
  while(1) // Don't want to exit
  {
    /* Start ADC1 Software Conversion */
    ADC_StartConversion(ADC1);
 
    while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
 
        ADCConvertedValue = ADC_GetConversionValue(ADC1);
         
    printf("%03X\n", ADCConvertedValue); // 0x6DB on my test
 
    STM_EVAL_LEDToggle(LED3);
  }
}
 
//******************************************************************************
// Rough SWV support in Keil, STM32F3-Discovery, Make SB10 to connect PB3/SWO
//******************************************************************************
 
#include <rt_misc.h>
 
#pragma import(__use_no_semihosting_swi)
 
struct __FILE { int handle; /* Add whatever you need here */ };
FILE __stdout;
FILE __stdin;
 
int fputc(int ch, FILE *f)
{
    ITM_SendChar(ch);
 
  return(ch);
}
 
int fgetc(FILE *f)
{
  return(-1);
}
 
int ferror(FILE *f)
{
  /* Your implementation of ferror */
  return EOF;
}
 
void _ttywrch(int ch)
{
    ITM_SendChar(ch);
}
 
void _sys_exit(int return_code)
{
label:  goto label;  /* endless loop */
}
 
//******************************************************************************
 
#ifdef  USE_FULL_ASSERT
 
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
 
  /* Infinite loop */
  while (1)
  {
  }
}
#endif
 
//******************************************************************************
