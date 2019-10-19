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
    static volatile uint8_t square_pulse = 0;
    TIMER_A0->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG;
    square_pulse ^= 1;
    if (square_pulse == 1) {
        Send_DAC_Voltage(2.0);
    }
    else {
        Send_DAC_Voltage(0.0);
    }
}

void TA1_0_IRQHandler(void) {
    static volatile uint32_t timer_a0_count = 0;
    static volatile uint8_t triangle_pulse = 0;
    static volatile float out_volt = 0;
    TIMER_A1->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG;
    if (triangle_pulse == 0) {
        Send_DAC_Voltage(out_volt);
        out_volt += .025;
        if (timer_a0_count == 80) {
            triangle_pulse = 1;
        }
        else {
            timer_a0_count++;
        }
    }
    else {
        Send_DAC_Voltage(out_volt);
        out_volt -= .025;
        if (timer_a0_count == 1) {
            triangle_pulse = 0;
        }
        else {
            timer_a0_count--;
        }
    }
}

void Run_Trianglewave(void) {

    P2->DIR |= BIT5;
    P2->REN &= ~BIT5;
    P2->SEL0 &= ~BIT5;
    P2->SEL1 &= ~BIT5;

    TIMER_A1->CCR[0] = 300;
    TIMER_A1->CCTL[0] = TIMER_A_CCTLN_CCIE;
    TIMER_A1->CTL = TIMER_A_CTL_TASSEL_2 | TIMER_A_CTL_MC_1;
    NVIC->ISER[0] = (1 << (TA1_0_IRQn & 0x1F));

}

void Run_Squarewave(void) {

    TIMER_A0->CCR[0] = 30000;
    TIMER_A0->CCTL[0] = TIMER_A_CCTLN_CCIE;
    TIMER_A0->CTL = TIMER_A_CTL_TASSEL_2 | TIMER_A_CTL_MC_1;
    NVIC->ISER[0] = (1 << (TA0_0_IRQn & 0x1F));

}
