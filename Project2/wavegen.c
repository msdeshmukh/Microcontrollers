/*
 * wavegen.c
 *
 *  Created on: Oct 18, 2019
 *      Author: ryanmyers
 */

#include "msp.h"
#include "wavegen.h"
#include "spi_driver.h"
#include <math.h>

void Initialize_Wavegen(void) {
    int i;
    float sin_x = 0;
    for (i = 0; i < 4096; i++) {
        sin_table[i] = (3.0 * sin(sin_x)) + 1.5;
        sin_x += SIN_INC;
    }
}

void TA0_0_IRQHandler(void) {
    TIMER_A0->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG;
    out_volt = 3.0;
}

void TA0_N_IRQHandler(void) {
    TIMER_A0->CCTL[1] &= ~TIMER_A_CCTLN_CCIFG;
    out_volt = 0;
}

void TA1_0_IRQHandler(void) {

}

void TA2_0_IRQHandler(void) {

}


void Run_Squarewave(void) {
    // disable sawtoooth and sin timer interrupts
    TIMER_A1->CCTL[0] &= ~TIMER_A_CCTLN_CCIE;
    TIMER_A2->CCTL[0] &= ~TIMER_A_CCTLN_CCIE;

    // default to 100 Hz at 50% duty cycle
    TIMER_A0->CCR[0] = SQUARE_100_Hz;
    TIMER_A0->CCTL[0] = TIMER_A_CCTLN_CCIE;
    TIMER_A0->CTL = TIMER_A_CTL_TASSEL_2 | TIMER_A_CTL_MC_1;
    TIMER_A0->CCR[1] = SQUARE_100_Hz / 2;
    TIMER_A0->CCTL[1] = TIMER_A_CCTLN_CCIE;
    NVIC->ISER[0] = (1 << (TA0_0_IRQn & 0x1F));

}

void Run_Sawtooth(void) {
    // disable square and sin wave timer interrupts
    TIMER_A0->CCTL[0] &= ~TIMER_A_CCTLN_CCIE;
    TIMER_A0->CCTL[1] &= ~TIMER_A_CCTLN_CCIE;
    TIMER_A2->CCTL[0] &= ~TIMER_A_CCTLN_CCIE;

    // default to 100 Hz

}

void Run_Sinwave(void) {
    // disable square and sawtooth wave timer interrupts
    TIMER_A0->CCTL[0] &= ~TIMER_A_CCTLN_CCIE;
    TIMER_A0->CCTL[1] &= ~TIMER_A_CCTLN_CCIE;
    TIMER_A1->CCTL[0] &= ~TIMER_A_CCTLN_CCIE;

    // default to 100 Hz

}
