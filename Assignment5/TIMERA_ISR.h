/*
 * TIMERA_ISR.h
 *
 *  Created on: Oct 10, 2019
 *      Author: ryanmyers
 */

#ifndef TIMERA_ISR_H_
#define TIMERA_ISR_H_

#define MODE_25_DUTY
//#define MODE_50_DUTY
//#define MODE_2BIT

void Initialize_Timers(void);

void TA0_0_IRQHandler(void);
void TA0_N_IRQHandler(void);
void TA1_0_IRQHandler(void);

#endif /* TIMERA_ISR_H_ */
