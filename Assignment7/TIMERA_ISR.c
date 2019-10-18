/*
 * TIMERA_ISR.c
 *
 *  Created on: Oct 10, 2019
 *      Author: ryanmyers
 */

#include "msp.h"
#include "TIMERA_ISR.h"

void Initialize_Timers(void) {

    P1->DIR |= BIT5;
    P1->REN &= ~BIT5;
    P1->SEL0 &= ~BIT5;
    P1->SEL1 &= ~BIT5;

    TIMER_A0->CCR[0] = 60000;
    TIMER_A0->CCTL[0] = TIMER_A_CCTLN_CCIE;
    NVIC->ISER[0] = (1 << (TA0_0_IRQn & 0x1F));



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
