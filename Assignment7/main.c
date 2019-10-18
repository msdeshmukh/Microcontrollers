#include "msp.h"
#include "spi_driver.h"

/**
 * main.c
 */
void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
	__disable_irq();
	Initialize_SPI();


	while(1)
	{
	    Send_DAC_Voltage(2.0);
	}
}
