/*
 * uart_driver.c
 *
 *  Created on: Oct 24, 2019
 *      Author: ryanmyers
 */

#include "msp.h"
#include "uart_driver.h"

static volatile int input_flag = INPUT_UNAVAILABLE;
static volatile uint8_t inValue[4];

void EUSCIA0_IRQHandler(void) {
    static volatile int char_index = 0;
    EUSCI_A0->IFG &= ~EUSCI_A_IFG_RXIFG;
    unsigned char rx_char = EUSCI_A0->RXBUF;
    EUSCI_A0->TXBUF = rx_char;
    if (char_index < 4 && input_flag == INPUT_UNAVAILABLE) {
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

}

int CheckInputFlag(void) {
    return input_flag;
}

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
