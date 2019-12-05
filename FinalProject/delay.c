/*
 * delay.c
 *
 *  Created on: Sep 26, 2019
 *      Author: ryanmyers
 */

#include "delay.h"
#include "msp.h"

void set_DCO(uint32_t MHz_freq) {

    // Select the correct power mode if we are operating at 48MHz
    if (MHz_freq == DCORSEL_48_MHz) {
        /* Transition to VCORE Level 1: AM0_LDO --> AM1_LDO */
        while ((PCM->CTL1 & PCM_CTL1_PMR_BUSY));
            PCM->CTL0 = PCM_CTL0_KEY_VAL | PCM_CTL0_AMR_1;
        while ((PCM->CTL1 & PCM_CTL1_PMR_BUSY));

        /* Configure Flash wait-state to 1 for both banks 0 & 1 */
        FLCTL->BANK0_RDCTL = (FLCTL->BANK0_RDCTL &
                ~(FLCTL_BANK0_RDCTL_WAIT_MASK)) | FLCTL_BANK0_RDCTL_WAIT_1;
        FLCTL->BANK1_RDCTL = (FLCTL->BANK0_RDCTL &
                ~(FLCTL_BANK1_RDCTL_WAIT_MASK)) | FLCTL_BANK1_RDCTL_WAIT_1;
    }

    CS->KEY = CS_KEY_VAL;   // Unlock clock registers
    CS->CTL0 &= ~CS_CTL0_DCOTUNE_MASK;  // Clear DCO tune and select registers
    CS->CTL0 &= ~CS_CTL0_DCORSEL_MASK;
    CS->CTL0 |= CS_CTL0_DCORSEL_MASK & MHz_freq;    // Set select to given nominal frequency
    CS->KEY = 0;    // Lock clock registers
    return;
}

// Delay calculations are scaled based on function enter and exit times as well as
// while loop iteration times for a 1.5MHz operating frequency
void delay_us(int us_delay) {
    uint32_t dco_freq;
    uint32_t scale;
    int i;
    // Determine currently configured DCO RSEL and TUNE values
    uint32_t dco_rsel = CS->CTL0 & CS_CTL0_DCORSEL_MASK;
    int dco_tune = (CS->CTL0 & CS_CTL0_DCOTUNE_MASK) >> CS_CTL0_DCOTUNE_OFS;
    // if DCOTUNE is negative, expand it to a signed 32 bit number
    if (dco_tune & DCOTUNE_SIGN_MASK) {
        dco_tune = -1 * (~dco_tune + 1);
    }
    // Add DCOTUNE value to nominal frequency to find current DC0
    // operating frequency
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

    // Calculate timing scaler based on 1.5MHz timing measurements
    scale = dco_freq/FREQ_1POINT5_MHz;
    i = FUNC_ENTER_EXIT_TIME / scale;
    // Timing values are scaled by 1000 to provide greater precision
    // without the use of floating point numbers
    while(i < (us_delay * 1000))
        i += LOOP_ITER_TIME / scale;
}


