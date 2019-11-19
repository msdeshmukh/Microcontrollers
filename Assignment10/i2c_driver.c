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
    EUSCI_B1->CTLW0 = EUSCI_B_CTLW0_SWRST;

    //Configure Registers
    EUSCI_B1->CTLW0 = EUSCI_B_CTLW0_SWRST
                        | EUSCI_B_CTLW0_MODE_3
                        | EUSCI_B_CTLW0_UCSSEL_2
                        | EUSCI_B_CTLW0_MST
                        | EUSCI_B_CTLW0_MSB
                        | EUSCI_B_CTLW0_SYNC;
                        //| EUSCI_B_CTLW0_TXSTT
                        //| EUSCI_B_CTLW0_TXSTP;

    EUSCI_B1->BRW = 30;

    I2C_PORT->REN &= ~(SDA | SCL);
    I2C_PORT->SEL0 |= SDA | SCL;
    I2C_PORT->SEL1 &= ~(SDA | SCL);

    EUSCI_B1->CTLW0 &= ~EUSCI_B_CTLW0_SWRST;


}

void WriteEEPROM(uint16_t addr, uint8_t data) {
    volatile uint8_t addr_hi = (addr >> 8) & 0xFF;
    volatile uint8_t addr_lo = addr & 0xFF;
    EUSCI_B1->CTLW0 |= EUSCI_B_CTLW0_TR;
    EUSCI_B1->I2CSA = EEPROM_ADDR;
    // Send start condition
    EUSCI_B1->CTLW0 |= EUSCI_B_CTLW0_TXSTT;
    while(!(EUSCI_B1->IFG & EUSCI_B_IFG_TXIFG0));
    EUSCI_B1->TXBUF = addr_hi;
    while(!(EUSCI_B1->IFG & EUSCI_B_IFG_TXIFG0));
    EUSCI_B1->TXBUF = addr_hi;
    while(!(EUSCI_B1->IFG & EUSCI_B_IFG_TXIFG0));
    EUSCI_B1->TXBUF = addr_lo;
    while(!(EUSCI_B1->IFG & EUSCI_B_IFG_TXIFG0));
    EUSCI_B1->TXBUF = data;
    while(!(EUSCI_B1->IFG & EUSCI_B_IFG_TXIFG0));
    while(!(EUSCI_B1->IFG & EUSCI_B_IFG_BIT9IFG));
    EUSCI_B1->CTLW0 |= EUSCI_B_CTLW0_TXSTP;

}

uint8_t ReadEEPROM(uint16_t addr) {
    volatile uint8_t data;
    uint8_t addr_hi = (addr >> 8) & 0xFF;
    uint8_t addr_lo = addr & 0xFF;
    EUSCI_B1->CTLW0 |= EUSCI_B_CTLW0_TR;
    EUSCI_B1->I2CSA = EEPROM_ADDR;
    // Send start condition
    EUSCI_B1->CTLW0 |= EUSCI_B_CTLW0_TXSTT;
    while(!(EUSCI_B1->IFG & EUSCI_B_IFG_TXIFG0));
    // Send address to EEPROM
    EUSCI_B1->TXBUF = addr_hi;
    while(!(EUSCI_B1->IFG & EUSCI_B_IFG_TXIFG0));
    EUSCI_B1->TXBUF = addr_hi;
    while(!(EUSCI_B1->IFG & EUSCI_B_IFG_TXIFG0));
    EUSCI_B1->TXBUF = addr_lo;
    while(!(EUSCI_B1->IFG & EUSCI_B_IFG_TXIFG0));
    while(!(EUSCI_B1->IFG & EUSCI_B_IFG_BIT9IFG));
    // Enter receive mode and generate start
    EUSCI_B1->CTLW0 &= ~EUSCI_B_CTLW0_TR;
    EUSCI_B1->I2CSA = EEPROM_ADDR;
    EUSCI_B1->CTLW0 |= EUSCI_B_CTLW0_TXSTT;
    // Receive data from EEPROM
    while(EUSCI_B1->CTLW0 & EUSCI_B_CTLW0_TXSTT);
    while(!(EUSCI_B1->IFG & EUSCI_B_IFG_RXIFG0));
    data = EUSCI_B1->RXBUF;
    EUSCI_B1->CTLW0 |= EUSCI_B_CTLW0_TXSTP;
    return data;
}


