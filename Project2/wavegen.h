/*
 * wavegen.h
 *
 *  Created on: Oct 18, 2019
 *      Author: ryanmyers
 */

#ifndef WAVEGEN_H_
#define WAVEGEN_H_

#define SQUARE_100_Hz 30000
#define SQUARE_200_Hz 15000
#define SQUARE_300_Hz 10000
#define SQUARE_400_Hz 7500
#define SQUARE_500_Hz 6000

#define SIN_100_Hz 938
#define SIN_200_Hz 469
#define SIN_300_Hz 313
#define SIN_400_Hz 235
#define SIN_500_Hz 188

#define SAW_100_Hz 938
#define SAW_200_Hz 469
#define SAW_300_Hz 313
#define SAW_400_Hz 235
#define SAW_500_Hz 188

#define SIN_INC 0.0490873852f
#define SAW_INC 0.0234375f

volatile float out_volt;
float sin_table[128];
float saw_table[128];

void Initialize_Wavegen(void);

void Run_Squarewave(void);
void Run_Sawtooth(void);
void Run_Sinwave(void);

void TA0_0_IRQHandler(void);
void TA0_N_IRQHandler(void);
void TA1_0_IRQHandler(void);
void TA2_0_IRQHandler(void);

#endif /* WAVEGEN_H_ */
