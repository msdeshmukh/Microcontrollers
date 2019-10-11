/*
 * TIMERA_ISR.c
 *
 *  Created on: Oct 10, 2019
 *      Author: ryanmyers
 */

#include "msp.h"
#include "delay.h"
#include "TIMERA_ISR.h"

#ifdef MODE_25_DUTY
void TA0_0_IRQHandler(void) {
    TIMER_A0->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG;
    P1->OUT |= BIT5;
}

void TA0_N_IRQHandler(void) {
    TIMER_A0->CCTL[1] &= ~TIMER_A_CCTLN_CCIFG;
    P1->OUT &= ~BIT5;
}
#endif

#ifdef MODE_50_DUTY
void TA1_0_IRQHandler(void) {
    P1->OUT |= BIT6;
    TIMER_A1->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG;
    P1->OUT ^= BIT5;
    P1->OUT &= ~BIT6;
}
#endif

#ifdef MODE_2BIT
void TA0_0_IRQHandler(void) {
    TIMER_A0->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG;
    TIMER_A0->CCR[0] += 1500;
    P1->OUT ^= BIT5;
}

void TA0_N_IRQHandler(void) {
    TIMER_A0->CCTL[1] &= ~TIMER_A_CCTLN_CCIFG;
    TIMER_A0->CCR[1] += 750;
    P1->OUT ^= BIT6;
}
#endif

void Initialize_Timers(void) {

    P1->DIR |= BIT5;
    P1->REN &= ~BIT5;
    P1->SEL0 &= ~BIT5;
    P1->SEL1 &= ~BIT5;

    P1->DIR |= BIT6;
    P1->REN &= ~BIT6;
    P1->SEL0 &= ~BIT6;
    P1->SEL1 &= ~BIT6;

    CS->KEY = CS_KEY_VAL;   // Unlock clock registers

    CS->CTL1 &= ~(CS_CTL1_DIVM_MASK | CS_CTL1_DIVS_MASK | CS_CTL1_SELM_MASK | CS_CTL1_SELS_MASK); // Clear CS registers
    CS->CTL1 |= CS_CTL1_DIVM_0 | CS_CTL1_DIVS_0 | CS_CTL1_SELM_3 | CS_CTL1_SELS_3; // Set DCO to drive MCLK and SCLK

    CS->KEY = 0; // Lock clock registers

#ifdef MODE_25_DUTY
    set_DCO(DCORSEL_24_MHz);

    // Use TIMER_A0 for 25% duty cycle
    TIMER_A0->CCR[1] = 240;
    TIMER_A0->CCR[0] = 960;
    TIMER_A0->CCTL[1] = TIMER_A_CCTLN_CCIE;
    TIMER_A0->CCTL[0] = TIMER_A_CCTLN_CCIE;
    TIMER_A0->CTL = TIMER_A_CTL_TASSEL_2 | TIMER_A_CTL_MC_1;
    NVIC->ISER[0] = (1 << (TA0_0_IRQn & 0x1F));
    NVIC->ISER[0] = (1 << (TA0_N_IRQn & 0x1F));
#endif

#ifdef MODE_50_DUTY
    set_DCO(DCORSEL_24_MHz);

    // Use TIMER_A1 for 50% duty cycle
    TIMER_A1->CCR[0] = 480;
    TIMER_A1->CCTL[0] = TIMER_A_CCTLN_CCIE;
    TIMER_A1->CTL = TIMER_A_CTL_TASSEL_2 | TIMER_A_CTL_MC_1;
    NVIC->ISER[0] = (1 << (TA1_0_IRQn & 0x1F));
#endif

#ifdef MODE_2BIT
    set_DCO(DCORSEL_1POINT5_MHz);

    TIMER_A0->CCR[1] = 750;
    TIMER_A0->CCR[0] = 1500;
    TIMER_A0->CCTL[1] = TIMER_A_CCTLN_CCIE;
    TIMER_A0->CCTL[0] = TIMER_A_CCTLN_CCIE;
    TIMER_A0->CTL = TIMER_A_CTL_TASSEL_2 | TIMER_A_CTL_MC_2;
    NVIC->ISER[0] = (1 << (TA0_0_IRQn & 0x1F));
    NVIC->ISER[0] = (1 << (TA0_N_IRQn & 0x1F));
#endif


}
