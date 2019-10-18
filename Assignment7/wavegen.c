/*
 * wavegen.c
 *
 *  Created on: Oct 18, 2019
 *      Author: ryanmyers
 */

#include "msp.h"
#include "wavegen.h"
#include "spi_driver.h"

void TA0_0_IRQHandler(void) {
    square_pulse ^= 1;
    P2->OUT ^= BIT5;
}

void TA1_0_IRQHandler(void) {

}

void Run_Trianglewave(void) {

    TIMER_A1->CCR[0] = 60000;
    TIMER_A1->CCTL[0] = TIMER_A_CCTLN_CCIE;
    TIMER_A1->CTL = TIMER_A_CTL_TASSEL_2 | TIMER_A_CTL_MC_1;
    NVIC->ISER[0] = (1 << (TA1_0_IRQn & 0x1F));

}

void Run_Squarewave(void) {

    P2->DIR |= BIT5;
    P2->REN &= ~BIT5;
    P2->SEL0 &= ~BIT5;
    P2->SEL1 &= ~BIT5;

    TIMER_A0->CCR[0] = 30000;
    TIMER_A0->CCTL[0] = TIMER_A_CCTLN_CCIE;
    TIMER_A0->CTL = TIMER_A_CTL_TASSEL_2 | TIMER_A_CTL_MC_1;
    NVIC->ISER[0] = (1 << (TA0_0_IRQn & 0x1F));

}
