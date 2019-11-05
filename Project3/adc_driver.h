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
#define ADC_READY 1
#define ADC_UNAVAILABLE 0

void Initialize_ADC(void);
void ADC14_IRQHandler(void);

#endif /* ADC_DRIVER_H_ */
