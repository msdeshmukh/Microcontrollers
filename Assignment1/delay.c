/*
 * delay.c
 *
 *  Created on: Sep 26, 2019
 *      Author: ryanmyers
 */

#include "delay.h"
#include "msp.h"
#include <stdint.h>

void set_DCO(uint32_t MHz_freq) {
    CS->KEY = CS_KEY_VAL;
    CS->CTL0 &= ~CS_CTL0_DCOTUNE_MASK;
    CS->CTL0 |= CS_CTL0_DCORSEL_MASK & MHz_freq;
    CS->KEY = 0;
}

void delay_us(int us_delay) {

}


