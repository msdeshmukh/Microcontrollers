#include "msp.h"
#include "lcd_driver.h"
#include "delay.h"

/**
 * main.c
 */
void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
	set_DCO(DCORSEL_24_MHz);
	Initialize_LCD();
	while(1) {

	}
}
