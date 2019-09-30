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

	Write_string_LCD("Hello World\nAssignment 3", 24);
	Home_LCD();
	while(1) {

	}
}
