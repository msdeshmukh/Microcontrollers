/*
 * i2c_driver.c
 *
 *  Created on: Nov 13, 2019
 *      Author: ryanmyers
 */

#include "msp.h"
#include "i2c_driver.h"


void Initialize_I2C(void) {
    CS->KEY = CS_KEY_VAL;   // Unlock clock registers

    CS->CTL1 &= ~(CS_CTL1_DIVHS_MASK | CS_CTL1_DIVS_MASK | CS_CTL1_SELS_MASK); // Clear CS registers
    CS->CTL1 |= CS_CTL1_DIVHS_0 | CS_CTL1_DIVS_0 | CS_CTL1_SELS_3; // Set DCO to drive HSCLK and SMCLK

    CS->KEY = 0; // Lock clock registers

    //Reset CTLW0 registers
    EUSCI_B0->CTLW0 = EUSCI_B_CTLW0_SWRST;

    //Configure Registers
    EUSCI_B0->CTLW0 = EUSCI_B_CTLW0_SWRST
                        | EUSCI_B_CTLW0_MODE_3
                        | EUSCI_B_CTLW0_UCSSEL_2
                        | EUSCI_B_CTLW0_TR;
                        //| EUSCI_B_CTLW0_TXSTT
                        //| EUSCI_B_CTLW0_TXSTP;



}


