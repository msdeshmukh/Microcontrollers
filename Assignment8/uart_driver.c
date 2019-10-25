/*
 * uart_driver.c
 *
 *  Created on: Oct 24, 2019
 *      Author: ryanmyers
 */

#include "msp.h"
#include "uart_driver.h"

void Initialize_UART(void)
{
    CS->KEY = CS_KEY_VAL;   // Unlock clock registers

    CS->CTL1 &= ~(CS_CTL1_DIVHS_MASK | CS_CTL1_DIVS_MASK | CS_CTL1_SELS_MASK); // Clear CS registers
    CS->CTL1 |= CS_CTL1_DIVHS_0 | CS_CTL1_DIVS_0 | CS_CTL1_SELS_3; // Set DCO to drive HSCLK and SMCLK

    CS->KEY = 0; // Lock clock registers

    EUSCI_A0->CTLW0 = EUSCI_A_CTLW0_SWRST;
}
