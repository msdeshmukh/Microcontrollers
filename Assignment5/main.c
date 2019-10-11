#include "msp.h"
#include "delay.h"
#include "TIMERA_ISR.h"

/**
 * main.c
 */
void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
	Initialize_Timers();
	P4->DIR |= BIT3;    // Set P4.3 to output
    P4->REN &= ~BIT3;   // Disable P4.3 pull up/down resistor
    P4->SEL0 |= BIT3;   // Select MCLK as source of output
    P4->SEL1 &= ~BIT3;
	__enable_irq();
	while(1);
}

