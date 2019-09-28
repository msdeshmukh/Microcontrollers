/*
 * delay.h
 *
 *  Created on: Sep 26, 2019
 *      Author: ryanmyers
 */

#ifndef DELAY_H_
#define DELAY_H_

#include <stdint.h>

// DCORSEL definitions used to pass into set_DCO function
#define DCORSEL_1POINT5_MHz CS_CTL0_DCORSEL_0
#define DCORSEL_3_MHz CS_CTL0_DCORSEL_1
#define DCORSEL_6_MHz CS_CTL0_DCORSEL_2
#define DCORSEL_12_MHz CS_CTL0_DCORSEL_3
#define DCORSEL_24_MHz CS_CTL0_DCORSEL_4
#define DCORSEL_48_MHz CS_CTL0_DCORSEL_5
// FREQ definitions used for delay calculations
#define FREQ_1POINT5_MHz 1500000u
#define FREQ_3_MHz 3000000u
#define FREQ_6_MHz 6000000u
#define FREQ_12_MHz 12000000u
#define FREQ_24_MHz 24000000u
#define FREQ_48_MHz 48000000u
// Used to check DCOTUNE sign
#define DCOTUNE_SIGN_MASK 0x200
// Measured while loop iteration time at 1.5MHz
#define LOOP_ITER_TIME 14000u
// Measured function enter/exit time at 1.5MHz
#define FUNC_ENTER_EXIT_TIME 72000u


void set_DCO(uint32_t MHz_freq);
void delay_us(int us_delay);

#endif /* DELAY_H_ */
