/*
 * adc_driver.c
 *
 *  Created on: Oct 30, 2019
 *      Author: ryanmyers
 */

#include "msp.h"
#include "adc_driver.h"

static volatile uint16_t ADC_Value;
static volatile uint8_t ADC_Flag;

void Initialize_ADC(void) {
    CS->KEY = CS_KEY_VAL;   // Unlock clock registers

    CS->CTL1 &= ~(CS_CTL1_DIVHS_MASK | CS_CTL1_DIVS_MASK | CS_CTL1_SELS_MASK); // Clear CS registers
    CS->CTL1 |= CS_CTL1_DIVHS_0 | CS_CTL1_DIVS_0 | CS_CTL1_SELS_3; // Set DCO to drive HSCLK and SMCLK

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
}

void ADC14_IRQHandler(void) {
    __disable_irq();
    ADC_Value = ADC14->MEM[0];
    ADC_Flag = ADC_READY;
}

int ReadADC(void) {
    if (ADC_Flag == ADC_UNAVAILABLE) {
        return -1;
    }
    else if (ADC_Flag == ADC_READY) {
        ADC_Flag = ADC_UNAVAILABLE;
        return ADC_Value;
    }
}



