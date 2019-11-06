/*
 * adc_driver.c
 *
 *  Created on: Oct 30, 2019
 *      Author: ryanmyers
 */

#include "msp.h"
#include "adc_driver.h"
#include <math.h>

static volatile uint32_t ms_cnt = 0;
static volatile uint8_t Measurement_Flag;
static volatile uint16_t ADC_Value;
static volatile uint16_t peak = 0;
static volatile uint16_t trough = 16384;
static volatile uint16_t max_per_cycle;
static volatile uint16_t min_per_cycle;
static volatile uint16_t dc_measurements[10];
static volatile uint16_t ac_measurements[10];
static volatile uint32_t freq = 1;

void Initialize_ADC(void) {
    CS->KEY = CS_KEY_VAL;   // Unlock clock registers

    CS->CTL1 &= ~(CS_CTL1_DIVHS_MASK | CS_CTL1_DIVS_MASK | CS_CTL1_SELS_MASK | CS_CTL1_DIVA_MASK | CS_CTL1_SELA_MASK); // Clear CS registers
    CS->CTL1 |= CS_CTL1_DIVHS_0 | CS_CTL1_DIVS_0 | CS_CTL1_SELS_3 | CS_CTL1_DIVA_0 | CS_CTL1_SELA__REFOCLK; // Set DCO to drive HSCLK and SMCLK

    CS->KEY = 0; // Lock clock registers

    ADC14->CTL0 &= ~ADC14_CTL0_ENC;      //Disable conversion
    ADC14->CTL0 = ADC14_CTL0_SHP |       //Enable internal sample timer
                  ADC14_CTL0_SSEL_4 |    //Select SMCLK
                  ADC14_CTL0_CONSEQ_2 |  //Single repeat channel
                  ADC14_CTL0_SHT0_2 |    //Sample 192 clocks
                  ADC14_CTL0_MSC |       //Auto repeat
                  ADC14_CTL0_ON;         //Turn on ADC
    ADC14->CTL1 = ADC14_CTL1_RES_3;      //14 Bit resolution and mem[0]
    ADC14->MCTL[0] = ADC14_MCTLN_INCH_0; //Set all to 0 as not needed
    ADC14->IER0 = ADC14_IER0_IE0;        //Enable interrupts on mem[0]
    ADCPORT->SEL0 |= ADCPIN;             //Initialize port to accept input
    ADCPORT->SEL1 |= ADCPIN;
    ADC14->CTL0 |= ADC14_CTL0_ENC |       //Enable conversion again
                   ADC14_CTL0_SC;         //Enable sampling
    NVIC->ISER[0] = 1 << (ADC14_IRQn & 0x1F);

    Measurement_Flag = MEASUREMENT_UNAVAILABLE;

    TIMER_A0->CCR[0] = 65535;
    TIMER_A0->CCTL[0] = TIMER_A_CCTLN_CCIE;
    TIMER_A0->CTL = TIMER_A_CTL_TASSEL_2 | TIMER_A_CTL_MC_1;
    NVIC->ISER[0] = (1 << (TA0_0_IRQn & 0x1F));

    TIMER_A1->CCR[0] = 1200;
    TIMER_A1->CCTL[0] = TIMER_A_CCTLN_CCIE;
    TIMER_A1->CTL = TIMER_A_CTL_TASSEL_2 | TIMER_A_CTL_MC_1;
    NVIC->ISER[0] = (1 << (TA1_0_IRQn & 0x1F));

//    TIMER_A2->CCR[0] = 6000;
//    TIMER_A2->CCTL[0] = TIMER_A_CCTLN_CCIE;
//    TIMER_A2->CTL = TIMER_A_CTL_TASSEL_2 | TIMER_A_CTL_MC_1;
//

    P5->DIR &= ~BIT6;
    P5->SEL0 |= BIT6;
    P5->SEL1 &= ~BIT6;

    TIMER_A2->CCTL[1] = TIMER_A_CCTLN_CCIE |
                        TIMER_A_CCTLN_CM_2 |
                        TIMER_A_CCTLN_CCIS_0 |
                        TIMER_A_CCTLN_CAP;

    TIMER_A2->CTL = TIMER_A_CTL_TASSEL_1;
    NVIC->ISER[0] = (1 << (TA2_N_IRQn & 0x1F));
}

void ADC14_IRQHandler(void) {
    static volatile uint32_t irq_cnt = 0;
    ADC14->CLRIFGR0 |= ADC14_CLRIFGR0_CLRIFG0;
    ADC_Value = ADC14->MEM[0];
    if (ADC_Value > peak) {
        peak = ADC_Value;
    }
    else if (ADC_Value < trough) {
        trough = ADC_Value;
    }
    dc_measurements[irq_cnt++ % 10] = ADC_Value;
}

void TA0_0_IRQHandler(void) {
    TIMER_A0->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG;
    static uint32_t irq_cnt = 0;
    if (irq_cnt++ == 200) {
        __disable_irq();
        max_per_cycle = peak;
        min_per_cycle = trough;
        freq = 1000/ms_cnt;
        peak = 0;
        trough = 16384;
        Measurement_Flag = MEASUREMENT_READY;
        irq_cnt = 0;
    }
}

void TA1_0_IRQHandler(void) {
    TIMER_A1->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG;
    static uint32_t irq_cnt = 0;
    ac_measurements[irq_cnt++ % 10] = ADC_Value;
}

void TA2_N_IRQHandler(void) {
    static uint32_t last_cap = 0;
    TIMER_A2->CCTL[1] &= ~TIMER_A_CCTLN_CCIFG;
    uint32_t current_cap = TIMER_A2->CCR[1];
    if (TIMER_A2->CCTL[1] & TIMER_A_CCTLN_COV) {
        ms_cnt = ((current_cap + 65525) - last_cap) / 128;
        TIMER_A2->CCTL[1] &= ~TIMER_A_CCTLN_COV;
    }
    else {
        ms_cnt = (current_cap - last_cap) / 128;
    }
    last_cap = current_cap;
}

float Read_AC_PP(void) {
    float pp_val = .000198 * (max_per_cycle - min_per_cycle) - .00477;
    return pp_val;
}

float Read_AC_RMS(void) {
    float rms_val = 0;
    uint64_t rms_total = 0;
    int i;
    for (i = 0; i < 10; i++) {
        rms_total += (ac_measurements[i] * ac_measurements[i]);
    }
    rms_total /= 10;
    rms_total = sqrt(rms_total);
    rms_val = .000198 * rms_total - .00477;
    return rms_val;
}

uint32_t Read_Freq(void) {
    return freq;
}

float Read_DC(void) {
    uint32_t dc_total = 0;
    int i;
    for (i = 0; i < 10; i++) {
        dc_total += dc_measurements[i];
    }
    dc_total /= 10;
    return .000198 * dc_total - .00477;
}

uint8_t Read_Measurement_Flag(void) {
    if (Measurement_Flag) {
        Measurement_Flag = MEASUREMENT_UNAVAILABLE;
        return MEASUREMENT_READY;
    }
    else {
        return MEASUREMENT_UNAVAILABLE;
    }
}

float Read_Center(void) {
    return .000198 * ((max_per_cycle + min_per_cycle) / 2) - .00477;
}

