/*
 * wavegen.c
 *
 *  Created on: Oct 18, 2019
 *      Author: Ryan Myers, Mihir Deshmukh
 */

#include "msp.h"
#include "wavegen.h"
#include "spi_driver.h"
#include <math.h>

//Populate sawtooth and sine wave table with points
//instead of at runtime.
void Initialize_Wavegen(void) {
    int i;
    float sin_x = 0;
    float saw_y = 0;
    for (i = 0; i < 128; i++) {
        sin_table[i] = (1 + sin(sin_x)) * 1.5;
        sin_x += SIN_INC;
    }
    for (i = 0; i < 128; i++) {
        saw_table[i] = saw_y;
        saw_y += SAW_INC;
    }
}
//Interrupt for square wave for high duration
void TA0_0_IRQHandler(void) {
    TIMER_A0->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG;
    out_volt = 3.0;
}

//Interrupt for square wave for low duration
void TA0_N_IRQHandler(void) {
    TIMER_A0->CCTL[1] &= ~TIMER_A_CCTLN_CCIFG;
    out_volt = 0;
}

//Interrupt for sin wave. Change out_volt
//which is the voltage to send to the dac. Then
//send_dac_voltage will send the voltage in main.
void TA1_0_IRQHandler(void) {
    static volatile uint32_t sin_x = 0;
    TIMER_A1->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG;
    if (sin_x == 127) {
        sin_x = 0;
    }
    else {
        sin_x++;
    }
    out_volt = sin_table[sin_x];
}

//Interrupt for sawtooth wave. Change out_volt
//which is the voltage to send to the dac. Then
//send_dac_voltage will send the voltage in main.
void TA2_0_IRQHandler(void) {
    static volatile uint32_t saw_x = 0;
    TIMER_A2->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG;
    if (saw_x == 127) {
        saw_x = 0;
    }
    else {
        saw_x++;
    }
    out_volt = saw_table[saw_x];
}


void Run_Squarewave(void) {
    // disable sawtoooth and sin timer interrupts
    TIMER_A1->CCTL[0] &= ~TIMER_A_CCTLN_CCIE;
    TIMER_A2->CCTL[0] &= ~TIMER_A_CCTLN_CCIE;

    // default to 100 Hz at 50% duty cycle
    TIMER_A0->CCR[0] = SQUARE_100_Hz;
    TIMER_A0->CCTL[0] = TIMER_A_CCTLN_CCIE;
    TIMER_A0->CTL = TIMER_A_CTL_TASSEL_2 | TIMER_A_CTL_MC_1 | TIMER_A_CTL_ID_2;
    TIMER_A0->CCR[1] = SQUARE_100_Hz / 2;
    TIMER_A0->CCTL[1] = TIMER_A_CCTLN_CCIE;
    NVIC->ISER[0] = (1 << (TA0_0_IRQn & 0x1F));
    NVIC->ISER[0] = (1 << (TA0_N_IRQn & 0x1F));

}

void Run_Sinwave(void) {
    // disable square and sawtooth wave timer interrupts
    TIMER_A0->CCTL[0] &= ~TIMER_A_CCTLN_CCIE;
    TIMER_A0->CCTL[1] &= ~TIMER_A_CCTLN_CCIE;
    TIMER_A2->CCTL[0] &= ~TIMER_A_CCTLN_CCIE;

    // default to 100 Hz
    TIMER_A1->CCR[0] = SIN_100_Hz;
    TIMER_A1->CCTL[0] = TIMER_A_CCTLN_CCIE;
    TIMER_A1->CTL = TIMER_A_CTL_TASSEL_2 | TIMER_A_CTL_MC_1;
    NVIC->ISER[0] = (1 << (TA1_0_IRQn & 0x1F));
}

void Run_Sawtooth(void) {
    // disable square and sin wave timer interrupts
    TIMER_A0->CCTL[0] &= ~TIMER_A_CCTLN_CCIE;
    TIMER_A0->CCTL[1] &= ~TIMER_A_CCTLN_CCIE;
    TIMER_A1->CCTL[0] &= ~TIMER_A_CCTLN_CCIE;

    // default to 100 Hz
    TIMER_A2->CCR[0] = SAW_100_Hz;
    TIMER_A2->CCTL[0] = TIMER_A_CCTLN_CCIE;
    TIMER_A2->CTL = TIMER_A_CTL_TASSEL_2 | TIMER_A_CTL_MC_1;
    NVIC->ISER[0] = (1 << (TA2_0_IRQn & 0x1F));
}
