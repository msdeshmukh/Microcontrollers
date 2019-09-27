/*
 * delay.h
 *
 *  Created on: Sep 26, 2019
 *      Author: ryanmyers
 */

#ifndef DELAY_H_
#define DELAY_H_

#include <stdint.h>

#define DCORSEL_1POINT5_MHz 0x0
#define DCORSEL_3_MHz 0x1
#define DCORSEL_6_MHz 0x2
#define DCORSEL_12_MHz 0x3
#define DCORSEL_24_MHz 0x4
#define DCORSEL_48_MHz 0x5
#define FREQ_1POINT5_MHz 1500000u
#define FREQ_3_MHz 3000000u
#define FREQ_6_MHz 6000000u
#define FREQ_12_MHz 12000000u
#define FREQ_24_MHz 24000000u
#define FREQ_48_MHz 48000000u
#define DCOTUNE_SIGN_MASK 0x200

void set_DCO(uint32_t MHz_freq);
void delay_us(int us_delay);

#endif /* DELAY_H_ */
