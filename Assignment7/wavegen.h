/*
 * wavegen.h
 *
 *  Created on: Oct 18, 2019
 *      Author: ryanmyers
 */

#ifndef WAVEGEN_H_
#define WAVEGEN_H_

static uint8_t square_pulse = 0;
static uint8_t triangle_pulse = 0;

void Run_Squarewave(void);
void Run_Trianglewave(void);

void TA0_0_IRQHandler(void);
void TA0_N_IRQHandler(void);
void TA1_0_IRQHandler(void);

#endif /* WAVEGEN_H_ */
