/*
 * TIMERA_ISR.c
 *
 *  Created on: Oct 10, 2019
 *      Author: ryanmyers
 */

#include "msp.h"


void TA0_0_IRQHandler(void) {
    P1->OUT ^= BIT5;
    TIMER_A0->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG;
}

void TA0_N_IRQHandler(void) {
    P1->OUT ^= BIT5;
    TIMER_A0->CCTL[1] &= ~TIMER_A_CCTLN_CCIFG;
}

void TA1_0_IRQHandler(void) {
    P1->OUT |= BIT7;
    P1->OUT ^= BIT6;
    TIMER_A1->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG;
    P1->OUT &= ~BIT7;
}

void Initialize_Timers(void) {
    set_DCO(DCORSEL_24_MHz);
    CS->KEY = CS_KEY_VAL;   // Unlock clock registers

    CS->CTL1 &= ~(CS_CTL1_DIVM_MASK | CS_CTL1_DIVS_MASK | CS_CTL1_SELM_MASK | CS_CTL1_SELS_MASK); // Clear CS registers
    CS->CTL1 |= CS_CTL1_DIVM0 | CS_CTL1_DIVS0 | CS_CTL1_SELM_3 | CS_CTL1_SELS_3; // Set DCO to drive MCLK and SCLK

    CS->KEY = 0; // Lock clock registers

    // Use TIMER_A0 for 25% duty cycle
    TIMER_A0->CCR[1] = 240;
    TIMER_A0->CCR[0] = 920;
    TIMER_A0->CCTL[1] = TIMER_A_CCTLN_CCIE;
    TIMER_A0->CCTL[0] = TIMER_A_CCTLN_CCIE;
    TIMER_A0->CTL = TIMER_A_CTL_TASSEL_2 | TIMER_A_CTL_MC_1;
    NVIC->ISER[0] = (1 << (TA0_0_IRQn & 0x1F));
    NVIC->ISER[0] = (1 << (TA0_N_IRQn & 0x1F));

    // Use TIMER_A1 for 50% duty cycle
    TIMER_A1->CCR[0] = 480;
    TIMER_A1->CCTL[0] = TIMER_A_CCTLN_CCIE;
    TIMER_A1->CTL = TIMER_A_CTL_TASSEL_2 | TIMER_A_CTL_MC_1;
    NVIC->ISER[0] = (1 << (TA1_0_IRQn & 0x1F));


}
