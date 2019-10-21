/*
 * wavegen.h
 *
 *  Created on: Oct 18, 2019
 *      Author: ryanmyers
 */

#ifndef WAVEGEN_H_
#define WAVEGEN_H_

#define _100_Hz 15000
#define _200_Hz 7500
#define _300_Hz 5000
#define _400_Hz 3750
#define _500_Hz 3000

#define SIN_INC 0.001533980787f

float sin_table[4096];

void Initialize_Wavegen(void);

void Run_Squarewave(void);
void Run_Sawtooth(void);
void Run_Sinwave(void);

void TA0_0_IRQHandler(void);
void TA0_N_IRQHandler(void);
void TA1_0_IRQHandler(void);
void TA2_0_IRQHandler(void);

#endif /* WAVEGEN_H_ */
