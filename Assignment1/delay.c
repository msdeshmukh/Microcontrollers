/*
 * delay.c
 *
 *  Created on: Sep 26, 2019
 *      Author: ryanmyers
 */

#include "delay.h"
#include "msp.h"

void set_DCO(uint32_t MHz_freq) {
    CS->KEY = CS_KEY_VAL;
    CS->CTL0 = 0;
    CS->CTL0 &= ~CS_CTL0_DCOTUNE_MASK;
    CS->CTL0 &= ~CS_CTL0_DCORSEL_MASK;
    CS->CTL0 |= CS_CTL0_DCORSEL_MASK & MHz_freq;
    CS->KEY = 0;
    return;
}

void delay_us(int us_delay) {
    //P4->OUT |= BIT0;
    //P4->OUT |= ~BIT0;
    uint32_t dco_freq;
    uint32_t dco_rsel = CS->CTL0 & CS_CTL0_DCORSEL_MASK;
    int dco_tune = (CS->CTL0 & CS_CTL0_DCOTUNE_MASK) >> CS_CTL0_DCOTUNE_OFS;
    if (dco_tune & DCOTUNE_SIGN_MASK) {
        dco_tune = -1 * (~dco_tune + 1);
    }
    switch (dco_rsel) {
    case DCORSEL_1POINT5_MHz:
        dco_freq = FREQ_1POINT5_MHz + dco_tune;
        break;
    case DCORSEL_3_MHz:
        dco_freq = FREQ_3_MHz + dco_tune;
        break;
    case DCORSEL_6_MHz:
        dco_freq = FREQ_6_MHz + dco_tune;
        break;
    case DCORSEL_12_MHz:
        dco_freq = FREQ_12_MHz + dco_tune;
        break;
    case DCORSEL_24_MHz:
        dco_freq = FREQ_24_MHz + dco_tune;
        break;
    case DCORSEL_48_MHz:
        dco_freq = FREQ_48_MHz + dco_tune;
        break;
    }
}


