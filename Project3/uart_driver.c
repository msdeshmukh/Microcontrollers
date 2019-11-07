/*
 * uart_driver.c
 *
 *  Created on: Oct 24, 2019
 *      Author: ryanmyers
 */

#include "msp.h"
#include "uart_driver.h"
#include "adc_driver.h"

static volatile int input_flag = INPUT_UNAVAILABLE;
static volatile uint8_t inValue[4];

/*void EUSCIA0_IRQHandler(void) {
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
}*/

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

    //EUSCI_A0->IE |= EUSCI_A_IE_RXIE;
    //NVIC->ISER[0] = (1 << (EUSCIA0_IRQn & 0x1F));

    Init_Desc_Values_To_VT100();
}

void Write_Desc_Values_To_VT100(void)
{
    float val = 1.0;
    uint32_t i;
    uint32_t temp = 0;
    uint32_t Vpp = 0;
    uint32_t ones = 0;
    uint32_t tens = 0;
    uint32_t hundreds = 0;

    // Return to Top left corner
    EUSCI_A0->TXBUF = 0x1B;                       //Esc
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '[';                        //[
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '3';                        //8 position
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '9';                        //8 position
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = 'A';                        //Up
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));

    EUSCI_A0->TXBUF = 0x1B;                       //Esc
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '[';                        //[
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '8';                        //8 position
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '8';                        //8 position
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = 'D';                        //left
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));

    //Reset cursor to AC RMS
    EUSCI_A0->TXBUF = 0x1B;                       //Esc
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '[';                        //[
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '7';                        //7 position
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = 'C';                        //right
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));

    //Write Value Here in the format x.xx
    val = Read_AC_RMS();
    temp = (int)(val * 100);
    hundreds = ((temp - (temp % 100)) / 100 ) + ZERO;
    EUSCI_A0->TXBUF = hundreds;                       //New-line
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '.';                            //Esc
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    tens = (val * 100 - (hundreds-ZERO) * 100)/10 + ZERO;
    EUSCI_A0->TXBUF = tens;                            //[
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    ones = ((val * 100) - (hundreds - ZERO)*100 - (tens - ZERO)*10) + ZERO;
    EUSCI_A0->TXBUF = ones;                           //4 position
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));

    //Reset cursor to AC RMS Bar
    EUSCI_A0->TXBUF = 0x0A;                       //New-line
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = 0x1B;                       //Esc
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '[';                        //[
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '4';                        //4 position
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = 'D';                        //left
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));

    //Write number of bars as RMS
    for(i = 0; i < 33*(val/3.3); i++)
    {
        EUSCI_A0->TXBUF = 178;                        //New-line
        while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    }
    for(i = 0; i < 33-(33*(val/3.3)); i++)
    {
        EUSCI_A0->TXBUF = ' ';                        //New-line
        while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    }

    //Reset cursor to AC Vpp
    EUSCI_A0->TXBUF = 0x0A;                       //New-line
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = 0x0A;                       //New-line (down two lines to vpp)
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = 0x1B;                       //Esc
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '[';                        //[
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '5';                        //Reset cursor to very left
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '0';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = 'D';                        //left
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = 0x1B;                       //Esc
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '[';                        //[
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '7';                        //Reset cursor to start of Vpp (8 right)
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = 'C';                        //left
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));

    //Write Value Here in the format x.xx
    val = Read_AC_PP();
    temp = (int)(val * 100);
    hundreds = ((temp - (temp % 100)) / 100 ) + ZERO;
    EUSCI_A0->TXBUF = hundreds;                       //New-line
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '.';                            //Esc
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    tens = (val * 100 - (hundreds-ZERO) * 100)/10 + ZERO;
    EUSCI_A0->TXBUF = tens;                            //[
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    ones = ((val * 100) - (hundreds - ZERO)*100 - (tens - ZERO)*10) + ZERO;
    EUSCI_A0->TXBUF = ones;                           //4 position
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));

    //Go down to AC Frq
    EUSCI_A0->TXBUF = 0x0A;                       //New-line (down one line to AC Frq)
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = 0x1B;                       //Esc
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '[';                        //[
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '3';                        //Reset cursor to very left
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '0';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = 'D';                        //left
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = 0x1B;                       //Esc
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '[';                        //[
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '7';                        //Reset cursor to start of Vpp (8 right)
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = 'C';                        //left
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));

    //Write Value Here in the format x.xx
    temp = Read_Freq();
    if(temp == 1000)
    {
        EUSCI_A0->TXBUF = '1';                       //New-line
        while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
        EUSCI_A0->TXBUF = '0';                            //Esc
        while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
        EUSCI_A0->TXBUF = '0';                            //[
        while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
        EUSCI_A0->TXBUF = '0';                           //4 position
        while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    }
    else if(temp < 1000 && temp >= 100)
    {
        hundreds = temp/100;
        EUSCI_A0->TXBUF = hundreds + ZERO;
        while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
        tens = (temp - hundreds*100)/10;
        EUSCI_A0->TXBUF = tens + ZERO;
        while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
        EUSCI_A0->TXBUF = temp - hundreds*100 - tens*10 + ZERO;
        while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    }
    else if(temp < 100 && temp >= 10)
    {
        EUSCI_A0->TXBUF = ZERO + temp/10;
        while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
        EUSCI_A0->TXBUF = ZERO + (temp - (temp/10)*10);
        while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    }
    else
    {
        EUSCI_A0->TXBUF = ZERO + temp;
        while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    }

    //Go down to DC Val
    EUSCI_A0->TXBUF = 0x0A;                       //New-line (down one line to DCV)
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = 0x1B;                       //Esc
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '[';                        //[
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '3';                        //Reset cursor to very left
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '0';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = 'D';                        //left
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = 0x1B;                       //Esc
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '[';                        //[
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '7';                        //Reset cursor to start of Vpp (8 right)
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = 'C';                        //left
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));

    //Write Value Here in the format x.xx
    val = Read_DC();
    temp = (int)(val * 100);
    hundreds = ((temp - (temp % 100)) / 100 ) + ZERO;
    EUSCI_A0->TXBUF = hundreds;                       //New-line
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '.';                            //Esc
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    tens = (val * 100 - (hundreds-ZERO) * 100)/10 + ZERO;
    EUSCI_A0->TXBUF = tens;                            //[
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    ones = ((val * 100) - (hundreds - ZERO)*100 - (tens - ZERO)*10) + ZERO;
    EUSCI_A0->TXBUF = ones;                           //4 position
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));

    //Go down to DC Bar
    EUSCI_A0->TXBUF = 0x0A;                       //New-line (down one line to DC Bar)
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = 0x1B;                       //Esc
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '[';                        //[
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '3';                        //Reset cursor to very left
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '0';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = 'D';                        //left
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = 0x1B;                       //Esc
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '[';                        //[
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '7';                        //Reset cursor to start of Vpp (8 right)
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = 'C';                        //left
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));

    //Write number of bars as RMS
    for(i = 0; i < 33*(val/3.3); i++)
    {
        EUSCI_A0->TXBUF = 178;                        //New-line
        while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    }
    for(i = 0; i < 33-(33*(val/3.3)); i++)
    {
        EUSCI_A0->TXBUF = ' ';                        //New-line
        while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    }

    // Return to Top left corner
    EUSCI_A0->TXBUF = 0x1B;                       //Esc
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '[';                        //[
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '3';                        //8 position
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '9';                        //8 position
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = 'A';                        //Up
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));

    EUSCI_A0->TXBUF = 0x1B;                       //Esc
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '[';                        //[
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '8';                        //8 position
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '8';                        //8 position
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = 'D';                        //left
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
}

