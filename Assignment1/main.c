#include "msp.h"
#include "delay.h"


/**
 * main.c
 */
void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
	set_DCO(DCORSEL_1POINT5_MHz);
	P4->DIR |= BIT3;
	P4->REN &= ~BIT3;
	P4->SEL0 |= BIT3;
	P4->SEL1 &= ~BIT3;
	P4->DIR |= BIT1;
	P4->REN &= ~BIT1;
	P4->SEL0 &= ~BIT1;
	P4->SEL1 &= ~BIT1;
	P4->OUT |= BIT1;
	//delay_us(10);
	while (1) {
	    //P4->OUT |= ~BIT0;
	    //delay_us(10);
	}
}
