/*
 * TIMERA_ISR.h
 *
 *  Created on: Oct 10, 2019
 *      Author: Ryan Myers, Mihir Deshmukh
 */

#ifndef TIMERA_ISR_H_
#define TIMERA_ISR_H_

void Initialize_Timers(void);

void TA0_0_IRQHandler(void);
void TA0_N_IRQHandler(void);
void TA1_0_IRQHandler(void);

#endif /* TIMERA_ISR_H_ */
