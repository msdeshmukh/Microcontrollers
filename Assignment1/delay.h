/*
 * delay.h
 *
 *  Created on: Sep 26, 2019
 *      Author: ryanmyers
 */

#ifndef DELAY_H_
#define DELAY_H_

#define FREQ_1POINT5_MHz 0x0
#define FREQ_3_MHz 0x1
#define FREQ_6_MHz 0x2
#define FREQ_12_MHz 0x3
#define FREQ_24_MHz 0x4
#define FREQ_48_MHz 0x5

void set_DCO(uint32_t MHz_freq);
void delay_us(int us_delay);

#endif /* DELAY_H_ */
