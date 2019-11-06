/*
 * adc_driver.h
 *
 *  Created on: Oct 30, 2019
 *      Author: ryanmyers
 */

#ifndef ADC_DRIVER_H_
#define ADC_DRIVER_H_

#define ADCPORT P4
#define ADCPIN BIT7
#define MEASUREMENT_READY 1
#define MEASUREMENT_UNAVAILABLE 0
#define ADC_RES 16383

void Initialize_ADC(void);
void ADC14_IRQHandler(void);
float Read_AC_PP(void);
float Read_AC_RMS(void);
float Read_DC(void);
void TA0_0_IRQHandler(void);
void TA1_0_IRQHandler(void);
void TA2_0_IRQHandler(void);
uint32_t Read_Freq(void);
uint8_t Read_Measurement_Flag(void);

#endif /* ADC_DRIVER_H_ */
