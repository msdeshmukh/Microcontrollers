/*
 * uart_driver.c
 *
 *  Created on: Oct 24, 2019
 *      Author: Ryan Myers, Mihir Deshmukh
 */

#include "msp.h"
#include "uart_driver.h"

static volatile int input_flag = INPUT_UNAVAILABLE;
static volatile uint8_t inValue[4];

//Interrupt handler for data on RX buffer
void EUSCIA0_IRQHandler(void) {
    //Process value and write back to terminal immediately
    static volatile int char_index = 0;
    EUSCI_A0->IFG &= ~EUSCI_A_IFG_RXIFG;
    unsigned char rx_char = EUSCI_A0->RXBUF;
    EUSCI_A0->TXBUF = rx_char;

    //Look up ascii values for numbers coming in RX buffer
    //Build number in inValue that will be written to DAC
    switch(rx_char) {
        case ZERO:
            inValue[char_index] = 0;
            char_index++;
            break;

        case ONE:
            inValue[char_index] = 1;
            char_index++;
            break;

        case TWO:
            inValue[char_index] = 2;
            char_index++;
            break;

        case THREE:
            inValue[char_index] = 3;
            char_index++;
            break;

        case FOUR:
            inValue[char_index] = 4;
            char_index++;
            break;

        case FIVE:
            inValue[char_index] = 5;
            char_index++;
            break;

        case SIX:
            inValue[char_index] = 6;
            char_index++;
            break;

        case SEVEN:
            inValue[char_index] = 7;
            char_index++;
            break;

        case EIGHT:
            inValue[char_index] = 8;
            char_index++;
            break;

        case NINE:
            inValue[char_index] = 9;
            char_index++;
            break;

        case NEW_LINE:
            input_flag = INPUT_READY;
            char_index = 0;
            break;

        case RETURN:
            input_flag = INPUT_READY;
            char_index = 0;
            break;

        default:
            break;

    }

}

//Lets you know if a value is available on the RX buffer
int CheckInputFlag(void) {
    return input_flag;
}

//Convert array to a 16bit value that can be written to the DAC
uint16_t GetInputValue(void) {
    uint16_t input = 0;
    int i;
    int scaler = 1;
    for (i = 3; i >= 0; i--) {
        input += inValue[i] * scaler;
        scaler *= 10;
    }
    input_flag = INPUT_UNAVAILABLE;
    return input;
}


//Initialization procedure for UART
void Initialize_UART(void)
{

    CS->KEY = CS_KEY_VAL;   // Unlock clock registers

    CS->CTL1 &= ~(CS_CTL1_DIVHS_MASK | CS_CTL1_DIVS_MASK | CS_CTL1_SELS_MASK); // Clear CS registers
    CS->CTL1 |= CS_CTL1_DIVHS_0 | CS_CTL1_DIVS_0 | CS_CTL1_SELS_3; // Set DCO to drive HSCLK and SMCLK

    CS->KEY = 0; // Lock clock registers

    //Reset CTLW0 registers
    EUSCI_A0->CTLW0 = EUSCI_A_CTLW0_SWRST;

    //Keep SWRST and choose SMCLK
    EUSCI_A0->CTLW0 = EUSCI_A_CTLW0_SWRST
                        | EUSCI_A_CTLW0_UCSSEL_2;

    //Set the appropriate baud rate
    EUSCI_A0->BRW = FBRCLK_12MHz_UCBRW;
    //Set the fractional modulation divisors and enable oversampling
    EUSCI_A0->MCTLW = (FBRCLK_12MHz_UCBRS << 8)
                            | (FBRCLK_12MHz_UCBRF << 4)
                            | (EUSCI_A_MCTLW_OS16);

    //Configure UART ports
    UART_PORT->SEL0 |= UART_TXD | UART_RXD;
    UART_PORT->SEL1 &= ~(UART_TXD | UART_RXD);

    //Turn of swrst
    EUSCI_A0->CTLW0 &= ~EUSCI_A_CTLW0_SWRST;

    //Enable interrupts for UART
    EUSCI_A0->IE |= EUSCI_A_IE_RXIE;
    NVIC->ISER[0] = (1 << (EUSCIA0_IRQn & 0x1F));

}
