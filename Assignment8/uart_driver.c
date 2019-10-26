/*
 * uart_driver.c
 *
 *  Created on: Oct 24, 2019
 *      Author: ryanmyers
 */

#include "msp.h"
#include "uart_driver.h"

void EUSCIA0_IRQHandler(void) {
    EUSCI_A0->IFG &= ~EUSCI_A_IFG_RXIFG;
    unsigned char rx_char = EUSCI_A0->RXBUF;
    EUSCI_A0->TXBUF = rx_char;
}

void Initialize_UART(void)
{

    CS->KEY = CS_KEY_VAL;   // Unlock clock registers

    CS->CTL1 &= ~(CS_CTL1_DIVHS_MASK | CS_CTL1_DIVS_MASK | CS_CTL1_SELS_MASK); // Clear CS registers
    CS->CTL1 |= CS_CTL1_DIVHS_0 | CS_CTL1_DIVS_0 | CS_CTL1_SELS_3; // Set DCO to drive HSCLK and SMCLK

    CS->KEY = 0; // Lock clock registers

    EUSCI_A0->CTLW0 = EUSCI_A_CTLW0_SWRST;

    EUSCI_A0->CTLW0 = EUSCI_A_CTLW0_SWRST
                        | EUSCI_A_CTLW0_UCSSEL_2;

    EUSCI_A0->BRW = FBRCLK_12MHz_UCBRW;
    EUSCI_A0->MCTLW = (FBRCLK_12MHz_UCBRS << 8)
                            | (FBRCLK_12MHz_UCBRF << 4)
                            | (EUSCI_A_MCTLW_OS16);

    //UART_PORT->DIR |= UART_TXD;
    //UART_PORT->DIR &= ~UART_RXD;
    UART_PORT->SEL0 |= UART_TXD | UART_RXD;
    UART_PORT->SEL1 &= ~(UART_TXD | UART_RXD);
    //UART_PORT->REN &= ~(UART_TXD | UART_RXD);

    EUSCI_A0->CTLW0 &= ~EUSCI_A_CTLW0_SWRST;

    EUSCI_A0->IE |= EUSCI_A_IE_RXIE;
    NVIC->ISER[0] = (1 << (EUSCIA0_IRQn & 0x1F));

}
