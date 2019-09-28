#include "msp.h"
#include "delay.h"


/**
 * main.c
 */
void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
	set_DCO(DCORSEL_12_MHz);   // Set DCO frequency to 1.5MHz
	P4->DIR |= BIT3;    // Set P4.3 to output
	P4->REN &= ~BIT3;   // Disable P4.3 pull up/down resistor
	P4->SEL0 |= BIT3;   // Select MCLK as source of output
	P4->SEL1 &= ~BIT3;
	P4->DIR |= BIT1;    // Set P4.1 to output
	P4->REN &= ~BIT1;   // Disable P4.1 pull up/down resistor
	P4->SEL0 &= ~BIT1;  // Setup P4.1 as GPIO
	P4->SEL1 &= ~BIT1;
	// loop infinitely to time delay function
	while (1) {
	    P4->OUT ^= BIT1;    // Toggle bit to show beginning and end of delay
	    delay_us(40);
	}
}
