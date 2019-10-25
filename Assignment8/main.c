#include "msp.h"
#include "uart_driver.h"
#include "delay.h"

/**
 * main.c
 */
void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
	set_DCO(DCORSEL_12_MHz);
	Initialize_UART();
	__enable_irq();
	while(1) {

	}

}
