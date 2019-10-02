#include "msp.h"
#include "lcd_driver.h"
#include "keypad_driver.h"


/**
 * main.c
 */
void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
	Initialize_LCD();
	Initialize_keypad();
	while(1) {

	}
}