/*
 * Initiates the displaying of the following values
 * AC RMS (True RMS including DC offset)
 * AC RMS bar graph
 * AC RMS bar graph scale
 * AC VPP
 * AC Frq
 * DC Vol
 * DC Vol bar graph
 * DC Vol bar graph scale
 */
void Init_Desc_Values_To_VT100(void)
{
    //AC RMS
    EUSCI_A0->TXBUF = 'A';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = 'C';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = ' ';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = 'R';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = 'M';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = 'S';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = ':';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = ' ';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = 0x0A;                       //New-line
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = 0x1B;                       //Esc
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '[';                        //[
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '8';                        //8 position
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = 'D';                        //left
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));

    //AC Bar graph
    EUSCI_A0->TXBUF = 'A';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = 'C';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = ' ';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = 'B';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = 'A';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = 'R';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = ':';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = ' ';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = 0x0A;                       //New-line
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = 0x1B;                       //Esc
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '[';                        //[
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '8';                        //6 position
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = 'D';                        //left
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));

    //AC Bar scale
    EUSCI_A0->TXBUF = 'A';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = 'C';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = ' ';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = 'S';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = 'C';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = 'L';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = ':';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '0';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '|';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '|';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '|';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '|';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '|';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '|';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '|';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '|';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '|';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '1';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '|';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '|';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '|';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '|';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '|';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '|';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '|';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '|';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '|';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '2';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '|';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '|';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '|';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '|';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '|';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '|';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '|';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '|';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '|';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '3';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '|';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '|';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '3';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '.';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '3';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = 0x0A;                       //New-line
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = 0x1B;                       //Esc
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '[';                        //[
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '4';                        //43 position
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '3';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = 'D';                        //left
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));

    //AC Vpp
    EUSCI_A0->TXBUF = 'A';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = 'C';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = ' ';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = 'V';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = 'p';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = 'p';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = ':';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = ' ';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = 0x0A;                       //New-line
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = 0x1B;                       //Esc
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '[';                        //[
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '8';                        //6 position
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = 'D';                        //left
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));

    //AC Frequency
    EUSCI_A0->TXBUF = 'A';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = 'C';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = ' ';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = 'F';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = 'R';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = 'Q';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = ':';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = ' ';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = 0x0A;                       //New-line
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = 0x1B;                       //Esc
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '[';                        //[
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '8';                        //6 position
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = 'D';                        //left
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));

    //DC Voltage
    EUSCI_A0->TXBUF = 'D';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = 'C';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = ' ';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = 'V';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = 'O';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = 'L';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = ':';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = ' ';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = 0x0A;                       //New-line
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = 0x1B;                       //Esc
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '[';                        //[
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '8';                        //6 position
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = 'D';                        //left
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));

    //DC Bar
    EUSCI_A0->TXBUF = 'D';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = 'C';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = ' ';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = 'B';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = 'A';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = 'R';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = ':';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = ' ';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = 0x0A;                       //New-line
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = 0x1B;                       //Esc
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '[';                        //[
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '8';                        //6 position
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = 'D';                        //left
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));

    //DC Scale
    EUSCI_A0->TXBUF = 'D';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = 'C';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = ' ';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = 'S';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = 'C';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = 'L';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = ':';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '0';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '|';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '|';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '|';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '|';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '|';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '|';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '|';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '|';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '|';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '1';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '|';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '|';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '|';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '|';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '|';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '|';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '|';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '|';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '|';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '2';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '|';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '|';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '|';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '|';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '|';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '|';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '|';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '|';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '|';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '3';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '|';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '|';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '3';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '.';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '3';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = 0x0A;                       //New-line
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = 0x1B;                       //Esc
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '[';                        //[
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '4';                        //43 position
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '3';
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = 'D';                        //left
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));

    // Return to Top left corner
    EUSCI_A0->TXBUF = 0x1B;                       //Esc
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '[';                        //[
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '3';                        //8 position
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '9';                        //8 position
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = 'A';                        //Up
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));

    EUSCI_A0->TXBUF = 0x1B;                       //Esc
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '[';                        //[
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '8';                        //8 position
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = '8';                        //8 position
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = 'D';                        //left
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
}
